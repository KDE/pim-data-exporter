/*
   SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importalarmjobinterface.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>

#include <KArchive>
#include <KArchiveEntry>
#include <KLocalizedString>
#include <KZip>

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>

ImportAlarmJobInterface::ImportAlarmJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportAlarmJobInterface::~ImportAlarmJobInterface() = default;

void ImportAlarmJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import KAlarm settings..."));
    createProgressDialog(i18n("Import KAlarm settings"));
    mArchiveDirectory = archive()->directory();
    searchAllFiles(mArchiveDirectory, QString(), QStringLiteral("alarm"));
    initializeListStep();
    QTimer::singleShot(0, this, &ImportAlarmJobInterface::slotNextStep);
}

void ImportAlarmJobInterface::slotNextStep()
{
    ++mIndex;
    if (mIndex < mListStep.count()) {
        const Utils::StoredType type = mListStep.at(mIndex);
        if (type == Utils::Resources) {
            restoreResources();
        } else if (type == Utils::Config) {
            restoreConfig();
        } else {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << Q_FUNC_INFO << " not supported type " << type;
            slotNextStep();
        }
    } else {
        Q_EMIT jobFinished();
    }
}

bool ImportAlarmJobInterface::isAConfigFile(const QString &name) const
{
    return name.endsWith(QLatin1String("rc"))
        && (name.contains(QLatin1String("akonadi_kalarm_resource_")) || name.contains(QLatin1String("akonadi_kalarm_dir_resource_")));
}

QString ImportAlarmJobInterface::configLocation() const
{
    return installConfigLocation();
}

QString ImportAlarmJobInterface::applicationName() const
{
    return QStringLiteral("[Alarm]");
}

QString ImportAlarmJobInterface::installConfigLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/');
}

void ImportAlarmJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString kalarmStr(QStringLiteral("kalarmrc"));
    const KArchiveEntry *kalarmrcentry = mArchiveDirectory->entry(Utils::configsPath() + kalarmStr);
    if (kalarmrcentry && kalarmrcentry->isFile()) {
        const auto kalarmrcFile = static_cast<const KArchiveFile *>(kalarmrcentry);
        const QString searchExistingkalarmrc = configLocation() + kalarmStr;
        const QString installPathkalarmrc = installConfigLocation() + kalarmStr;
        if (QFileInfo::exists(searchExistingkalarmrc)) {
            if (overwriteConfigMessageBox(kalarmStr)) {
                importkalarmConfig(kalarmrcFile, installPathkalarmrc, kalarmStr, Utils::configsPath());
            }
        } else {
            importkalarmConfig(kalarmrcFile, installPathkalarmrc, kalarmStr, Utils::configsPath());
        }
    }
    restoreUiRcFile(QStringLiteral("kalarmui.rc"), QStringLiteral("kalarm"));
    emitInfo(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportAlarmJobInterface::slotNextStep);
}

void ImportAlarmJobInterface::importkalarmConfig(const KArchiveFile *kalarmFile, const QString &kalarmrc, const QString &filename, const QString &prefix)
{
    copyToFile(kalarmFile, kalarmrc, filename, prefix);
    KSharedConfig::Ptr kalarmConfig = KSharedConfig::openConfig(kalarmrc);

    convertCollectionListStrToAkonadiId(kalarmConfig, QStringLiteral("Collections"), QStringLiteral("FavoriteCollectionIds"), false);

    kalarmConfig->sync();
}

void ImportAlarmJobInterface::restoreResources()
{
    emitInfo(i18n("Restore resources..."));
    setProgressDialogLabel(i18n("Restore resources..."));
    increaseProgressDialog();
    QStringList listResource;
    listResource << restoreResourceFile(QStringLiteral("akonadi_kalarm_resource"), Utils::alarmPath(), Utils::storeAlarm(), false);
    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(Utils::alarmPath());
        const QString copyToDirName(mTempDirName + QLatin1Char('/') + Utils::alarmPath());
        QDir().mkpath(copyToDirName);

        const int numberOfResourceFile = mListResourceFile.size();
        for (int i = 0; i < numberOfResourceFile; ++i) {
            ResourceFiles value = mListResourceFile.at(i);
            QMap<QString, QVariant> settings;
            if (value.akonadiConfigFile.contains(QLatin1String("akonadi_kalarm_dir_resource_"))
                || value.akonadiConfigFile.contains(QLatin1String("akonadi_kalarm_resource_"))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                    const auto file = static_cast<const KArchiveFile *>(fileResouceEntry);
                    copyArchiveFileTo(file, copyToDirName);
                    QString resourceName(file->name());

                    QString filename(resourceName);
                    // TODO adapt filename otherwise it will use all the time the
                    // same filename.
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " filename :" << filename;

                    KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + QLatin1Char('/') + resourceName);

                    const QString newUrl = adaptResourcePath(resourceConfig, Utils::storeAlarm());
                    QFileInfo newUrlInfo(newUrl);
                    const QString dataFile = value.akonadiResources;
                    const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                    bool isDirResource = value.akonadiConfigFile.contains(QLatin1String("akonadi_kalarm_dir_resource_"));
                    if (dataResouceEntry->isFile()) {
                        const auto fileEntry = static_cast<const KArchiveFile *>(dataResouceEntry);
                        // TODO  adapt directory name too
                        extractZipFile(fileEntry, copyToDirName, newUrlInfo.path(), isDirResource);
                    }
                    settings.insert(QStringLiteral("Path"), newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                            const auto akonadiAgentConfigEntryFile = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                            copyArchiveFileTo(akonadiAgentConfigEntryFile, copyToDirName);
                            resourceName = akonadiAgentConfigEntryFile->name();
                            filename = Utils::akonadiAgentName(copyToDirName + QLatin1Char('/') + resourceName);
                        }
                    }
                    const QString archiveNameType = isDirResource ? QStringLiteral("akonadi_kalarm_dir_resource") : QStringLiteral("akonadi_kalarm_resource");
                    const QString newResource = createResource(archiveNameType, filename, settings, true);
                    infoAboutNewResource(newResource);
                    listResource << newResource;
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " newResource" << newResource;
                }
            }
        }
    }
    // It's maildir support. Need to add support
    synchronizeResource(listResource);
}

#include "moc_importalarmjobinterface.cpp"
