/*
   SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importcalendarjobinterface.h"
using namespace Qt::Literals::StringLiterals;

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

ImportCalendarJobInterface::~ImportCalendarJobInterface() = default;

void ImportCalendarJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import KOrganizer settings..."));
    mArchiveDirectory = archive()->directory();
    createProgressDialog(i18n("Import KOrganizer settings"));
    searchAllFiles(mArchiveDirectory, QString(), u"calendar"_s);
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
    if (resourceName == QLatin1StringView("akonadi_ical_resource")) {
        KConfigGroup general = resourceConfig->group(u"General"_s);
        if (general.hasKey(u"DisplayName"_s)) {
            settings.insert(u"DisplayName"_s, general.readEntry(u"DisplayName"_s));
        }
        if (general.hasKey(u"ReadOnly"_s)) {
            settings.insert(u"ReadOnly"_s, general.readEntry(u"ReadOnly"_s, false));
        }
        if (general.hasKey(u"MonitorFile"_s)) {
            settings.insert(u"MonitorFile"_s, general.readEntry(u"MonitorFile"_s, true));
        }
    }
}

bool ImportCalendarJobInterface::isAConfigFile(const QString &name) const
{
    return name.endsWith(QLatin1StringView("rc"))
        && (name.contains(QLatin1StringView("akonadi_ical_resource_")) || name.contains(QLatin1StringView("akonadi_davgroupware_resource_"))
            || name.contains(QLatin1StringView("akonadi_icaldir_resource_")) || name.contains(QLatin1StringView("akonadi_openxchange_resource_"))
            || name.contains(QLatin1StringView("akonadi_google_resource_")));
}

void ImportCalendarJobInterface::restoreConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    const QString korganizerPrinterrcStr(u"calendar_printing.rc"_s);
    const QString oldKorganizerPrintrrcStr(u"korganizer_printing.rc"_s);
    const KArchiveEntry *oldKorganizerPrinterEntry = mArchiveDirectory->entry(Utils::configsPath() + oldKorganizerPrintrrcStr);
    if (oldKorganizerPrinterEntry && oldKorganizerPrinterEntry->isFile()) {
        const auto korganizerFile = static_cast<const KArchiveFile *>(oldKorganizerPrinterEntry);
        const QString oldKorganizerPrintrrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + korganizerPrinterrcStr;
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
            const QString korganizerPrinterrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + korganizerPrinterrcStr;
            if (QFileInfo::exists(korganizerPrinterrc)) {
                if (overwriteConfigMessageBox(korganizerPrinterrcStr)) {
                    copyToFile(korganizerFile, korganizerPrinterrc, korganizerPrinterrcStr, Utils::configsPath());
                }
            } else {
                copyToFile(korganizerFile, korganizerPrinterrc, korganizerPrinterrcStr, Utils::configsPath());
            }
        }
    }

    {
        const QString korganizerStr(u"korganizerrc"_s);
        const KArchiveEntry *korganizerrcentry = mArchiveDirectory->entry(Utils::configsPath() + korganizerStr);
        if (korganizerrcentry && korganizerrcentry->isFile()) {
            const auto korganizerrcFile = static_cast<const KArchiveFile *>(korganizerrcentry);
            const QString korganizerrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + korganizerStr;
            if (QFileInfo::exists(korganizerrc)) {
                if (overwriteConfigMessageBox(korganizerStr)) {
                    importkorganizerConfig(korganizerrcFile, korganizerrc, korganizerStr, Utils::configsPath());
                }
            } else {
                importkorganizerConfig(korganizerrcFile, korganizerrc, korganizerStr, Utils::configsPath());
            }
        }
    }

    {
        const QString eventviewStr(u"eventviewsrc"_s);
        const KArchiveEntry *eventviewentry = mArchiveDirectory->entry(Utils::configsPath() + eventviewStr);
        if (eventviewentry && eventviewentry->isFile()) {
            const auto eventviewrcFile = static_cast<const KArchiveFile *>(eventviewentry);
            const QString eventviewrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + eventviewStr;
            if (QFileInfo::exists(eventviewrc)) {
                if (overwriteConfigMessageBox(eventviewStr)) {
                    importeventViewConfig(eventviewrcFile, eventviewrc, eventviewStr, Utils::configsPath());
                }
            } else {
                importeventViewConfig(eventviewrcFile, eventviewrc, eventviewStr, Utils::configsPath());
            }
        }
    }
    {
        const QString kalendaracStr(u"kalendaracrc"_s);
        const KArchiveEntry *kalendaracentry = mArchiveDirectory->entry(Utils::configsPath() + kalendaracStr);
        if (kalendaracentry && kalendaracentry->isFile()) {
            const auto kalendaracFile = static_cast<const KArchiveFile *>(kalendaracentry);
            const QString kalendaracrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + kalendaracStr;
            if (QFileInfo::exists(kalendaracrc)) {
                if (overwriteConfigMessageBox(kalendaracStr)) {
                    copyToFile(kalendaracFile, kalendaracrc, kalendaracStr, Utils::configsPath());
                }
            } else {
                copyToFile(kalendaracFile, kalendaracrc, kalendaracStr, Utils::configsPath());
            }
        }
    }
    {
        const QString kalendarStr(u"kalendarrc"_s);
        const KArchiveEntry *kalendarentry = mArchiveDirectory->entry(Utils::configsPath() + kalendarStr);
        if (kalendarentry && kalendarentry->isFile()) {
            const auto kalendarentryFile = static_cast<const KArchiveFile *>(kalendarentry);
            const QString kalendarentrycrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + kalendarStr;
            if (QFileInfo::exists(kalendarentrycrc)) {
                if (overwriteConfigMessageBox(kalendarStr)) {
                    importKalendarConfig(kalendarentryFile, kalendarentrycrc, kalendarStr, Utils::configsPath());
                }
            } else {
                importKalendarConfig(kalendarentryFile, kalendarentrycrc, kalendarStr, Utils::configsPath());
            }
        }
    }

    {
        const QString freebusyStr(u"freebusyurls"_s);
        const KArchiveEntry *freebusyentry = mArchiveDirectory->entry(Utils::dataPath() + QLatin1StringView("korganizer/") + freebusyStr);
        if (freebusyentry && freebusyentry->isFile()) {
            const auto freebusyrcFile = static_cast<const KArchiveFile *>(freebusyentry);

            const QString freebusypath =
                QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1StringView("/korganizer/") + freebusyStr;
            if (QFileInfo::exists(freebusypath)) {
                // TODO 4.12 merge it.
                if (overwriteConfigMessageBox(freebusyStr)) {
                    copyToFile(freebusyrcFile, freebusypath, freebusyStr, Utils::dataPath());
                }
            } else {
                copyToFile(freebusyrcFile, freebusypath, freebusyStr, Utils::dataPath());
            }
        }
    }

    {
        const KArchiveEntry *templateEntry = mArchiveDirectory->entry(Utils::dataPath() + u"korganizer/templates/"_s);
        if (templateEntry && templateEntry->isDirectory()) {
            // TODO 4.12 verify if template already exists.
            const QString templatePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u'/' + u"korganizer/templates/"_s;
            const auto templateDir = static_cast<const KArchiveDirectory *>(templateEntry);
            if (!templateDir->copyTo(templatePath)) {
                qCDebug(PIMDATAEXPORTERCORE_LOG) << "template cannot copy to " << templatePath;
            }
        }
    }

    {
        const KArchiveEntry *designerEntry = mArchiveDirectory->entry(Utils::dataPath() + u"korganizer/designer/"_s);
        if (designerEntry && designerEntry->isDirectory()) {
            const QString templatePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u'/' + u"korganizer/designer/"_s;
            const auto templateDir = static_cast<const KArchiveDirectory *>(designerEntry);
            if (!templateDir->copyTo(templatePath)) {
                qCDebug(PIMDATAEXPORTERCORE_LOG) << "template cannot copy to " << templatePath;
            }
        }
    }

    restoreUiRcFile(u"korganizerui.rc"_s, u"korganizer"_s);
    restoreUiRcFile(u"korganizer_part.rc"_s, u"korganizer"_s);

    restoreConfigFile(u"eventviewsrc"_s);
    emitInfo(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportCalendarJobInterface::slotNextStep);
}

void ImportCalendarJobInterface::importkorganizerConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr korganizerConfig = KSharedConfig::openConfig(config);

    convertCollectionListStrToAkonadiId(korganizerConfig, u"GlobalCollectionSelection"_s, u"Selection"_s, true);
    convertCollectionListStrToAkonadiId(korganizerConfig, u"CollectionTreeView"_s, u"Expansion"_s, true);
    convertCollectionListStrToAkonadiId(korganizerConfig, u"GlobalCollectionView"_s, u"Expansion"_s, true);

    korganizerConfig->sync();
}

void ImportCalendarJobInterface::importKalendarConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr kalendarConfig = KSharedConfig::openConfig(config);

    convertCollectionListStrToAkonadiId(kalendarConfig, u"GlobalCollectionSelection"_s, u"Selection"_s, true);
    convertResourceColors(kalendarConfig);
    kalendarConfig->sync();
}

void ImportCalendarJobInterface::importeventViewConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix)
{
    copyToFile(file, config, filename, prefix);
    KSharedConfig::Ptr eventviewConfig = KSharedConfig::openConfig(config);
    convertResourceColors(eventviewConfig);
    eventviewConfig->sync();
}

void ImportCalendarJobInterface::convertResourceColors(const KSharedConfig::Ptr &config)
{
    const QString resourceColorStr(u"Resources Colors"_s);
    if (config->hasGroup(resourceColorStr)) {
        KConfigGroup group = config->group(resourceColorStr);
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
}

void ImportCalendarJobInterface::restoreResources()
{
    emitInfo(i18n("Restore resources..."));
    setProgressDialogLabel(i18n("Restore resources..."));
    increaseProgressDialog();
    QStringList listResource;
    listResource << restoreResourceFile(u"akonadi_ical_resource"_s, Utils::calendarPath(), Utils::storeCalendar());
    listResource << restoreResourceFile(u"akonadi_davgroupware_resource"_s, Utils::calendarPath(), Utils::storeCalendar());
    listResource << restoreResourceFile(u"akonadi_openxchange_resource"_s, Utils::calendarPath(), Utils::storeCalendar());
    listResource << restoreResourceFile(u"akonadi_google_resource"_s, Utils::calendarPath(), Utils::storeCalendar());

    if (!mListResourceFile.isEmpty()) {
        QDir dir(mTempDirName);
        dir.mkdir(Utils::addressbookPath());
        const QString copyToDirName(mTempDirName + u'/' + Utils::calendarPath());
        QDir().mkpath(copyToDirName);
        const int numberOfResourceFile = mListResourceFile.size();
        for (int i = 0; i < numberOfResourceFile; ++i) {
            ResourceFiles value = mListResourceFile.at(i);
            if (value.akonadiConfigFile.contains(QLatin1StringView("akonadi_icaldir_resource_"))) {
                const KArchiveEntry *fileResouceEntry = mArchiveDirectory->entry(value.akonadiConfigFile);
                if (fileResouceEntry && fileResouceEntry->isFile()) {
                    const auto file = static_cast<const KArchiveFile *>(fileResouceEntry);
                    copyArchiveFileTo(file, copyToDirName);
                    QString resourceName(file->name());

                    QString filename(resourceName);
                    // TODO adapt filename otherwise it will use all the time the
                    // same filename.
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << " filename :" << filename;

                    KSharedConfig::Ptr resourceConfig = KSharedConfig::openConfig(copyToDirName + u'/' + resourceName);

                    const QString newUrl = adaptResourcePath(resourceConfig, Utils::storeCalendar());
                    QFileInfo newUrlInfo(newUrl);

                    const QString dataFile = value.akonadiResources;
                    const KArchiveEntry *dataResouceEntry = mArchiveDirectory->entry(dataFile);
                    bool isDirResource = value.akonadiConfigFile.contains(QLatin1StringView("akonadi_icaldir_resource_"));
                    if (dataResouceEntry->isFile()) {
                        const auto fileEntry = static_cast<const KArchiveFile *>(dataResouceEntry);
                        // TODO  adapt directory name too
                        extractZipFile(fileEntry,
                                       copyToDirName,
                                       newUrlInfo.path(),
                                       value.akonadiConfigFile.contains(QLatin1StringView("akonadi_icaldir_resource_")));
                    }
                    QMap<QString, QVariant> settings;
                    settings.insert(u"Path"_s, newUrl);

                    const QString agentConfigFile = value.akonadiAgentConfigFile;
                    if (!agentConfigFile.isEmpty()) {
                        const KArchiveEntry *akonadiAgentConfigEntry = mArchiveDirectory->entry(agentConfigFile);
                        if (akonadiAgentConfigEntry->isFile()) {
                            const auto fileEntry = static_cast<const KArchiveFile *>(akonadiAgentConfigEntry);
                            copyArchiveFileTo(fileEntry, copyToDirName);
                            resourceName = fileEntry->name();
                            filename = Utils::akonadiAgentName(copyToDirName + u'/' + resourceName);
                        }
                    }
                    const QString resourceTypeName = isDirResource ? u"akonadi_icaldir_resource"_s : u"akonadi_ical_resource"_s;
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

QString ImportCalendarJobInterface::applicationName() const
{
    return u"[Korganizer]"_s;
}

QString ImportCalendarJobInterface::installConfigLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/';
}

#include "moc_importcalendarjobinterface.cpp"
