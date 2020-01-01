/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "pimdataexporterwindow.h"
#include "dialog/showarchivestructuredialog.h"
#include "importexportprogressindicatorgui.h"
#include "widgets/logwidget.h"
#include "pimdataexportgui_debug.h"
#include "job/fullsynchronizeresourcesjob.h"
#include "trayicon/pimdatatrayicon.h"

#include "pimdataimportdatainfofile.h"
#include "pimdataexporterkernel.h"
#include "dialog/selectiontypedialog.h"
#include "utils.h"
#include "pimdatabackuprestoreui.h"
#include "dialog/synchronizeresourcedialog.h"

#include "dialog/backupfilestructureinfodialog.h"

#include <MailCommon/MailKernel>
#include <MailCommon/FilterManager>

#include "PimCommon/PimUtil"

#include <AkonadiWidgets/ControlGui>

#include <KStandardAction>
#include <KConfigGroup>
#include <KActionCollection>
#include <KMessageBox>
#include <KLocalizedString>
#include <QStatusBar>
#include <KRecentFilesAction>
#include <QPointer>
#include <KSharedConfig>
#include <KFileWidget>
#include <KRecentDirs>
#include <QStandardPaths>
#include <QFileDialog>
#include <QCommandLineParser>

#include <dialog/pimdataexporterconfiguredialog.h>

PimDataExporterWindow::PimDataExporterWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    //Initialize filtermanager
    (void)MailCommon::FilterManager::instance();
    PimDataExporterKernel *kernel = new PimDataExporterKernel(this);
    CommonKernel->registerKernelIf(kernel);   //register KernelIf early, it is used by the Filter classes
    CommonKernel->registerSettingsIf(kernel);   //SettingsIf is used in FolderTreeWidget

    setupActions(true);
    setupGUI(Keys | StatusBar | Save | Create, QStringLiteral("pimdataexporter.rc"));
    mLogWidget = new LogWidget(this);

    setCentralWidget(mLogWidget);
    resize(800, 600);
    Akonadi::ControlGui::widgetNeedsAkonadi(this);
    statusBar()->hide();
    mTrayIcon = new PimDataTrayIcon(this);
}

PimDataExporterWindow::~PimDataExporterWindow()
{
    MailCommon::FilterManager::instance()->cleanup();
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup groupConfig = config->group(QStringLiteral("Recent File"));
    mRecentFilesAction->saveEntries(groupConfig);
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
            FullSynchronizeResourcesJob *job = new FullSynchronizeResourcesJob(this);
            job->setWindowParent(this);
            job->setResources(list);
            connect(job, &FullSynchronizeResourcesJob::synchronizeFinished, this, &PimDataExporterWindow::slotFullSyncFinished);
            connect(job, &FullSynchronizeResourcesJob::synchronizeInstanceDone, this, &PimDataExporterWindow::slotFullSyncInstanceDone);
            connect(job, &FullSynchronizeResourcesJob::synchronizeInstanceFailed, this, &PimDataExporterWindow::slotFullSyncInstanceFailed);
            job->start();
        }
    } else {
        slotUpdateActions(false);
    }
}

void PimDataExporterWindow::slotShowBackupFinishDialogInformation()
{
    showFinishInformation();
}

void PimDataExporterWindow::slotFullSyncFinished()
{
    slotUpdateActions(false);
    const QString str = i18n("Full sync finished.");
    slotAddInfo(str);
    mTrayIcon->setStatus(KStatusNotifierItem::Passive);
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
    KMessageBox::information(
        this,
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

    mShowArchiveInformationsAboutCurrentArchiveAction = ac->addAction(QStringLiteral("show_current_archive_info"), this, &PimDataExporterWindow::slotShowCurrentArchiveInformations);
    mShowArchiveInformationsAboutCurrentArchiveAction->setText(i18n("Show Information on current Archive..."));
    mShowArchiveInformationsAboutCurrentArchiveAction->setEnabled(false);

    KStandardAction::quit(this, &PimDataExporterWindow::close, ac);
    mRecentFilesAction = KStandardAction::openRecent(this, &PimDataExporterWindow::slotRestoreFile, ac);

    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup groupConfig = config->group(QStringLiteral("Recent File"));
    mRecentFilesAction->loadEntries(groupConfig);

    KStandardAction::preferences(this, &PimDataExporterWindow::slotConfigure, ac);
}

void PimDataExporterWindow::slotConfigure()
{
    QPointer<PimDataExporterConfigureDialog> dlg = new PimDataExporterConfigureDialog(this);
    dlg->exec();
    delete dlg;
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
        loadData(url.path());
    }
}

void PimDataExporterWindow::slotShowArchiveInformations()
{
    const QString filename = QFileDialog::getOpenFileName(this, i18n("Select Archive"), QString(), QStringLiteral("%1 (*.zip)").arg(i18n("Zip file")));
    if (filename.isEmpty()) {
        return;
    }

    QPointer<ShowArchiveStructureDialog> dlg = new ShowArchiveStructureDialog(filename, this);
    dlg->exec();
    delete dlg;
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
    if (KMessageBox::warningContinueCancel(
            this,
            i18n("Please quit all kdepim applications before backing up your data."),
            i18n("Backup")) == KMessageBox::Cancel) {
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
            currentFileName = QFileDialog::getSaveFileName(this, i18n("Create backup"),
                                                           KFileWidget::getStartUrl(QUrl(QStringLiteral("kfiledialog:///pimsettingexporter")), recentDirClass).toLocalFile(),
                                                           i18n("Zip file (*.zip)"));
            if (currentFileName.isEmpty()) {
                return;
            }
            if (!recentDirClass.isEmpty()) {
                KRecentDirs::add(recentDirClass, currentFileName);
            }
            mRecentFilesAction->addUrl(QUrl::fromLocalFile(currentFileName));
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
    if (KMessageBox::warningYesNo(
            this,
            i18n("Before restoring data you must close all kdepim applications. "
                 "Do you want to continue?"),
            i18n("Backup")) == KMessageBox::No) {
        return;
    }

    //First select filename.
    QString currentFileName = filename;
    if (currentFileName.isEmpty()) {
        QString recentDirClass;
        currentFileName
            = QFileDialog::getOpenFileName(
                  this,
                  i18n("Restore backup"),
                  KFileWidget::getStartUrl(QUrl(QStringLiteral("kfiledialog:///pimdataexporter")),
                                           recentDirClass).toLocalFile(),
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
    QPointer<BackupFileStructureInfoDialog> dlg = new BackupFileStructureInfoDialog(this);
    dlg->exec();
    delete dlg;
}

void PimDataExporterWindow::slotShowCurrentArchiveInformations()
{
    if (!mLastArchiveFileName.isEmpty()) {
        QPointer<ShowArchiveStructureDialog> dlg = new ShowArchiveStructureDialog(mLastArchiveFileName, this);
        dlg->exec();
        delete dlg;
    }
}
