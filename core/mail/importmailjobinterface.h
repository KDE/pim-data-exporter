/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
#include <Akonadi/Collection>
#include <Akonadi/SpecialMailCollections>
#include <KArchive>
#include <QHash>
#include <QStringList>
class KArchiveDirectory;
class KArchiveFile;
class ArchiveStorage;
class SmtpMailTransport;
namespace MailTransport
{
class Transport;
}
namespace MailCommon
{
}

class PIMDATAEXPORTER_TESTS_EXPORT ImportMailJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportMailJobInterface(QObject *widget, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportMailJobInterface() override;

    void start() override;

    // For debugging
    void importMailTransport(const QString &tempDirName);

protected:
    void slotNextStep() override;

    void convertCollectionStrToAkonadiId(const KSharedConfig::Ptr &config, const QString &groupName, const QString &key);
    virtual void restoreMails();
    void restoreResources();
    virtual void registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId) = 0;
    virtual void synchronizeResource(const QStringList &lst) = 0;
    virtual void importFilters(const QString &filename) = 0;
    Q_REQUIRED_RESULT virtual QString configLocation() const = 0;
    Q_REQUIRED_RESULT virtual QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) = 0;
    virtual void addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) = 0;
    virtual void importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) = 0;
    virtual void importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId) = 0;

    Q_REQUIRED_RESULT QString applicationName() const override;

    virtual void importFolderAttributes() = 0;

    QHash<int, uint> mHashIdentity;
    QHash<int, int> mHashTransport;

private:
    void restoreTransports();
    void restoreConfig();
    void restoreIdentity();

    void
    importTemplatesConfig(const KArchiveFile *templatesconfiguration, const QString &templatesconfigurationrc, const QString &filename, const QString &prefix);
    void importKmailConfig(const KArchiveFile *kmailsnippet, const QString &kmail2rc, const QString &filename, const QString &prefix);
    void importArchiveConfig(const KArchiveFile *archiveconfiguration, const QString &archiveconfigurationrc, const QString &filename, const QString &prefix);
    void
    importFolderArchiveConfig(const KArchiveFile *archiveconfiguration, const QString &archiveconfigurationrc, const QString &filename, const QString &prefix);
    void searchAllMailsFiles(const KArchiveDirectory *dir, const QString &prefix);
    void storeMailArchiveResource(const KArchiveDirectory *dir, const QString &prefix);

    void
    importMailArchiveConfig(const KArchiveFile *archiveconfiguration, const QString &archiveconfigurationrc, const QString &filename, const QString &prefix);
    void mergeLdapConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void mergeKmailSnippetConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void mergeArchiveMailAgentConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void copyArchiveMailAgentConfigGroup(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination);
    void copyMailArchiveConfig(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination);
    void mergeMailArchiveConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void mergeUnifiedMailBoxConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void mergeSieveTemplate(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void importSimpleFilesInDirectory(const QString &relativePath);
    void copyUnifiedMailBoxConfig(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination);
    void
    importUnifiedMailBoxConfig(const KArchiveFile *archiveconfiguration, const QString &archiveconfigurationrc, const QString &filename, const QString &prefix);

    QHash<QString, QString> mHashResources;
    QStringList mFileList;
};
