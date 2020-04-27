/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#include "importaddressbookjobinterface.h"
#include "archivestorage.h"

#include <KLocalizedString>
#include <KConfigGroup>
#include <KZip>
#include <KArchiveEntry>

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QStandardPaths>


ImportAddressbookJobInterface::ImportAddressbookJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportAddressbookJobInterface::~ImportAddressbookJobInterface()
{
}

void ImportAddressbookJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import KAddressBook settings..."));
    mArchiveDirectory = archive()->directory();
    createProgressDialog(i18n("Import KAddressBook settings"));
    searchAllFiles(mArchiveDirectory, QString(), QStringLiteral("addressbook"));
    initializeListStep();
    QTimer::singleShot(0, this, &ImportAddressbookJobInterface::slotNextStep);
}

void ImportAddressbookJobInterface::slotNextStep()
{
    ++mIndex;
    if (mIndex < mListStep.count()) {
        const Utils::StoredType type = mListStep.at(mIndex);
        if (type == Utils::Resources) {
            restoreResources();
        } else if (type == Utils::Config) {
            restoreConfig();
        }
    } else {
        Q_EMIT jobFinished();
    }
}

void ImportAddressbookJobInterface::addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings)
{
    if (resourceName == QLatin1String("akonadi_vcard_resource")) {
        KConfigGroup general = resourceConfig->group(QStringLiteral("General"));
        if (general.hasKey(QStringLiteral("DisplayName"))) {
            settings.insert(QStringLiteral("DisplayName"), general.readEntry(QStringLiteral("DisplayName")));
        }
        if (general.hasKey(QStringLiteral("ReadOnly"))) {
            settings.insert(QStringLiteral("ReadOnly"), general.readEntry(QStringLiteral("ReadOnly"), false));
        }
        if (general.hasKey(QStringLiteral("MonitorFile"))) {
            settings.insert(QStringLiteral("MonitorFile"), general.readEntry(QStringLiteral("MonitorFile"), true));
        }
    }
}

bool ImportAddressbookJobInterface::isAConfigFile(const QString &name) const
{
    return name.endsWith(QLatin1String("rc")) && (name.startsWith(QLatin1String("akonadi_vcarddir_resource_"))
                                                  || name.startsWith(QLatin1String("akonadi_vcard_resource_"))
                                                  || name.startsWith(QLatin1String("akonadi_contacts_resource_")));
}

void ImportAddressbookJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString kaddressbookStr(QStringLiteral("kaddressbookrc"));
    const KArchiveEntry *kaddressbookrcentry = mArchiveDirectory->entry(Utils::configsPath() + kaddressbookStr);
    if (kaddressbookrcentry && kaddressbookrcentry->isFile()) {
        const KArchiveFile *kaddressbookrcFile = static_cast<const KArchiveFile *>(kaddressbookrcentry);
        const QString kaddressbookrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + kaddressbookStr;
        if (QFileInfo::exists(kaddressbookrc)) {
            if (overwriteConfigMessageBox(kaddressbookStr)) {
                importkaddressBookConfig(kaddressbookrcFile, kaddressbookrc, kaddressbookStr, Utils::configsPath());
            }
        } else {
            importkaddressBookConfig(kaddressbookrcFile, kaddressbookrc, kaddressbookStr, Utils::configsPath());
        }
    }
    restoreUiRcFile(QStringLiteral("kaddressbookui.rc"), QStringLiteral("kaddressbook"));
    Q_EMIT info(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportAddressbookJobInterface::slotNextStep);
}

void ImportAddressbookJobInterface::importkaddressBookConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr kaddressBookConfig = KSharedConfig::openConfig(config);

    convertCollectionListStrToAkonadiId(kaddressBookConfig, QStringLiteral("CollectionViewCheckState"), QStringLiteral("Selection"), true);

    const QString collectionViewStateStr(QStringLiteral("CollectionViewState"));
    if (kaddressBookConfig->hasGroup(collectionViewStateStr)) {
        KConfigGroup group = kaddressBookConfig->group(collectionViewStateStr);
        QString currentKey(QStringLiteral("Current"));
        convertRealPathToCollection(group, currentKey, true);

        currentKey = QStringLiteral("Expansion");
        convertRealPathToCollection(group, currentKey, true);

        currentKey = QStringLiteral("Selection");
        convertRealPathToCollection(group, currentKey, true);
    }
    const QString cvsTemplateDirName = QStringLiteral("/kaddressbook/csv-templates/");
    const KArchiveEntry *csvtemplateEntry = mArchiveDirectory->entry(Utils::dataPath() + cvsTemplateDirName);
    if (csvtemplateEntry && csvtemplateEntry->isDirectory()) {
        const KArchiveDirectory *csvTemplateDir = static_cast<const KArchiveDirectory *>(csvtemplateEntry);
        const QStringList lst = csvTemplateDir->entries();
        for (const QString &entryName : lst) {
            const KArchiveEntry *entry = csvTemplateDir->entry(entryName);
            if (entry && entry->isFile()) {
                const KArchiveFile *csvTemplateFile = static_cast<const KArchiveFile *>(entry);
                const QString name = csvTemplateFile->name();
                QString autocorrectionPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + cvsTemplateDirName;
                if (QFileInfo::exists(autocorrectionPath)) {
                    if (overwriteConfigMessageBox(name)) {
                        copyToFile(csvTemplateFile, autocorrectionPath + QLatin1Char('/') + name, name, Utils::dataPath() + cvsTemplateDirName);
                    }
                } else {
                    autocorrectionPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + cvsTemplateDirName;
                    copyToFile(csvTemplateFile, autocorrectionPath + QLatin1Char('/') + name, name, Utils::dataPath() + cvsTemplateDirName);
                }
            }
        }
    }

    importDataSubdirectory(QStringLiteral("/kaddressbook/viewertemplates/"));
    importDataSubdirectory(QStringLiteral("/kaddressbook/printing/"));
    kaddressBookConfig->sync();
}
