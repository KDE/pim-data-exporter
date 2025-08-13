/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importaddressbookjobinterface.h"
using namespace Qt::Literals::StringLiterals;

#include "archivestorage.h"

#include <KArchive>
#include <KArchiveEntry>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KZip>

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>

ImportAddressbookJobInterface::ImportAddressbookJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportAddressbookJobInterface::~ImportAddressbookJobInterface() = default;

QString ImportAddressbookJobInterface::configLocation() const
{
    return installConfigLocation();
}

QString ImportAddressbookJobInterface::installConfigLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/';
}

void ImportAddressbookJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import KAddressBook settings..."));
    mArchiveDirectory = archive()->directory();
    createProgressDialog(i18n("Import KAddressBook settings"));
    searchAllFiles(mArchiveDirectory, QString(), u"addressbook"_s);
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

void ImportAddressbookJobInterface::addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig,
                                                                const QString &resourceName,
                                                                QMap<QString, QVariant> &settings)
{
    if (resourceName == QLatin1StringView("akonadi_vcard_resource")) {
        KConfigGroup general = resourceConfig->group(u"General"_s);
        if (general.hasKey(u"DisplayName"_s)) {
            settings.insert(u"DisplayName"_s, general.readEntry(u"DisplayName"_s));
        }
        if (general.hasKey(u"ReadOnly"_s)) {
            settings.insert(u"ReadOnly"_s, general.readEntry(u"ReadOnly"_s, false));
        }
        if (general.hasKey(u"MonitorFile"_s)) {
            settings.insert(u"MonitorFile"_s, general.readEntry(u"MonitorFile"_s, true));
        }
    }
}

bool ImportAddressbookJobInterface::isAConfigFile(const QString &name) const
{
    return name.endsWith(QLatin1StringView("rc"))
        && (name.startsWith(QLatin1StringView("akonadi_vcarddir_resource_")) || name.startsWith(QLatin1StringView("akonadi_vcard_resource_"))
            || name.startsWith(QLatin1StringView("akonadi_contacts_resource_")));
}

void ImportAddressbookJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString kaddressbookStr(u"kaddressbookrc"_s);
    const KArchiveEntry *kaddressbookrcentry = mArchiveDirectory->entry(Utils::configsPath() + kaddressbookStr);
    if (kaddressbookrcentry && kaddressbookrcentry->isFile()) {
        const auto kaddressbookrcFile = static_cast<const KArchiveFile *>(kaddressbookrcentry);
        const QString searchExistingFilerc = configLocation() + kaddressbookStr;
        const QString installPathFilerc = installConfigLocation() + kaddressbookStr;

        if (QFileInfo::exists(searchExistingFilerc)) {
            if (overwriteConfigMessageBox(kaddressbookStr)) {
                importkaddressBookConfig(kaddressbookrcFile, installPathFilerc, kaddressbookStr, Utils::configsPath());
            } // Else merge!
        } else {
            importkaddressBookConfig(kaddressbookrcFile, installPathFilerc, kaddressbookStr, Utils::configsPath());
        }
    }
    restoreUiRcFile(u"kaddressbookui.rc"_s, u"kaddressbook"_s);
    emitInfo(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportAddressbookJobInterface::slotNextStep);
}

void ImportAddressbookJobInterface::importkaddressBookConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr kaddressBookConfig = KSharedConfig::openConfig(config);

    convertCollectionListStrToAkonadiId(kaddressBookConfig, u"CollectionViewCheckState"_s, u"Selection"_s, true);

    const QString collectionViewStateStr(u"CollectionViewState"_s);
    if (kaddressBookConfig->hasGroup(collectionViewStateStr)) {
        KConfigGroup group = kaddressBookConfig->group(collectionViewStateStr);
        QString currentKey(u"Current"_s);
        convertRealPathToCollection(group, currentKey, true);

        currentKey = u"Expansion"_s;
        convertRealPathToCollectionList(group, currentKey, true);

        currentKey = u"Selection"_s;
        convertRealPathToCollection(group, currentKey, true);
    }
    const QString cvsTemplateDirName = u"/kaddressbook/csv-templates/"_s;
    const KArchiveEntry *csvtemplateEntry = mArchiveDirectory->entry(Utils::dataPath() + cvsTemplateDirName);
    if (csvtemplateEntry && csvtemplateEntry->isDirectory()) {
        const auto csvTemplateDir = static_cast<const KArchiveDirectory *>(csvtemplateEntry);
        const QStringList lst = csvTemplateDir->entries();
        for (const QString &entryName : lst) {
            const KArchiveEntry *entry = csvTemplateDir->entry(entryName);
            if (entry && entry->isFile()) {
                const auto csvTemplateFile = static_cast<const KArchiveFile *>(entry);
                const QString name = csvTemplateFile->name();
                QString autocorrectionPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + cvsTemplateDirName;
                if (QFileInfo::exists(autocorrectionPath)) {
                    if (overwriteConfigMessageBox(name)) {
                        copyToFile(csvTemplateFile, autocorrectionPath + u'/' + name, name, Utils::dataPath() + cvsTemplateDirName);
                    }
                } else {
                    autocorrectionPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + cvsTemplateDirName;
                    copyToFile(csvTemplateFile, autocorrectionPath + u'/' + name, name, Utils::dataPath() + cvsTemplateDirName);
                }
            }
        }
    }

    importDataSubdirectory(u"/kaddressbook/viewertemplates/"_s);
    importDataSubdirectory(u"/kaddressbook/printing/"_s);
    kaddressBookConfig->sync();
}

void ImportAddressbookJobInterface::restoreResources()
{
    emitInfo(i18n("Restore resources..."));
    setProgressDialogLabel(i18n("Restore resources..."));
    increaseProgressDialog();
    QStringList listResource;
    listResource << restoreResourceFile(u"akonadi_vcard_resource"_s, Utils::addressbookPath(), Utils::storeAddressbook());

    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(Utils::addressbookPath());
        const QString copyToDirName(mTempDirName + u'/' + Utils::addressbookPath());
        QDir().mkpath(copyToDirName);
        const int numberOfResourceFile = mListResourceFile.size();
        for (int i = 0; i < numberOfResourceFile; ++i) {
            ResourceFiles value = mListResourceFile.at(i);
            if (value.akonadiConfigFile.contains(QLatin1StringView("akonadi_vcarddir_resource_"))
                || value.akonadiConfigFile.contains(QLatin1StringView("akonadi_contacts_resource_"))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                    const auto file = static_cast<const KArchiveFile *>(fileResouceEntry);
                    copyArchiveFileTo(file, copyToDirName);
                    QString resourceName(file->name());

                    QString filename(resourceName);
                    KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + u'/' + resourceName);

                    // TODO fix default path ????? backupaddressbook ???
                    const QString newUrl = adaptResourcePath(resourceConfig, Utils::storeAddressbook());
                    QFileInfo newUrlInfo(newUrl);
                    const QString dataFile = value.akonadiResources;
                    const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                    if (dataResouceEntry->isFile()) {
                        const auto fileEntry = static_cast<const KArchiveFile *>(dataResouceEntry);
                        // TODO  adapt directory name too
                        extractZipFile(fileEntry,
                                       copyToDirName,
                                       newUrlInfo.path(),
                                       value.akonadiConfigFile.contains(QLatin1StringView("akonadi_contacts_resource_")));
                    }
                    QMap<QString, QVariant> settings;
                    settings.insert(u"Path"_s, newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                            const auto akonadiAgentConfigEntryFile = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                            copyArchiveFileTo(akonadiAgentConfigEntryFile, copyToDirName);
                            resourceName = akonadiAgentConfigEntryFile->name();
                            filename = Utils::akonadiAgentName(copyToDirName + u'/' + resourceName);
                        }
                    }
                    QString instanceType;
                    if (value.akonadiConfigFile.contains(QLatin1StringView("akonadi_vcarddir_resource_"))) {
                        instanceType = u"akonadi_vcarddir_resource"_s;
                    } else if (value.akonadiConfigFile.contains(QLatin1StringView("akonadi_contacts_resource_"))) {
                        instanceType = u"akonadi_contacts_resource"_s;
                    } else {
                        qCWarning(PIMDATAEXPORTERCORE_LOG) << " not supported" << value.akonadiConfigFile;
                    }

                    const QString newResource = createResource(instanceType, filename, settings, true);
                    infoAboutNewResource(newResource);
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " newResource" << newResource;
                    listResource << newResource;
                }
            }
        }
    }

    emitInfo(i18n("Resources restored."));
    // It's maildir support. Need to add support
    synchronizeResource(listResource);
}

QString ImportAddressbookJobInterface::applicationName() const
{
    return u"[KAddressBook]"_s;
}

#include "moc_importaddressbookjobinterface.cpp"
