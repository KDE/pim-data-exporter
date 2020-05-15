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

#ifndef ABSTRACTIMPORTEXPORTJOB_H
#define ABSTRACTIMPORTEXPORTJOB_H

#include <QObject>
#include "utils.h"
#include <AkonadiCore/Collection>
#include "pimdataexporter_export.h"
#include <QStringList>

class ArchiveStorage;
class KArchiveDirectory;
class QTemporaryDir;
class KZip;
class KArchiveFile;
class KArchiveEntry;

namespace KIdentityManagement {
class Identity;
class IdentityManager;
}

namespace PimCommon {
class CreateResource;
}
class ImportExportProgressIndicatorBase;
class PIMDATAEXPORTER_EXPORT AbstractImportExportJob : public QObject
{
    Q_OBJECT
public:
    explicit AbstractImportExportJob(QObject *parent, ArchiveStorage *archiveStorage, Utils::StoredTypes typeSelected, int numberOfStep);
    ~AbstractImportExportJob();

    virtual void start() = 0;

    Q_REQUIRED_RESULT bool wasCanceled() const;

    static int archiveVersion();
    static void setArchiveVersion(int version);

    void setImportExportProgressIndicator(ImportExportProgressIndicatorBase *importExportProgressIndicator);

    ImportExportProgressIndicatorBase *importExportProgressIndicator() const;

Q_SIGNALS:
    void info(const QString &);
    void error(const QString &);
    void title(const QString &);
    void endLine();
    void needSynchronizeResource(const QString &name, const QString &identifier);
    void jobFinished();

protected:
    virtual void slotNextStep();

protected:
    Q_REQUIRED_RESULT bool copyArchiveFileTo(const KArchiveFile *file, const QString &destination);
    void initializeListStep();
    virtual void startSynchronizeResources(const QStringList &listResourceToSync);
    void infoAboutNewResource(const QString &resourceName);
    void copyToDirectory(const KArchiveEntry *entry, const QString &dest);
    void extractZipFile(const KArchiveFile *file, const QString &source, const QString &destination, bool isStoredAsZippedArchive = true);

    Q_REQUIRED_RESULT qint64 convertRealPathToCollection(KConfigGroup &group, const QString &currentKey, bool addCollectionPrefix = false);
    void copyToFile(const KArchiveFile *archivefile, const QString &dest, const QString &filename, const QString &prefix);
    void initializeImportJob();
    void backupFile(const QString &filename, const QString &path, const QString &storedName);
    void backupConfigFile(const QString &configFileName);
    void backupUiRcFile(const QString &configFileName, const QString &applicationName);
    void restoreUiRcFile(const QString &configNameStr, const QString &applicationName);

    Q_REQUIRED_RESULT int mergeConfigMessageBox(const QString &configName) const;
    Q_REQUIRED_RESULT bool overwriteConfigMessageBox(const QString &configName) const;
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertPathToId(const QString &path);

    void backupResourceFile(const QString &agentIdentifier, const QString &defaultPath);

    Q_REQUIRED_RESULT QStringList restoreResourceFile(const QString &resourceName, const QString &defaultPath, const QString &storePath, bool overwriteResources = false);
    virtual void addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings);
    void restoreConfigFile(const QString &configNameStr);
    Q_REQUIRED_RESULT bool overwriteDirectoryMessageBox(const QString &directory) const;
    void overwriteDirectory(const QString &path, const KArchiveEntry *entry);
    virtual bool isAConfigFile(const QString &name) const;
    void searchAllFiles(const KArchiveDirectory *dir, const QString &prefix, const QString &searchEntryName);
    void storeArchiveInfoResources(const KArchiveDirectory *dir, const QString &prefix);

    Q_REQUIRED_RESULT KZip *archive() const;

    void increaseProgressDialog();
    void createProgressDialog(const QString &title = QString());

    void setProgressDialogLabel(const QString &text);
    void storeDirectory(const QString &subDirectory);
    void importDataSubdirectory(const QString &subdirectoryRelativePath);

    void convertCollectionListStrToAkonadiId(const KSharedConfig::Ptr &config, const QString &groupName, const QString &key, bool addCollectionPrefix);
    void convertRealPathToCollectionList(KConfigGroup &group, const QString &currentKey, bool addCollectionPrefix = true);
    virtual Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) = 0;
    virtual QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) = 0;

    QHash<QString, Akonadi::Collection::Id> mHashConvertPathCollectionId;
    QVector<ResourceFiles> mListResourceFile;

    QString mTempDirName;
    Utils::StoredTypes mTypeSelected;
    ArchiveStorage *mArchiveStorage = nullptr;
    KIdentityManagement::IdentityManager *mIdentityManager = nullptr;
    QTemporaryDir *mTempDir = nullptr;
    const KArchiveDirectory *mArchiveDirectory = nullptr;
    int mNumberOfStep = -1;
    PimCommon::CreateResource *mCreateResource = nullptr;
    QStringList mAgentPaths;
    QVector<Utils::StoredType> mListStep;
    QVector<Utils::AkonadiInstanceInfo> mAkonadiInstanceInfo;
    int mIndex = -1;
    static int sArchiveVersion;

private:
    void slotAllResourceSynchronized();
    void slotSynchronizeInstanceDone(const QString &, const QString &identifier);
    void slotSynchronizeInstanceFailed(const QString &instance);
    void slotTaskCanceled();

    ImportExportProgressIndicatorBase *mImportExportProgressIndicator = nullptr;
};

#endif // ABSTRACTIMPORTEXPORTJOB_H
