/*
   SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailjobinterface.h"
#include "importexportmailutil.h"

#include <MailCommon/FilterImporterExporter>

#include <MailTransport/TransportManager>

#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>

#include "pimdataexportcore_debug.h"
#include <KMime/Message>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTimer>
#include <resourceconverterimpl.h>

#include <KIdentityManagementCore/Identity>

ExportMailJobInterface::ExportMailJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportMailJobInterface::~ExportMailJobInterface() = default;

bool ExportMailJobInterface::checkBackupType(Utils::StoredType type) const
{
    return mTypeSelected & type;
}

void ExportMailJobInterface::start()
{
    Q_EMIT title(i18n("Start export KMail settings..."));
    createProgressDialog(i18n("Export KMail settings"));
    if (checkBackupType(Utils::Identity)) {
        QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupIdentity);
    } else if (checkBackupType(Utils::MailTransport)) {
        QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupMailTransport);
    } else if (checkBackupType(Utils::Config)) {
        QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupConfig);
    } else if (checkBackupType(Utils::Mails)) {
        QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupMails);
    } else if (checkBackupType(Utils::Resources)) {
        QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupResources);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportMailJobInterface::slotCheckBackupIdentity()
{
    if (checkBackupType(Utils::Identity)) {
        backupIdentity();
        increaseProgressDialog();
        if (wasCanceled()) {
            Q_EMIT jobFinished();
            return;
        }
    }
    QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupMailTransport);
}

void ExportMailJobInterface::slotCheckBackupMailTransport()
{
    if (checkBackupType(Utils::MailTransport)) {
        backupTransports();
        increaseProgressDialog();
        if (wasCanceled()) {
            Q_EMIT jobFinished();
            return;
        }
    }
    QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupConfig);
}

void ExportMailJobInterface::slotCheckBackupConfig()
{
    if (checkBackupType(Utils::Config)) {
        backupConfig();
        increaseProgressDialog();
        if (wasCanceled()) {
            Q_EMIT jobFinished();
            return;
        }
    }
    QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupMails);
}

void ExportMailJobInterface::slotCheckBackupMails()
{
    mAkonadiInstanceInfo = listOfResource();
    if (checkBackupType(Utils::Mails)) {
        increaseProgressDialog();
        backupFolderAttributes();
        return;
    }
    QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupResources);
}

void ExportMailJobInterface::backupFolderAttributes()
{
    setProgressDialogLabel(i18n("Backing up Folder Attributes..."));
    connect(this, &ExportMailJobInterface::exportAttributeDone, this, [this]() {
        setProgressDialogLabel(i18n("Backing up Mails..."));
        emitInfo(i18n("Start export resource..."));
        QTimer::singleShot(0, this, &ExportMailJobInterface::slotWriteNextArchiveResource);
    });
    exportFolderAttributes();
}

void ExportMailJobInterface::backupTransports()
{
    setProgressDialogLabel(i18n("Backing up transports..."));

    const QString mailtransportsStr(QStringLiteral("mailtransports"));
    const QString maitransportsrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + mailtransportsStr;
    if (!QFileInfo::exists(maitransportsrc)) {
        emitInfo(i18n("Transports backup done."));
    } else {
        KSharedConfigPtr mailtransportsConfig = KSharedConfig::openConfig(mailtransportsStr);

        QTemporaryFile tmp;
        tmp.open();
        KConfig *config = mailtransportsConfig->copyTo(tmp.fileName());

        config->sync();
        const bool fileAdded = archive()->addLocalFile(tmp.fileName(), Utils::transportsPath() + QStringLiteral("mailtransports"));
        delete config;
        if (fileAdded) {
            emitInfo(i18n("Transports backup done."));
        } else {
            emitError(i18n("Transport file cannot be added to backup file."));
        }
    }
}

void ExportMailJobInterface::slotCheckBackupResources()
{
    if (checkBackupType(Utils::Resources)) {
        backupResources();
        increaseProgressDialog();
        if (wasCanceled()) {
            Q_EMIT jobFinished();
            return;
        }
    }
    Q_EMIT jobFinished();
}

QString ExportMailJobInterface::applicationName() const
{
    return QStringLiteral("[KMail]");
}

void ExportMailJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    exportFilters();

    backupUiRcFile(QStringLiteral("sieveeditorui.rc"), QStringLiteral("sieveeditor"));
    backupUiRcFile(QStringLiteral("kmreadermainwin.rc"), QStringLiteral("kmail2"));
    backupUiRcFile(QStringLiteral("kmcomposerui.rc"), QStringLiteral("kmail2"));
    backupUiRcFile(QStringLiteral("kmmainwin.rc"), QStringLiteral("kmail2"));
    backupUiRcFile(QStringLiteral("kmail_part.rc"), QStringLiteral("kmail2"));
    backupUiRcFile(QStringLiteral("kontactsummary_part.rc"), QStringLiteral("kontactsummary"));
    backupUiRcFile(QStringLiteral("kontactui.rc"), QStringLiteral("kontact"));
    backupUiRcFile(QStringLiteral("kleopatra.rc"), QStringLiteral("kleopatra"));
    backupUiRcFile(QStringLiteral("headerthemeeditorui.rc"), QStringLiteral("headerthemeeditor"));
    backupUiRcFile(QStringLiteral("contactthemeeditorui.rc"), QStringLiteral("contactthemeeditor"));
    backupUiRcFile(QStringLiteral("contactprintthemeeditorui.rc"), QStringLiteral("contactprintthemeeditor"));
    backupUiRcFile(QStringLiteral("kwatchgnupgui.rc"), QStringLiteral("kwatchgnupg"));
    backupUiRcFile(QStringLiteral("akonadiconsoleui.rc"), QStringLiteral("akonadiconsole"));

    backupConfigFile(QStringLiteral("kabldaprc"));
    backupConfigFile(QStringLiteral("kmailsnippetrc"));
    backupConfigFile(QStringLiteral("sievetemplaterc"));
    backupConfigFile(QStringLiteral("customtemplatesrc"));
    backupConfigFile(QStringLiteral("kontactrc"));
    backupConfigFile(QStringLiteral("kontact_summaryrc"));
    backupConfigFile(QStringLiteral("storageservicerc"));
    backupConfigFile(QStringLiteral("kpimbalooblacklist"));
    backupConfigFile(QStringLiteral("kleopatrarc"));
    backupConfigFile(QStringLiteral("sieveeditorrc"));
    backupConfigFile(QStringLiteral("kwatchgnupgrc"));
    backupConfigFile(QStringLiteral("pimpluginsrc"));
    backupConfigFile(QStringLiteral("texttospeechrc"));
    backupConfigFile(QStringLiteral("kleopatracertificateselectiondialogrc"));
    backupConfigFile(QStringLiteral("dkimsettingsrc"));
    backupConfigFile(QStringLiteral("confirmbeforedeletingrc"));

    // Notify file config
    backupConfigFile(QStringLiteral("akonadi_mailfilter_agent.notifyrc"));
    backupConfigFile(QStringLiteral("akonadi_sendlater_agent.notifyrc"));
    backupConfigFile(QStringLiteral("akonadi_archivemail_agent.notifyrc"));
    backupConfigFile(QStringLiteral("kmail2.notifyrc"));
    backupConfigFile(QStringLiteral("akonadi_newmailnotifier_agent.notifyrc"));
    backupConfigFile(QStringLiteral("akonadi_maildispatcher_agent.notifyrc"));
    backupConfigFile(QStringLiteral("akonadi_followupreminder_agent.notifyrc"));
    backupConfigFile(QStringLiteral("messagevieweradblockrc"));
    backupConfigFile(QStringLiteral("messageviewer.notifyrc"));
    backupConfigFile(QStringLiteral("akonadi_newmailnotifier_agentrc"));

    const QString folderMailArchiveStr(QStringLiteral("foldermailarchiverc"));
    const QString folderMailArchiverc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + folderMailArchiveStr;
    if (QFileInfo::exists(folderMailArchiverc)) {
        KSharedConfigPtr archivemailrc = KSharedConfig::openConfig(folderMailArchiveStr);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *archiveConfig = archivemailrc->copyTo(tmp.fileName());
        const QStringList archiveList = archiveConfig->groupList().filter(QRegularExpression(QStringLiteral("FolderArchiveAccount")));

        for (const QString &str : archiveList) {
            KConfigGroup oldGroup = archiveConfig->group(str);
            const qint64 id = oldGroup.readEntry("topLevelCollectionId", -1);
            if (id != -1) {
                const QString realPath = convertToFullCollectionPath(id);
                if (!realPath.isEmpty()) {
                    oldGroup.writeEntry(QStringLiteral("topLevelCollectionId"), realPath);
                }
            }
        }
        archiveConfig->sync();

        backupFile(tmp.fileName(), Utils::configsPath(), folderMailArchiveStr);
        delete archiveConfig;
    }
    const QString unifiedMailBoxStr(QStringLiteral("akonadi_unifiedmailbox_agentrc"));
    const QString unifiedMailBoxrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + unifiedMailBoxStr;
    if (QFileInfo::exists(unifiedMailBoxrc)) {
        KSharedConfigPtr mboxrc = KSharedConfig::openConfig(unifiedMailBoxrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *archiveConfig = mboxrc->copyTo(tmp.fileName());
        auto group = archiveConfig->group(QStringLiteral("UnifiedMailboxes"));
        const auto boxGroups = group.groupList();
        for (const auto &str : boxGroups) {
            KConfigGroup oldGroup = group.group(str);

            const qint64 id = oldGroup.readEntry("collectionId", -1);
            if (id != -1) {
                const QString realPath = convertToFullCollectionPath(id);
                if (!realPath.isEmpty()) {
                    oldGroup.writeEntry(QStringLiteral("collectionId"), realPath);
                }
            }
            const QString sourceKey(QStringLiteral("sources"));
            convertCollectionListToRealPath(oldGroup, sourceKey);
        }
        archiveConfig->sync();

        backupFile(tmp.fileName(), Utils::configsPath(), unifiedMailBoxStr);
        delete archiveConfig;
    }

    const QString archiveMailAgentConfigurationStr(QStringLiteral("akonadi_archivemail_agentrc"));
    const QString archiveMailAgentconfigurationrc =
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + archiveMailAgentConfigurationStr;
    if (QFileInfo::exists(archiveMailAgentconfigurationrc)) {
        KSharedConfigPtr archivemailrc = KSharedConfig::openConfig(archiveMailAgentConfigurationStr);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *archiveConfig = archivemailrc->copyTo(tmp.fileName());
        const QStringList archiveList = archiveConfig->groupList().filter(QRegularExpression(QStringLiteral("ArchiveMailCollection \\d+")));
        const QString archiveGroupPattern = QStringLiteral("ArchiveMailCollection ");

        for (const QString &str : archiveList) {
            bool found = false;
            const int collectionId = QStringView(str).right(str.length() - archiveGroupPattern.length()).toInt(&found);
            if (found) {
                KConfigGroup oldGroup = archiveConfig->group(str);
                const QString realPath = convertToFullCollectionPath(collectionId);
                if (!realPath.isEmpty()) {
                    const QString collectionPath(archiveGroupPattern + realPath);
                    KConfigGroup newGroup(archiveConfig, collectionPath);
                    oldGroup.copyTo(&newGroup);
                    newGroup.writeEntry(QStringLiteral("saveCollectionId"), realPath);
                }
                oldGroup.deleteGroup();
            }
        }
        archiveConfig->sync();

        backupFile(tmp.fileName(), Utils::configsPath(), archiveMailAgentConfigurationStr);
        delete archiveConfig;
    }

    const QString templatesconfigurationrcStr(QStringLiteral("templatesconfigurationrc"));
    const QString templatesconfigurationrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + templatesconfigurationrcStr;
    if (QFileInfo::exists(templatesconfigurationrc)) {
        KSharedConfigPtr templaterc = KSharedConfig::openConfig(templatesconfigurationrcStr);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *templateConfig = templaterc->copyTo(tmp.fileName());
        const QString templateGroupPattern = QStringLiteral("Templates #");
        const QStringList templateList = templateConfig->groupList().filter(QRegularExpression(QStringLiteral("Templates #\\d+")));
        for (const QString &str : templateList) {
            bool found = false;
            const int collectionId = QStringView(str).right(str.length() - templateGroupPattern.length()).toInt(&found);
            if (found) {
                KConfigGroup oldGroup = templateConfig->group(str);
                const QString realPath = convertToFullCollectionPath(collectionId);
                if (!realPath.isEmpty()) {
                    KConfigGroup newGroup(templateConfig, templateGroupPattern + realPath);
                    oldGroup.copyTo(&newGroup);
                }
                oldGroup.deleteGroup();
            }
        }
        templateConfig->sync();

        backupFile(tmp.fileName(), Utils::configsPath(), templatesconfigurationrcStr);
        delete templateConfig;
    }

    storeDirectory(QStringLiteral("/messageviewerplugins/"));
    storeDirectory(QStringLiteral("/messageviewer/themes/"));

    const QDir gravatarDirectory(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/gravatar/"));
    if (gravatarDirectory.exists()) {
        const QFileInfoList listFileInfo = gravatarDirectory.entryInfoList(QStringList() << QStringLiteral("*.png"), QDir::Files);
        const int listSize(listFileInfo.size());
        for (int i = 0; i < listSize; ++i) {
            backupFile(listFileInfo.at(i).absoluteFilePath(), Utils::dataPath() + QStringLiteral("gravatar/"), listFileInfo.at(i).fileName());
        }
    }

    const QDir autocorrectDirectory(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/autocorrect/"));
    if (autocorrectDirectory.exists()) {
        const QFileInfoList listFileInfo = autocorrectDirectory.entryInfoList(QStringList() << QStringLiteral("*.xml"), QDir::Files);
        const int listSize(listFileInfo.size());
        for (int i = 0; i < listSize; ++i) {
            backupFile(listFileInfo.at(i).absoluteFilePath(), Utils::dataPath() + QStringLiteral("autocorrect/"), listFileInfo.at(i).fileName());
        }
    }
    const QString adblockFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/kmail2/adblockrules_local");
    if (QFileInfo::exists(adblockFilePath)) {
        backupFile(adblockFilePath, Utils::dataPath() + QStringLiteral("kmail2/"), QStringLiteral("adblockrules_local"));
    }

    const QString kmailStr(QStringLiteral("kmail2rc"));
    const QString kmail2rc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + kmailStr;
    if (QFileInfo::exists(kmail2rc)) {
        KSharedConfigPtr kmailrc = KSharedConfig::openConfig(kmail2rc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *kmailConfig = kmailrc->copyTo(tmp.fileName());
        const QString folderGroupPattern = QStringLiteral("Folder-");
        const QStringList folderList = kmailConfig->groupList().filter(QRegularExpression(QStringLiteral("Folder-\\d+")));
        for (const QString &str : folderList) {
            bool found = false;
            const int collectionId = QStringView(str).right(str.length() - folderGroupPattern.length()).toInt(&found);
            if (found) {
                KConfigGroup oldGroup = kmailConfig->group(str);
                ImportExportMailUtil::cleanupFolderSettings(oldGroup);
                const QString realPath = convertToFullCollectionPath(collectionId);
                if (!realPath.isEmpty()) {
                    KConfigGroup newGroup(kmailConfig, folderGroupPattern + realPath);
                    oldGroup.copyTo(&newGroup);
                }
                oldGroup.deleteGroup();
            }
        }
        const QString composerStr(QStringLiteral("Composer"));
        if (kmailConfig->hasGroup(composerStr)) {
            KConfigGroup composerGroup = kmailConfig->group(composerStr);
            const QString previousStr(QStringLiteral("previous-fcc"));
            if (composerGroup.hasKey(previousStr)) {
                const int collectionId = composerGroup.readEntry(previousStr, -1);
                if (collectionId != -1) {
                    const QString realPath = convertToFullCollectionPath(collectionId);
                    composerGroup.writeEntry(previousStr, realPath);
                }
            }
        }

        const QString generalStr(QStringLiteral("General"));
        if (kmailConfig->hasGroup(generalStr)) {
            KConfigGroup generalGroup = kmailConfig->group(generalStr);
            const QString startupFolderStr(QStringLiteral("startupFolder"));
            if (generalGroup.hasKey(startupFolderStr)) {
                const int collectionId = generalGroup.readEntry(startupFolderStr, -1);
                if (collectionId != -1) {
                    const QString realPath = convertToFullCollectionPath(collectionId);
                    generalGroup.writeEntry(startupFolderStr, realPath);
                }
            }
        }

        const QString storageModelSelectedMessageStr(QStringLiteral("MessageListView::StorageModelSelectedMessages"));
        if (kmailConfig->hasGroup(storageModelSelectedMessageStr)) {
            KConfigGroup storageGroup = kmailConfig->group(storageModelSelectedMessageStr);
            const QString storageModelSelectedPattern(QStringLiteral("MessageUniqueIdForStorageModel"));
            const QStringList storageList = storageGroup.keyList().filter(QRegularExpression(QStringLiteral("MessageUniqueIdForStorageModel\\d+")));
            for (const QString &str : storageList) {
                bool found = false;
                const int collectionId = QStringView(str).right(str.length() - storageModelSelectedPattern.length()).toInt(&found);
                const QString oldValue = storageGroup.readEntry(str);
                if (found) {
                    const QString realPath = convertToFullCollectionPath(collectionId);
                    if (!realPath.isEmpty()) {
                        storageGroup.writeEntry(QStringLiteral("%1%2").arg(storageModelSelectedPattern, realPath), oldValue);
                        storageGroup.deleteEntry(str);
                    } else {
                        storageGroup.deleteEntry(str);
                    }
                }
            }
        }

        const QString collectionFolderViewStr(QStringLiteral("CollectionFolderView"));
        if (kmailConfig->hasGroup(collectionFolderViewStr)) {
            KConfigGroup favoriteGroup = kmailConfig->group(collectionFolderViewStr);

            const QString currentKey(QStringLiteral("Current"));
            convertCollectionToRealPath(favoriteGroup, currentKey);

            const QString expensionKey(QStringLiteral("Expansion"));
            convertCollectionListToRealPath(favoriteGroup, expensionKey);
        }

        const QString favoriteCollectionStr(QStringLiteral("FavoriteCollections"));
        if (kmailConfig->hasGroup(favoriteCollectionStr)) {
            KConfigGroup favoriteGroup = kmailConfig->group(favoriteCollectionStr);

            const QString favoriteCollectionIdsStr(QStringLiteral("FavoriteCollectionIds"));
            convertCollectionIdsToRealPath(favoriteGroup, favoriteCollectionIdsStr);
        }

        const QString favoriteCollectionOrderStr(QStringLiteral("FavoriteCollectionsOrder"));
        if (kmailConfig->hasGroup(favoriteCollectionOrderStr)) {
            KConfigGroup favoriteGroup = kmailConfig->group(favoriteCollectionOrderStr);
            // For favorite id for root collection == 0 and we store only folder => c
            const QString favoriteCollectionIdsStr(QStringLiteral("0"));
            const QString prefixCollection(QStringLiteral("c"));
            convertCollectionIdsToRealPath(favoriteGroup, favoriteCollectionIdsStr, prefixCollection);
        }

        // Event collection
        const QString eventCollectionStr(QStringLiteral("Event"));
        if (kmailConfig->hasGroup(eventCollectionStr)) {
            KConfigGroup eventGroup = kmailConfig->group(eventCollectionStr);
            const QString eventLastEventSelectedFolder(QStringLiteral("LastEventSelectedFolder"));
            convertCollectionIdsToRealPath(eventGroup, eventLastEventSelectedFolder);
        }

        // Todo collection
        const QString todoCollectionStr(QStringLiteral("Todo"));
        if (kmailConfig->hasGroup(todoCollectionStr)) {
            KConfigGroup todoGroup = kmailConfig->group(todoCollectionStr);
            const QString todoLastEventSelectedFolder(QStringLiteral("LastSelectedFolder"));
            convertCollectionIdsToRealPath(todoGroup, todoLastEventSelectedFolder);
        }
        // FolderSelectionDialog collection
        const QString folderSelectionCollectionStr(QStringLiteral("FolderSelectionDialog"));
        if (kmailConfig->hasGroup(folderSelectionCollectionStr)) {
            KConfigGroup folderSelectionGroup = kmailConfig->group(folderSelectionCollectionStr);
            const QString folderSelectionSelectedFolder(QStringLiteral("LastSelectedFolder"));
            convertCollectionIdsToRealPath(folderSelectionGroup, folderSelectionSelectedFolder);
        }

        // Note collection
        const QString noteCollectionStr(QStringLiteral("Note"));
        if (kmailConfig->hasGroup(noteCollectionStr)) {
            KConfigGroup noteGroup = kmailConfig->group(noteCollectionStr);
            const QString noteLastEventSelectedFolder(QStringLiteral("LastNoteSelectedFolder"));
            convertCollectionIdsToRealPath(noteGroup, noteLastEventSelectedFolder);
        }

        // Convert MessageListTab collection id
        const QString messageListPaneStr(QStringLiteral("MessageListPane"));
        if (kmailConfig->hasGroup(messageListPaneStr)) {
            KConfigGroup messageListPaneGroup = kmailConfig->group(messageListPaneStr);
            const int numberOfTab = messageListPaneGroup.readEntry(QStringLiteral("tabNumber"), 0);
            for (int i = 0; i < numberOfTab; ++i) {
                KConfigGroup messageListPaneTabGroup = kmailConfig->group(QStringLiteral("MessageListTab%1").arg(i));
                const QString messageListPaneTabFolderStr(QStringLiteral("collectionId"));
                convertCollectionIdsToRealPath(messageListPaneTabGroup, messageListPaneTabFolderStr);
            }
        }

        // Automatic Add Contacts
        const QList<uint> listIdentities = listIdentityUoid();
        for (uint identity : listIdentities) {
            const QString groupId = QStringLiteral("Automatic Add Contacts %1").arg(identity);
            if (kmailConfig->hasGroup(groupId)) {
                KConfigGroup identityGroup = kmailConfig->group(groupId);
                const QString automaticAddContactStr(QStringLiteral("Collection"));
                convertCollectionIdsToRealPath(identityGroup, automaticAddContactStr);
            }
        }

        // TODO add confirm address too

        // Clean up kmail2rc
        const QString tipOfDaysStr(QStringLiteral("TipOfDay"));
        if (kmailConfig->hasGroup(tipOfDaysStr)) {
            kmailConfig->deleteGroup(tipOfDaysStr);
        }
        const QString startupStr(QStringLiteral("Startup"));
        if (kmailConfig->hasGroup(startupStr)) {
            kmailConfig->deleteGroup(startupStr);
        }

        const QString search(QStringLiteral("Search"));
        if (kmailConfig->hasGroup(search)) {
            KConfigGroup searchGroup = kmailConfig->group(search);
            searchGroup.deleteGroup();
        }

        kmailConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), kmailStr);
        delete kmailConfig;
    }

    emitInfo(i18n("Config backup done."));
}

void ExportMailJobInterface::backupIdentity()
{
    setProgressDialogLabel(i18n("Backing up identity..."));

    const QString emailidentitiesStr(QStringLiteral("emailidentities"));
    const QString emailidentitiesrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + emailidentitiesStr;
    if (QFileInfo::exists(emailidentitiesrc)) {
        KSharedConfigPtr identity = KSharedConfig::openConfig(emailidentitiesrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *identityConfig = identity->copyTo(tmp.fileName());
        const QStringList accountList = identityConfig->groupList().filter(QRegularExpression(QStringLiteral("Identity #\\d+")));
        for (const QString &account : accountList) {
            KConfigGroup group = identityConfig->group(account);
            const QString fcc = QStringLiteral("Fcc");
            if (group.hasKey(fcc)) {
                const QString realPath = convertToFullCollectionPath(group.readEntry(fcc).toLongLong());
                group.writeEntry(fcc, realPath);
            }
            const QString draft = QStringLiteral("Drafts");
            if (group.hasKey(draft)) {
                const QString realPath = convertToFullCollectionPath(group.readEntry(draft).toLongLong());
                group.writeEntry(draft, realPath);
            }
            const QString templates = QStringLiteral("Templates");
            if (group.hasKey(templates)) {
                const QString realPath = convertToFullCollectionPath(group.readEntry(templates).toLongLong());
                group.writeEntry(templates, realPath);
            }
            const QString vcard = QStringLiteral("VCardFile");
            if (group.hasKey(vcard)) {
                const QString vcardFileName = group.readEntry(vcard);
                if (!vcardFileName.isEmpty()) {
                    const int uoid = group.readEntry(QStringLiteral("uoid"), -1);
                    QFile file(vcardFileName);
                    if (file.exists()) {
                        const bool fileAdded =
                            archive()->addLocalFile(vcardFileName, Utils::identitiesPath() + QString::number(uoid) + QLatin1Char('/') + file.fileName());
                        if (!fileAdded) {
                            emitError(i18n("vCard file \"%1\" cannot be saved.", file.fileName()));
                        }
                    } else {
                        group.deleteEntry(vcard);
                    }
                }
            }
        }

        identityConfig->sync();
        const bool fileAdded = archive()->addLocalFile(tmp.fileName(), Utils::identitiesPath() + QStringLiteral("emailidentities"));
        delete identityConfig;
        if (fileAdded) {
            emitInfo(i18n("Identity backup done."));
        } else {
            emitError(i18n("Identity file cannot be added to backup file."));
        }
    }
}

void ExportMailJobInterface::slotMailsJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportMailJobInterface::slotWriteNextArchiveResource);
}

void ExportMailJobInterface::slotWriteNextArchiveResource()
{
    if (mIndexIdentifier < mAkonadiInstanceInfo.count()) {
        const Utils::AkonadiInstanceInfo agent = mAkonadiInstanceInfo.at(mIndexIdentifier);
        const QStringList capabilities(agent.capabilities);
        if (agent.mimeTypes.contains(KMime::Message::mimeType())) {
            if (capabilities.contains(QLatin1StringView("Resource")) && !capabilities.contains(QLatin1String("Virtual"))
                && !capabilities.contains(QLatin1StringView("MailTransport"))) {
                const QString identifier = agent.identifier;
                if (identifier.contains(QLatin1StringView("akonadi_maildir_resource_"))
                    || identifier.contains(QLatin1String("akonadi_mixedmaildir_resource_"))) {
                    const QString archivePath = Utils::mailsPath() + identifier + QLatin1Char('/');
                    const QString url = resourcePath(identifier);
                    if (!mAgentPaths.contains(url)) {
                        if (!url.isEmpty()) {
                            mAgentPaths << url;
                            exportResourceToArchive(archivePath, url, identifier);
                        } else {
                            qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                            QTimer::singleShot(0, this, &ExportMailJobInterface::slotMailsJobTerminated);
                        }
                    } else {
                        QTimer::singleShot(0, this, &ExportMailJobInterface::slotMailsJobTerminated);
                    }
                } else if (identifier.contains(QLatin1StringView("akonadi_mbox_resource_"))) {
                    backupMailResourceFile(identifier, Utils::mailsPath()); // FIXME addressbookPath or MailPAth ???
                    QTimer::singleShot(0, this, &ExportMailJobInterface::slotMailsJobTerminated);
                } else {
                    QTimer::singleShot(0, this, &ExportMailJobInterface::slotMailsJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportMailJobInterface::slotMailsJobTerminated);
            }
        } else {
            QTimer::singleShot(0, this, &ExportMailJobInterface::slotMailsJobTerminated);
        }
    } else {
        QTimer::singleShot(0, this, &ExportMailJobInterface::slotCheckBackupResources);
    }
}

void ExportMailJobInterface::backupResources()
{
    setProgressDialogLabel(i18n("Backing up resources..."));

    for (const Utils::AkonadiInstanceInfo &agent : std::as_const(mAkonadiInstanceInfo)) {
        const QStringList capabilities = agent.capabilities;
        if (agent.mimeTypes.contains(KMime::Message::mimeType())) {
            if (capabilities.contains(QLatin1StringView("Resource")) && !capabilities.contains(QLatin1String("Virtual"))
                && !capabilities.contains(QLatin1StringView("MailTransport"))) {
                const QString identifier = agent.identifier;
                // Store just pop3/imap/kolab/gmail account. Store other config when we copy data.
                if (identifier.contains(QLatin1StringView("pop3")) || identifier.contains(QLatin1String("imap"))
                    || identifier.contains(QLatin1String("_kolab_")) || identifier.contains(QLatin1StringView("_gmail_"))) {
                    const QString errorStr = storeResources(archive(), identifier, Utils::resourcesPath());
                    if (!errorStr.isEmpty()) {
                        emitError(errorStr);
                    }
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " resource \"" << identifier << "\" will not store";
                }
            }
        }
    }

    emitInfo(i18n("Resources backup done."));
}

#include "moc_exportmailjobinterface.cpp"
