/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportalarmjobinterface.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <KConfigGroup>
#include <QTemporaryFile>
#include <QTimer>

#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

ExportAlarmJobInterface::ExportAlarmJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportAlarmJobInterface::~ExportAlarmJobInterface() = default;

void ExportAlarmJobInterface::start()
{
    Q_EMIT title(i18n("Start export KAlarm settings..."));
    createProgressDialog(i18n("Export KAlarm settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportAlarmJobInterface::slotAlarmJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotWriteNextArchiveResource);
}

void ExportAlarmJobInterface::slotWriteNextArchiveResource()
{
    if (mIndexIdentifier < mAkonadiInstanceInfo.count()) {
        const Utils::AkonadiInstanceInfo agent = mAkonadiInstanceInfo.at(mIndexIdentifier);
        const QString identifier = agent.identifier;
        if (identifier.contains(QLatin1StringView("akonadi_kalarm_dir_resource_"))) {
            const QString archivePath = Utils::alarmPath() + identifier + u'/';

            const QString url = resourcePath(identifier);
            if (!mAgentPaths.contains(url)) {
                if (!url.isEmpty()) {
                    mAgentPaths << url;
                    exportResourceToArchive(archivePath, url, identifier);
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotAlarmJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotAlarmJobTerminated);
            }
        } else if (identifier.contains(QLatin1StringView("akonadi_kalarm_resource_"))) {
            backupAlarmResourceFile(identifier, Utils::alarmPath());
            QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotAlarmJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotAlarmJobTerminated);
        }
    } else {
        emitInfo(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotCheckBackupConfig);
    }
}

void ExportAlarmJobInterface::slotCheckBackupResource()
{
    mAkonadiInstanceInfo = listOfResource();
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotWriteNextArchiveResource);
}

void ExportAlarmJobInterface::slotCheckBackupConfig()
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

QString ExportAlarmJobInterface::applicationName() const
{
    return u"[Alarm]"_s;
}

void ExportAlarmJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    const QString kalarmStr(u"kalarmrc"_s);
    const QString kalarmrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + kalarmStr;
    if (QFileInfo::exists(kalarmrc)) {
        KSharedConfigPtr kalarm = KSharedConfig::openConfig(kalarmrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *kalarmConfig = kalarm->copyTo(tmp.fileName());

        const QString collectionsStr(u"Collections"_s);
        if (kalarmConfig->hasGroup(collectionsStr)) {
            KConfigGroup group = kalarmConfig->group(collectionsStr);
            const QString selectionKey(u"FavoriteCollectionIds"_s);
            convertCollectionIdsToRealPath(group, selectionKey);
        }

        kalarmConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), kalarmStr);
        delete kalarmConfig;
    }

    backupUiRcFile(u"kalarmui.rc"_s, u"kalarm"_s);

    emitInfo(i18n("Config backup done."));
}

#include "moc_exportalarmjobinterface.cpp"
