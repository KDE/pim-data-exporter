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

#include "exportakregatorjobinterface.h"
#include <KLocalizedString>
#include <KZip>

#include <QDir>
#include <QStandardPaths>
#include <QTimer>

ExportAkregatorJobInterface::ExportAkregatorJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportAkregatorJobInterface::~ExportAkregatorJobInterface()
{
}

void ExportAkregatorJobInterface::start()
{
    Q_EMIT title(i18n("Start export Akregator settings..."));
    createProgressDialog(i18n("Export Akregator settings"));
    if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportAkregatorJobInterface::slotCheckBackupConfig);
    } else if (mTypeSelected & Utils::Data) {
        QTimer::singleShot(0, this, &ExportAkregatorJobInterface::slotCheckBackupData);
    } else {
        Q_EMIT jobFinished();
    }
}

Akonadi::Collection::Id ExportAkregatorJobInterface::convertFolderPathToCollectionId(const QString &path)
{
    //Unusued
    return -1;
}

void ExportAkregatorJobInterface::slotCheckBackupConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Backing up config..."));

    backupConfigFile(QStringLiteral("akregatorrc"));
    backupUiRcFile(QStringLiteral("akregator_part.rc"), QStringLiteral("akregator"));
    backupUiRcFile(QStringLiteral("akregator_shell.rc"), QStringLiteral("akregator"));
    backupConfigFile(QStringLiteral("akregator.notifyrc"));

    Q_EMIT info(i18n("Config backup done."));
    QTimer::singleShot(0, this, &ExportAkregatorJobInterface::slotCheckBackupData);
}

void ExportAkregatorJobInterface::slotCheckBackupData()
{
    if (mTypeSelected & Utils::Data) {
        increaseProgressDialog();
        setProgressDialogLabel(i18n("Backing up data..."));

        const QString akregatorDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/akregator");
        QDir akregatorDirectory(akregatorDir);
        if (akregatorDirectory.exists()) {
            const bool akregatorDirAdded = archive()->addLocalDirectory(akregatorDir, Utils::dataPath() +  QStringLiteral("akregator"));
            if (!akregatorDirAdded) {
                Q_EMIT error(i18n("\"%1\" directory cannot be added to backup file.", akregatorDir));
            }
        }
        Q_EMIT info(i18n("Data backup done."));
    }
    Q_EMIT jobFinished();
}
