/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include "utils.h"
#include <Akonadi/Collection>
#include <QObject>
#include <QStringList>

class ArchiveStorage;
class KArchiveDirectory;
class QTemporaryDir;
class KZip;
class KArchiveFile;
class KArchiveEntry;

namespace PimCommon
{
class CreateResource;
}
class ImportExportProgressIndicatorBase;
class PIMDATAEXPORTER_EXPORT AbstractImportExportJob : public QObject
{
    Q_OBJECT
public:
    friend class ImportMailFolderAttributeJob;
    explicit AbstractImportExportJob(QObject *parent, ArchiveStorage *archiveStorage, Utils::StoredTypes typeSelected, int numberOfStep);
    ~AbstractImportExportJob() override;

    virtual void start() = 0;

    Q_REQUIRED_RESULT bool wasCanceled() const;

    static int archiveVersion();
    static void setArchiveVersion(int version);

    void setImportExportProgressIndicator(ImportExportProgressIndicatorBase *importExportProgressIndicator);

    ImportExportProgressIndicatorBase *importExportProgressIndicator() const;

    // Only for test
    void setTempDirName(const QString &tempDirName);

Q_SIGNALS:
    void info(const QString &);
    void error(const QString &);
    void title(const QString &);
    void endLine();
    void needSynchronizeResource(const QString &name, const QString &identifier);
    void jobFinished();
    void exportAttributeDone();

protected:
    virtual void slotNextStep();

protected:
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

    void backupResourceFile(const QString &agentIdentifier, const QString &defaultPath);

    Q_REQUIRED_RESULT QStringList restoreResourceFile(const QString &resourceName,
                                                      const QString &defaultPath,
                                                      const QString &storePath,
                                                      bool overwriteResources = false);
    virtual void addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings);
    void restoreConfigFile(const QString &configNameStr);
    Q_REQUIRED_RESULT bool overwriteDirectoryMessageBox(const QString &directory) const;
    void overwriteDirectory(const QString &path, const KArchiveEntry *entry);
    virtual bool isAConfigFile(const QString &name) const;
    void searchAllFiles(const KArchiveDirectory *dir, const QString &prefix, const QString &searchEntryName);
    void storeArchiveInfoResources(const KArchiveDirectory *dir, const QString &prefix);

    bool copyArchiveFileTo(const KArchiveFile *file, const QString &destination);
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertPathToId(const QString &path);

    Q_REQUIRED_RESULT KZip *archive() const;

    void increaseProgressDialog();
    void createProgressDialog(const QString &title = QString());

    void setProgressDialogLabel(const QString &text);
    void storeDirectory(const QString &subDirectory);
    void importDataSubdirectory(const QString &subdirectoryRelativePath);

    void convertCollectionListStrToAkonadiId(const KSharedConfig::Ptr &config, const QString &groupName, const QString &key, bool addCollectionPrefix);
    void convertRealPathToCollectionList(KConfigGroup &group, const QString &currentKey, bool addCollectionPrefix = true);
    Q_REQUIRED_RESULT virtual Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) = 0;
    virtual QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) = 0;
    Q_REQUIRED_RESULT virtual QString
    createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) = 0;

    QHash<QString, Akonadi::Collection::Id> mHashConvertPathCollectionId;
    QVector<ResourceFiles> mListResourceFile;

    QString mTempDirName;
    const Utils::StoredTypes mTypeSelected;
    ArchiveStorage *const mArchiveStorage;
    QTemporaryDir *mTempDir = nullptr;
    const KArchiveDirectory *mArchiveDirectory = nullptr;
    const int mNumberOfStep = -1;
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
