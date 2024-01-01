/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "showarchivestructuredialog.h"
#include "core/utils.h"
#include "pimdataexportgui_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KZip>
#include <PimCommon/PimUtil>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <KIO/JobUiDelegateFactory>
#include <KIO/OpenUrlJob>
#include <KTreeWidgetSearchLine>
#include <KWindowConfig>
#include <QFileDialog>
#include <QTemporaryDir>
#include <QWindow>

namespace
{
static const char myShowArchiveStructureDialogGroupName[] = "ShowArchiveStructureDialog";
}

ShowArchiveStructureDialog::ShowArchiveStructureDialog(const QString &filename, QWidget *parent)
    : QDialog(parent)
    , mFileName(filename)
    , mTreeWidget(new QTreeWidget(this))
    , mExtractFile(new QPushButton(i18n("Extract Selected File"), this))
    , mOpenFile(new QPushButton(i18n("Open Selected File"), this))
{
    setWindowTitle(i18nc("@title:window", "Show Archive Content on file \"%1\"", filename));
    setModal(true);
    auto mainLayout = new QVBoxLayout(this);

    mTreeWidget->header()->hide();
    mTreeWidget->setAlternatingRowColors(true);

    auto searchLine = new KTreeWidgetSearchLine(this, mTreeWidget);
    searchLine->setPlaceholderText(i18n("Search..."));
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    auto user1Button = new QPushButton(this);
    user1Button->setText(i18n("Save As Text..."));
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &ShowArchiveStructureDialog::reject);

    mExtractFile->setEnabled(false);
    connect(mExtractFile, &QPushButton::clicked, this, &ShowArchiveStructureDialog::slotExtractFile);
    buttonBox->addButton(mExtractFile, QDialogButtonBox::ActionRole);

    mOpenFile->setEnabled(false);
    connect(mOpenFile, &QPushButton::clicked, this, &ShowArchiveStructureDialog::slotOpenFile);
    buttonBox->addButton(mOpenFile, QDialogButtonBox::ActionRole);

    connect(mTreeWidget, &QTreeWidget::itemActivated, this, &ShowArchiveStructureDialog::slotItemClicked);

    mainLayout->addWidget(searchLine);
    mainLayout->addWidget(mTreeWidget);
    mainLayout->addWidget(buttonBox);
    const bool result = fillTree();
    if (result) {
        mTreeWidget->expandAll();
        connect(user1Button, &QPushButton::clicked, this, &ShowArchiveStructureDialog::slotExportAsLogFile);
    } else {
        user1Button->setEnabled(false);
    }
    readConfig();
}

ShowArchiveStructureDialog::~ShowArchiveStructureDialog()
{
    writeConfig();
    delete mZip;
    delete mTempDir;
}

void ShowArchiveStructureDialog::slotOpenFile()
{
    QTreeWidgetItem *currentItem = mTreeWidget->currentItem();
    if (currentItem) {
        const QString fullPath = currentItem->data(0, FullPath).toString();
        if (!fullPath.isEmpty()) {
            const KArchiveDirectory *topDirectory = mZip->directory();
            const KArchiveEntry *currentEntry = topDirectory->entry(fullPath);
            if (currentEntry && currentEntry->isFile()) {
                const auto currentFile = static_cast<const KArchiveFile *>(currentEntry);

                if (!mTempDir) {
                    mTempDir = new QTemporaryDir;
                }
                const QString fileName = mTempDir->path() + QLatin1Char('/') + currentItem->text(0);
                QFile f(fileName);
                if (!f.open(QIODevice::WriteOnly)) {
                    qCWarning(PIMDATAEXPORTERGUI_LOG) << "Impossible to extract file: " << currentItem->text(0);
                    return;
                }
                const QByteArray data = currentFile->data();
                if (f.write(data) != data.length()) {
                    qCWarning(PIMDATAEXPORTERGUI_LOG) << "Impossible to copy file: " << currentItem->text(0);
                    return;
                }
                auto job = new KIO::OpenUrlJob(QUrl::fromLocalFile(fileName));
                job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
                job->setRunExecutables(false);
                job->start();
            }
        }
    }
}

void ShowArchiveStructureDialog::slotExtractFile()
{
    QTreeWidgetItem *currentItem = mTreeWidget->currentItem();
    if (currentItem) {
        const QString fullPath = currentItem->data(0, FullPath).toString();
        if (!fullPath.isEmpty()) {
            const KArchiveDirectory *topDirectory = mZip->directory();
            const KArchiveEntry *currentEntry = topDirectory->entry(fullPath);
            if (currentEntry && currentEntry->isFile()) {
                const auto currentFile = static_cast<const KArchiveFile *>(currentEntry);
                const QString dir = QFileDialog::getExistingDirectory(this,
                                                                      i18n("Select Directory"),
                                                                      QDir::homePath(),
                                                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                if (!dir.isEmpty()) {
                    if (QFile(dir + QLatin1Char('/') + currentFile->name()).exists()) {
                        if (KMessageBox::questionTwoActions(this,
                                                            i18n("Do you want to overwrite %1?", currentFile->name()),
                                                            i18n("File Already Exist"),
                                                            KStandardGuiItem::overwrite(),
                                                            KStandardGuiItem::cancel())
                            == KMessageBox::ButtonCode::SecondaryAction) {
                            return;
                        }
                    }
                    if (!currentFile->copyTo(dir)) {
                        KMessageBox::error(this, i18n("Impossible to copy %1 in %2.", currentFile->name(), dir));
                        qCWarning(PIMDATAEXPORTERGUI_LOG) << "Impossible to extract file: " << currentItem->text(0) << " to " << dir;
                    }
                }
            }
        }
    }
}

void ShowArchiveStructureDialog::slotItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item) {
        const QString fullPath = item->data(0, FullPath).toString();
        const bool enableButton = !fullPath.isEmpty();
        mExtractFile->setEnabled(enableButton);
        mOpenFile->setEnabled(enableButton);
    }
}

void ShowArchiveStructureDialog::slotExportAsLogFile()
{
    PimCommon::Util::saveTextAs(mLogFile,
                                QStringLiteral("%1 (*.txt)").arg(i18nc("qfiledialog filter files text", "Text Files")),
                                this,
                                QUrl(),
                                i18nc("@title:window", "Export Log File"));
}

bool ShowArchiveStructureDialog::fillTree()
{
    mZip = new KZip(mFileName);
    bool result = mZip->open(QIODevice::ReadOnly);
    if (!result) {
        KMessageBox::error(this, i18n("Archive cannot be opened in read mode."), i18n("Cannot open archive"));
        delete mZip;
        mZip = nullptr;
        return false;
    }
    const KArchiveDirectory *topDirectory = mZip->directory();
    const bool isAValidArchive = searchArchiveElement(Utils::infoPath(), topDirectory, i18n("Info"));
    if (!isAValidArchive) {
        KMessageBox::error(this, i18n("This is not pim archive."), i18n("Show information"));
        result = false;
    } else {
        (void)searchArchiveElement(Utils::mailsPath(), topDirectory, Utils::appTypeToI18n(Utils::KMail));
        (void)searchArchiveElement(Utils::alarmPath(), topDirectory, Utils::appTypeToI18n(Utils::KAlarm));
        (void)searchArchiveElement(Utils::calendarPath(), topDirectory, Utils::appTypeToI18n(Utils::KOrganizer));
        (void)searchArchiveElement(Utils::addressbookPath(), topDirectory, Utils::appTypeToI18n(Utils::KAddressBook));
        (void)searchArchiveElement(Utils::identitiesPath(), topDirectory, Utils::storedTypeToI18n(Utils::Identity));
        (void)searchArchiveElement(Utils::resourcesPath(), topDirectory, Utils::storedTypeToI18n(Utils::Resources));
        (void)searchArchiveElement(Utils::configsPath(), topDirectory, Utils::storedTypeToI18n(Utils::Config));
        (void)searchArchiveElement(Utils::transportsPath(), topDirectory, Utils::storedTypeToI18n(Utils::MailTransport));
        (void)searchArchiveElement(Utils::dataPath(), topDirectory, Utils::storedTypeToI18n(Utils::Data));
        (void)searchArchiveElement(Utils::notePath(), topDirectory, Utils::appTypeToI18n(Utils::KNotes));
    }
    return result;
}

bool ShowArchiveStructureDialog::searchArchiveElement(const QString &path, const KArchiveDirectory *topDirectory, const QString &name)
{
    const KArchiveEntry *topEntry = topDirectory->entry(path);
    bool result = true;
    if (topEntry) {
        mLogFile += name + QLatin1Char('\n');
        QTreeWidgetItem *item = addTopItem(name);
        addSubItems(path, item, topEntry, 0);
    } else {
        result = false;
    }
    return result;
}

void ShowArchiveStructureDialog::addSubItems(const QString &topLevelPath,
                                             QTreeWidgetItem *parent,
                                             const KArchiveEntry *entry,
                                             int indent,
                                             const QString &fullpath)
{
    const auto dir = static_cast<const KArchiveDirectory *>(entry);
    ++indent;
    const QString space = QString(indent * 2, QLatin1Char(' '));
    const QStringList lst = dir->entries();
    for (const QString &entryName : lst) {
        const KArchiveEntry *archiveEntry = dir->entry(entryName);
        if (archiveEntry) {
            if (archiveEntry->isDirectory()) {
                const auto dirEntry = static_cast<const KArchiveDirectory *>(archiveEntry);
                QTreeWidgetItem *newTopItem = addItem(parent, dirEntry->name(), QString());
                QFont font(newTopItem->font(0));
                font.setBold(true);
                mLogFile += space + dirEntry->name() + QLatin1Char('\n');
                newTopItem->setFont(0, font);
                addSubItems(topLevelPath, newTopItem, archiveEntry, indent, (fullpath.isEmpty() ? QString() : fullpath + QLatin1Char('/')) + dirEntry->name());
            } else if (archiveEntry->isFile()) {
                const auto file = static_cast<const KArchiveFile *>(archiveEntry);
                const QString fileFullPath = topLevelPath + (fullpath.isEmpty() ? QString() : fullpath + QLatin1Char('/')) + file->name();
                // qDebug() << " fileFullPath " <<fileFullPath;
                addItem(parent, file->name(), fileFullPath);
                mLogFile += space + file->name() + QLatin1Char('\n');
            }
        }
    }
}

QTreeWidgetItem *ShowArchiveStructureDialog::addItem(QTreeWidgetItem *parent, const QString &name, const QString &fillFullPath)
{
    auto item = new QTreeWidgetItem(parent);
    item->setText(0, name);
    item->setData(0, FullPath, fillFullPath);
    return item;
}

QTreeWidgetItem *ShowArchiveStructureDialog::addTopItem(const QString &name)
{
    auto item = new QTreeWidgetItem;
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);
    item->setText(0, name);
    mTreeWidget->addTopLevelItem(item);
    return item;
}

void ShowArchiveStructureDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myShowArchiveStructureDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowArchiveStructureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myShowArchiveStructureDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_showarchivestructuredialog.cpp"
