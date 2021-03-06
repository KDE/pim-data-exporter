/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportcalendarjobinterface.h"

#include <KLocalizedString>

#include <QTemporaryFile>
#include <KConfigGroup>

#include <QTimer>
#include <QFile>
#include <QColor>
#include <resourceconverterimpl.h>

#include <QFileInfo>
#include <QStandardPaths>

ExportCalendarJobInterface::ExportCalendarJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportCalendarJobInterface::~ExportCalendarJobInterface()
{
}

void ExportCalendarJobInterface::slotCalendarJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotWriteNextArchiveResource);
}

void ExportCalendarJobInterface::slotWriteNextArchiveResource()
{
    if (mIndexIdentifier < mAkonadiInstanceInfo.count()) {
        const Utils::AkonadiInstanceInfo agent = mAkonadiInstanceInfo.at(mIndexIdentifier);
        const QString identifier = agent.identifier;
        if (identifier.contains(QLatin1String("akonadi_icaldir_resource_"))) {
            const QString archivePath = Utils::calendarPath() + identifier + QLatin1Char('/');

            const QString url = resourcePath(identifier);
            if (!mAgentPaths.contains(url)) {
                if (!url.isEmpty()) {
                    mAgentPaths << url;
                    exportResourceToArchive(archivePath, url, identifier);
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
            }
        } else if (identifier.contains(QLatin1String("akonadi_ical_resource_"))) {
            backupCalendarResourceFile(identifier, Utils::calendarPath());
            QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
        }
    } else {
        Q_EMIT info(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCheckBackupConfig);
    }
}

void ExportCalendarJobInterface::start()
{
    Q_EMIT title(i18n("Start export KOrganizer settings..."));
    createProgressDialog(i18n("Export KOrganizer settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportCalendarJobInterface::slotCheckBackupResource()
{
    mAkonadiInstanceInfo = listOfResource();
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotWriteNextArchiveResource);
}

void ExportCalendarJobInterface::slotCheckBackupConfig()
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

void ExportCalendarJobInterface::backupConfig()
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
            convertCollectionListToRealPath(group, selectionKey);
        }

        const QString collectionTreeViewStr(QStringLiteral("CollectionTreeView"));
        if (korganizerConfig->hasGroup(collectionTreeViewStr)) {
            KConfigGroup group = korganizerConfig->group(collectionTreeViewStr);
            const QString selectionKey(QStringLiteral("Expansion"));
            convertCollectionListToRealPath(group, selectionKey);
        }

        const QString globalCollectionViewStr(QStringLiteral("GlobalCollectionView"));
        if (korganizerConfig->hasGroup(globalCollectionViewStr)) {
            KConfigGroup group = korganizerConfig->group(globalCollectionViewStr);
            const QString selectionKey(QStringLiteral("Expansion"));
            convertCollectionListToRealPath(group, selectionKey);
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
                    const QString realPath = convertToFullCollectionPath(collectionValue);
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
