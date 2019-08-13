/*
   Copyright (C) 2013-2019 Montel Laurent <montel@kde.org>

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

#include "showarchivestructuredialog.h"
#include "core/utils.h"
#include "pimdataexportgui_debug.h"
#include "PimCommon/PimUtil"
#include <QDialog>
#include <KLocalizedString>
#include <KZip>
#include <KMessageBox>

#include <QTreeWidget>
#include <QHeaderView>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <QPushButton>
#include <QVBoxLayout>

#include <QFileDialog>
#include <ktreewidgetsearchline.h>

ShowArchiveStructureDialog::ShowArchiveStructureDialog(const QString &filename, QWidget *parent)
    : QDialog(parent)
    , mFileName(filename)
{
    setWindowTitle(i18n("Show Archive Content on file \"%1\"", filename));
    setModal(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mTreeWidget = new QTreeWidget(this);
    mTreeWidget->header()->hide();
    mTreeWidget->setAlternatingRowColors(true);

    KTreeWidgetSearchLine *searchLine = new KTreeWidgetSearchLine(this, mTreeWidget);
    searchLine->setPlaceholderText(i18n("Search..."));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    QPushButton *user1Button = new QPushButton(this);
    user1Button->setText(i18n("Save As Text..."));
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);

    connect(buttonBox, &QDialogButtonBox::rejected, this, &ShowArchiveStructureDialog::reject);

    mExtractFile = new QPushButton(this);
    mExtractFile->setText(i18n("Extract Selected File"));
    mExtractFile->setEnabled(false);
    connect(mExtractFile, &QPushButton::clicked, this, &ShowArchiveStructureDialog::slotExtractFile);
    buttonBox->addButton(mExtractFile, QDialogButtonBox::ActionRole);


    mOpenFile = new QPushButton(this);
    mOpenFile->setText(i18n("Open Selected File"));
    mOpenFile->setEnabled(false);
    connect(mOpenFile, &QPushButton::clicked, this, &ShowArchiveStructureDialog::slotOpenFile);
    buttonBox->addButton(mOpenFile, QDialogButtonBox::ActionRole);

    connect(mTreeWidget, &QTreeWidget::itemClicked, this, &ShowArchiveStructureDialog::slotItemClicked);

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
                //TODO
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
                const KArchiveFile *currentFile = static_cast<const KArchiveFile *>(currentEntry);
                const QString dir = QFileDialog::getExistingDirectory(this, i18n("Select Directory"),
                                                                      QDir::homePath(),
                                                                      QFileDialog::ShowDirsOnly
                                                                      | QFileDialog::DontResolveSymlinks);
                if (!dir.isEmpty()) {
                    if (QFile(dir + QLatin1Char('/') + currentFile->name()).exists()) {
                        if (KMessageBox::questionYesNo(this, i18n("Do you want to override %1?", currentFile->name()), i18n("File Already Exist") ) == KMessageBox::No) {
                            return;
                        }
                    }
                    if (!currentFile->copyTo(dir)) {
                        KMessageBox::error(this, i18n("Impossible to copy %1 in %2.", currentFile->name()), dir);
                        qCWarning(PIMDATAEXPORTERGUI_LOG) << "Impossible to extract file: " << currentItem->text(0) << " to " << dir;
                    }
                }
            }
        }
    }
}

void ShowArchiveStructureDialog::slotItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (item) {
        const QString fullPath = item->data(0, FullPath).toString();
        const bool enableButton = !fullPath.isEmpty();
        mExtractFile->setEnabled(enableButton);
        mOpenFile->setEnabled(enableButton);
    }
}

void ShowArchiveStructureDialog::slotExportAsLogFile()
{
    PimCommon::Util::saveTextAs(mLogFile, QStringLiteral("%1 (*.txt)").arg(i18nc("qfiledialog filter files text", "Text Files")), this, QUrl(),
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
        searchArchiveElement(Utils::mailsPath(), topDirectory, Utils::appTypeToI18n(Utils::KMail));
        searchArchiveElement(Utils::alarmPath(), topDirectory, Utils::appTypeToI18n(Utils::KAlarm));
        searchArchiveElement(Utils::calendarPath(), topDirectory, Utils::appTypeToI18n(Utils::KOrganizer));
        searchArchiveElement(Utils::addressbookPath(), topDirectory, Utils::appTypeToI18n(Utils::KAddressBook));
        searchArchiveElement(Utils::identitiesPath(), topDirectory, Utils::storedTypeToI18n(Utils::Identity));
        searchArchiveElement(Utils::resourcesPath(), topDirectory, Utils::storedTypeToI18n(Utils::Resources));
        searchArchiveElement(Utils::configsPath(), topDirectory, Utils::storedTypeToI18n(Utils::Config));
        searchArchiveElement(Utils::transportsPath(), topDirectory, Utils::storedTypeToI18n(Utils::MailTransport));
        searchArchiveElement(Utils::dataPath(), topDirectory, Utils::storedTypeToI18n(Utils::Data));
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

void ShowArchiveStructureDialog::addSubItems(const QString &topLevelPath, QTreeWidgetItem *parent, const KArchiveEntry *entry, int indent, const QString &fullpath)
{
    const KArchiveDirectory *dir = static_cast<const KArchiveDirectory *>(entry);
    ++indent;
    const QString space = QString(indent * 2, QLatin1Char(' '));
    const QStringList lst = dir->entries();
    for (const QString &entryName : lst) {
        const KArchiveEntry *entry = dir->entry(entryName);
        if (entry) {
            if (entry->isDirectory()) {
                const KArchiveDirectory *dirEntry = static_cast<const KArchiveDirectory *>(entry);
                QTreeWidgetItem *newTopItem = addItem(parent, dirEntry->name(), QString());
                QFont font(newTopItem->font(0));
                font.setBold(true);
                mLogFile += space + dirEntry->name() + QLatin1Char('\n');
                newTopItem->setFont(0, font);
                addSubItems(topLevelPath, newTopItem, entry, indent, (fullpath.isEmpty() ? QString() : fullpath + QLatin1Char('/')) + dirEntry->name());
            } else if (entry->isFile()) {
                const KArchiveFile *file = static_cast<const KArchiveFile *>(entry);
                const QString fileFullPath = topLevelPath + (fullpath.isEmpty() ? QString() : fullpath + QLatin1Char('/')) + file->name();
                //qDebug() << " fileFullPath " <<fileFullPath;
                addItem(parent, file->name(), fileFullPath);
                mLogFile += space + file->name() + QLatin1Char('\n');
            }
        }
    }
}

QTreeWidgetItem *ShowArchiveStructureDialog::addItem(QTreeWidgetItem *parent, const QString &name, const QString &fillFullPath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, name);
    item->setData(0, FullPath, fillFullPath);
    return item;
}

QTreeWidgetItem *ShowArchiveStructureDialog::addTopItem(const QString &name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);
    item->setText(0, name);
    mTreeWidget->addTopLevelItem(item);
    return item;
}

void ShowArchiveStructureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "ShowArchiveStructureDialog");
    group.writeEntry("Size", size());
}

void ShowArchiveStructureDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "ShowArchiveStructureDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(600, 400));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
