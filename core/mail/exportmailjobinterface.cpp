/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailjobinterface.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportmailutil.h"

#include <MailCommon/FilterImporterExporter>

#include <MailTransport/TransportManager>

#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>

#include "pimdataexportcore_debug.h"
#include "resourceconverterimpl.h"
#include <KMime/Message>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTimer>

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

    const QString mailtransportsStr(u"mailtransports"_s);
    const QString maitransportsrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + mailtransportsStr;
    if (!QFileInfo::exists(maitransportsrc)) {
        emitInfo(i18n("Transports backup done."));
    } else {
        KSharedConfigPtr mailtransportsConfig = KSharedConfig::openConfig(mailtransportsStr);

        QTemporaryFile tmp;
        tmp.open();
        KConfig *config = mailtransportsConfig->copyTo(tmp.fileName());

        config->sync();
        const bool fileAdded = archive()->addLocalFile(tmp.fileName(), Utils::transportsPath() + u"mailtransports"_s);
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
    return u"[KMail]"_s;
}

void ExportMailJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    exportFilters();

    backupUiRcFile(u"sieveeditorui.rc"_s, u"sieveeditor"_s);
    backupUiRcFile(u"kmreadermainwin.rc"_s, u"kmail2"_s);
    backupUiRcFile(u"kmcomposerui.rc"_s, u"kmail2"_s);
    backupUiRcFile(u"kmmainwin.rc"_s, u"kmail2"_s);
    backupUiRcFile(u"kmail_part.rc"_s, u"kmail2"_s);
    backupUiRcFile(u"kontactsummary_part.rc"_s, u"kontactsummary"_s);
    backupUiRcFile(u"kontactui.rc"_s, u"kontact"_s);
    backupUiRcFile(u"kleopatra.rc"_s, u"kleopatra"_s);
    backupUiRcFile(u"headerthemeeditorui.rc"_s, u"headerthemeeditor"_s);
    backupUiRcFile(u"contactthemeeditorui.rc"_s, u"contactthemeeditor"_s);
    backupUiRcFile(u"contactprintthemeeditorui.rc"_s, u"contactprintthemeeditor"_s);
    backupUiRcFile(u"kwatchgnupgui.rc"_s, u"kwatchgnupg"_s);
    backupUiRcFile(u"akonadiconsoleui.rc"_s, u"akonadiconsole"_s);

    backupConfigFile(u"kabldaprc"_s);
    backupConfigFile(u"kmailsnippetrc"_s);
    backupConfigFile(u"sievetemplaterc"_s);
    backupConfigFile(u"customtemplatesrc"_s);
    backupConfigFile(u"kontactrc"_s);
    backupConfigFile(u"kontact_summaryrc"_s);
    backupConfigFile(u"storageservicerc"_s);
    backupConfigFile(u"kpimbalooblacklist"_s);
    backupConfigFile(u"kleopatrarc"_s);
    backupConfigFile(u"sieveeditorrc"_s);
    backupConfigFile(u"kwatchgnupgrc"_s);
    backupConfigFile(u"pimpluginsrc"_s);
    backupConfigFile(u"texttospeechrc"_s);
    backupConfigFile(u"kleopatracertificateselectiondialogrc"_s);
    backupConfigFile(u"dkimsettingsrc"_s);
    backupConfigFile(u"confirmbeforedeletingrc"_s);

    // Notify file config
    backupConfigFile(u"akonadi_mailfilter_agent.notifyrc"_s);
    backupConfigFile(u"akonadi_sendlater_agent.notifyrc"_s);
    backupConfigFile(u"akonadi_archivemail_agent.notifyrc"_s);
    backupConfigFile(u"kmail2.notifyrc"_s);
    backupConfigFile(u"akonadi_newmailnotifier_agent.notifyrc"_s);
    backupConfigFile(u"akonadi_maildispatcher_agent.notifyrc"_s);
    backupConfigFile(u"akonadi_followupreminder_agent.notifyrc"_s);
    backupConfigFile(u"messagevieweradblockrc"_s);
    backupConfigFile(u"messageviewer.notifyrc"_s);
    backupConfigFile(u"akonadi_newmailnotifier_agentrc"_s);

    const QString folderMailArchiveStr(u"foldermailarchiverc"_s);
    const QString folderMailArchiverc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + folderMailArchiveStr;
    if (QFileInfo::exists(folderMailArchiverc)) {
        KSharedConfigPtr archivemailrc = KSharedConfig::openConfig(folderMailArchiveStr);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *archiveConfig = archivemailrc->copyTo(tmp.fileName());
        const QStringList archiveList = archiveConfig->groupList().filter(QRegularExpression(u"FolderArchiveAccount"_s));

        for (const QString &str : archiveList) {
            KConfigGroup oldGroup = archiveConfig->group(str);
            const qint64 id = oldGroup.readEntry("topLevelCollectionId", -1);
            if (id != -1) {
                const QString realPath = convertToFullCollectionPath(id);
                if (!realPath.isEmpty()) {
                    oldGroup.writeEntry(u"topLevelCollectionId"_s, realPath);
                }
            }
        }
        archiveConfig->sync();

        backupFile(tmp.fileName(), Utils::configsPath(), folderMailArchiveStr);
        delete archiveConfig;
    }
    const QString unifiedMailBoxStr(u"akonadi_unifiedmailbox_agentrc"_s);
    const QString unifiedMailBoxrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + unifiedMailBoxStr;
    if (QFileInfo::exists(unifiedMailBoxrc)) {
        KSharedConfigPtr mboxrc = KSharedConfig::openConfig(unifiedMailBoxrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *archiveConfig = mboxrc->copyTo(tmp.fileName());
        auto group = archiveConfig->group(u"UnifiedMailboxes"_s);
        const auto boxGroups = group.groupList();
        for (const auto &str : boxGroups) {
            KConfigGroup oldGroup = group.group(str);

            const qint64 id = oldGroup.readEntry("collectionId", -1);
            if (id != -1) {
                const QString realPath = convertToFullCollectionPath(id);
                if (!realPath.isEmpty()) {
                    oldGroup.writeEntry(u"collectionId"_s, realPath);
                }
            }
            const QString sourceKey(u"sources"_s);
            convertCollectionListToRealPath(oldGroup, sourceKey);
        }
        archiveConfig->sync();

        backupFile(tmp.fileName(), Utils::configsPath(), unifiedMailBoxStr);
        delete archiveConfig;
    }

    const QString archiveMailAgentConfigurationStr(u"akonadi_archivemail_agentrc"_s);
    const QString archiveMailAgentconfigurationrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + archiveMailAgentConfigurationStr;
    if (QFileInfo::exists(archiveMailAgentconfigurationrc)) {
        KSharedConfigPtr archivemailrc = KSharedConfig::openConfig(archiveMailAgentConfigurationStr);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *archiveConfig = archivemailrc->copyTo(tmp.fileName());
        const QStringList archiveList = archiveConfig->groupList().filter(QRegularExpression(u"ArchiveMailCollection \\d+"_s));
        const QString archiveGroupPattern = u"ArchiveMailCollection "_s;

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
                    newGroup.writeEntry(u"saveCollectionId"_s, realPath);
                }
                oldGroup.deleteGroup();
            }
        }
        archiveConfig->sync();

        backupFile(tmp.fileName(), Utils::configsPath(), archiveMailAgentConfigurationStr);
        delete archiveConfig;
    }

    const QString templatesconfigurationrcStr(u"templatesconfigurationrc"_s);
    const QString templatesconfigurationrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + templatesconfigurationrcStr;
    if (QFileInfo::exists(templatesconfigurationrc)) {
        KSharedConfigPtr templaterc = KSharedConfig::openConfig(templatesconfigurationrcStr);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *templateConfig = templaterc->copyTo(tmp.fileName());
        const QString templateGroupPattern = u"Templates #"_s;
        const QStringList templateList = templateConfig->groupList().filter(QRegularExpression(u"Templates #\\d+"_s));
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

    storeDirectory(u"/messageviewerplugins/"_s);
    storeDirectory(u"/messageviewer/themes/"_s);

    const QDir gravatarDirectory(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/gravatar/"_s);
    if (gravatarDirectory.exists()) {
        const QFileInfoList listFileInfo = gravatarDirectory.entryInfoList(QStringList() << u"*.png"_s, QDir::Files);
        const int listSize(listFileInfo.size());
        for (int i = 0; i < listSize; ++i) {
            backupFile(listFileInfo.at(i).absoluteFilePath(), Utils::dataPath() + u"gravatar/"_s, listFileInfo.at(i).fileName());
        }
    }

    const QDir autocorrectDirectory(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/autocorrect/"_s);
    if (autocorrectDirectory.exists()) {
        const QFileInfoList listFileInfo = autocorrectDirectory.entryInfoList(QStringList() << u"*.xml"_s, QDir::Files);
        const int listSize(listFileInfo.size());
        for (int i = 0; i < listSize; ++i) {
            backupFile(listFileInfo.at(i).absoluteFilePath(), Utils::dataPath() + u"autocorrect/"_s, listFileInfo.at(i).fileName());
        }
    }
    const QString adblockFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/kmail2/adblockrules_local"_s;
    if (QFileInfo::exists(adblockFilePath)) {
        backupFile(adblockFilePath, Utils::dataPath() + u"kmail2/"_s, u"adblockrules_local"_s);
    }

    const QString kmailStr(u"kmail2rc"_s);
    const QString kmail2rc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + kmailStr;
    if (QFileInfo::exists(kmail2rc)) {
        KSharedConfigPtr kmailrc = KSharedConfig::openConfig(kmail2rc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *kmailConfig = kmailrc->copyTo(tmp.fileName());
        const QString folderGroupPattern = u"Folder-"_s;
        const QStringList folderList = kmailConfig->groupList().filter(QRegularExpression(u"Folder-\\d+"_s));
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
        const QString composerStr(u"Composer"_s);
        if (kmailConfig->hasGroup(composerStr)) {
            KConfigGroup composerGroup = kmailConfig->group(composerStr);
            const QString previousStr(u"previous-fcc"_s);
            if (composerGroup.hasKey(previousStr)) {
                const int collectionId = composerGroup.readEntry(previousStr, -1);
                if (collectionId != -1) {
                    const QString realPath = convertToFullCollectionPath(collectionId);
                    composerGroup.writeEntry(previousStr, realPath);
                }
            }
        }

        const QString generalStr(u"General"_s);
        if (kmailConfig->hasGroup(generalStr)) {
            KConfigGroup generalGroup = kmailConfig->group(generalStr);
            const QString startupFolderStr(u"startupFolder"_s);
            if (generalGroup.hasKey(startupFolderStr)) {
                const int collectionId = generalGroup.readEntry(startupFolderStr, -1);
                if (collectionId != -1) {
                    const QString realPath = convertToFullCollectionPath(collectionId);
                    generalGroup.writeEntry(startupFolderStr, realPath);
                }
            }
        }

        const QString storageModelSelectedMessageStr(u"MessageListView::StorageModelSelectedMessages"_s);
        if (kmailConfig->hasGroup(storageModelSelectedMessageStr)) {
            KConfigGroup storageGroup = kmailConfig->group(storageModelSelectedMessageStr);
            const QString storageModelSelectedPattern(u"MessageUniqueIdForStorageModel"_s);
            const QStringList storageList = storageGroup.keyList().filter(QRegularExpression(u"MessageUniqueIdForStorageModel\\d+"_s));
            for (const QString &str : storageList) {
                bool found = false;
                const int collectionId = QStringView(str).right(str.length() - storageModelSelectedPattern.length()).toInt(&found);
                const QString oldValue = storageGroup.readEntry(str);
                if (found) {
                    const QString realPath = convertToFullCollectionPath(collectionId);
                    if (!realPath.isEmpty()) {
                        storageGroup.writeEntry(u"%1%2"_s.arg(storageModelSelectedPattern, realPath), oldValue);
                        storageGroup.deleteEntry(str);
                    } else {
                        storageGroup.deleteEntry(str);
                    }
                }
            }
        }

        const QString collectionFolderViewStr(u"CollectionFolderView"_s);
        if (kmailConfig->hasGroup(collectionFolderViewStr)) {
            KConfigGroup favoriteGroup = kmailConfig->group(collectionFolderViewStr);

            const QString currentKey(u"Current"_s);
            convertCollectionToRealPath(favoriteGroup, currentKey);

            const QString expensionKey(u"Expansion"_s);
            convertCollectionListToRealPath(favoriteGroup, expensionKey);
        }

        const QString favoriteCollectionStr(u"FavoriteCollections"_s);
        if (kmailConfig->hasGroup(favoriteCollectionStr)) {
            KConfigGroup favoriteGroup = kmailConfig->group(favoriteCollectionStr);

            const QString favoriteCollectionIdsStr(u"FavoriteCollectionIds"_s);
            convertCollectionIdsToRealPath(favoriteGroup, favoriteCollectionIdsStr);
        }

        const QString favoriteCollectionOrderStr(u"FavoriteCollectionsOrder"_s);
        if (kmailConfig->hasGroup(favoriteCollectionOrderStr)) {
            KConfigGroup favoriteGroup = kmailConfig->group(favoriteCollectionOrderStr);
            // For favorite id for root collection == 0 and we store only folder => c
            const QString favoriteCollectionIdsStr(u"0"_s);
            const QString prefixCollection(u"c"_s);
            convertCollectionIdsToRealPath(favoriteGroup, favoriteCollectionIdsStr, prefixCollection);
        }

        // Event collection
        const QString eventCollectionStr(u"Event"_s);
        if (kmailConfig->hasGroup(eventCollectionStr)) {
            KConfigGroup eventGroup = kmailConfig->group(eventCollectionStr);
            const QString eventLastEventSelectedFolder(u"LastEventSelectedFolder"_s);
            convertCollectionIdsToRealPath(eventGroup, eventLastEventSelectedFolder);
        }

        // Todo collection
        const QString todoCollectionStr(u"Todo"_s);
        if (kmailConfig->hasGroup(todoCollectionStr)) {
            KConfigGroup todoGroup = kmailConfig->group(todoCollectionStr);
            const QString todoLastEventSelectedFolder(u"LastSelectedFolder"_s);
            convertCollectionIdsToRealPath(todoGroup, todoLastEventSelectedFolder);
        }
        // FolderSelectionDialog collection
        const QString folderSelectionCollectionStr(u"FolderSelectionDialog"_s);
        if (kmailConfig->hasGroup(folderSelectionCollectionStr)) {
            KConfigGroup folderSelectionGroup = kmailConfig->group(folderSelectionCollectionStr);
            const QString folderSelectionSelectedFolder(u"LastSelectedFolder"_s);
            convertCollectionIdsToRealPath(folderSelectionGroup, folderSelectionSelectedFolder);
        }

        // Note collection
        const QString noteCollectionStr(u"Note"_s);
        if (kmailConfig->hasGroup(noteCollectionStr)) {
            KConfigGroup noteGroup = kmailConfig->group(noteCollectionStr);
            const QString noteLastEventSelectedFolder(u"LastNoteSelectedFolder"_s);
            convertCollectionIdsToRealPath(noteGroup, noteLastEventSelectedFolder);
        }

        // Convert MessageListTab collection id
        const QString messageListPaneStr(u"MessageListPane"_s);
        if (kmailConfig->hasGroup(messageListPaneStr)) {
            KConfigGroup messageListPaneGroup = kmailConfig->group(messageListPaneStr);
            const int numberOfTab = messageListPaneGroup.readEntry(u"tabNumber"_s, 0);
            for (int i = 0; i < numberOfTab; ++i) {
                KConfigGroup messageListPaneTabGroup = kmailConfig->group(u"MessageListTab%1"_s.arg(i));
                const QString messageListPaneTabFolderStr(u"collectionId"_s);
                convertCollectionIdsToRealPath(messageListPaneTabGroup, messageListPaneTabFolderStr);
            }
        }

        // Automatic Add Contacts
        const QList<uint> listIdentities = listIdentityUoid();
        for (uint identity : listIdentities) {
            const QString groupId = u"Automatic Add Contacts %1"_s.arg(identity);
            if (kmailConfig->hasGroup(groupId)) {
                KConfigGroup identityGroup = kmailConfig->group(groupId);
                const QString automaticAddContactStr(u"Collection"_s);
                convertCollectionIdsToRealPath(identityGroup, automaticAddContactStr);
            }
        }

        // TODO add confirm address too

        // Clean up kmail2rc
        const QString tipOfDaysStr(u"TipOfDay"_s);
        if (kmailConfig->hasGroup(tipOfDaysStr)) {
            kmailConfig->deleteGroup(tipOfDaysStr);
        }
        const QString startupStr(u"Startup"_s);
        if (kmailConfig->hasGroup(startupStr)) {
            kmailConfig->deleteGroup(startupStr);
        }

        const QString search(u"Search"_s);
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

    const QString emailidentitiesStr(u"emailidentities"_s);
    const QString emailidentitiesrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + emailidentitiesStr;
    if (QFileInfo::exists(emailidentitiesrc)) {
        KSharedConfigPtr identity = KSharedConfig::openConfig(emailidentitiesrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *identityConfig = identity->copyTo(tmp.fileName());
        const QStringList accountList = identityConfig->groupList().filter(QRegularExpression(u"Identity #\\d+"_s));
        for (const QString &account : accountList) {
            KConfigGroup group = identityConfig->group(account);
            const QString fcc = u"Fcc"_s;
            if (group.hasKey(fcc)) {
                const QString realPath = convertToFullCollectionPath(group.readEntry(fcc).toLongLong());
                group.writeEntry(fcc, realPath);
            }
            const QString draft = u"Drafts"_s;
            if (group.hasKey(draft)) {
                const QString realPath = convertToFullCollectionPath(group.readEntry(draft).toLongLong());
                group.writeEntry(draft, realPath);
            }
            const QString templates = u"Templates"_s;
            if (group.hasKey(templates)) {
                const QString realPath = convertToFullCollectionPath(group.readEntry(templates).toLongLong());
                group.writeEntry(templates, realPath);
            }
            const QString vcard = u"VCardFile"_s;
            if (group.hasKey(vcard)) {
                const QString vcardFileName = group.readEntry(vcard);
                if (!vcardFileName.isEmpty()) {
                    const int uoid = group.readEntry(u"uoid"_s, -1);
                    QFile file(vcardFileName);
                    if (file.exists()) {
                        const bool fileAdded = archive()->addLocalFile(vcardFileName, Utils::identitiesPath() + QString::number(uoid) + u'/' + file.fileName());
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
        const bool fileAdded = archive()->addLocalFile(tmp.fileName(), Utils::identitiesPath() + u"emailidentities"_s);
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
            if (capabilities.contains(QLatin1StringView("Resource")) && !capabilities.contains(QLatin1StringView("Virtual"))
                && !capabilities.contains(QLatin1StringView("MailTransport"))) {
                const QString identifier = agent.identifier;
                if (identifier.contains(QLatin1StringView("akonadi_maildir_resource_"))
                    || identifier.contains(QLatin1StringView("akonadi_mixedmaildir_resource_"))) {
                    const QString archivePath = Utils::mailsPath() + identifier + u'/';
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
            if (capabilities.contains(QLatin1StringView("Resource")) && !capabilities.contains(QLatin1StringView("Virtual"))
                && !capabilities.contains(QLatin1StringView("MailTransport"))) {
                const QString identifier = agent.identifier;
                // Store just pop3/imap/kolab/gmail account. Store other config when we copy data.
                if (identifier.contains(QLatin1StringView("pop3")) || identifier.contains(QLatin1StringView("imap"))
                    || identifier.contains(QLatin1StringView("_kolab_")) || identifier.contains(QLatin1StringView("_gmail_"))) {
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
