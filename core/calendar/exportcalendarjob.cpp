/*
   Copyright (C) 2012-2019 Montel Laurent <montel@kde.org>

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

#include "exportcalendarjob.h"
#include <MailCommon/MailUtil>
#include <AkonadiCore/AgentManager>

#include <KLocalizedString>

#include <QTemporaryFile>
#include <KConfigGroup>

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QColor>

#include <QStandardPaths>
#include <exportresourcearchivejob.h>

ExportCalendarJob::ExportCalendarJob(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportCalendarJob::~ExportCalendarJob()
{
}

void ExportCalendarJob::start()
{
    Q_EMIT title(i18n("Start export KOrganizer settings..."));
    createProgressDialog(i18n("Export KOrganizer settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportCalendarJob::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportCalendarJob::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportCalendarJob::slotCheckBackupResource()
{
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    QTimer::singleShot(0, this, &ExportCalendarJob::slotWriteNextArchiveResource);
}

void ExportCalendarJob::slotCheckBackupConfig()
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

void ExportCalendarJob::slotCalendarJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportCalendarJob::slotWriteNextArchiveResource);
}

void ExportCalendarJob::slotWriteNextArchiveResource()
{
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    if (mIndexIdentifier < list.count()) {
        Akonadi::AgentInstance agent = list.at(mIndexIdentifier);
        const QString identifier = agent.identifier();
        if (identifier.contains(QLatin1String("akonadi_icaldir_resource_"))) {
            const QString archivePath = Utils::calendarPath() + identifier + QDir::separator();

            QString url = Utils::resourcePath(agent);
            if (!mAgentPaths.contains(url)) {
                mAgentPaths << url;
                if (!url.isEmpty()) {
                    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
                    resourceJob->setArchivePath(archivePath);
                    resourceJob->setUrl(url);
                    resourceJob->setIdentifier(identifier);
                    resourceJob->setArchive(archive());
                    resourceJob->setArchiveName(QStringLiteral("calendar.zip"));
                    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportCalendarJob::error);
                    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportCalendarJob::info);
                    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportCalendarJob::slotCalendarJobTerminated);
                    resourceJob->start();
                } else {
                    qCDebug(PIMSETTINGEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportCalendarJob::slotCalendarJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportCalendarJob::slotCalendarJobTerminated);
            }
        } else if (identifier.contains(QLatin1String("akonadi_ical_resource_"))) {
            backupResourceFile(agent, Utils::calendarPath());
            QTimer::singleShot(0, this, &ExportCalendarJob::slotCalendarJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportCalendarJob::slotCalendarJobTerminated);
        }
    } else {
        Q_EMIT info(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportCalendarJob::slotCheckBackupConfig);
    }
}

void ExportCalendarJob::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    const QString korganizerStr(QStringLiteral("korganizerrc"));
    const QString korganizerrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + korganizerStr;
    if (QFileInfo::exists(korganizerrc)) {
        KSharedConfigPtr korganizer = KSharedConfig::openConfig(korganizerrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *korganizerConfig = korganizer->copyTo(tmp.fileName());

        const QString globalCollectionsStr(QStringLiteral("GlobalCollectionSelection"));
        if (korganizerConfig->hasGroup(globalCollectionsStr)) {
            KConfigGroup group = korganizerConfig->group(globalCollectionsStr);
            const QString selectionKey(QStringLiteral("Selection"));
            Utils::convertCollectionListToRealPath(group, selectionKey);
        }

        const QString collectionTreeViewStr(QStringLiteral("CollectionTreeView"));
        if (korganizerConfig->hasGroup(collectionTreeViewStr)) {
            KConfigGroup group = korganizerConfig->group(collectionTreeViewStr);
            const QString selectionKey(QStringLiteral("Expansion"));
            Utils::convertCollectionListToRealPath(group, selectionKey);
        }

        korganizerConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), korganizerStr);
        delete korganizerConfig;
    }
    const QString eventviewsrcStr(QStringLiteral("eventviewsrc"));
    const QString eventviewsrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + eventviewsrcStr;
    if (QFileInfo::exists(eventviewsrc)) {
        KSharedConfigPtr eventviews = KSharedConfig::openConfig(eventviewsrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *eventviewConfig = eventviews->copyTo(tmp.fileName());

        const QString resourceColorStr(QStringLiteral("Resources Colors"));
        if (eventviewConfig->hasGroup(resourceColorStr)) {
            KConfigGroup group = eventviewConfig->group(resourceColorStr);

            const QStringList keyList = group.keyList();
            bool found = false;
            for (const QString &key : keyList) {
                const int collectionValue = key.toInt(&found);
                if (found && collectionValue != -1) {
                    const QString realPath = MailCommon::Util::fullCollectionPath(Akonadi::Collection(collectionValue));
                    const QColor color = group.readEntry(key, QColor());
                    group.writeEntry(realPath, color);
                    group.deleteEntry(key);
                }
            }
        }


        eventviewConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), eventviewsrcStr);
        delete eventviewConfig;
    }


    backupConfigFile(QStringLiteral("calendar_printing.rc"));
    backupConfigFile(QStringLiteral("korgacrc"));

    const QString freebusyurlsStr(QStringLiteral("korganizer/freebusyurls"));
    const QString freebusyurls = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + freebusyurlsStr;
    if (QFileInfo::exists(freebusyurls)) {
        backupFile(freebusyurls, Utils::dataPath(), freebusyurlsStr);
    }

    storeDirectory(QStringLiteral("/korganizer/templates/"));
    storeDirectory(QStringLiteral("/korganizer/designer/"));

    backupUiRcFile(QStringLiteral("korganizerui.rc"), QStringLiteral("korganizer"));
    backupUiRcFile(QStringLiteral("korganizer_part.rc"), QStringLiteral("korganizer"));
    Q_EMIT info(i18n("Config backup done."));
}
