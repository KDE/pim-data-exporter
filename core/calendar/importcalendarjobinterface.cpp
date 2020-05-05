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

#include "importcalendarjobinterface.h"
#include "resourceconverterimpl.h"
#include "archivestorage.h"

#include <KLocalizedString>
#include <KConfigGroup>
#include <KArchiveDirectory>
#include <KArchiveEntry>
#include <KArchive>
#include <KZip>

#include "pimdataexportcore_debug.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QColor>

namespace {
inline const QString storeCalendar()
{
    return QStringLiteral("backupcalendar/");
}
}

ImportCalendarJobInterface::ImportCalendarJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportCalendarJobInterface::~ImportCalendarJobInterface()
{
}

void ImportCalendarJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import KOrganizer settings..."));
    mArchiveDirectory = archive()->directory();
    createProgressDialog(i18n("Import KOrganizer settings"));
    searchAllFiles(mArchiveDirectory, QString(), QStringLiteral("calendar"));
    initializeListStep();
    QTimer::singleShot(0, this, &ImportCalendarJobInterface::slotNextStep);
}

void ImportCalendarJobInterface::slotNextStep()
{
    ++mIndex;
    if (mIndex < mListStep.count()) {
        Utils::StoredType type = mListStep.at(mIndex);
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

void ImportCalendarJobInterface::addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings)
{
    if (resourceName == QLatin1String("akonadi_ical_resource")) {
        KConfigGroup general = resourceConfig->group(QStringLiteral("General"));
        if (general.hasKey(QStringLiteral("DisplayName"))) {
            settings.insert(QStringLiteral("DisplayName"), general.readEntry(QStringLiteral("DisplayName")));
        }
        if (general.hasKey(QStringLiteral("ReadOnly"))) {
            settings.insert(QStringLiteral("ReadOnly"), general.readEntry(QStringLiteral("ReadOnly"), false));
        }
        if (general.hasKey(QStringLiteral("MonitorFile"))) {
            settings.insert(QStringLiteral("MonitorFile"), general.readEntry(QStringLiteral("MonitorFile"), true));
        }
    }
}

bool ImportCalendarJobInterface::isAConfigFile(const QString &name) const
{
    return name.endsWith(QLatin1String("rc")) && (name.contains(QLatin1String("akonadi_ical_resource_"))
                                                  || name.contains(QLatin1String("akonadi_icaldir_resource_")));
}

void ImportCalendarJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString korganizerPrinterrcStr(QStringLiteral("calendar_printing.rc"));
    const QString oldKorganizerPrintrrcStr(QStringLiteral("korganizer_printing.rc"));
    const KArchiveEntry *oldKorganizerPrinterEntry = mArchiveDirectory->entry(Utils::configsPath() + oldKorganizerPrintrrcStr);
    if (oldKorganizerPrinterEntry && oldKorganizerPrinterEntry->isFile()) {
        const KArchiveFile *korganizerFile = static_cast<const KArchiveFile *>(oldKorganizerPrinterEntry);
        const QString oldKorganizerPrintrrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + korganizerPrinterrcStr;
        if (QFileInfo::exists(oldKorganizerPrintrrc)) {
            if (overwriteConfigMessageBox(oldKorganizerPrintrrc)) {
                copyToFile(korganizerFile, oldKorganizerPrintrrc, oldKorganizerPrintrrcStr, Utils::configsPath());
            }
        } else {
            copyToFile(korganizerFile, oldKorganizerPrintrrc, oldKorganizerPrintrrcStr, Utils::configsPath());
        }
    } else {
        const KArchiveEntry *korganizerPrinterEntry = mArchiveDirectory->entry(Utils::configsPath() + korganizerPrinterrcStr);
        if (korganizerPrinterEntry && korganizerPrinterEntry->isFile()) {
            const KArchiveFile *korganizerFile = static_cast<const KArchiveFile *>(korganizerPrinterEntry);
            const QString korganizerPrinterrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + korganizerPrinterrcStr;
            if (QFileInfo::exists(korganizerPrinterrc)) {
                if (overwriteConfigMessageBox(korganizerPrinterrcStr)) {
                    copyToFile(korganizerFile, korganizerPrinterrc, korganizerPrinterrcStr, Utils::configsPath());
                }
            } else {
                copyToFile(korganizerFile, korganizerPrinterrc, korganizerPrinterrcStr, Utils::configsPath());
            }
        }
    }

    const QString korganizerStr(QStringLiteral("korganizerrc"));
    const KArchiveEntry *korganizerrcentry = mArchiveDirectory->entry(Utils::configsPath() + korganizerStr);
    if (korganizerrcentry && korganizerrcentry->isFile()) {
        const KArchiveFile *korganizerrcFile = static_cast<const KArchiveFile *>(korganizerrcentry);
        const QString korganizerrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + korganizerStr;
        if (QFileInfo::exists(korganizerrc)) {
            if (overwriteConfigMessageBox(korganizerStr)) {
                importkorganizerConfig(korganizerrcFile, korganizerrc, korganizerStr, Utils::configsPath());
            }
        } else {
            importkorganizerConfig(korganizerrcFile, korganizerrc, korganizerStr, Utils::configsPath());
        }
    }

    const QString eventviewStr(QStringLiteral("eventviewsrc"));
    const KArchiveEntry *eventviewentry = mArchiveDirectory->entry(Utils::configsPath() + eventviewStr);
    if (eventviewentry && eventviewentry->isFile()) {
        const KArchiveFile *eventviewrcFile = static_cast<const KArchiveFile *>(eventviewentry);
        const QString eventviewrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + eventviewStr;
        if (QFileInfo::exists(eventviewrc)) {
            if (overwriteConfigMessageBox(eventviewStr)) {
                importeventViewConfig(eventviewrcFile, eventviewrc, eventviewStr, Utils::configsPath());
            }
        } else {
            importeventViewConfig(eventviewrcFile, eventviewrc, eventviewStr, Utils::configsPath());
        }
    }

    const QString korgacStr(QStringLiteral("korgacrc"));
    const KArchiveEntry *korgacrcentry = mArchiveDirectory->entry(Utils::configsPath() + korgacStr);
    if (korgacrcentry && korgacrcentry->isFile()) {
        const KArchiveFile *korgacrcFile = static_cast<const KArchiveFile *>(korgacrcentry);
        const QString korgacrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + korgacStr;
        if (QFileInfo::exists(korgacrc)) {
            if (overwriteConfigMessageBox(korgacStr)) {
                copyToFile(korgacrcFile, korgacrc, korgacStr, Utils::configsPath());
            }
        } else {
            copyToFile(korgacrcFile, korgacrc, korgacStr, Utils::configsPath());
        }
    }

    const QString freebusyStr(QStringLiteral("freebusyurls"));
    const KArchiveEntry *freebusyentry = mArchiveDirectory->entry(Utils::dataPath() + QLatin1String("korganizer/") + freebusyStr);
    if (freebusyentry && freebusyentry->isFile()) {
        const KArchiveFile *freebusyrcFile = static_cast<const KArchiveFile *>(freebusyentry);

        const QString freebusypath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/korganizer/") + freebusyStr;
        if (QFileInfo::exists(freebusypath)) {
            //TODO 4.12 merge it.
            if (overwriteConfigMessageBox(freebusyStr)) {
                copyToFile(freebusyrcFile, freebusypath, freebusyStr, Utils::dataPath());
            }
        } else {
            copyToFile(freebusyrcFile, freebusypath, freebusyStr, Utils::dataPath());
        }
    }

    const KArchiveEntry *templateEntry = mArchiveDirectory->entry(Utils::dataPath() + QStringLiteral("korganizer/templates/"));
    if (templateEntry && templateEntry->isDirectory()) {
        //TODO 4.12 verify if template already exists.
        const QString templatePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QStringLiteral("korganizer/templates/");
        const KArchiveDirectory *templateDir = static_cast<const KArchiveDirectory *>(templateEntry);
        if (!templateDir->copyTo(templatePath)) {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << "template cannot copy to " << templatePath;
        }
    }

    const KArchiveEntry *designerEntry = mArchiveDirectory->entry(Utils::dataPath() + QStringLiteral("korganizer/designer/"));
    if (designerEntry && designerEntry->isDirectory()) {
        const QString templatePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QStringLiteral("korganizer/designer/");
        const KArchiveDirectory *templateDir = static_cast<const KArchiveDirectory *>(designerEntry);
        if (!templateDir->copyTo(templatePath)) {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << "template cannot copy to " << templatePath;
        }
    }

    restoreUiRcFile(QStringLiteral("korganizerui.rc"), QStringLiteral("korganizer"));
    restoreUiRcFile(QStringLiteral("korganizer_part.rc"), QStringLiteral("korganizer"));

    restoreConfigFile(QStringLiteral("eventviewsrc"));
    Q_EMIT info(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportCalendarJobInterface::slotNextStep);
}

void ImportCalendarJobInterface::importkorganizerConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr korganizerConfig = KSharedConfig::openConfig(config);

    convertCollectionListStrToAkonadiId(korganizerConfig, QStringLiteral("GlobalCollectionSelection"), QStringLiteral("Selection"), true);
    convertCollectionListStrToAkonadiId(korganizerConfig, QStringLiteral("CollectionTreeView"), QStringLiteral("Expansion"), true);

    korganizerConfig->sync();
}

void ImportCalendarJobInterface::importeventViewConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr eventviewConfig = KSharedConfig::openConfig(config);

    const QString resourceColorStr(QStringLiteral("Resources Colors"));
    if (eventviewConfig->hasGroup(resourceColorStr)) {
        KConfigGroup group = eventviewConfig->group(resourceColorStr);
        const QStringList keyList = group.keyList();
        for (const QString &key : keyList) {
            const Akonadi::Collection::Id id = convertPathToId(key);
            if (id != -1) {
                const QColor color = group.readEntry(key, QColor());
                group.writeEntry(QString::number(id), color);
            }
            group.deleteEntry(key);
        }
    }

    eventviewConfig->sync();
}

void ImportCalendarJobInterface::restoreResources()
{
    Q_EMIT info(i18n("Restore resources..."));
    setProgressDialogLabel(i18n("Restore resources..."));
    increaseProgressDialog();
    QStringList listResource;
    listResource << restoreResourceFile(QStringLiteral("akonadi_ical_resource"), Utils::calendarPath(), storeCalendar());

    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(Utils::addressbookPath());
        const QString copyToDirName(mTempDirName + QLatin1Char('/') + Utils::calendarPath());

        const int numberOfResourceFile = mListResourceFile.size();
        for (int i = 0; i < numberOfResourceFile; ++i) {
            ResourceFiles value = mListResourceFile.at(i);
            QMap<QString, QVariant> settings;
            if (value.akonadiConfigFile.contains(QLatin1String("akonadi_icaldir_resource_"))
                || value.akonadiConfigFile.contains(QLatin1String("akonadi_ical_resource_"))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                    const KArchiveFile *file = static_cast<const KArchiveFile *>(fileResouceEntry);
                    copyArchiveFileTo(file, copyToDirName);
                    QString resourceName(file->name());

                    QString filename(resourceName);
                    //TODO adapt filename otherwise it will use all the time the same filename.
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " filename :" << filename;

                    KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + QLatin1Char('/') + resourceName);

                    ResourceConverterImpl converter;
                    const QString newUrl = converter.adaptResourcePath(resourceConfig, storeCalendar());
                    QFileInfo newUrlInfo(newUrl);

                    const QString dataFile = value.akonadiResources;
                    const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                    bool isDirResource = value.akonadiConfigFile.contains(QLatin1String("akonadi_icaldir_resource_"));
                    if (dataResouceEntry->isFile()) {
                        const KArchiveFile *file = static_cast<const KArchiveFile *>(dataResouceEntry);
                        //TODO  adapt directory name too
                        extractZipFile(file, copyToDirName, newUrlInfo.path(), value.akonadiConfigFile.contains(QLatin1String("akonadi_icaldir_resource_")));
                    }
                    settings.insert(QStringLiteral("Path"), newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                            const KArchiveFile *file = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                            copyArchiveFileTo(file, copyToDirName);
                            resourceName = file->name();
                            filename = Utils::akonadiAgentName(copyToDirName + QLatin1Char('/') + resourceName);
                        }
                    }
                    const QString resourceTypeName = isDirResource ? QStringLiteral("akonadi_icaldir_resource") : QStringLiteral("akonadi_ical_resource");
                    const QString newResource = createResource(resourceTypeName, filename, settings, true);
                    infoAboutNewResource(newResource);
                    listResource << newResource;
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " newResource" << newResource;
                }
            }
        }
    }
    //It's maildir support. Need to add support
    synchronizeResource(listResource);
}
