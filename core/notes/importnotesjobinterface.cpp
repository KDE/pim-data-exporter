/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importnotesjobinterface.h"
#include "archivestorage.h"

#include <KArchive>
#include <KLocalizedString>

#include <KZip>
#include <KConfigGroup>

#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>

ImportNotesJobInterface::ImportNotesJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportNotesJobInterface::~ImportNotesJobInterface()
{
}

void ImportNotesJobInterface::slotNextStep()
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

void ImportNotesJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import KNotes settings..."));
    mArchiveDirectory = archive()->directory();
    searchAllFiles(mArchiveDirectory, QString(), QStringLiteral("note"));
    createProgressDialog(i18n("Import KNotes settings"));
    initializeListStep();
    QTimer::singleShot(0, this, &ImportNotesJobInterface::slotNextStep);
}

bool ImportNotesJobInterface::isAConfigFile(const QString &name) const
{
    return name.endsWith(QLatin1String("rc")) && (name.contains(QLatin1String("akonadi_akonotes_resource_")));
}

QString ImportNotesJobInterface::configLocation() const
{
    return installConfigLocation();
}

QString ImportNotesJobInterface::installConfigLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/');
}

void ImportNotesJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString knotesStr(QStringLiteral("knotesrc"));
    restoreConfigFile(knotesStr);
    if (archiveVersion() <= 1) {
        const QString globalNoteSettingsStr(QStringLiteral("globalnotesettings"));
        restoreConfigFile(globalNoteSettingsStr);
    } else {
        const QString globalNoteStr(QStringLiteral("globalnotesettings"));
        const KArchiveEntry *globalNotecentry = mArchiveDirectory->entry(Utils::configsPath() + globalNoteStr);
        if (globalNotecentry && globalNotecentry->isFile()) {
          const auto globalNotecentryrc =
              static_cast<const KArchiveFile *>(globalNotecentry);
          const QString searchExistingGlobalNoterc =
              configLocation() + globalNoteStr;
          const QString installPathGlobalNoterc =
              installConfigLocation() + globalNoteStr;

          if (QFileInfo::exists(searchExistingGlobalNoterc)) {
            if (overwriteConfigMessageBox(globalNoteStr)) {
              importKNoteGlobalSettings(globalNotecentryrc,
                                        installPathGlobalNoterc, globalNoteStr,
                                        Utils::configsPath());
            } // Else merge!
          } else {
            importKNoteGlobalSettings(globalNotecentryrc,
                                      installPathGlobalNoterc, globalNoteStr,
                                      Utils::configsPath());
          }
        }
    }

    restoreUiRcFile(QStringLiteral("knotesappui.rc"), QStringLiteral("knotes"));
    restoreUiRcFile(QStringLiteral("knotesui.rc"), QStringLiteral("knotes"));
    restoreUiRcFile(QStringLiteral("knotes_part.rc"), QStringLiteral("knotes"));
    restoreConfigFile(QStringLiteral("akonadi_notes_agent.notifyrc"));

    Q_EMIT info(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportNotesJobInterface::slotNextStep);
}

void ImportNotesJobInterface::restoreData()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore data..."));
    if (archiveVersion() <= 1) {
        //Knote < knote-akonadi
        const KArchiveEntry *notesEntry = mArchiveDirectory->entry(Utils::dataPath() + QStringLiteral("knotes/"));
        if (notesEntry && notesEntry->isDirectory()) {
            const QString notesPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QStringLiteral("knotes/");
            overwriteDirectory(notesPath, notesEntry);
        }
        QTimer::singleShot(0, this, &ImportNotesJobInterface::slotNextStep);
    } else {
        restoreResources();
    }
    importDataSubdirectory(QStringLiteral("/knotes/print/theme/"));
    Q_EMIT info(i18n("Data restored."));
}

void ImportNotesJobInterface::importKNoteGlobalSettings(const KArchiveFile *archive, const QString &configrc, const QString &filename, const QString &prefix)
{
    copyToFile(archive, configrc, filename, prefix);
    KSharedConfig::Ptr globalSettingsConfig = KSharedConfig::openConfig(configrc);

    const QString composerStr(QStringLiteral("SelectNoteFolder"));
    if (globalSettingsConfig->hasGroup(composerStr)) {
        KConfigGroup composerGroup = globalSettingsConfig->group(composerStr);
        const QString previousStr(QStringLiteral("DefaultFolder"));
        (void)convertRealPathToCollection(composerGroup, previousStr);
    }
    globalSettingsConfig->sync();
}

void ImportNotesJobInterface::restoreResources()
{
    Q_EMIT info(i18n("Restore resources..."));
    setProgressDialogLabel(i18n("Restore resources..."));
    QStringList listResource;
    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(Utils::notePath());
        const QString copyToDirName(mTempDirName + QLatin1Char('/') + Utils::notePath());
        QDir().mkpath(copyToDirName);
        const int numberOfResourceFile = mListResourceFile.size();
        for (int i = 0; i < numberOfResourceFile; ++i) {
            ResourceFiles value = mListResourceFile.at(i);
            QMap<QString, QVariant> settings;
            if (value.akonadiConfigFile.contains(QLatin1String("akonadi_akonotes_resource_"))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                  const auto file =
                      static_cast<const KArchiveFile *>(fileResouceEntry);
                  copyArchiveFileTo(file, copyToDirName);
                  QString resourceName(file->name());

                  QString filename(resourceName);
                  // TODO adapt filename otherwise it will use all the time the
                  // same filename.
                  qCDebug(PIMDATAEXPORTERCORE_LOG) << " filename :" << filename;

                  KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(
                      copyToDirName + QLatin1Char('/') + resourceName);

                  const QString newUrl =
                      adaptResourcePath(resourceConfig, Utils::backupnote());
                  QFileInfo newUrlInfo(newUrl);
                  const QString dataFile = value.akonadiResources;
                  const KArchiveEntry *dataResouceEntry =
                      mArchiveDirectory->entry(dataFile);
                  if (dataResouceEntry->isFile()) {
                    const auto file =
                        static_cast<const KArchiveFile *>(dataResouceEntry);
                    // TODO  adapt directory name too
                    extractZipFile(file, copyToDirName, newUrlInfo.path());
                  }
                    settings.insert(QStringLiteral("Path"), newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                          const auto fileEntry = static_cast<const KArchiveFile *>(
                              akonadiAgentConfigEntry);
                          copyArchiveFileTo(fileEntry, copyToDirName);
                          resourceName = fileEntry->name();
                          filename = Utils::akonadiAgentName(
                              copyToDirName + QLatin1Char('/') + resourceName);
                        }
                    }

                    const QString newResource = createResource(QStringLiteral("akonadi_akonotes_resource"), filename, settings, true);
                    infoAboutNewResource(newResource);
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " newResource" << newResource;
                    listResource << newResource;
                }
            }
        }
    }
    synchronizeResource(listResource);
}
