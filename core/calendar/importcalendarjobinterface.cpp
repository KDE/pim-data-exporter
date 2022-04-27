/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importcalendarjobinterface.h"
#include "archivestorage.h"

#include <KArchive>
#include <KArchiveDirectory>
#include <KArchiveEntry>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KZip>

#include "pimdataexportcore_debug.h"

#include <QColor>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>

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

void ImportCalendarJobInterface::addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig,
                                                             const QString &resourceName,
                                                             QMap<QString, QVariant> &settings)
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
    return name.endsWith(QLatin1String("rc"))
        && (name.contains(QLatin1String("akonadi_ical_resource_")) || name.contains(QLatin1String("akonadi_davgroupware_resource_"))
            || name.contains(QLatin1String("akonadi_icaldir_resource_")) || name.contains(QLatin1String("akonadi_openxchange_resource_"))
            || name.contains(QLatin1String("akonadi_google_resource_")));
}

void ImportCalendarJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString korganizerPrinterrcStr(QStringLiteral("calendar_printing.rc"));
    const QString oldKorganizerPrintrrcStr(QStringLiteral("korganizer_printing.rc"));
    const KArchiveEntry *oldKorganizerPrinterEntry = mArchiveDirectory->entry(Utils::configsPath() + oldKorganizerPrintrrcStr);
    if (oldKorganizerPrinterEntry && oldKorganizerPrinterEntry->isFile()) {
        const auto korganizerFile = static_cast<const KArchiveFile *>(oldKorganizerPrinterEntry);
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
            const auto korganizerFile = static_cast<const KArchiveFile *>(korganizerPrinterEntry);
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
        const auto korganizerrcFile = static_cast<const KArchiveFile *>(korganizerrcentry);
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
        const auto eventviewrcFile = static_cast<const KArchiveFile *>(eventviewentry);
        const QString eventviewrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + eventviewStr;
        if (QFileInfo::exists(eventviewrc)) {
            if (overwriteConfigMessageBox(eventviewStr)) {
                importeventViewConfig(eventviewrcFile, eventviewrc, eventviewStr, Utils::configsPath());
            }
        } else {
            importeventViewConfig(eventviewrcFile, eventviewrc, eventviewStr, Utils::configsPath());
        }
    }

    const QString kalendaracStr(QStringLiteral("kalendaracrc"));
    const KArchiveEntry *kalendaracentry = mArchiveDirectory->entry(Utils::configsPath() + kalendaracStr);
    if (kalendaracentry && kalendaracentry->isFile()) {
        const auto kalendaracFile = static_cast<const KArchiveFile *>(kalendaracentry);
        const QString kalendaracrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + kalendaracStr;
        if (QFileInfo::exists(kalendaracrc)) {
            if (overwriteConfigMessageBox(kalendaracStr)) {
                importReminderAgentConfig(kalendaracFile, kalendaracrc, kalendaracStr, Utils::configsPath());
            }
        } else {
            importReminderAgentConfig(kalendaracFile, kalendaracrc, kalendaracStr, Utils::configsPath());
        }
    }

    const QString kalendarStr(QStringLiteral("kalendarrc"));
    const KArchiveEntry *kalendarentry = mArchiveDirectory->entry(Utils::configsPath() + kalendarStr);
    if (kalendarentry && kalendarentry->isFile()) {
        const auto kalendarentryFile = static_cast<const KArchiveFile *>(kalendarentry);
        const QString kalendarentrycrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + kalendaracStr;
        if (QFileInfo::exists(kalendarentrycrc)) {
            if (overwriteConfigMessageBox(kalendaracStr)) {
                importKalendarConfig(kalendarentryFile, kalendarentrycrc, kalendaracStr, Utils::configsPath());
            }
        } else {
            importKalendarConfig(kalendarentryFile, kalendarentrycrc, kalendaracStr, Utils::configsPath());
        }
    }

    const QString freebusyStr(QStringLiteral("freebusyurls"));
    const KArchiveEntry *freebusyentry = mArchiveDirectory->entry(Utils::dataPath() + QLatin1String("korganizer/") + freebusyStr);
    if (freebusyentry && freebusyentry->isFile()) {
        const auto freebusyrcFile = static_cast<const KArchiveFile *>(freebusyentry);

        const QString freebusypath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/korganizer/") + freebusyStr;
        if (QFileInfo::exists(freebusypath)) {
            // TODO 4.12 merge it.
            if (overwriteConfigMessageBox(freebusyStr)) {
                copyToFile(freebusyrcFile, freebusypath, freebusyStr, Utils::dataPath());
            }
        } else {
            copyToFile(freebusyrcFile, freebusypath, freebusyStr, Utils::dataPath());
        }
    }

    const KArchiveEntry *templateEntry = mArchiveDirectory->entry(Utils::dataPath() + QStringLiteral("korganizer/templates/"));
    if (templateEntry && templateEntry->isDirectory()) {
        // TODO 4.12 verify if template already exists.
        const QString templatePath =
            QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QStringLiteral("korganizer/templates/");
        const auto templateDir = static_cast<const KArchiveDirectory *>(templateEntry);
        if (!templateDir->copyTo(templatePath)) {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << "template cannot copy to " << templatePath;
        }
    }

    const KArchiveEntry *designerEntry = mArchiveDirectory->entry(Utils::dataPath() + QStringLiteral("korganizer/designer/"));
    if (designerEntry && designerEntry->isDirectory()) {
        const QString templatePath =
            QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QStringLiteral("korganizer/designer/");
        const auto templateDir = static_cast<const KArchiveDirectory *>(designerEntry);
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
    convertCollectionListStrToAkonadiId(korganizerConfig, QStringLiteral("GlobalCollectionView"), QStringLiteral("Expansion"), true);

    korganizerConfig->sync();
}

void ImportCalendarJobInterface::importKalendarConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr kalendarConfig = KSharedConfig::openConfig(config);

    convertCollectionListStrToAkonadiId(kalendarConfig, QStringLiteral("GlobalCollectionSelection"), QStringLiteral("Selection"), true);
    // TODO Resources Colors

    kalendarConfig->sync();
}

void ImportCalendarJobInterface::importReminderAgentConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr reminderAgentConfig = KSharedConfig::openConfig(config);
    convertCollectionListStrToAkonadiId(reminderAgentConfig, QStringLiteral("Alarms"), QStringLiteral("CalendarsLastChecked"), false);
    reminderAgentConfig->sync();
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
    listResource << restoreResourceFile(QStringLiteral("akonadi_ical_resource"), Utils::calendarPath(), Utils::storeCalendar());
    listResource << restoreResourceFile(QStringLiteral("akonadi_davgroupware_resource"), Utils::calendarPath(), Utils::storeCalendar());
    listResource << restoreResourceFile(QStringLiteral("akonadi_openxchange_resource"), Utils::calendarPath(), Utils::storeCalendar());
    listResource << restoreResourceFile(QStringLiteral("akonadi_google_resource"), Utils::calendarPath(), Utils::storeCalendar());

    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(Utils::addressbookPath());
        const QString copyToDirName(mTempDirName + QLatin1Char('/') + Utils::calendarPath());
        QDir().mkpath(copyToDirName);
        const int numberOfResourceFile = mListResourceFile.size();
        for (int i = 0; i < numberOfResourceFile; ++i) {
            ResourceFiles value = mListResourceFile.at(i);
            QMap<QString, QVariant> settings;
            if (value.akonadiConfigFile.contains(QLatin1String("akonadi_icaldir_resource_"))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                    const auto file = static_cast<const KArchiveFile *>(fileResouceEntry);
                    copyArchiveFileTo(file, copyToDirName);
                    QString resourceName(file->name());

                    QString filename(resourceName);
                    // TODO adapt filename otherwise it will use all the time the
                    // same filename.
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " filename :" << filename;

                    KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + QLatin1Char('/') + resourceName);

                    const QString newUrl = adaptResourcePath(resourceConfig, Utils::storeCalendar());
                    QFileInfo newUrlInfo(newUrl);

                    const QString dataFile = value.akonadiResources;
                    const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                    bool isDirResource = value.akonadiConfigFile.contains(QLatin1String("akonadi_icaldir_resource_"));
                    if (dataResouceEntry->isFile()) {
                        const auto fileEntry = static_cast<const KArchiveFile *>(dataResouceEntry);
                        // TODO  adapt directory name too
                        extractZipFile(fileEntry,
                                       copyToDirName,
                                       newUrlInfo.path(),
                                       value.akonadiConfigFile.contains(QLatin1String("akonadi_icaldir_resource_")));
                    }
                    settings.insert(QStringLiteral("Path"), newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                            const auto fileEntry = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                            copyArchiveFileTo(fileEntry, copyToDirName);
                            resourceName = fileEntry->name();
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
    // It's maildir support. Need to add support
    synchronizeResource(listResource);
}

QString ImportCalendarJobInterface::configLocation() const
{
    return installConfigLocation();
}

QString ImportCalendarJobInterface::installConfigLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/');
}
