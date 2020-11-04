/*
   SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractimportexportjob.h"
#include "archivestorage.h"
#include "backupresourcefilejobbase.h"
#include "backupresourcefilejobimpl.h"
#include "importexportprogressindicatorbase.h"
#include "storeresourcejob.h"
#include "synchronizeresourcejob.h"

#include <PimCommonAkonadi/CreateResource>
#include <AkonadiCore/AgentInstance>

#include <KZip>
#include <QTemporaryDir>
#include <KLocalizedString>

#include <AkonadiCore/ServerManager>

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <KConfigGroup>

int AbstractImportExportJob::sArchiveVersion = -1;

AbstractImportExportJob::AbstractImportExportJob(QObject *parent, ArchiveStorage *archiveStorage, Utils::StoredTypes typeSelected, int numberOfStep)
    : QObject(parent)
    , mTypeSelected(typeSelected)
    , mArchiveStorage(archiveStorage)
    , mNumberOfStep(numberOfStep)
    , mImportExportProgressIndicator(new ImportExportProgressIndicatorBase(this))
{
    mImportExportProgressIndicator->setNumberOfStep(numberOfStep);
    connect(mImportExportProgressIndicator, &ImportExportProgressIndicatorBase::info, this, &AbstractImportExportJob::info);
}

AbstractImportExportJob::~AbstractImportExportJob()
{
    delete mCreateResource;
    delete mTempDir;
}

void AbstractImportExportJob::createProgressDialog(const QString &title)
{
    mImportExportProgressIndicator->createProgressDialog(title);
    connect(mImportExportProgressIndicator, &ImportExportProgressIndicatorBase::canceled, this, &AbstractImportExportJob::slotTaskCanceled);
}

void AbstractImportExportJob::slotTaskCanceled()
{
    Q_EMIT error(i18n("Task Canceled"));
    Q_EMIT jobFinished();
}

bool AbstractImportExportJob::wasCanceled() const
{
    return mImportExportProgressIndicator->wasCanceled();
}

void AbstractImportExportJob::increaseProgressDialog()
{
    mImportExportProgressIndicator->increaseProgressDialog();
}

void AbstractImportExportJob::setProgressDialogLabel(const QString &text)
{
    mImportExportProgressIndicator->setProgressDialogLabel(text);
}

ImportExportProgressIndicatorBase *AbstractImportExportJob::importExportProgressIndicator() const
{
    return mImportExportProgressIndicator;
}

void AbstractImportExportJob::setImportExportProgressIndicator(ImportExportProgressIndicatorBase *importExportProgressIndicator)
{
    delete mImportExportProgressIndicator;
    mImportExportProgressIndicator = importExportProgressIndicator;
    mImportExportProgressIndicator->setNumberOfStep(mNumberOfStep);
}

KZip *AbstractImportExportJob::archive() const
{
    return mArchiveStorage->archive();
}

void AbstractImportExportJob::backupUiRcFile(const QString &configFileName, const QString &application)
{
    const QString configrcStr(configFileName);
    const QString configrc = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/kxmlgui5/") + application + QLatin1Char('/') + configrcStr;
    if (QFileInfo::exists(configrc)) {
        backupFile(configrc, Utils::configsPath(), configrcStr);
    }
}

void AbstractImportExportJob::backupConfigFile(const QString &configFileName)
{
    const QString configrcStr(configFileName);
    const QString configrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + configrcStr;
    if (QFileInfo::exists(configrc)) {
        backupFile(configrc, Utils::configsPath(), configrcStr);
    }
}

void AbstractImportExportJob::backupFile(const QString &filename, const QString &path, const QString &storedName)
{
    if (QFileInfo::exists(filename)) {
        const bool fileAdded = archive()->addLocalFile(filename, path + storedName);
        if (fileAdded) {
            Q_EMIT info(i18n("\"%1\" backup done.", path + storedName));
        } else {
            Q_EMIT error(i18n("\"%1\" cannot be exported.", path + storedName));
        }
    } else {
        Q_EMIT error(i18n("\"%1\" does not exist.", filename));
    }
}

int AbstractImportExportJob::mergeConfigMessageBox(const QString &configName) const
{
    return mImportExportProgressIndicator->mergeConfigMessageBox(configName);
}

bool AbstractImportExportJob::overwriteConfigMessageBox(const QString &configName) const
{
    return mImportExportProgressIndicator->overwriteConfigMessageBox(configName);
}

void AbstractImportExportJob::overwriteDirectory(const QString &path, const KArchiveEntry *entry)
{
    if (QDir(path).exists()) {
        if (overwriteDirectoryMessageBox(path)) {
            const auto *dirEntry = static_cast<const KArchiveDirectory *>(entry);
            if (!dirEntry->copyTo(path)) {
                qCWarning(PIMDATAEXPORTERCORE_LOG) << "directory cannot overwrite to " << path;
            }
        }
    } else {
        const auto *dirEntry = static_cast<const KArchiveDirectory *>(entry);
        if (dirEntry->copyTo(path)) {
            qCWarning(PIMDATAEXPORTERCORE_LOG) << "directory cannot overwrite to " << path;
        }
    }
}

void AbstractImportExportJob::searchAllFiles(const KArchiveDirectory *dir, const QString &prefix, const QString &searchEntryName)
{
    const QStringList lst = dir->entries();
    for (const QString &entryName : lst) {
        const KArchiveEntry *entry = dir->entry(entryName);
        if (entry && entry->isDirectory()) {
            const QString newPrefix = (prefix.isEmpty() ? prefix : prefix + QLatin1Char('/')) + entryName;
            if (entryName == searchEntryName) {
                storeArchiveInfoResources(static_cast<const KArchiveDirectory *>(entry), entryName);
            } else {
                searchAllFiles(static_cast<const KArchiveDirectory *>(entry), newPrefix, searchEntryName);
            }
        }
    }
}

void AbstractImportExportJob::storeArchiveInfoResources(const KArchiveDirectory *dir, const QString &prefix)
{
    const QStringList lst = dir->entries();
    for (const QString &entryName : lst) {
        const KArchiveEntry *entry = dir->entry(entryName);
        if (entry && entry->isDirectory()) {
            const auto *resourceDir = static_cast<const KArchiveDirectory *>(entry);
            const QStringList lst = resourceDir->entries();

            if (lst.count() >= 2) {
                const QString archPath(prefix + QLatin1Char('/') + entryName + QLatin1Char('/'));
                ResourceFiles files;
                for (const QString &name : lst) {
                    if (isAConfigFile(name)) {
                        files.akonadiConfigFile = archPath + name;
                    } else if (name.startsWith(Utils::prefixAkonadiConfigFile())) {
                        files.akonadiAgentConfigFile = archPath + name;
                    } else {
                        files.akonadiResources = archPath + name;
                    }
                }
                files.debug();
                mListResourceFile.append(files);
            } else {
                qCWarning(PIMDATAEXPORTERCORE_LOG) << " Problem in archive. number of file " << lst.count();
            }
        }
    }
    std::sort(mListResourceFile.begin(), mListResourceFile.end());
}

bool AbstractImportExportJob::isAConfigFile(const QString &name) const
{
    Q_UNUSED(name);
    //Redefine in subclass
    return true;
}

bool AbstractImportExportJob::overwriteDirectoryMessageBox(const QString &directory) const
{
    return mImportExportProgressIndicator->overwriteDirectoryMessageBox(directory);
}

qint64 AbstractImportExportJob::convertRealPathToCollection(KConfigGroup &group, const QString &currentKey, bool addCollectionPrefix)
{
    qint64 colId = -1;
    if (group.hasKey(currentKey)) {
        const QString path = group.readEntry(currentKey);
        if (!path.isEmpty()) {
            const Akonadi::Collection::Id id = convertPathToId(path);
            if (id != -1) {
                if (addCollectionPrefix) {
                    group.writeEntry(currentKey, QStringLiteral("c%1").arg(id));
                } else {
                    group.writeEntry(currentKey, id);
                }
            } else {
                group.deleteEntry(currentKey);
            }
            colId = id;
        }
    }
    return colId;
}

Akonadi::Collection::Id AbstractImportExportJob::convertPathToId(const QString &path)
{
    if (path.isEmpty()) {
        return -1;
    }
    Akonadi::Collection::Id val = mHashConvertPathCollectionId.value(path, -1);
    if (val != -1) {
        return val;
    }
    const Akonadi::Collection::Id id = convertFolderPathToCollectionId(path);
    if (id != -1) {
        mHashConvertPathCollectionId.insert(path, id);
    }
    return id;
}

void AbstractImportExportJob::initializeImportJob()
{
    if (mTempDir) {
        qCDebug(PIMDATAEXPORTERCORE_LOG) << " initializeImportJob already called";
    } else {
        mTempDir = new QTemporaryDir();
        mTempDirName = mTempDir->path();
        mCreateResource = new PimCommon::CreateResource();
        connect(mCreateResource, &PimCommon::CreateResource::createResourceInfo, this, &AbstractImportExportJob::info);
        connect(mCreateResource, &PimCommon::CreateResource::createResourceError, this, &AbstractImportExportJob::error);
    }
}

void AbstractImportExportJob::copyToDirectory(const KArchiveEntry *entry, const QString &dest)
{
    const auto *subfolderDir = static_cast<const KArchiveDirectory *>(entry);
    if (!subfolderDir->copyTo(dest)) {
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "directory cannot copy to " << dest;
    }
    Q_EMIT info(i18n("\"%1\" was copied.", dest));
}

void AbstractImportExportJob::copyToFile(const KArchiveFile *archivefile, const QString &dest, const QString &filename, const QString &prefix)
{
    QDir dir(mTempDirName);
    const QString copyToDirName(mTempDirName + QLatin1Char('/') + prefix);
    const bool created = dir.mkpath(copyToDirName);
    if (!created) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << " directory :" << prefix << " not created";
    }

    if (!archivefile->copyTo(copyToDirName)) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "file " << filename << " can not copy to " << dest;
    }
    QFile file;
    file.setFileName(copyToDirName + QLatin1Char('/') + filename);

    //QFile doesn't overwrite => remove old file before
    //qCDebug(PIMDATAEXPORTERCORE_LOG)<<" dest "<<dest;
    //qCDebug(PIMDATAEXPORTERCORE_LOG)<<" file "<<file.fileName();
    QFile destination(dest);
    if (destination.exists()) {
        destination.remove();
    }
    QFileInfo destFileInfo(dest);

    QDir().mkpath(destFileInfo.path());
    if (!file.copy(dest)) {
        mImportExportProgressIndicator->showErrorMessage(i18n("File \"%1\" cannot be copied to \"%2\".", filename, dest), i18n("Copy file"));
    } else {
        Q_EMIT info(i18n("\"%1\" was restored.", filename));
    }
}

void AbstractImportExportJob::backupResourceFile(const QString &identifier, const QString &defaultPath)
{
    auto *job = new BackupResourceFileJobImpl(this);
    job->setDefaultPath(defaultPath);
    job->setIdentifier(identifier);
    job->setZip(archive());
    connect(job, &BackupResourceFileJobImpl::error, this, &AbstractImportExportJob::error);
    connect(job, &BackupResourceFileJobImpl::info, this, &AbstractImportExportJob::info);
    job->start();
}

QStringList AbstractImportExportJob::restoreResourceFile(const QString &resourceBaseName, const QString &defaultPath, const QString &storePath, bool overwriteResources)
{
    QStringList resourceToSync;
    //TODO fix sync config after created a resource
    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(defaultPath);
        const QString copyToDirName(mTempDirName + QLatin1Char('/') + defaultPath);
        QDir().mkpath(copyToDirName);

        for (int i = 0, total = mListResourceFile.size(); i < total; ++i) {
            ResourceFiles value = mListResourceFile.at(i);
            QMap<QString, QVariant> settings;
            if (value.akonadiConfigFile.contains(resourceBaseName + QLatin1Char('_'))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                    const auto *file = static_cast<const KArchiveFile *>(fileResouceEntry);
                    if (!file->copyTo(copyToDirName)) {
                        qCWarning(PIMDATAEXPORTERCORE_LOG) << "file " << value.akonadiConfigFile << " can not copy to " << copyToDirName;
                    }
                    QString resourceName(file->name());

                    QString filename(file->name());
                    //TODO adapt filename otherwise it will use all the time the same filename.
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " filename :" << filename;

                    KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + QLatin1Char('/') + resourceName);

                    QString newUrl = adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
                    const QString dataFile = value.akonadiResources;
                    const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                    if (dataResouceEntry->isFile()) {
                        const auto *file = static_cast<const KArchiveFile *>(dataResouceEntry);
                        if (!file->copyTo(newUrl)) {
                            qCWarning(PIMDATAEXPORTERCORE_LOG) << "file " << dataFile << " can not copy to " << newUrl;
                        }
                    }
                    settings.insert(QStringLiteral("Path"), newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                            const auto *file = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                            file->copyTo(copyToDirName);
                            resourceName = file->name();
                            const QString configPath = copyToDirName + QLatin1Char('/') + resourceName;
                            filename = Utils::akonadiAgentName(configPath);
                        }
                    }

                    addSpecificResourceSettings(resourceConfig, resourceBaseName, settings);

                    const QString newResource = createResource(resourceBaseName, filename, settings);
                    infoAboutNewResource(newResource);
                    resourceToSync << newResource;
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " newResource" << newResource;
                }
            }
        }
        Q_EMIT info(i18n("Resources restored."));
    } else {
        Q_EMIT error(i18n("No resources files found."));
        qDebug() << " resourceBaseName " << resourceBaseName;
    }
    return resourceToSync;
}

void AbstractImportExportJob::addSpecificResourceSettings(const KSharedConfig::Ptr /*resourceConfig*/ &, const QString & /*resourceName*/, QMap<QString, QVariant> & /*settings*/)
{
    //Redefine it in subclass
}

bool AbstractImportExportJob::copyArchiveFileTo(const KArchiveFile *file, const QString &destination)
{
    const bool result = file->copyTo(destination);
    if (!result) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "file " << file->name() << " can not copy to " << destination;
    }
    return result;
}

void AbstractImportExportJob::extractZipFile(const KArchiveFile *file, const QString &source, const QString &destination, bool isStoredAsZippedArchive)
{
    if (!file->copyTo(source)) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "file " << file->name() << " can not copy to " << source;
    }
    QDir dest(destination);
    if (!dest.exists()) {
        dest.mkpath(destination);
    }
    if (isStoredAsZippedArchive) {
        QString errorMsg;
        KZip *zip = Utils::openZip(source + QLatin1Char('/') + file->name(), errorMsg);
        if (zip) {
            const KArchiveDirectory *zipDir = zip->directory();
            const QStringList lst = zipDir->entries();
            for (const QString &entryName : lst) {
                const KArchiveEntry *entry = zipDir->entry(entryName);
                if (entry) {
                    if (entry->isDirectory()) {
                        const auto *dir = static_cast<const KArchiveDirectory *>(entry);
                        dir->copyTo(destination + QLatin1Char('/') + dir->name(), true);
                    } else if (entry->isFile()) {
                        const auto *dir = static_cast<const KArchiveFile *>(entry);
                        dir->copyTo(destination);
                    }
                }
                qApp->processEvents();
            }
            delete zip;
        } else {
            Q_EMIT error(errorMsg);
        }
    } else {
        QFile archiveFile(source + QLatin1Char('/') + file->name());
        if (!archiveFile.copy(destination + QLatin1Char('/') + file->name())) {
            Q_EMIT error(i18n("Unable to copy file %1", file->name()));
        }
    }
}

void AbstractImportExportJob::restoreUiRcFile(const QString &configNameStr, const QString &applicationName)
{
    const KArchiveEntry *configNameentry = mArchiveDirectory->entry(Utils::configsPath() + configNameStr);
    if (configNameentry && configNameentry->isFile()) {
        const auto *configNameconfiguration = static_cast<const KArchiveFile *>(configNameentry);
        const QString configNamerc = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/kxmlgui5/") + applicationName + QLatin1Char('/') + configNameStr;
        if (QFileInfo::exists(configNamerc)) {
            if (overwriteConfigMessageBox(configNameStr)) {
                copyToFile(configNameconfiguration, configNamerc, configNameStr, Utils::configsPath());
            }
        } else {
            copyToFile(configNameconfiguration, configNamerc, configNameStr, Utils::configsPath());
        }
    }
}

void AbstractImportExportJob::restoreConfigFile(const QString &configNameStr)
{
    const KArchiveEntry *configNameentry = mArchiveDirectory->entry(Utils::configsPath() + configNameStr);
    if (configNameentry && configNameentry->isFile()) {
        const auto *configNameconfiguration = static_cast<const KArchiveFile *>(configNameentry);
        const QString configNamerc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + configNameStr;
        if (QFileInfo::exists(configNamerc)) {
            //TODO 4.12 allow to merge config.
            if (overwriteConfigMessageBox(configNameStr)) {
                copyToFile(configNameconfiguration, configNamerc, configNameStr, Utils::configsPath());
            }
        } else {
            copyToFile(configNameconfiguration, configNamerc, configNameStr, Utils::configsPath());
        }
    }
}

void AbstractImportExportJob::infoAboutNewResource(const QString &resourceName)
{
    Q_EMIT info(i18n("Resource \'%1\' created.", resourceName));
}

int AbstractImportExportJob::archiveVersion()
{
    return sArchiveVersion;
}

void AbstractImportExportJob::setArchiveVersion(int version)
{
    sArchiveVersion = version;
}

void AbstractImportExportJob::slotSynchronizeInstanceFailed(const QString &instance)
{
    Q_EMIT error(i18n("Failed to synchronize %1.", instance));
}

void AbstractImportExportJob::slotSynchronizeInstanceDone(const QString &name, const QString &identifier)
{
    Q_EMIT info(i18n("Resource %1 synchronized.", name));
    Q_EMIT needSynchronizeResource(name, identifier);
}

void AbstractImportExportJob::slotAllResourceSynchronized()
{
    Q_EMIT info(i18n("All resources synchronized."));
    slotNextStep();
}

void AbstractImportExportJob::slotNextStep()
{
    //Implement in sub class.
}

void AbstractImportExportJob::startSynchronizeResources(const QStringList &listResourceToSync)
{
    Q_EMIT info(i18n("Start synchronizing..."));
    auto *job = new SynchronizeResourceJob(this);
    job->setListResources(listResourceToSync);
    connect(job, &SynchronizeResourceJob::synchronizationFinished, this, &AbstractImportExportJob::slotAllResourceSynchronized);
    connect(job, &SynchronizeResourceJob::synchronizationInstanceDone, this, &AbstractImportExportJob::slotSynchronizeInstanceDone);
    connect(job, &SynchronizeResourceJob::synchronizationInstanceFailed, this, &AbstractImportExportJob::slotSynchronizeInstanceFailed);
    job->start();
}

void AbstractImportExportJob::initializeListStep()
{
    if (mTypeSelected & Utils::MailTransport) {
        mListStep << Utils::MailTransport;
    }
    if (mTypeSelected & Utils::Mails) {
        mListStep << Utils::Mails;
    }
    if (mTypeSelected & Utils::Resources) {
        mListStep << Utils::Resources;
    }
    if (mTypeSelected & Utils::Identity) {
        mListStep << Utils::Identity;
    }
    if (mTypeSelected & Utils::Config) {
        mListStep << Utils::Config;
    }
    if (mTypeSelected & Utils::Data) {
        mListStep << Utils::Data;
    }
}

void AbstractImportExportJob::storeDirectory(const QString &subDirectory)
{
    const QDir directoryToStore(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + subDirectory);
    if (directoryToStore.exists()) {
        const bool templateDirAdded = archive()->addLocalDirectory(directoryToStore.path(), Utils::dataPath() + subDirectory);
        if (templateDirAdded) {
            Q_EMIT info(i18n("Directory \"%1\" added to backup file.", directoryToStore.path()));
        } else {
            Q_EMIT error(i18n("Directory \"%1\" cannot be added to backup file.", directoryToStore.path()));
        }
    }
}

void AbstractImportExportJob::importDataSubdirectory(const QString &subdirectoryRelativePath)
{
    const KArchiveEntry *themeEntry = mArchiveDirectory->entry(Utils::dataPath() + subdirectoryRelativePath);
    if (themeEntry && themeEntry->isDirectory()) {
        const auto *themeDir = static_cast<const KArchiveDirectory *>(themeEntry);
        const QStringList lst = themeDir->entries();
        for (const QString &entryName : lst) {
            const KArchiveEntry *entry = themeDir->entry(entryName);
            if (entry && entry->isDirectory()) {
                QString subFolderName = entryName;
                const QString topLevelPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + subdirectoryRelativePath;
                QDir themeDirectory(topLevelPath + QStringLiteral("/%1").arg(entryName));
                int i = 1;
                while (themeDirectory.exists()) {
                    subFolderName = entryName + QStringLiteral("_%1").arg(i);
                    themeDirectory = QDir(topLevelPath + QStringLiteral("/%1").arg(subFolderName));
                    ++i;
                }
                copyToDirectory(entry, topLevelPath + QStringLiteral("/%1").arg(subFolderName));
            }
        }
    }
}

void AbstractImportExportJob::convertCollectionListStrToAkonadiId(const KSharedConfig::Ptr &config, const QString &groupName, const QString &key, bool addCollectionPrefix)
{
    if (config->hasGroup(groupName)) {
        KConfigGroup group = config->group(groupName);
        convertRealPathToCollectionList(group, key, addCollectionPrefix);
    }
}

void AbstractImportExportJob::convertRealPathToCollectionList(KConfigGroup &group, const QString &currentKey, bool addCollectionPrefix)
{
    if (group.hasKey(currentKey)) {
        const QStringList listExpension = group.readEntry(currentKey, QStringList());
        QStringList result;
        if (!listExpension.isEmpty()) {
            for (const QString &collection : listExpension) {
                const Akonadi::Collection::Id id = convertPathToId(collection);
                if (id != -1) {
                    if (addCollectionPrefix) {
                        result << QStringLiteral("c%1").arg(id);
                    } else {
                        result << QStringLiteral("%1").arg(id);
                    }
                }
            }
            if (result.isEmpty()) {
                group.deleteEntry(currentKey);
            } else {
                group.writeEntry(currentKey, result);
            }
        }
    }
}

void AbstractImportExportJob::setTempDirName(const QString &tempDirName)
{
    mTempDirName = tempDirName;
}
