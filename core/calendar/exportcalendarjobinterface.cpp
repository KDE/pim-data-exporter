/*
   SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportcalendarjobinterface.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <KConfigGroup>
#include <QTemporaryFile>

#include "resourceconverterimpl.h"
#include <QColor>
#include <QFile>
#include <QTimer>

#include <QFileInfo>
#include <QStandardPaths>

ExportCalendarJobInterface::ExportCalendarJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportCalendarJobInterface::~ExportCalendarJobInterface() = default;

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
        if (identifier.contains(QLatin1StringView("akonadi_icaldir_resource_"))) {
            const QString archivePath = Utils::calendarPath() + identifier + u'/';

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
        } else if (identifier.contains(QLatin1StringView("akonadi_ical_resource_"))) {
            backupCalendarResourceFile(identifier, Utils::calendarPath());
            QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
        } else if (identifier.contains(QLatin1StringView("akonadi_davgroupware_resource_"))) {
            backupCalendarResourceFile(identifier, Utils::calendarPath());
            QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
        } else if (identifier.contains(QLatin1StringView("akonadi_openxchange_resource_"))) {
            backupCalendarResourceFile(identifier, Utils::calendarPath());
            QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
        } else if (identifier.contains(QLatin1StringView("akonadi_google_resource_"))) {
            backupCalendarResourceFile(identifier, Utils::calendarPath());
            QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportCalendarJobInterface::slotCalendarJobTerminated);
        }
    } else {
        emitInfo(i18n("Resources backup done."));
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

QString ExportCalendarJobInterface::applicationName() const
{
    return u"[Calendar]"_s;
}

void ExportCalendarJobInterface::exportEventViewConfig()
{
    const QString eventviewsrcStr(u"eventviewsrc"_s);
    const QString eventviewsrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + eventviewsrcStr;
    if (QFileInfo::exists(eventviewsrc)) {
        KSharedConfigPtr eventviews = KSharedConfig::openConfig(eventviewsrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *eventviewConfig = eventviews->copyTo(tmp.fileName());
        exportResourceColors(eventviewConfig);

        eventviewConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), eventviewsrcStr);
        delete eventviewConfig;
    }
}

void ExportCalendarJobInterface::exportResourceColors(KConfig *config)
{
    const QString resourceColorStr(u"Resources Colors"_s);
    if (config->hasGroup(resourceColorStr)) {
        KConfigGroup group = config->group(resourceColorStr);

        const QStringList keyList = group.keyList();
        bool found = false;
        for (const QString &key : keyList) {
            const int collectionValue = key.toInt(&found);
            if (found && collectionValue != -1) {
                const QString realPath = convertToFullCollectionPath(collectionValue);
                const QColor color = group.readEntry(key, QColor());
                if (color.isValid()) {
                    group.writeEntry(realPath, color);
                }
                group.deleteEntry(key);
            }
        }
    }
}

void ExportCalendarJobInterface::exportKalendarConfig()
{
    const QString kalendarStr(u"kalendarrc"_s);
    const QString kalendarrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + kalendarStr;
    if (QFileInfo::exists(kalendarrc)) {
        KSharedConfigPtr kalendar = KSharedConfig::openConfig(kalendarrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *kalendarConfig = kalendar->copyTo(tmp.fileName());

        const QString globalCollectionsStr(u"GlobalCollectionSelection"_s);
        if (kalendarConfig->hasGroup(globalCollectionsStr)) {
            KConfigGroup group = kalendarConfig->group(globalCollectionsStr);
            const QString selectionKey(u"Selection"_s);
            convertCollectionListToRealPath(group, selectionKey);
            // Add Current
        }

        exportResourceColors(kalendarConfig);
        kalendarConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), kalendarStr);
        delete kalendarConfig;
    }
}

void ExportCalendarJobInterface::exportKorganizerConfig()
{
    const QString korganizerStr(u"korganizerrc"_s);
    const QString korganizerrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + korganizerStr;
    if (QFileInfo::exists(korganizerrc)) {
        KSharedConfigPtr korganizer = KSharedConfig::openConfig(korganizerrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *korganizerConfig = korganizer->copyTo(tmp.fileName());

        const QString globalCollectionsStr(u"GlobalCollectionSelection"_s);
        if (korganizerConfig->hasGroup(globalCollectionsStr)) {
            KConfigGroup group = korganizerConfig->group(globalCollectionsStr);
            const QString selectionKey(u"Selection"_s);
            convertCollectionListToRealPath(group, selectionKey);
        }

        const QString collectionTreeViewStr(u"CollectionTreeView"_s);
        if (korganizerConfig->hasGroup(collectionTreeViewStr)) {
            KConfigGroup group = korganizerConfig->group(collectionTreeViewStr);
            const QString selectionKey(u"Expansion"_s);
            convertCollectionListToRealPath(group, selectionKey);
        }

        const QString globalCollectionViewStr(u"GlobalCollectionView"_s);
        if (korganizerConfig->hasGroup(globalCollectionViewStr)) {
            KConfigGroup group = korganizerConfig->group(globalCollectionViewStr);
            const QString selectionKey(u"Expansion"_s);
            convertCollectionListToRealPath(group, selectionKey);
        }

        korganizerConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), korganizerStr);
        delete korganizerConfig;
    }
}

void ExportCalendarJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    exportKorganizerConfig();
    exportEventViewConfig();
    backupConfigFile(u"kalendaracrc"_s);
    exportKalendarConfig();

    backupConfigFile(u"calendar_printing.rc"_s);

    const QString freebusyurlsStr(u"korganizer/freebusyurls"_s);
    const QString freebusyurls = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u'/' + freebusyurlsStr;
    if (QFileInfo::exists(freebusyurls)) {
        backupFile(freebusyurls, Utils::dataPath(), freebusyurlsStr);
    }

    storeDirectory(u"/korganizer/templates/"_s);
    storeDirectory(u"/korganizer/designer/"_s);

    backupUiRcFile(u"korganizerui.rc"_s, u"korganizer"_s);
    backupUiRcFile(u"korganizer_part.rc"_s, u"korganizer"_s);
    emitInfo(i18n("Config backup done."));
}

#include "moc_exportcalendarjobinterface.cpp"
