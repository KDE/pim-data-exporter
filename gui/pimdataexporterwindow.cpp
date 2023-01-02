/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterwindow.h"
#include "dialog/showarchivestructuredialog.h"
#include "importexportprogressindicatorgui.h"
#include "job/fullsynchronizeresourcesjob.h"
#include "pimdataexportgui_debug.h"
#include "trayicon/pimdatatrayicon.h"
#include "widgets/logwidget.h"

#include "dialog/selectiontypedialog.h"
#include "dialog/synchronizeresourcedialog.h"
#include "pimdatabackuprestoreui.h"
#include "pimdataexporterkernel.h"
#include "pimdataimportdatainfofile.h"

#include "dialog/backupfilestructureinfodialog.h"

#include <MailCommon/FilterManager>
#include <MailCommon/MailKernel>

#include <PimCommon/PimUtil>

#include <Akonadi/ControlGui>

#include <KActionCollection>
#include <KFileWidget>
#include <KLocalizedString>
#include <KMessageBox>
#include <KRecentDirs>
#include <KRecentFilesMenu>
#include <KSharedConfig>
#include <KStandardAction>
#include <QCommandLineParser>
#include <QFileDialog>
#include <QPointer>
#include <QStandardPaths>
#include <QStatusBar>

#include <dialog/pimdataexporterconfiguredialog.h>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif

PimDataExporterWindow::PimDataExporterWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , mLogWidget(new LogWidget(this))
{
    // Initialize filtermanager
    (void)MailCommon::FilterManager::instance();
#ifdef WITH_KUSERFEEDBACK
    // Initialize
    (void)UserFeedBackManager::self();
#endif
    auto kernel = new PimDataExporterKernel(this);
    CommonKernel->registerKernelIf(kernel); // register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel); // SettingsIf is used in FolderTreeWidget

    setupActions(true);
    setupGUI(Keys | StatusBar | Save | Create, QStringLiteral("pimdataexporter.rc"));

    setCentralWidget(mLogWidget);
    resize(800, 600);
    Akonadi::ControlGui::widgetNeedsAkonadi(this);
    statusBar()->hide();
    mTrayIcon = new PimDataTrayIcon(this);
#ifdef WITH_KUSERFEEDBACK
    auto userFeedBackNotificationPopup = new KUserFeedback::NotificationPopup(this);
    userFeedBackNotificationPopup->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
}

PimDataExporterWindow::~PimDataExporterWindow()
{
    MailCommon::FilterManager::instance()->cleanup();
}

void PimDataExporterWindow::initializeBackupRestoreUi()
{
    mPimDataBackupRestoreUI = new PimDataBackupRestoreUI(this, this);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::addInfo, this, &PimDataExporterWindow::slotAddInfo);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::addEndLine, this, &PimDataExporterWindow::slotAddEndLine);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::addError, this, &PimDataExporterWindow::slotAddError);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::addTitle, this, &PimDataExporterWindow::slotAddTitle);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::updateActions, this, &PimDataExporterWindow::slotUpdateActions);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::jobFinished, this, &PimDataExporterWindow::slotJobFinished);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::backupDone, this, &PimDataExporterWindow::slotShowBackupFinishDialogInformation);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::jobFailed, this, &PimDataExporterWindow::slotJobFailed);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestoreUI::needSyncResource, this, &PimDataExporterWindow::slotAddResourceToSync);
    connect(mPimDataBackupRestoreUI, &PimDataBackupRestore::restoreDone, this, &PimDataExporterWindow::slotRestoreDone);
}

void PimDataExporterWindow::slotAddResourceToSync(const QString &name, const QString &identifier)
{
    mNeedToSyncResources.insert(name, identifier);
}

void PimDataExporterWindow::slotJobFinished()
{
    mPimDataBackupRestoreUI->nextStep();
}

void PimDataExporterWindow::slotJobFailed()
{
    mPimDataBackupRestoreUI->closeArchive();
}

void PimDataExporterWindow::slotRestoreDone()
{
    if (!mNeedToSyncResources.isEmpty()) {
        QPointer<SynchronizeResourceDialog> dlg = new SynchronizeResourceDialog(this);
        dlg->setResources(mNeedToSyncResources);
        QStringList list;
        if (dlg->exec()) {
            list = dlg->resources();
        }
        delete dlg;
        if (!list.isEmpty()) {
            slotAddInfo(i18n("Full sync starts..."));
            auto job = new FullSynchronizeResourcesJob(this);
            job->setWindowParent(this);
            job->setResources(list);
            connect(job, &FullSynchronizeResourcesJob::synchronizeFinished, this, &PimDataExporterWindow::slotFullSyncFinished);
            connect(job, &FullSynchronizeResourcesJob::synchronizeInstanceDone, this, &PimDataExporterWindow::slotFullSyncInstanceDone);
            connect(job, &FullSynchronizeResourcesJob::synchronizeInstanceFailed, this, &PimDataExporterWindow::slotFullSyncInstanceFailed);
            job->start();
        } else {
            importDone();
        }
    } else {
        importDone();
    }
}

void PimDataExporterWindow::slotShowBackupFinishDialogInformation()
{
    showFinishInformation();
}

void PimDataExporterWindow::importDone()
{
    slotUpdateActions(false);
    mTrayIcon->setStatus(KStatusNotifierItem::Passive);
}

void PimDataExporterWindow::slotFullSyncFinished()
{
    importDone();
    const QString str = i18n("Full sync finished.");
    slotAddInfo(str);
    mTrayIcon->setToolTipSubTitle(str);
}

void PimDataExporterWindow::slotFullSyncInstanceDone(const QString &identifier)
{
    slotAddInfo(i18n("Full sync for \"%1\" done.", identifier));
}

void PimDataExporterWindow::slotFullSyncInstanceFailed(const QString &identifier)
{
    slotAddError(i18n("Full sync for \"%1\" failed.", identifier));
}

void PimDataExporterWindow::showFinishInformation()
{
    KMessageBox::information(this,
                             i18n("For restoring data, you must use \"pimdataexporter\". "
                                  "Be careful as it can overwrite your existing settings and data."),
                             i18n("Backup"),
                             QStringLiteral("setProgressDialogLabelBackupInfos"));
    mTrayIcon->setStatus(KStatusNotifierItem::Passive);
}

void PimDataExporterWindow::handleCommandLine(const QCommandLineParser &parser)
{
    QString templateFile;
    if (parser.isSet(QStringLiteral("template"))) {
        templateFile = parser.value(QStringLiteral("template"));
    }
    if (parser.isSet(QStringLiteral("import"))) {
        if (!parser.positionalArguments().isEmpty()) {
            loadData(parser.positionalArguments().at(0), templateFile);
        }
    } else if (parser.isSet(QStringLiteral("export"))) {
        if (!parser.positionalArguments().isEmpty()) {
            backupData(parser.positionalArguments().at(0), templateFile);
        }
    }
}

void PimDataExporterWindow::setupActions(bool canZipFile)
{
    KActionCollection *ac = actionCollection();

    mBackupAction = ac->addAction(QStringLiteral("backup"), this, &PimDataExporterWindow::slotBackupData);
    mBackupAction->setText(i18n("Export Data..."));
    mBackupAction->setIcon(QIcon::fromTheme(QStringLiteral("document-export")));
    mBackupAction->setEnabled(canZipFile);

    mRestoreAction = ac->addAction(QStringLiteral("restore"), this, &PimDataExporterWindow::slotRestoreData);
    mRestoreAction->setText(i18n("Import Data..."));
    mRestoreAction->setIcon(QIcon::fromTheme(QStringLiteral("document-import")));
    mRestoreAction->setEnabled(canZipFile);

    mSaveLogAction = ac->addAction(QStringLiteral("save_log"), this, &PimDataExporterWindow::slotSaveLog);
    mSaveLogAction->setText(i18n("Save log..."));

    mArchiveStructureInfo = ac->addAction(QStringLiteral("show_structure_info"), this, &PimDataExporterWindow::slotShowStructureInfos);
    mArchiveStructureInfo->setText(i18n("Show Archive Structure Information..."));

    mShowArchiveInformationsAction = ac->addAction(QStringLiteral("show_archive_info"), this, &PimDataExporterWindow::slotShowArchiveInformations);
    mShowArchiveInformationsAction->setText(i18n("Show Archive Information..."));

    mShowArchiveInformationsAboutCurrentArchiveAction =
        ac->addAction(QStringLiteral("show_current_archive_info"), this, &PimDataExporterWindow::slotShowCurrentArchiveInformations);
    mShowArchiveInformationsAboutCurrentArchiveAction->setText(i18n("Show Information on current Archive..."));
    mShowArchiveInformationsAboutCurrentArchiveAction->setEnabled(false);

    KStandardAction::quit(this, &PimDataExporterWindow::close, ac);
    mRecentFilesMenu = new KRecentFilesMenu(this);
    actionCollection()->addAction(QStringLiteral("pimdataexporter_file_open_recent"), mRecentFilesMenu->menuAction());
    connect(mRecentFilesMenu, &KRecentFilesMenu::urlTriggered, this, &PimDataExporterWindow::slotRestoreFile);

    KStandardAction::preferences(this, &PimDataExporterWindow::slotConfigure, ac);
}

void PimDataExporterWindow::slotConfigure()
{
    PimDataExporterConfigureDialog dlg(this);
    dlg.exec();
}

void PimDataExporterWindow::slotUpdateActions(bool inAction)
{
    mBackupAction->setEnabled(!inAction);
    mRestoreAction->setEnabled(!inAction);
    mSaveLogAction->setEnabled(!inAction);
    mArchiveStructureInfo->setEnabled(!inAction);
    mShowArchiveInformationsAction->setEnabled(!inAction);
    mShowArchiveInformationsAboutCurrentArchiveAction->setEnabled(!inAction && !mLastArchiveFileName.isEmpty());
}

void PimDataExporterWindow::slotRestoreFile(const QUrl &url)
{
    if (!url.isEmpty()) {
        loadData(url.toLocalFile());
    }
}

void PimDataExporterWindow::slotShowArchiveInformations()
{
    const QString filename = QFileDialog::getOpenFileName(this, i18n("Select Archive"), QString(), QStringLiteral("%1 (*.zip)").arg(i18n("Zip file")));
    if (filename.isEmpty()) {
        return;
    }

    ShowArchiveStructureDialog dlg(filename, this);
    dlg.exec();
}

void PimDataExporterWindow::slotSaveLog()
{
    if (mLogWidget->isEmpty()) {
        KMessageBox::information(this, i18n("Log is empty."), i18n("Save log"));
        return;
    }
    const QString log = mLogWidget->toHtml();
    const QString filter(i18n("HTML Files (*.html)"));
    PimCommon::Util::saveTextAs(log, filter, this);
}

void PimDataExporterWindow::slotBackupData()
{
    if (KMessageBox::warningContinueCancel(this, i18n("Please quit all kdepim applications before backing up your data."), i18n("Backup"))
        == KMessageBox::Cancel) {
        return;
    }
    backupData();
}

void PimDataExporterWindow::backupData(const QString &filename, const QString &templateFile)
{
    QString currentFileName = filename;
    QPointer<SelectionTypeDialog> dialog = new SelectionTypeDialog(true, this);
    dialog->loadTemplate(templateFile);
    if (dialog->exec()) {
        mLogWidget->clear();
        initializeBackupRestoreUi();
        mPimDataBackupRestoreUI->setStoredParameters(dialog->storedType());
        mPimDataBackupRestoreUI->setExportedInfoFileName(dialog->exportedFileInfo());
        delete dialog;

        if (currentFileName.isEmpty()) {
            QString recentDirClass;
            currentFileName =
                QFileDialog::getSaveFileName(this,
                                             i18n("Create backup"),
                                             KFileWidget::getStartUrl(QUrl(QStringLiteral("kfiledialog:///pimsettingexporter")), recentDirClass).toLocalFile(),
                                             i18n("Zip file (*.zip)"));
            if (currentFileName.isEmpty()) {
                return;
            }
            if (!recentDirClass.isEmpty()) {
                KRecentDirs::add(recentDirClass, currentFileName);
            }
            mRecentFilesMenu->addUrl(QUrl::fromLocalFile(currentFileName));
        }
        mTrayIcon->setStatus(KStatusNotifierItem::Active);
        mTrayIcon->setToolTipSubTitle(i18n("Backup in progress..."));
        if (!mPimDataBackupRestoreUI->backupStart(currentFileName)) {
            qCDebug(PIMDATAEXPORTERGUI_LOG) << " backup Start failed";
        }
        mLastArchiveFileName = currentFileName;
    } else {
        delete dialog;
    }
}

void PimDataExporterWindow::slotAddInfo(const QString &info)
{
    mLogWidget->addInfoLogEntry(info);
    qApp->processEvents();
}

void PimDataExporterWindow::slotAddError(const QString &info)
{
    mLogWidget->addErrorLogEntry(info);
    qApp->processEvents();
}

void PimDataExporterWindow::slotAddTitle(const QString &info)
{
    mLogWidget->addTitleLogEntry(info);
    qApp->processEvents();
}

void PimDataExporterWindow::slotAddEndLine()
{
    mLogWidget->addEndLineLogEntry();
    qApp->processEvents();
}

void PimDataExporterWindow::slotRestoreData()
{
    loadData();
}

void PimDataExporterWindow::loadData(const QString &filename, const QString &templateFile)
{
    const int answer = KMessageBox::warningTwoActions(this,
                                                      i18n("Before restoring data you must close all kdepim applications. Do you want to continue?"),
                                                      i18n("Backup"),
                                                      KStandardGuiItem::cont(),
                                                      KStandardGuiItem::cancel());
    if (answer == KMessageBox::ButtonCode::SecondaryAction) {
        return;
    }

    // First select filename.
    QString currentFileName = filename;
    if (currentFileName.isEmpty()) {
        QString recentDirClass;
        currentFileName =
            QFileDialog::getOpenFileName(this,
                                         i18n("Restore backup"),
                                         KFileWidget::getStartUrl(QUrl(QStringLiteral("kfiledialog:///pimdataexporter")), recentDirClass).toLocalFile(),
                                         QStringLiteral("%1 (*.zip)").arg(i18n("Zip File")));
        if (currentFileName.isEmpty()) {
            return;
        }
        if (!recentDirClass.isEmpty()) {
            KRecentDirs::add(recentDirClass, currentFileName);
        }
    }
    // Don't put it in 'if' otherwise temporary file will be removed after that.
    QString templateFileName;
    PimDataImportDataInfoFile dataInfo;
    bool cleanupItems = false;
    if (templateFile.isEmpty()) {
        dataInfo.setCurrentFileName(currentFileName);
        templateFileName = dataInfo.importDataInfoPath();
        cleanupItems = true;
    } else {
        templateFileName = templateFile;
    }
    QPointer<SelectionTypeDialog> dialog = new SelectionTypeDialog(false, this);
    dialog->loadTemplate(templateFileName);
    if (cleanupItems) {
        dialog->removeNotSelectedItems();
    }
    if (dialog->exec()) {
        mLogWidget->clear();
        mNeedToSyncResources.clear();
        initializeBackupRestoreUi();
        mPimDataBackupRestoreUI->setStoredParameters(dialog->storedType());
        delete dialog;

        mTrayIcon->setStatus(KStatusNotifierItem::Active);
        mTrayIcon->setToolTipSubTitle(i18n("Restore in progress..."));
        if (!mPimDataBackupRestoreUI->restoreStart(currentFileName)) {
            qCDebug(PIMDATAEXPORTERGUI_LOG) << " PimDataExporterWindow restore failed";
        }
    } else {
        delete dialog;
    }
}

void PimDataExporterWindow::slotShowStructureInfos()
{
    BackupFileStructureInfoDialog dlg(this);
    dlg.exec();
}

void PimDataExporterWindow::slotShowCurrentArchiveInformations()
{
    if (!mLastArchiveFileName.isEmpty()) {
        ShowArchiveStructureDialog dlg(mLastArchiveFileName, this);
        dlg.exec();
    }
}
