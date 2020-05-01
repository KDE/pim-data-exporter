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

#include "importalarmjobinterface.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>

#include <KArchive>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KZip>
#include <KArchiveEntry>

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>

ImportAlarmJobInterface::ImportAlarmJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportAlarmJobInterface::~ImportAlarmJobInterface()
{
}

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
    return name.endsWith(QLatin1String("rc")) && (name.contains(QLatin1String("akonadi_kalarm_resource_"))
                                                  || name.contains(QLatin1String("akonadi_kalarm_dir_resource_")));
}

void ImportAlarmJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString kalarmStr(QStringLiteral("kalarmrc"));
    const KArchiveEntry *kalarmrcentry = mArchiveDirectory->entry(Utils::configsPath() + kalarmStr);
    if (kalarmrcentry && kalarmrcentry->isFile()) {
        const KArchiveFile *kalarmrcFile = static_cast<const KArchiveFile *>(kalarmrcentry);
        const QString kalarmrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + kalarmStr;
        if (QFileInfo::exists(kalarmrc)) {
            if (overwriteConfigMessageBox(kalarmStr)) {
                importkalarmConfig(kalarmrcFile, kalarmrc, kalarmStr, Utils::configsPath());
            }
        } else {
            importkalarmConfig(kalarmrcFile, kalarmrc, kalarmStr, Utils::configsPath());
        }
    }
    restoreUiRcFile(QStringLiteral("kalarmui.rc"), QStringLiteral("kalarm"));
    Q_EMIT info(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportAlarmJobInterface::slotNextStep);
}

void ImportAlarmJobInterface::importkalarmConfig(const KArchiveFile *kalarmFile, const QString &kalarmrc, const QString &filename, const QString &prefix)
{
    copyToFile(kalarmFile, kalarmrc, filename, prefix);
    KSharedConfig::Ptr kalarmConfig = KSharedConfig::openConfig(kalarmrc);

    convertCollectionListStrToAkonadiId(kalarmConfig, QStringLiteral("Collections"), QStringLiteral("FavoriteCollectionIds"), false);

    kalarmConfig->sync();
}
