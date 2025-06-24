/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailjobinterface.h"
#include "archivestorage.h"
#include "importexportmailutil.h"
#include "pimdataexportcore_debug.h"
#include "resourceconverterimpl.h"
#include "smtpmailtransport.h"

#include <MailCommon/MailUtil>

#include <MailTransport/TransportManager>

#include <KArchiveFile>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <KZip>

#include <QDir>
#include <QMetaMethod>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTimer>

using namespace Akonadi;
using namespace Qt::Literals::StringLiterals;
ImportMailJobInterface::ImportMailJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportMailJobInterface::~ImportMailJobInterface() = default;

void ImportMailJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import KMail settings..."));
    createProgressDialog(i18n("Import KMail settings"));
    mArchiveDirectory = archive()->directory();
    searchAllMailsFiles(mArchiveDirectory, QString());
    if (!mFileList.isEmpty() || !mListResourceFile.isEmpty()) {
        initializeListStep();
        QTimer::singleShot(0, this, &ImportMailJobInterface::slotNextStep);
    } else {
        Q_EMIT jobFinished();
    }
}

void ImportMailJobInterface::slotNextStep()
{
    ++mIndex;
    if (mIndex < mListStep.count()) {
        const Utils::StoredType type = mListStep.at(mIndex);
        if (type == Utils::MailTransport) {
            restoreTransports();
        } else if (type == Utils::Mails) {
            restoreMails();
        } else if (type == Utils::Resources) {
            restoreResources();
        } else if (type == Utils::Identity) {
            restoreIdentity();
        } else if (type == Utils::Config) {
            restoreConfig();
        } else {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << Q_FUNC_INFO << " not supported type " << type;
            slotNextStep();
        }
    } else {
        // Reapply attribute at the end (after resource syncing)
        if (mListStep.contains(Utils::Mails)) {
            importFolderAttributes();
        } else {
            Q_EMIT jobFinished();
        }
    }
}

void ImportMailJobInterface::searchAllMailsFiles(const KArchiveDirectory *dir, const QString &prefix)
{
    const QStringList lst = dir->entries();
    for (const QString &entryName : lst) {
        const KArchiveEntry *entry = dir->entry(entryName);
        if (entry && entry->isDirectory()) {
            const QString newPrefix = (prefix.isEmpty() ? prefix : prefix + u'/') + entryName;
            if (entryName == QLatin1StringView("mails")) {
                storeMailArchiveResource(static_cast<const KArchiveDirectory *>(entry), entryName);
            } else {
                searchAllMailsFiles(static_cast<const KArchiveDirectory *>(entry), newPrefix);
            }
        } else if (entry) {
            const QString fileName = prefix.isEmpty() ? entry->name() : prefix + u'/' + entry->name();
            mFileList << fileName;
        }
    }
    std::sort(mFileList.begin(), mFileList.end());
}

void ImportMailJobInterface::storeMailArchiveResource(const KArchiveDirectory *dir, const QString &prefix)
{
    const QStringList lst = dir->entries();
    for (const QString &entryName : lst) {
        const KArchiveEntry *entry = dir->entry(entryName);
        if (entry && entry->isDirectory()) {
            const auto resourceDir = static_cast<const KArchiveDirectory *>(entry);
            const QStringList lstResourceDirEntries = resourceDir->entries();
            if (lstResourceDirEntries.count() >= 2) {
                const QString archPath(prefix + u'/' + entryName + u'/');
                ResourceFiles files;
                for (const QString &name : lstResourceDirEntries) {
                    if (name.endsWith(QLatin1StringView("rc"))
                        && (name.contains(QLatin1StringView("akonadi_mbox_resource_")) || name.contains(QLatin1StringView("akonadi_mixedmaildir_resource_"))
                            || name.contains(QLatin1StringView("akonadi_maildir_resource_")))) {
                        files.akonadiConfigFile = archPath + name;
                    } else if (name.startsWith(Utils::prefixAkonadiConfigFile())) {
                        files.akonadiAgentConfigFile = archPath + name;
                    } else {
                        files.akonadiResources = archPath + name;
                    }
                }
                // Show debug:
                files.debug();
                mListResourceFile.append(files);
            } else {
                qCDebug(PIMDATAEXPORTERCORE_LOG) << " Problem in archive. number of file " << lstResourceDirEntries.count();
            }
        }
    }
    std::sort(mListResourceFile.begin(), mListResourceFile.end());
}

void ImportMailJobInterface::importMailTransport(const QString &tempDirName)
{
    KSharedConfig::Ptr transportConfig = KSharedConfig::openConfig(tempDirName + u'/' + u"mailtransports"_s);

    int defaultTransport = -1;
    if (transportConfig->hasGroup(u"General"_s)) {
        KConfigGroup group = transportConfig->group(u"General"_s);
        defaultTransport = group.readEntry(u"default-transport"_s, -1);
    }

    QStringList transportList = transportConfig->groupList().filter(QRegularExpression(u"Transport \\d+"_s));
    std::sort(transportList.begin(), transportList.end());
    for (const QString &transport : std::as_const(transportList)) {
        KConfigGroup group = transportConfig->group(transport);
        const int transportId = group.readEntry(u"id"_s, -1);
        if (transportId == -1) {
            qCWarning(PIMDATAEXPORTERCORE_LOG) << "Mail Transport is incorrect. Missing Id";
            continue;
        }
        const QString identifierStr(u"identifier"_s);
        if (group.hasKey(identifierStr)) {
            const QString identifierValue = group.readEntry(identifierStr);
            importCustomMailTransport(identifierValue, group, defaultTransport, transportId);
        } else {
            SmtpMailTransport smtpMailTransport(group);
            importSmtpMailTransport(smtpMailTransport, defaultTransport, transportId);
        }
    }
}

void ImportMailJobInterface::restoreTransports()
{
    setProgressDialogLabel(i18n("Restore transports..."));
    increaseProgressDialog();
    const QString path = Utils::transportsPath() + u"mailtransports"_s;
    if (!mFileList.contains(path)) {
        emitError(i18n("mailtransports file could not be found in the archive."));
    } else {
        emitInfo(i18n("Restore transports..."));

        const KArchiveEntry *transport = mArchiveDirectory->entry(path);
        bool importDone = false;
        if (transport && transport->isFile()) {
            const auto fileTransport = static_cast<const KArchiveFile *>(transport);

            importDone = copyArchiveFileTo(fileTransport, mTempDirName);
            if (importDone) {
                importMailTransport(mTempDirName);
            }
        }
        if (importDone) {
            emitInfo(i18n("Transports restored."));
        } else {
            emitError(i18n("Failed to restore transports file."));
        }
    }
    QTimer::singleShot(0, this, &ImportMailJobInterface::slotNextStep);
}

void ImportMailJobInterface::restoreResources()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore resources..."));
    emitInfo(i18n("Restore resources..."));

    QDir dir(mTempDirName);
    dir.mkdir(Utils::resourcesPath());
    ResourceConverterImpl converter;
    QStringList listResourceToSync;
    for (const QString &filename : std::as_const(mFileList)) {
        // We need to find akonadi_* and agent_config_akonadi_*
        if (filename.startsWith(Utils::resourcesPath() + u"akonadi_"_s)) {
            const QString agentFileConfigName = converter.agentFileName(filename);
            QString resourceName;
            if (mFileList.contains(agentFileConfigName)) {
                // Parse config file => get name
                const KArchiveEntry *agentFileConfigEntry = mArchiveDirectory->entry(agentFileConfigName);
                if (agentFileConfigEntry && agentFileConfigEntry->isFile()) {
                    const auto file = static_cast<const KArchiveFile *>(agentFileConfigEntry);
                    const QString destDirectory = mTempDirName + u'/' + Utils::resourcesPath();
                    // qDebug() << " destDirectory " << destDirectory;
                    copyArchiveFileTo(file, destDirectory);
                    const QString filename(file->name());
                    const QString agentResourceFileName = destDirectory + u'/' + filename;
                    resourceName = Utils::akonadiAgentName(agentResourceFileName);
                }
            }
            const KArchiveEntry *fileEntry = mArchiveDirectory->entry(filename);
            if (fileEntry && fileEntry->isFile()) {
                const auto file = static_cast<const KArchiveFile *>(fileEntry);
                const QString destDirectory = mTempDirName + u'/' + Utils::resourcesPath();

                copyArchiveFileTo(file, destDirectory);

                const QString filename(file->name());
                const QString resourceFileName = destDirectory + u'/' + filename;
                KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(resourceFileName);
                QMap<QString, QVariant> settings;
                if (filename.contains(QLatin1StringView("pop3"))) {
                    KConfigGroup general = resourceConfig->group(u"General"_s);
                    if (general.hasKey(u"login"_s)) {
                        settings.insert(u"Login"_s, general.readEntry("login"));
                    }
                    if (general.hasKey(u"host"_s)) {
                        settings.insert(u"Host"_s, general.readEntry("host"));
                    }
                    if (general.hasKey(u"port"_s)) {
                        settings.insert(u"Port"_s, general.readEntry("port", 110));
                    }
                    if (general.hasKey(u"authenticationMethod"_s)) {
                        settings.insert(u"AuthenticationMethod"_s, general.readEntry("authenticationMethod", 7));
                    }
                    if (general.hasKey(u"useSSL"_s)) {
                        settings.insert(u"UseSSL"_s, general.readEntry("useSSL", false));
                    }
                    if (general.hasKey(u"useTLS"_s)) {
                        settings.insert(u"UseTLS"_s, general.readEntry("useTLS", false));
                    }
                    if (general.hasKey(u"pipelining"_s)) {
                        settings.insert(u"Pipelining"_s, general.readEntry("pipelining", false));
                    }
                    if (general.hasKey(u"leaveOnServer"_s)) {
                        settings.insert(u"LeaveOnServer"_s, general.readEntry("leaveOnServer", false));
                    }
                    if (general.hasKey(u"leaveOnServerDays"_s)) {
                        settings.insert(u"LeaveOnServerDays"_s, general.readEntry("leaveOnServerDays", -1));
                    }
                    if (general.hasKey(u"leaveOnServerCount"_s)) {
                        settings.insert(u"LeaveOnServerCount"_s, general.readEntry("leaveOnServerCount", -1));
                    }
                    if (general.hasKey(u"leaveOnServerSize"_s)) {
                        settings.insert(u"LeaveOnServerSize"_s, general.readEntry("leaveOnServerSize", -1));
                    }
                    if (general.hasKey(u"filterOnServer"_s)) {
                        settings.insert(u"FilterOnServer"_s, general.readEntry("filterOnServer", false));
                    }
                    if (general.hasKey(u"filterCheckSize"_s)) {
                        settings.insert(u"FilterCheckSize"_s, general.readEntry("filterCheckSize"));
                    }
                    if (general.hasKey(u"targetCollection"_s)) {
                        const Akonadi::Collection::Id collection = convertPathToId(general.readEntry("targetCollection"));
                        if (collection != -1) {
                            settings.insert(u"TargetCollection"_s, collection);
                        }
                    }
                    if (general.hasKey(u"precommand"_s)) {
                        settings.insert(u"Precommand"_s, general.readEntry("precommand"));
                    }
                    if (general.hasKey(u"intervalCheckEnabled"_s)) {
                        settings.insert(u"IntervalCheckEnabled"_s, general.readEntry("intervalCheckEnabled", false));
                    }
                    if (general.hasKey(u"intervalCheckInterval"_s)) {
                        settings.insert(u"IntervalCheckInterval"_s, general.readEntry("intervalCheckInterval", 5));
                    }

                    KConfigGroup leaveOnserver = resourceConfig->group(u"LeaveOnServer"_s);

                    if (leaveOnserver.hasKey(u"seenUidList"_s)) {
                        settings.insert(u"SeenUidList"_s, leaveOnserver.readEntry("seenUidList", QStringList()));
                    }
#if 0
                    if (leaveOnserver.hasKey(u"seenUidTimeList"_s)) {
                        //FIXME
                        //settings.insert(QLatin1StringView("SeenUidTimeList"),QVariant::fromValue<QList<int> >(leaveOnserver.readEntry("seenUidTimeList",QList<int>())));
                    }
#endif
                    if (leaveOnserver.hasKey(u"downloadLater"_s)) {
                        settings.insert(u"DownloadLater"_s, leaveOnserver.readEntry("downloadLater", QStringList()));
                    }
                    const QString newResourceName = resourceName.isEmpty() ? filename : resourceName;
                    const QString newResource = createResource(u"akonadi_pop3_resource"_s, newResourceName, settings, false);
                    if (!newResource.isEmpty()) {
                        mHashResources.insert(filename, newResource);
                        infoAboutNewResource(newResource);
                        listResourceToSync << newResource;
                    }
                } else if (filename.contains(QLatin1StringView("imap")) || filename.contains(QLatin1StringView("kolab_"))
                           || filename.contains(QLatin1StringView("gmail_"))) {
                    KConfigGroup network = resourceConfig->group(u"network"_s);
                    if (network.hasKey(u"Authentication"_s)) {
                        settings.insert(u"Authentication"_s, network.readEntry("Authentication", 1));
                    }
                    if (network.hasKey(u"ImapPort"_s)) {
                        settings.insert(u"ImapPort"_s, network.readEntry("ImapPort", 993));
                    }
                    if (network.hasKey(u"ImapServer"_s)) {
                        settings.insert(u"ImapServer"_s, network.readEntry("ImapServer"));
                    }
                    if (network.hasKey(u"Safety"_s)) {
                        settings.insert(u"Safety"_s, network.readEntry("Safety", "SSL"));
                    }
                    if (network.hasKey(u"SubscriptionEnabled"_s)) {
                        settings.insert(u"SubscriptionEnabled"_s, network.readEntry("SubscriptionEnabled", false));
                    }
                    if (network.hasKey(u"UserName"_s)) {
                        settings.insert(u"UserName"_s, network.readEntry("UserName"));
                    }

                    if (network.hasKey(u"SessionTimeout"_s)) {
                        settings.insert(u"SessionTimeout"_s, network.readEntry("SessionTimeout", 30));
                    }

                    KConfigGroup cache = resourceConfig->group(u"cache"_s);

                    if (cache.hasKey(u"AccountIdentity"_s)) {
                        const int identity = cache.readEntry("AccountIdentity", -1);
                        if (identity != -1) {
                            if (mHashIdentity.contains(identity)) {
                                settings.insert(u"AccountIdentity"_s, mHashIdentity.value(identity));
                            } else {
                                settings.insert(u"AccountIdentity"_s, identity);
                            }
                        }
                    }
                    if (cache.hasKey(u"IntervalCheckEnabled"_s)) {
                        settings.insert(u"IntervalCheckEnabled"_s, cache.readEntry("IntervalCheckEnabled", true));
                    }
                    if (cache.hasKey(u"RetrieveMetadataOnFolderListing"_s)) {
                        settings.insert(u"RetrieveMetadataOnFolderListing"_s, cache.readEntry("RetrieveMetadataOnFolderListing", true));
                    }
                    if (cache.hasKey(u"AutomaticExpungeEnabled"_s)) {
                        settings.insert(u"AutomaticExpungeEnabled"_s, cache.readEntry("AutomaticExpungeEnabled", true));
                    }
                    if (cache.hasKey(u"DisconnectedModeEnabled"_s)) {
                        settings.insert(u"DisconnectedModeEnabled"_s, cache.readEntry("DisconnectedModeEnabled", false));
                    }
                    if (cache.hasKey(u"IntervalCheckTime"_s)) {
                        settings.insert(u"IntervalCheckTime"_s, cache.readEntry("IntervalCheckTime", -1));
                    }
                    if (cache.hasKey(u"UseDefaultIdentity"_s)) {
                        settings.insert(u"UseDefaultIdentity"_s, cache.readEntry("UseDefaultIdentity", true));
                    }
                    if (cache.hasKey(u"TrashCollection"_s)) {
                        const Akonadi::Collection::Id collection = convertPathToId(cache.readEntry("TrashCollection"));
                        if (collection != -1) {
                            settings.insert(u"TrashCollection"_s, collection);
                        } else {
                            qCDebug(PIMDATAEXPORTERCORE_LOG) << " Use default trash folder";
                        }
                    }

                    KConfigGroup siever = resourceConfig->group(u"siever"_s);
                    if (siever.hasKey(u"SieveSupport"_s)) {
                        settings.insert(u"SieveSupport"_s, siever.readEntry("SieveSupport", false));
                    }
                    if (siever.hasKey(u"SieveReuseConfig"_s)) {
                        settings.insert(u"SieveReuseConfig"_s, siever.readEntry("SieveReuseConfig", true));
                    }
                    if (siever.hasKey(u"SievePort"_s)) {
                        settings.insert(u"SievePort"_s, siever.readEntry("SievePort", 4190));
                    }
                    if (siever.hasKey(u"SieveAlternateUrl"_s)) {
                        settings.insert(u"SieveAlternateUrl"_s, siever.readEntry("SieveAlternateUrl"));
                    }
                    if (siever.hasKey(u"AlternateAuthentication"_s)) {
                        settings.insert(u"AlternateAuthentication"_s, siever.readEntry("AlternateAuthentication"));
                    }
                    if (siever.hasKey(u"SieveVacationFilename"_s)) {
                        settings.insert(u"SieveVacationFilename"_s, siever.readEntry("SieveVacationFilename"));
                    }
                    if (siever.hasKey(u"SieveCustomUsername"_s)) {
                        settings.insert(u"SieveCustomUsername"_s, siever.readEntry("SieveCustomUsername"));
                    }
                    if (siever.hasKey(u"SieveCustomAuthentification"_s)) {
                        settings.insert(u"SieveCustomAuthentification"_s, siever.readEntry("SieveCustomAuthentification"));
                    }

                    QString newResource;
                    const QString newResourceName = resourceName.isEmpty() ? filename : resourceName;
                    if (filename.contains(QLatin1StringView("kolab_"))) {
                        newResource = createResource(u"akonadi_kolab_resource"_s, newResourceName, settings, true);
                    } else if (filename.contains(QLatin1StringView("gmail_"))) {
                        newResource = createResource(u"akonadi_gmail_resource"_s, newResourceName, settings, true);
                    } else {
                        newResource = createResource(u"akonadi_imap_resource"_s, newResourceName, settings, true);
                    }
                    if (!newResource.isEmpty()) {
                        mHashResources.insert(filename, newResource);
                        infoAboutNewResource(newResource);
                        listResourceToSync << newResource;
                    }
                } else {
                    qCWarning(PIMDATAEXPORTERCORE_LOG) << " problem with resource: " << filename;
                }
            }
        }
    }
    emitInfo(i18n("Resources restored."));
    synchronizeResource(listResourceToSync);
}

QString ImportMailJobInterface::applicationName() const
{
    return u"[KMail]"_s;
}

void ImportMailJobInterface::restoreMails()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore mails..."));
    QStringList listResourceToSync;
    emitInfo(i18n("Restore mails..."));

    QDir dir(mTempDirName);
    dir.mkdir(Utils::mailsPath());
    const QString copyToDirName(mTempDirName + u'/' + Utils::mailsPath());
    QDir().mkpath(copyToDirName);
    const int numberOfResourceFile = mListResourceFile.size();
    for (int i = 0; i < numberOfResourceFile; ++i) {
        ResourceFiles value = mListResourceFile.at(i);
        value.debug();
        const QString resourceFile = value.akonadiConfigFile;
        const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(resourceFile);
        if (fileResouceEntry && fileResouceEntry->isFile()) {
            const auto file = static_cast<const KArchiveFile *>(fileResouceEntry);
            copyArchiveFileTo(file, copyToDirName);
            QString resourceName(file->name());
            QString filename(file->name());
            // qCDebug(PIMDATAEXPORTERCORE_LOG)<<" filename "<<filename<<"
            // resourceName"<<resourceName;
            KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + u'/' + resourceName);

            const QString newUrl = adaptResourcePath(resourceConfig, Utils::storeMails());

            const QString agentConfigFile = value.akonadiAgentConfigFile;
            if (!agentConfigFile.isEmpty()) {
                const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                if (akonadiAgentConfigEntry->isFile()) {
                    const auto fileEntry = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                    copyArchiveFileTo(fileEntry, copyToDirName);
                    resourceName = fileEntry->name();
                    filename = Utils::akonadiAgentName(copyToDirName + u'/' + resourceName);
                }
            }

            QMap<QString, QVariant> settings;
            if (resourceName.contains(QLatin1StringView("akonadi_mbox_resource_"))) {
                const QString dataFile = value.akonadiResources;
                const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                if (dataResouceEntry->isFile()) {
                    const auto file = static_cast<const KArchiveFile *>(dataResouceEntry);
                    copyArchiveFileTo(file, newUrl);
                }
                settings.insert(u"Path"_s, newUrl);

                KConfigGroup general = resourceConfig->group(u"General"_s);
                if (general.hasKey(u"DisplayName"_s)) {
                    settings.insert(u"DisplayName"_s, general.readEntry(u"DisplayName"_s));
                }
                if (general.hasKey(u"ReadOnly"_s)) {
                    settings.insert(u"ReadOnly"_s, general.readEntry(u"ReadOnly"_s, false));
                }
                if (general.hasKey(u"MonitorFile"_s)) {
                    settings.insert(u"MonitorFile"_s, general.readEntry(u"MonitorFile"_s, false));
                }
                if (resourceConfig->hasGroup(u"Locking"_s)) {
                    KConfigGroup locking = resourceConfig->group(u"Locking"_s);
                    if (locking.hasKey(u"Lockfile"_s)) {
                        settings.insert(u"Lockfile"_s, locking.readEntry(u"Lockfile"_s));
                    }
                    // TODO verify
                    if (locking.hasKey(u"LockfileMethod"_s)) {
                        settings.insert(u"LockfileMethod"_s, locking.readEntry(u"LockfileMethod"_s, 4));
                    }
                }
                if (resourceConfig->hasGroup(u"Compacting"_s)) {
                    KConfigGroup compacting = resourceConfig->group(u"Compacting"_s);
                    if (compacting.hasKey(u"CompactFrequency"_s)) {
                        settings.insert(u"CompactFrequency"_s, compacting.readEntry(u"CompactFrequency"_s, 1));
                    }
                    if (compacting.hasKey(u"MessageCount"_s)) {
                        settings.insert(u"MessageCount"_s, compacting.readEntry(u"MessageCount"_s, 50));
                    }
                }
                const QString newResource = createResource(u"akonadi_mbox_resource"_s, filename, settings, true);
                if (!newResource.isEmpty()) {
                    mHashResources.insert(filename, newResource);
                    infoAboutNewResource(newResource);
                }
            } else if (resourceName.contains(QLatin1StringView("akonadi_maildir_resource_"))
                       || resourceName.contains(QLatin1StringView("akonadi_mixedmaildir_resource_"))) {
                settings.insert(u"Path"_s, newUrl);
                KConfigGroup general = resourceConfig->group(u"General"_s);
                if (general.hasKey(u"TopLevelIsContainer"_s)) {
                    settings.insert(u"TopLevelIsContainer"_s, general.readEntry(u"TopLevelIsContainer"_s, false));
                }
                if (general.hasKey(u"ReadOnly"_s)) {
                    settings.insert(u"ReadOnly"_s, general.readEntry(u"ReadOnly"_s, false));
                }
                if (general.hasKey(u"MonitorFilesystem"_s)) {
                    settings.insert(u"MonitorFilesystem"_s, general.readEntry(u"MonitorFilesystem"_s, true));
                }

                const QString newResource =
                    createResource(resourceName.contains(QLatin1StringView("akonadi_mixedmaildir_resource_")) ? u"akonadi_mixedmaildir_resource"_s
                                                                                                              : u"akonadi_maildir_resource"_s,
                                   filename,
                                   settings,
                                   true);
                if (!newResource.isEmpty()) {
                    mHashResources.insert(filename, newResource);
                    infoAboutNewResource(newResource);
                }

                const QString mailFile = value.akonadiResources;
                const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(mailFile);
                if (dataResouceEntry && dataResouceEntry->isFile()) {
                    const auto fileEntry = static_cast<const KArchiveFile *>(dataResouceEntry);
                    // TODO Fix me not correct zip filename.
                    extractZipFile(fileEntry, copyToDirName, newUrl);
                }
                listResourceToSync << newResource;
            } else {
                qCDebug(PIMDATAEXPORTERCORE_LOG) << " resource name not supported " << resourceName;
            }
            // qCDebug(PIMDATAEXPORTERCORE_LOG)<<"url "<<url;
        }
    }
    emitInfo(i18n("Mails restored."));
    synchronizeResource(listResourceToSync);
}

void ImportMailJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore config..."));
    const QString filtersPath(Utils::configsPath() + u"filters"_s);
    if (!mFileList.contains(filtersPath)) {
        emitError(i18n("filters file could not be found in the archive."));
    } else {
        const KArchiveEntry *filter = mArchiveDirectory->entry(filtersPath);
        if (filter && filter->isFile()) {
            emitInfo(i18n("Restore filters..."));
            const auto fileFilter = static_cast<const KArchiveFile *>(filter);

            copyArchiveFileTo(fileFilter, mTempDirName);
            const QString filterFileName(mTempDirName + u'/' + u"filters"_s);
            KSharedConfig::Ptr filtersConfig = KSharedConfig::openConfig(filterFileName);
            const QStringList filterList = filtersConfig->groupList().filter(QRegularExpression(u"Filter #\\d+"_s));
            for (const QString &filterStr : filterList) {
                KConfigGroup group = filtersConfig->group(filterStr);
                const QString accountStr(u"accounts-set"_s);
                if (group.hasKey(accountStr)) {
                    const QString accounts = group.readEntry(accountStr);
                    if (!accounts.isEmpty()) {
                        const QStringList lstAccounts = accounts.split(u',');
                        QStringList newLstAccounts;
                        for (const QString &acc : lstAccounts) {
                            if (mHashResources.contains(acc)) {
                                newLstAccounts.append(mHashResources.value(acc));
                            } else {
                                newLstAccounts.append(acc);
                            }
                        }
                        group.writeEntry(accountStr, newLstAccounts);
                    }
                }
                const int numActions = group.readEntry("actions", 0);
                for (int i = 0; i < numActions; ++i) {
                    const QString actName = u"action-name-%1"_s.arg(i);
                    const QString argsName = u"action-args-%1"_s.arg(i);
                    const QString actValue = group.readEntry(actName);
                    if (actValue == QLatin1StringView("set identity")) {
                        const int argsValue = group.readEntry(argsName, -1);
                        if (argsValue != -1) {
                            if (mHashIdentity.contains(argsValue)) {
                                group.writeEntry(argsName, mHashIdentity.value(argsValue));
                            }
                        }
                    } else if (actValue == QLatin1StringView("set transport")) {
                        const int argsValue = group.readEntry(argsName, -1);
                        if (argsValue != -1) {
                            if (mHashTransport.contains(argsValue)) {
                                group.writeEntry(argsName, mHashTransport.value(argsValue));
                            }
                        }
                    }
                }
            }
            filtersConfig->sync();
            importFilters(filterFileName);
            emitInfo(i18n("Filters restored."));
        }
    }
    const QString kmailsnippetrcStr(u"kmailsnippetrc"_s);
    const KArchiveEntry *kmailsnippetentry = mArchiveDirectory->entry(Utils::configsPath() + kmailsnippetrcStr);
    if (kmailsnippetentry && kmailsnippetentry->isFile()) {
        const auto kmailsnippet = static_cast<const KArchiveFile *>(kmailsnippetentry);
        const QString kmailsnippetrc = configLocation() + kmailsnippetrcStr;
        if (QFileInfo::exists(kmailsnippetrc)) {
            // TODO 4.13 allow to merge config.
            if (overwriteConfigMessageBox(kmailsnippetrcStr)) {
                copyToFile(kmailsnippet, kmailsnippetrc, kmailsnippetrcStr, Utils::configsPath());
            }
        } else {
            copyToFile(kmailsnippet, kmailsnippetrc, kmailsnippetrcStr, Utils::configsPath());
        }
    }

    {
        const QString labldaprcStr(u"kabldaprc"_s);
        const KArchiveEntry *kabldapentry = mArchiveDirectory->entry(Utils::configsPath() + labldaprcStr);
        if (kabldapentry && kabldapentry->isFile()) {
            const auto kabldap = static_cast<const KArchiveFile *>(kabldapentry);
            const QString kabldaprc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + labldaprcStr;
            if (QFileInfo::exists(kabldaprc)) {
                const int result = mergeConfigMessageBox(labldaprcStr);
                if (result == KMessageBox::ButtonCode::PrimaryAction) {
                    copyToFile(kabldap, kabldaprc, labldaprcStr, Utils::configsPath());
                } else if (result == KMessageBox::ButtonCode::SecondaryAction) {
                    mergeLdapConfig(kabldap, labldaprcStr, Utils::configsPath());
                }
            } else {
                copyToFile(kabldap, kabldaprc, labldaprcStr, Utils::configsPath());
            }
        }
    }
    {
        const QString archiveconfigurationrcStr(u"akonadi_archivemail_agentrc"_s);
        const KArchiveEntry *archiveconfigurationentry = mArchiveDirectory->entry(Utils::configsPath() + archiveconfigurationrcStr);
        if (archiveconfigurationentry && archiveconfigurationentry->isFile()) {
            const auto archiveconfiguration = static_cast<const KArchiveFile *>(archiveconfigurationentry);
            const QString archiveconfigurationrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + archiveconfigurationrcStr;
            if (QFileInfo::exists(archiveconfigurationrc)) {
                const int result = mergeConfigMessageBox(archiveconfigurationrcStr);
                if (result == KMessageBox::ButtonCode::PrimaryAction) {
                    importArchiveConfig(archiveconfiguration, archiveconfigurationrc, archiveconfigurationrcStr, Utils::configsPath());
                } else if (result == KMessageBox::ButtonCode::SecondaryAction) {
                    mergeArchiveMailAgentConfig(archiveconfiguration, archiveconfigurationrcStr, Utils::configsPath());
                }
            } else {
                importArchiveConfig(archiveconfiguration, archiveconfigurationrc, archiveconfigurationrcStr, Utils::configsPath());
            }
        }
    }

    {
        const QString folderMailArchiveStr(u"foldermailarchiverc"_s);
        const KArchiveEntry *archivemailentry = mArchiveDirectory->entry(Utils::configsPath() + folderMailArchiveStr);
        if (archivemailentry && archivemailentry->isFile()) {
            const auto archiveconfiguration = static_cast<const KArchiveFile *>(archivemailentry);
            const QString archiveconfigurationrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + folderMailArchiveStr;
            if (QFileInfo::exists(archiveconfigurationrc)) {
                const int result = mergeConfigMessageBox(folderMailArchiveStr);
                if (result == KMessageBox::ButtonCode::PrimaryAction) {
                    importMailArchiveConfig(archiveconfiguration, archiveconfigurationrc, folderMailArchiveStr, Utils::configsPath());
                } else if (result == KMessageBox::ButtonCode::SecondaryAction) {
                    mergeMailArchiveConfig(archiveconfiguration, folderMailArchiveStr, Utils::configsPath());
                }
            } else {
                importMailArchiveConfig(archiveconfiguration, archiveconfigurationrc, folderMailArchiveStr, Utils::configsPath());
            }
        }
    }

    {
        const QString unifiedMailBoxStr(u"akonadi_unifiedmailbox_agentrc"_s);
        const KArchiveEntry *unifiedMailBoxEntry = mArchiveDirectory->entry(Utils::configsPath() + unifiedMailBoxStr);
        if (unifiedMailBoxEntry && unifiedMailBoxEntry->isFile()) {
            const auto archiveconfiguration = static_cast<const KArchiveFile *>(unifiedMailBoxEntry);
            const QString unifiedMailBoxrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + unifiedMailBoxStr;
            if (QFileInfo::exists(unifiedMailBoxrc)) {
                const int result = mergeConfigMessageBox(unifiedMailBoxStr);
                if (result == KMessageBox::ButtonCode::PrimaryAction) {
                    importUnifiedMailBoxConfig(archiveconfiguration, unifiedMailBoxrc, unifiedMailBoxStr, Utils::configsPath());
                } else if (result == KMessageBox::ButtonCode::SecondaryAction) {
                    mergeUnifiedMailBoxConfig(archiveconfiguration, unifiedMailBoxStr, Utils::configsPath());
                }
            } else {
                importUnifiedMailBoxConfig(archiveconfiguration, unifiedMailBoxrc, unifiedMailBoxStr, Utils::configsPath());
            }
        }
    }

    {
        const QString templatesconfigurationrcStr(u"templatesconfigurationrc"_s);
        const KArchiveEntry *templatesconfigurationentry = mArchiveDirectory->entry(Utils::configsPath() + templatesconfigurationrcStr);
        if (templatesconfigurationentry && templatesconfigurationentry->isFile()) {
            const auto templatesconfiguration = static_cast<const KArchiveFile *>(templatesconfigurationentry);
            const QString templatesconfigurationrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + templatesconfigurationrcStr;

            if (QFileInfo::exists(templatesconfigurationrc)) {
                // TODO 4.13 allow to merge config.
                if (overwriteConfigMessageBox(templatesconfigurationrcStr)) {
                    importTemplatesConfig(templatesconfiguration, templatesconfigurationrc, templatesconfigurationrcStr, Utils::configsPath());
                }
            } else {
                importTemplatesConfig(templatesconfiguration, templatesconfigurationrc, templatesconfigurationrcStr, Utils::configsPath());
            }
        }
    }
    const QString kmailStr(u"kmail2rc"_s);
    const KArchiveEntry *kmail2rcentry = mArchiveDirectory->entry(Utils::configsPath() + kmailStr);
    if (kmail2rcentry && kmail2rcentry->isFile()) {
        const auto kmailrc = static_cast<const KArchiveFile *>(kmail2rcentry);
        const QString kmail2rc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + kmailStr;
        if (QFileInfo::exists(kmail2rc)) {
            if (overwriteConfigMessageBox(kmailStr)) {
                importKmailConfig(kmailrc, kmail2rc, kmailStr, Utils::configsPath());
            }
        } else {
            importKmailConfig(kmailrc, kmail2rc, kmailStr, Utils::configsPath());
        }
    }

    const QString sievetemplatercStr(u"sievetemplaterc"_s);
    const KArchiveEntry *sievetemplatentry = mArchiveDirectory->entry(Utils::configsPath() + sievetemplatercStr);
    if (sievetemplatentry && sievetemplatentry->isFile()) {
        const auto sievetemplateconfiguration = static_cast<const KArchiveFile *>(sievetemplatentry);
        const QString sievetemplaterc = configLocation() + sievetemplatercStr;
        if (QFileInfo::exists(sievetemplaterc)) {
            const int result = mergeConfigMessageBox(sievetemplatercStr);
            if (result == KMessageBox::ButtonCode::PrimaryAction) {
                copyToFile(sievetemplateconfiguration, sievetemplaterc, sievetemplatercStr, Utils::configsPath());
            } else if (result == KMessageBox::ButtonCode::SecondaryAction) {
                mergeSieveTemplate(sievetemplateconfiguration, sievetemplatercStr, Utils::configsPath());
            }
        } else {
            copyToFile(sievetemplateconfiguration, sievetemplaterc, sievetemplatercStr, Utils::configsPath());
        }
    }
    {
        const QString customTemplateStr(u"customtemplatesrc"_s);
        const KArchiveEntry *customtemplatentry = mArchiveDirectory->entry(Utils::configsPath() + customTemplateStr);
        if (customtemplatentry && customtemplatentry->isFile()) {
            const auto customtemplateconfiguration = static_cast<const KArchiveFile *>(customtemplatentry);
            const QString customtemplaterc = configLocation() + customTemplateStr;
            if (QFileInfo::exists(customtemplaterc)) {
                // TODO 4.13 allow to merge config.
                if (overwriteConfigMessageBox(customTemplateStr)) {
                    copyToFile(customtemplateconfiguration, customtemplaterc, customTemplateStr, Utils::configsPath());
                }
            } else {
                copyToFile(customtemplateconfiguration, customtemplaterc, customTemplateStr, Utils::configsPath());
            }
        }
    }

    {
        const QString adblockStr(u"messagevieweradblockrc"_s);
        const KArchiveEntry *adblockentry = mArchiveDirectory->entry(Utils::configsPath() + adblockStr);
        if (adblockentry && adblockentry->isFile()) {
            const auto adblockconfiguration = static_cast<const KArchiveFile *>(adblockentry);
            const QString adblockrc = configLocation() + adblockStr;
            if (QFileInfo::exists(adblockrc)) {
                // TODO 4.13 allow to merge config.
                if (overwriteConfigMessageBox(adblockStr)) {
                    copyToFile(adblockconfiguration, adblockrc, adblockStr, Utils::configsPath());
                }
            } else {
                copyToFile(adblockconfiguration, adblockrc, adblockStr, Utils::configsPath());
            }
        }
    }

    restoreUiRcFile(u"sieveeditorui.rc"_s, u"sieveeditor"_s);
    restoreUiRcFile(u"headerthemeeditorui.rc"_s, u"headerthemeeditor"_s);
    restoreUiRcFile(u"contactthemeeditorui.rc"_s, u"contactthemeeditor"_s);
    restoreUiRcFile(u"contactprintthemeeditorui.rc"_s, u"contactprintthemeeditor"_s);

    restoreUiRcFile(u"kmreadermainwin.rc"_s, u"kmail2"_s);
    restoreUiRcFile(u"kmcomposerui.rc"_s, u"kmail2"_s);
    restoreUiRcFile(u"kmmainwin.rc"_s, u"kmail2"_s);
    restoreUiRcFile(u"kmail_part.rc"_s, u"kmail2"_s);
    restoreUiRcFile(u"kontactui.rc"_s, u"kontact"_s);
    restoreUiRcFile(u"kleopatra.rc"_s, u"kleopatra"_s);
    restoreUiRcFile(u"kontactsummary_part.rc"_s, u"kontactsummary"_s);
    restoreUiRcFile(u"kwatchgnupgui.rc"_s, u"kwatchgnupg"_s);
    restoreUiRcFile(u"akonadiconsoleui.rc"_s, u"akonadiconsole"_s);

    restoreConfigFile(u"kontactrc"_s);

    restoreConfigFile(u"kontact_summaryrc"_s);
    restoreConfigFile(u"storageservicerc"_s);
    restoreConfigFile(u"kpimbalooblacklist"_s);
    restoreConfigFile(u"kleopatrarc"_s);
    restoreConfigFile(u"sieveeditorrc"_s);
    restoreConfigFile(u"kwatchgnupgrc"_s);
    restoreConfigFile(u"pimpluginsrc"_s);
    restoreConfigFile(u"texttospeechrc"_s);
    restoreConfigFile(u"kleopatracertificateselectiondialogrc"_s);
    restoreConfigFile(u"dkimsettingsrc"_s);
    restoreConfigFile(u"confirmbeforedeletingrc"_s);

    // Restore notify file
    const QStringList lstNotify = {
        u"akonadi_mailfilter_agent.notifyrc"_s,
        u"akonadi_sendlater_agent.notifyrc"_s,
        u"akonadi_archivemail_agent.notifyrc"_s,
        u"kmail2.notifyrc"_s,
        u"akonadi_newmailnotifier_agent.notifyrc"_s,
        u"akonadi_maildispatcher_agent.notifyrc"_s,
        u"akonadi_followupreminder_agent.notifyrc"_s,
        u"messageviewer.notifyrc"_s,
    };

    // We can't merge it.
    for (const QString &filename : lstNotify) {
        restoreConfigFile(filename);
    }

    importSimpleFilesInDirectory(u"/autocorrect/"_s);
    importSimpleFilesInDirectory(u"/gravatar/"_s);
    const KArchiveEntry *kmail2Entry = mArchiveDirectory->entry(Utils::dataPath() + u"kmail2/adblockrules_local"_s);
    if (kmail2Entry && kmail2Entry->isFile()) {
        const auto entry = static_cast<const KArchiveFile *>(kmail2Entry);
        const QString adblockPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u'/' + u"kmail2/adblockrules_local"_s;
        if (QFileInfo::exists(adblockPath)) {
            if (overwriteConfigMessageBox(u"adblockrules_local"_s)) {
                copyToFile(entry,
                           QString(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/"_s + u"kmail2/adblockrules_local"_s),
                           u"adblockrules_local"_s,
                           Utils::dataPath() + u"kmail2/"_s);
            }
        } else {
            copyToFile(entry,
                       QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u'/' + u"kmail2/adblockrules_local"_s,
                       u"adblockrules_local"_s,
                       Utils::dataPath() + u"kmail2/"_s);
        }
    }

    importDataSubdirectory(u"/messageviewer/themes/"_s);
    importDataSubdirectory(u"/messageviewerplugins/"_s);

    emitInfo(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportMailJobInterface::slotNextStep);
}

void ImportMailJobInterface::importSimpleFilesInDirectory(const QString &relativePath)
{
    const KArchiveEntry *autocorrectionEntry = mArchiveDirectory->entry(Utils::dataPath() + relativePath);
    if (autocorrectionEntry && autocorrectionEntry->isDirectory()) {
        const auto autoCorrectionDir = static_cast<const KArchiveDirectory *>(autocorrectionEntry);
        const QStringList lst = autoCorrectionDir->entries();
        for (const QString &entryName : lst) {
            const KArchiveEntry *entry = autoCorrectionDir->entry(entryName);
            if (entry && entry->isFile()) {
                const auto autocorrectionFile = static_cast<const KArchiveFile *>(entry);
                const QString name = autocorrectionFile->name();
                QString autocorrectionPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u'/' + relativePath + u'/' + name;
                if (QFileInfo::exists(autocorrectionPath)) {
                    if (overwriteConfigMessageBox(name)) {
                        copyToFile(autocorrectionFile, autocorrectionPath, name, Utils::dataPath() + relativePath);
                    }
                } else {
                    autocorrectionPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u'/' + relativePath;
                    copyToFile(autocorrectionFile, autocorrectionPath + u'/' + name, name, Utils::dataPath() + relativePath);
                }
            }
        }
    }
}

void ImportMailJobInterface::restoreIdentity()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restoring identities..."));
    const QString path(Utils::identitiesPath() + u"emailidentities"_s);
    if (!mFileList.contains(path)) {
        emitError(i18n("emailidentities file could not be found in the archive."));
    } else {
        emitInfo(i18n("Restoring identities..."));
        const KArchiveEntry *identity = mArchiveDirectory->entry(path);
        if (identity && identity->isFile()) {
            const auto fileIdentity = static_cast<const KArchiveFile *>(identity);
            copyArchiveFileTo(fileIdentity, mTempDirName);
            KSharedConfig::Ptr identityConfig = KSharedConfig::openConfig(mTempDirName + u'/' + u"emailidentities"_s);
            KConfigGroup general = identityConfig->group(u"General"_s);
            const int defaultIdentity = general.readEntry(u"Default Identity"_s, -1);

            QStringList identityList = identityConfig->groupList().filter(QRegularExpression(u"Identity #\\d+"_s));
            std::sort(identityList.begin(), identityList.end());
            for (const QString &identityStr : std::as_const(identityList)) {
                KConfigGroup group = identityConfig->group(identityStr);
                int oldUid = -1;
                const QString uidStr(u"uoid"_s);
                if (group.hasKey(uidStr)) {
                    oldUid = group.readEntry(uidStr).toUInt();
                    group.deleteEntry(uidStr);
                }
                const QString fcc(u"Fcc"_s);
                qint64 fccId = convertRealPathToCollection(group, fcc);
                registerSpecialCollection(Akonadi::SpecialMailCollections::SentMail, fccId);

                const QString draft = u"Drafts"_s;
                qint64 draftId = convertRealPathToCollection(group, draft);
                registerSpecialCollection(Akonadi::SpecialMailCollections::Drafts, draftId);

                const QString templates = u"Templates"_s;
                qint64 templateId = convertRealPathToCollection(group, templates);
                registerSpecialCollection(Akonadi::SpecialMailCollections::Templates, templateId);

                if (oldUid != -1) {
                    const QString vcard = u"VCardFile"_s;
                    if (group.hasKey(vcard)) {
                        const QString vcardFileName = group.readEntry(vcard);
                        if (!vcardFileName.isEmpty()) {
                            const QFileInfo fileInfo(vcardFileName);
                            QFile file(vcardFileName);
                            const KArchiveEntry *vcardEntry =
                                mArchiveDirectory->entry(Utils::identitiesPath() + QString::number(oldUid) + u'/' + file.fileName());
                            if (vcardEntry && vcardEntry->isFile()) {
                                const auto vcardFile = static_cast<const KArchiveFile *>(vcardEntry);
                                QString vcardFilePath =
                                    QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/kmail2/%1"_s.arg(fileInfo.fileName());
                                int i = 1;
                                while (QFileInfo::exists(vcardFileName)) {
                                    vcardFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                                        + u"/kmail2/%1_%2"_s.arg(i).arg(fileInfo.fileName());
                                    ++i;
                                }
                                copyArchiveFileTo(vcardFile, QFileInfo(vcardFilePath).absolutePath());
                                group.writeEntry(vcard, vcardFilePath);
                            }
                        }
                    }
                }
                const QString name = group.readEntry(u"Identity"_s);
                addNewIdentity(name, group, defaultIdentity, oldUid);
            }
            emitInfo(i18n("Identities restored."));
        } else {
            emitError(i18n("Failed to restore identity file."));
        }
    }
    QTimer::singleShot(0, this, &ImportMailJobInterface::slotNextStep);
}

void ImportMailJobInterface::importUnifiedMailBoxConfig(const KArchiveFile *archiveconfiguration,
                                                        const QString &archiveconfigurationrc,
                                                        const QString &filename,
                                                        const QString &prefix)
{
    copyToFile(archiveconfiguration, archiveconfigurationrc, filename, prefix);
    KSharedConfig::Ptr archiveConfig = KSharedConfig::openConfig(archiveconfigurationrc);

    copyUnifiedMailBoxConfig(archiveConfig, archiveConfig);
    archiveConfig->sync();
}

void ImportMailJobInterface::importMailArchiveConfig(const KArchiveFile *archiveconfiguration,
                                                     const QString &archiveconfigurationrc,
                                                     const QString &filename,
                                                     const QString &prefix)
{
    copyToFile(archiveconfiguration, archiveconfigurationrc, filename, prefix);
    KSharedConfig::Ptr archiveConfig = KSharedConfig::openConfig(archiveconfigurationrc);

    copyArchiveMailAgentConfigGroup(archiveConfig, archiveConfig);
    archiveConfig->sync();
}

void ImportMailJobInterface::importArchiveConfig(const KArchiveFile *archiveconfiguration,
                                                 const QString &archiveconfigurationrc,
                                                 const QString &filename,
                                                 const QString &prefix)
{
    copyToFile(archiveconfiguration, archiveconfigurationrc, filename, prefix);
    KSharedConfig::Ptr archiveConfig = KSharedConfig::openConfig(archiveconfigurationrc);

    // TODO fix FolderArchiveAccount
    copyArchiveMailAgentConfigGroup(archiveConfig, archiveConfig);
    archiveConfig->sync();
}

void ImportMailJobInterface::importFolderArchiveConfig(const KArchiveFile *archiveconfiguration,
                                                       const QString &archiveconfigurationrc,
                                                       const QString &filename,
                                                       const QString &prefix)
{
    copyToFile(archiveconfiguration, archiveconfigurationrc, filename, prefix);
    KSharedConfig::Ptr archiveConfig = KSharedConfig::openConfig(archiveconfigurationrc);

    const QStringList archiveList = archiveConfig->groupList().filter(QRegularExpression(u"FolderArchiveAccount "_s));

    for (const QString &str : archiveList) {
        KConfigGroup oldGroup = archiveConfig->group(str);
        const Akonadi::Collection::Id id = convertPathToId(oldGroup.readEntry(u"topLevelCollectionId"_s));
        if (id != -1) {
            oldGroup.writeEntry(u"topLevelCollectionId"_s, id);
        }
    }

    archiveConfig->sync();
}

void ImportMailJobInterface::copyMailArchiveConfig(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination)
{
    const QString archiveGroupPattern = u"FolderArchiveAccount "_s;
    const QStringList archiveList = archiveConfigOrigin->groupList().filter(archiveGroupPattern);
    for (const QString &str : archiveList) {
        const QString resourcename = str.right(str.length() - archiveGroupPattern.length());
        if (!resourcename.isEmpty()) {
            KConfigGroup oldGroup = archiveConfigOrigin->group(str);
            QString newResourceName;
            if (mHashResources.contains(resourcename)) {
                newResourceName = mHashResources.value(resourcename);

                const Akonadi::Collection::Id id = convertPathToId(oldGroup.readEntry(u"topLevelCollectionId"_s));
                if (id != -1) {
                    KConfigGroup newGroup(archiveConfigDestination, archiveGroupPattern + newResourceName);
                    oldGroup.copyTo(&newGroup);
                    newGroup.writeEntry(u"topLevelCollectionId"_s, id);
                }
            }
            oldGroup.deleteGroup();
        }
    }
}

void ImportMailJobInterface::copyUnifiedMailBoxConfig(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination)
{
    auto group = archiveConfigOrigin->group(u"UnifiedMailboxes"_s);
    auto groupCopy = archiveConfigDestination->group(u"UnifiedMailboxes"_s);
    const auto boxGroups = group.groupList();
    for (const auto &str : boxGroups) {
        KConfigGroup oldGroup = group.group(str);
        const Akonadi::Collection::Id id = convertPathToId(oldGroup.readEntry(u"collectionId"_s));
        if (id != -1) {
            KConfigGroup newGroup = groupCopy.group(str);
            oldGroup.copyTo(&newGroup);
            newGroup.writeEntry(u"collectionId"_s, id);
        }
        const QString sourceKey(u"sources"_s);
        convertRealPathToCollectionList(oldGroup, sourceKey, false);
        // oldGroup.deleteGroup();
    }
}

void ImportMailJobInterface::copyArchiveMailAgentConfigGroup(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination)
{
    // adapt id
    const QString archiveGroupPattern = u"ArchiveMailCollection "_s;
    const QStringList archiveList = archiveConfigOrigin->groupList().filter(archiveGroupPattern);
    for (const QString &str : archiveList) {
        const QString path = str.right(str.length() - archiveGroupPattern.length());
        if (!path.isEmpty()) {
            KConfigGroup oldGroup = archiveConfigOrigin->group(str);
            const Akonadi::Collection::Id id = convertPathToId(path);
            if (id != -1) {
                KConfigGroup newGroup(archiveConfigDestination, archiveGroupPattern + QString::number(id));
                oldGroup.copyTo(&newGroup);
                newGroup.writeEntry(u"saveCollectionId"_s, id);
                QString storePath = newGroup.readEntry("storePath", QString());
                if (!QDir(storePath).exists()) {
                    newGroup.writeEntry(u"storePath"_s, QDir::homePath());
                }
            }
            oldGroup.deleteGroup();
        }
    }
}

void ImportMailJobInterface::importTemplatesConfig(const KArchiveFile *templatesconfiguration,
                                                   const QString &templatesconfigurationrc,
                                                   const QString &filename,
                                                   const QString &prefix)
{
    copyToFile(templatesconfiguration, templatesconfigurationrc, filename, prefix);
    KSharedConfig::Ptr templateConfig = KSharedConfig::openConfig(templatesconfigurationrc);

    // adapt id
    const QString templateGroupPattern = u"Templates #"_s;
    const QString templateGroupIdentityPattern = u"Templates #IDENTITY_"_s;
    const QStringList templateList = templateConfig->groupList().filter(templateGroupPattern);
    for (const QString &str : templateList) {
        // Identity
        if (str.startsWith(templateGroupIdentityPattern)) {
            bool found = false;
            const int identity = QStringView(str).right(str.length() - templateGroupIdentityPattern.length()).toInt(&found);
            if (found) {
                KConfigGroup oldGroup = templateConfig->group(str);
                if (mHashIdentity.contains(identity)) {
                    KConfigGroup newGroup(templateConfig, templateGroupPattern + QString::number(mHashIdentity.value(identity)));
                    oldGroup.copyTo(&newGroup);
                }
                oldGroup.deleteGroup();
            }
        } else { // Folder
            const QString path = str.right(str.length() - templateGroupPattern.length());
            if (!path.isEmpty()) {
                KConfigGroup oldGroup = templateConfig->group(str);
                const Akonadi::Collection::Id id = convertPathToId(path);
                if (id != -1) {
                    KConfigGroup newGroup(templateConfig, templateGroupPattern + QString::number(id));
                    oldGroup.copyTo(&newGroup);
                }
                oldGroup.deleteGroup();
            }
        }
    }
    templateConfig->sync();
}

void ImportMailJobInterface::importKmailConfig(const KArchiveFile *kmailsnippet, const QString &kmail2rc, const QString &filename, const QString &prefix)
{
    copyToFile(kmailsnippet, kmail2rc, filename, prefix);
    KSharedConfig::Ptr kmailConfig = KSharedConfig::openConfig(kmail2rc);

    // adapt folder id
    const QString folderGroupPattern = u"Folder-"_s;
    const QStringList folderList = kmailConfig->groupList().filter(folderGroupPattern);
    for (const QString &str : folderList) {
        const QString path = str.right(str.length() - folderGroupPattern.length());
        if (!path.isEmpty()) {
            KConfigGroup oldGroup = kmailConfig->group(str);
            ImportExportMailUtil::cleanupFolderSettings(oldGroup);
            if (!oldGroup.keyList().isEmpty()) {
                if (oldGroup.hasKey(u"Identity"_s)) {
                    const int identity = oldGroup.readEntry(u"Identity"_s, -1);
                    if (identity != -1) {
                        oldGroup.writeEntry(u"Identity"_s, mHashIdentity.value(identity));
                    }
                }
                const Akonadi::Collection::Id id = convertPathToId(path);
                if (id != -1) {
                    KConfigGroup newGroup(kmailConfig, folderGroupPattern + QString::number(id));
                    oldGroup.copyTo(&newGroup);
                }
            }
            oldGroup.deleteGroup();
        }
    }
    const QString accountOrder(u"AccountOrder"_s);
    if (kmailConfig->hasGroup(accountOrder)) {
        KConfigGroup group = kmailConfig->group(accountOrder);
        const QStringList orderList = group.readEntry(u"order"_s, QStringList());
        QStringList newOrderList;
        if (!orderList.isEmpty()) {
            for (const QString &account : orderList) {
                if (mHashResources.contains(account)) {
                    newOrderList.append(mHashResources.value(account));
                } else {
                    newOrderList.append(account);
                }
            }
        }
    }

    const QString composerStr(u"Composer"_s);
    if (kmailConfig->hasGroup(composerStr)) {
        KConfigGroup composerGroup = kmailConfig->group(composerStr);
        const QString previousStr(u"previous-fcc"_s);
        (void)convertRealPathToCollection(composerGroup, previousStr);

        const QString previousIdentityStr(u"previous-identity"_s);
        if (composerGroup.hasKey(previousIdentityStr)) {
            const int identityValue = composerGroup.readEntry(previousIdentityStr, -1);
            if (identityValue != -1) {
                if (mHashIdentity.contains(identityValue)) {
                    composerGroup.writeEntry(previousIdentityStr, mHashIdentity.value(identityValue));
                } else {
                    composerGroup.writeEntry(previousIdentityStr, identityValue);
                }
            }
        }
    }

    const QString collectionFolderViewStr(u"CollectionFolderView"_s);
    if (kmailConfig->hasGroup(collectionFolderViewStr)) {
        KConfigGroup favoriteGroup = kmailConfig->group(collectionFolderViewStr);
        const QString currentKey(u"Current"_s);
        (void)convertRealPathToCollection(favoriteGroup, currentKey, true);

        const QString expensionKey(u"Expansion"_s);
        convertRealPathToCollectionList(favoriteGroup, expensionKey);
    }

    convertCollectionListStrToAkonadiId(kmailConfig, u"FavoriteCollections"_s, u"FavoriteCollectionIds"_s, false);

    // For favorite id for root collection == 0 and we store only folder => c
    convertCollectionListStrToAkonadiId(kmailConfig, u"FavoriteCollectionsOrder"_s, u"0"_s, true);

    // Event collection
    convertCollectionStrToAkonadiId(kmailConfig, u"Event"_s, u"LastEventSelectedFolder"_s);
    // Todo collection
    convertCollectionStrToAkonadiId(kmailConfig, u"Todo"_s, u"LastSelectedFolder"_s);
    // Note collection
    convertCollectionStrToAkonadiId(kmailConfig, u"Note"_s, u"LastNoteSelectedFolder"_s);

    // FolderSelectionDialog collection
    convertCollectionListStrToAkonadiId(kmailConfig, u"FolderSelectionDialog"_s, u"LastSelectedFolder"_s, false);

    // Convert MessageListTab collection id
    const QString messageListPaneStr(u"MessageListPane"_s);
    if (kmailConfig->hasGroup(messageListPaneStr)) {
        KConfigGroup messageListPaneGroup = kmailConfig->group(messageListPaneStr);
        const int numberOfTab = messageListPaneGroup.readEntry(u"tabNumber"_s, 0);
        for (int i = 0; i < numberOfTab; ++i) {
            KConfigGroup messageListPaneTabGroup = kmailConfig->group(u"MessageListTab%1"_s.arg(i));
            const QString messageListPaneTabFolderStr(u"collectionId"_s);
            convertRealPathToCollectionList(messageListPaneTabGroup, messageListPaneTabFolderStr, false);
        }
    }

    // Automatic Add Contacts
    QHash<int, uint>::const_iterator i = mHashIdentity.constBegin();
    while (i != mHashIdentity.constEnd()) {
        // Use old key
        const QString groupId = u"Automatic Add Contacts %1"_s.arg(i.key());
        if (kmailConfig->hasGroup(groupId)) {
            KConfigGroup identityGroup = kmailConfig->group(groupId);

            KConfigGroup newGroup(kmailConfig, u"Automatic Add Contacts %1"_s.arg(i.value()));
            identityGroup.copyTo(&newGroup);
            const QString automaticAddContactStr(u"Collection"_s);
            convertRealPathToCollectionList(newGroup, automaticAddContactStr, false);
            identityGroup.deleteGroup();
        }
        ++i;
    }

    const QString generalStr(u"General"_s);
    if (kmailConfig->hasGroup(generalStr)) {
        KConfigGroup generalGroup = kmailConfig->group(generalStr);
        // Be sure to delete default domain
        const QString defaultDomainStr(u"Default domain"_s);
        if (generalGroup.hasKey(defaultDomainStr)) {
            generalGroup.deleteEntry(defaultDomainStr);
        }

        const QString startupFolderStr(u"startupFolder"_s);
        convertRealPathToCollection(generalGroup, startupFolderStr);
    }

    const QString resourceGroupPattern = u"Resource "_s;
    const QStringList resourceList = kmailConfig->groupList().filter(resourceGroupPattern);
    for (const QString &str : resourceList) {
        const QString res = str.right(str.length() - resourceGroupPattern.length());
        if (!res.isEmpty()) {
            KConfigGroup oldGroup = kmailConfig->group(str);
            if (mHashResources.contains(res)) {
                KConfigGroup newGroup(kmailConfig, folderGroupPattern + mHashResources.value(res));
                oldGroup.copyTo(&newGroup);
            }
            oldGroup.deleteGroup();
        }
    }

    kmailConfig->sync();
    emitInfo(i18n("\"%1\" migration done.", filename));
}

void ImportMailJobInterface::mergeLdapConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix)
{
    QDir dir(mTempDirName);
    dir.mkdir(prefix);

    const QString copyToDirName(mTempDirName + u'/' + prefix);
    copyArchiveFileTo(archivefile, copyToDirName);

    KSharedConfig::Ptr existingConfig = KSharedConfig::openConfig(filename);
    KConfigGroup grpExisting = existingConfig->group(u"LDAP"_s);
    int existingNumberHosts = grpExisting.readEntry(u"NumHosts"_s, 0);
    int existingNumberSelectedHosts = grpExisting.readEntry(u"NumSelectedHosts"_s, 0);

    KSharedConfig::Ptr importingLdapConfig = KSharedConfig::openConfig(copyToDirName + u'/' + filename);
    KConfigGroup grpImporting = importingLdapConfig->group(u"LDAP"_s);
    int importingNumberHosts = grpImporting.readEntry(u"NumHosts"_s, 0);
    int importingNumberSelectedHosts = grpImporting.readEntry(u"NumSelectedHosts"_s, 0);

    grpExisting.writeEntry(u"NumHosts"_s, (existingNumberHosts + importingNumberHosts));
    grpExisting.writeEntry(u"NumSelectedHosts"_s, (existingNumberSelectedHosts + importingNumberSelectedHosts));

    for (int i = 0; i < importingNumberSelectedHosts; ++i) {
        const QString auth = grpImporting.readEntry(u"SelectedAuth%1"_s.arg(i), QString());
        const int numberHosts = existingNumberSelectedHosts + i + 1;
        grpExisting.writeEntry(u"SelectedAuth%1"_s.arg(numberHosts), auth);
        grpExisting.writeEntry(u"SelectedBase%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedBase%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SelectedBind%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedBind%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SelectedHost%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedHost%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SelectedMech%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedMech%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SelectedPageSize%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedPageSize%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"SelectedPort%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedPort%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"SelectedPwdBind%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedPwdBind%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SelectedSecurity%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedSecurity%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SelectedSizeLimit%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedSizeLimit%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"SelectedTimeLimit%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedTimeLimit%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"SelectedUser%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedUser%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SelectedVersion%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedVersion%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"SelectedUserFilter%1"_s.arg(numberHosts), grpImporting.readEntry(u"SelectedUserFilter%1"_s.arg(i), 0));
    }

    for (int i = 0; i < importingNumberHosts; ++i) {
        const int numberHosts = existingNumberHosts + i + 1;
        grpExisting.writeEntry(u"Auth%1"_s.arg(numberHosts), grpImporting.readEntry(u"Auth%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"Base%1"_s.arg(numberHosts), grpImporting.readEntry(u"Base%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"Bind%1"_s.arg(numberHosts), grpImporting.readEntry(u"Bind%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"Host%1"_s.arg(numberHosts), grpImporting.readEntry(u"Host%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"Mech%1"_s.arg(numberHosts), grpImporting.readEntry(u"Mech%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"PageSize%1"_s.arg(numberHosts), grpImporting.readEntry(u"PageSize%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"Port%1"_s.arg(numberHosts), grpImporting.readEntry(u"Port%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"PwdBind%1"_s.arg(numberHosts), grpImporting.readEntry(u"PwdBind%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"Security%1"_s.arg(numberHosts), grpImporting.readEntry(u"Security%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"SizeLimit%1"_s.arg(numberHosts), grpImporting.readEntry(u"SizeLimit%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"TimeLimit%1"_s.arg(numberHosts), grpImporting.readEntry(u"TimeLimit%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"User%1"_s.arg(numberHosts), grpImporting.readEntry(u"User%1"_s.arg(i), QString()));
        grpExisting.writeEntry(u"Version%1"_s.arg(numberHosts), grpImporting.readEntry(u"Version%1"_s.arg(i), 0));
        grpExisting.writeEntry(u"UserFilter%1"_s.arg(numberHosts), grpImporting.readEntry(u"UserFilter%1"_s.arg(i), 0));
    }

    grpExisting.sync();
}

void ImportMailJobInterface::mergeUnifiedMailBoxConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix)
{
    QDir dir(mTempDirName);
    dir.mkdir(prefix);

    const QString copyToDirName(mTempDirName + u'/' + prefix);
    archivefile->copyTo(copyToDirName);

    KSharedConfig::Ptr existingConfig = KSharedConfig::openConfig(filename);

    KSharedConfig::Ptr importingMailArchiveConfig = KSharedConfig::openConfig(copyToDirName + u'/' + filename);

    copyUnifiedMailBoxConfig(importingMailArchiveConfig, existingConfig);
    existingConfig->sync();
}

void ImportMailJobInterface::mergeKmailSnippetConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix)
{
    // TODO
    QDir dir(mTempDirName);
    dir.mkdir(prefix);

    const QString copyToDirName(mTempDirName + u'/' + prefix);
    copyArchiveFileTo(archivefile, copyToDirName);

    KSharedConfig::Ptr existingConfig = KSharedConfig::openConfig(filename);

    KSharedConfig::Ptr importingKMailSnipperConfig = KSharedConfig::openConfig(copyToDirName + u'/' + filename);
}

void ImportMailJobInterface::mergeMailArchiveConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix)
{
    QDir dir(mTempDirName);
    dir.mkdir(prefix);

    const QString copyToDirName(mTempDirName + u'/' + prefix);
    archivefile->copyTo(copyToDirName);

    KSharedConfig::Ptr existingConfig = KSharedConfig::openConfig(filename);

    KSharedConfig::Ptr importingMailArchiveConfig = KSharedConfig::openConfig(copyToDirName + u'/' + filename);

    copyMailArchiveConfig(importingMailArchiveConfig, existingConfig);
    existingConfig->sync();
}

void ImportMailJobInterface::mergeArchiveMailAgentConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix)
{
    QDir dir(mTempDirName);
    dir.mkdir(prefix);

    const QString copyToDirName(mTempDirName + u'/' + prefix);
    archivefile->copyTo(copyToDirName);

    KSharedConfig::Ptr existingConfig = KSharedConfig::openConfig(filename);

    KSharedConfig::Ptr importingArchiveMailAgentConfig = KSharedConfig::openConfig(copyToDirName + u'/' + filename);

    copyArchiveMailAgentConfigGroup(importingArchiveMailAgentConfig, existingConfig);
    existingConfig->sync();
}

void ImportMailJobInterface::mergeSieveTemplate(const KArchiveFile *archivefile, const QString &filename, const QString &prefix)
{
    QDir dir(mTempDirName);
    dir.mkdir(prefix);

    const QString copyToDirName(mTempDirName + u'/' + prefix);
    copyArchiveFileTo(archivefile, copyToDirName);

    KSharedConfig::Ptr existingConfig = KSharedConfig::openConfig(filename);

    KSharedConfig::Ptr importingSieveTemplateConfig = KSharedConfig::openConfig(copyToDirName + u'/' + filename);

    KConfigGroup grpExisting = existingConfig->group(u"template"_s);
    int numberOfExistingTemplate = grpExisting.readEntry(u"templateCount"_s, 0);

    KConfigGroup grpImportExisting = importingSieveTemplateConfig->group(u"template"_s);
    const int numberOfImportingTemplate = grpImportExisting.readEntry(u"templateCount"_s, 0);

    for (int i = 0; i < numberOfImportingTemplate; ++i) {
        KConfigGroup templateDefine = importingSieveTemplateConfig->group(u"templateDefine_%1"_s.arg(i));

        KConfigGroup newTemplateDefineGrp = existingConfig->group(u"templateDefine_%1"_s.arg(numberOfExistingTemplate));
        newTemplateDefineGrp.writeEntry(u"Name"_s, templateDefine.readEntry(u"Name"_s));
        newTemplateDefineGrp.writeEntry(u"Text"_s, templateDefine.readEntry(u"Text"_s));
        ++numberOfExistingTemplate;
        newTemplateDefineGrp.sync();
    }
    grpExisting.writeEntry(u"templateCount"_s, numberOfExistingTemplate);
    grpExisting.sync();
}

void ImportMailJobInterface::convertCollectionStrToAkonadiId(const KSharedConfig::Ptr &config, const QString &groupName, const QString &key)
{
    if (config->hasGroup(groupName)) {
        KConfigGroup eventGroup = config->group(groupName);
        (void)convertRealPathToCollection(eventGroup, key, false);
    }
}

#include "moc_importmailjobinterface.cpp"
