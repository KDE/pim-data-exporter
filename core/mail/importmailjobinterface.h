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

#ifndef ImportMailJobINTERFACE_H
#define ImportMailJobINTERFACE_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
#include <AkonadiCore/Collection>
#include <Akonadi/KMime/SpecialMailCollections>
#include <QStringList>
#include <QHash>
#include <KArchive>
class KArchiveDirectory;
class KArchiveFile;
class ArchiveStorage;
namespace MailTransport {
class Transport;
}
namespace MailCommon {
class MailFilter;
}

class PIMDATAEXPORTER_TESTS_EXPORT SmtpMailTransport
{
    explicit SmtpMailTransport(const KConfigGroup &group);

public:
    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString host() const;
    void setHost(const QString &host);

    Q_REQUIRED_RESULT int port() const;
    void setPort(int port);

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString precommand() const;
    void setPrecommand(const QString &precommand);

    Q_REQUIRED_RESULT bool requiresAuthentication() const;
    void setRequiresAuthentication(bool requiresAuthentication);

    Q_REQUIRED_RESULT bool specifyHostname() const;
    void setSpecifyHostname(bool specifyHostname);

    Q_REQUIRED_RESULT QString localHostname() const;
    void setLocalHostname(const QString &localHostname);

    Q_REQUIRED_RESULT bool specifySenderOverwriteAddress() const;
    void setSpecifySenderOverwriteAddress(bool specifySenderOverwriteAddress);

    Q_REQUIRED_RESULT bool storePassword() const;
    void setStorePassword(bool storePassword);

    Q_REQUIRED_RESULT QString senderOverwriteAddress() const;
    void setSenderOverwriteAddress(const QString &senderOverwriteAddress);

    Q_REQUIRED_RESULT int encryption() const;
    void setEncryption(int encryption);

    Q_REQUIRED_RESULT int authenticationType() const;
    void setAuthenticationType(int authenticationType);

private:
    QString mName;
    QString mHost;
    QString mUserName;
    QString mPrecommand;
    QString mLocalHostname;
    QString mSenderOverwriteAddress;
    int mPort = -1;
    int mEncryption = -1;
    int mAuthenticationType = 1;
    bool mRequiresAuthentication = false;
    bool mSpecifyHostname = false;
    bool mSpecifySenderOverwriteAddress = false;
    bool mStorePassword = false;
};

class PIMDATAEXPORTER_TESTS_EXPORT ImportMailJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportMailJobInterface(QObject *widget, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportMailJobInterface() override;

    void start() override;

    //For debugging
    void importMailTransport(const QString &tempDirName);
protected:
    void slotNextStep() override;

    void convertCollectionStrToAkonadiId(const KSharedConfig::Ptr &config, const QString &groupName, const QString &key);
    virtual void restoreMails();
    void restoreResources();
    virtual Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) = 0;
    virtual void registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId) = 0;
    virtual void synchronizeResource(const QStringList &lst) = 0;
    virtual void importFilters(const QVector<MailCommon::MailFilter *> &filters) = 0;
    virtual Q_REQUIRED_RESULT QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) = 0;
    virtual void addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) = 0;
    virtual void importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) = 0;
    virtual void importSmtpMailTransport(const KConfigGroup &group, int defaultTransport, int transportId) = 0;

    QHash<int, uint> mHashIdentity;
    QHash<int, int> mHashTransport;

private:
    void restoreTransports();
    void restoreConfig();
    void restoreIdentity();

    void importTemplatesConfig(const KArchiveFile *templatesconfiguration, const QString &templatesconfigurationrc, const QString &filename, const QString &prefix);
    void importKmailConfig(const KArchiveFile *kmailsnippet, const QString &kmail2rc, const QString &filename, const QString &prefix);
    void importArchiveConfig(const KArchiveFile *archiveconfiguration, const QString &archiveconfigurationrc, const QString &filename, const QString &prefix);
    void importFolderArchiveConfig(const KArchiveFile *archiveconfiguration, const QString &archiveconfigurationrc, const QString &filename, const QString &prefix);
    void searchAllMailsFiles(const KArchiveDirectory *dir, const QString &prefix);
    void storeMailArchiveResource(const KArchiveDirectory *dir, const QString &prefix);

    void importMailArchiveConfig(const KArchiveFile *archiveconfiguration, const QString &archiveconfigurationrc, const QString &filename, const QString &prefix);
    void mergeLdapConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void mergeKmailSnippetConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void mergeArchiveMailAgentConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void copyArchiveMailAgentConfigGroup(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination);
    void copyMailArchiveConfig(const KSharedConfig::Ptr &archiveConfigOrigin, const KSharedConfig::Ptr &archiveConfigDestination);
    void mergeMailArchiveConfig(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void mergeSieveTemplate(const KArchiveFile *archivefile, const QString &filename, const QString &prefix);
    void importSimpleFilesInDirectory(const QString &relativePath);

    QHash<QString, QString> mHashResources;
    QStringList mFileList;
};

#endif // ImportMailJob_H
