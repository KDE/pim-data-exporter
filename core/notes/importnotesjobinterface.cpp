/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#include "importnotesjobinterface.h"
#include "archivestorage.h"
#include "resourceconverterimpl.h"

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
    // FIXME search archive ? searchAllFiles(mArchiveDirectory, QString());
    createProgressDialog(i18n("Import KNotes settings"));
    initializeListStep();
    QTimer::singleShot(0, this, &ImportNotesJobInterface::slotNextStep);
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
            const KArchiveFile *globalNotecentryrc = static_cast<const KArchiveFile *>(globalNotecentry);
            const QString globalNoterc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + globalNoteStr;
            if (QFileInfo::exists(globalNoterc)) {
                if (overwriteConfigMessageBox(globalNoteStr)) {
                    importKNoteGlobalSettings(globalNotecentryrc, globalNoterc, globalNoteStr, Utils::configsPath());
                }
            } else {
                importKNoteGlobalSettings(globalNotecentryrc, globalNoterc, globalNoteStr, Utils::configsPath());
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
