/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportnotesjobinterface.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>

#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <resourceconverterimpl.h>

ExportNotesJobInterface::ExportNotesJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportNotesJobInterface::~ExportNotesJobInterface() = default;

void ExportNotesJobInterface::start()
{
    Q_EMIT title(i18n("Start export KNotes settings..."));
    createProgressDialog(i18n("Export KNotes settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportNotesJobInterface::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportNotesJobInterface::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportNotesJobInterface::backupTheme()
{
    const QString notesThemeDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/knotes/print/");
    QDir notesThemeDirectory(notesThemeDir);
    if (notesThemeDirectory.exists()) {
        const bool notesDirAdded = archive()->addLocalDirectory(notesThemeDir, Utils::dataPath() + QStringLiteral("knotes/print"));
        if (!notesDirAdded) {
            emitError(i18n("\"%1\" directory cannot be added to backup file.", notesThemeDir));
        }
    }
}

void ExportNotesJobInterface::slotCheckBackupResource()
{
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    backupTheme();
    mAkonadiInstanceInfo = listOfResource();
    QTimer::singleShot(0, this, &ExportNotesJobInterface::slotWriteNextArchiveResource);
}

void ExportNotesJobInterface::slotCheckBackupConfig()
{
    if (mTypeSelected & Utils::Config) {
        backupConfig();
        increaseProgressDialog();
        if (wasCanceled()) {
            Q_EMIT jobFinished();
            return;
        }
    }
    Q_EMIT jobFinished();
}

void ExportNotesJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    const QString knotesStr(QStringLiteral("knotesrc"));
    const QString knotesrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + knotesStr;
    if (QFileInfo::exists(knotesrc)) {
        backupFile(knotesrc, Utils::configsPath(), knotesStr);
    }
    const QString globalNoteSettingsStr(QStringLiteral("globalnotesettings"));
    const QString globalNoteSettingsrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + globalNoteSettingsStr;

    if (QFileInfo::exists(globalNoteSettingsrc)) {
        KSharedConfigPtr globalnotesettingsrc = KSharedConfig::openConfig(globalNoteSettingsrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *knoteConfig = globalnotesettingsrc->copyTo(tmp.fileName());
        const QString selectFolderNoteStr(QStringLiteral("SelectNoteFolder"));
        if (knoteConfig->hasGroup(selectFolderNoteStr)) {
            KConfigGroup selectFolderNoteGroup = knoteConfig->group(selectFolderNoteStr);

            const QString selectFolderNoteGroupStr(QStringLiteral("DefaultFolder"));
            convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
        }
        knoteConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), globalNoteSettingsStr);
        delete knoteConfig;
    }
    backupUiRcFile(QStringLiteral("knotesappui.rc"), QStringLiteral("knotes"));
    backupUiRcFile(QStringLiteral("knotesui.rc"), QStringLiteral("knotes"));
    backupUiRcFile(QStringLiteral("knotes_part.rc"), QStringLiteral("knotes"));
    backupConfigFile(QStringLiteral("akonadi_notes_agent.notifyrc"));
    storeDirectory(QStringLiteral("/knotes/print/theme/"));
    emitInfo(i18n("Config backup done."));
}

void ExportNotesJobInterface::slotNoteJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportNotesJobInterface::slotWriteNextArchiveResource);
}

QString ExportNotesJobInterface::applicationName() const
{
    return QStringLiteral("[KNotes]");
}

void ExportNotesJobInterface::slotWriteNextArchiveResource()
{
    if (mIndexIdentifier < mAkonadiInstanceInfo.count()) {
        const Utils::AkonadiInstanceInfo agent = mAkonadiInstanceInfo.at(mIndexIdentifier);
        const QString identifier = agent.identifier;
        if (identifier.contains(QLatin1String("akonadi_akonotes_resource_"))) {
            const QString archivePath = Utils::notePath() + identifier + QLatin1Char('/');

            const QString url = resourcePath(identifier);
            if (!mAgentPaths.contains(url) && QDir(url).exists()) {
                if (!url.isEmpty()) {
                    mAgentPaths << url;
                    exportResourceToArchive(archivePath, url, identifier);
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportNotesJobInterface::slotNoteJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportNotesJobInterface::slotNoteJobTerminated);
            }
        } else {
            QTimer::singleShot(0, this, &ExportNotesJobInterface::slotNoteJobTerminated);
        }
    } else {
        emitInfo(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportNotesJobInterface::slotCheckBackupConfig);
    }
}
