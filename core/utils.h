/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "pimdataexportcore_debug.h"
#include "pimdataexporter_export.h"
#include <KSharedConfig>
#include <QString>
class KZip;

struct ResourceFiles {
    QString akonadiConfigFile;
    QString akonadiResources;
    QString akonadiAgentConfigFile;
    void debug() const
    {
        qDebug() << " akonadiconfigfile :" << akonadiConfigFile << " akonadiResources:" << akonadiResources
                 << " akonadiAgentConfigFile:" << akonadiAgentConfigFile;
    }

    bool operator<(const ResourceFiles &other) const
    {
        return other.akonadiConfigFile < akonadiConfigFile;
    }
};

namespace Utils
{
struct AkonadiInstanceInfo {
    QString identifier;
    QStringList mimeTypes;
    QStringList capabilities;
};
enum StoredType {
    None = 0,
    Identity = 1,
    Mails = 2,
    MailTransport = 4,
    Resources = 8,
    Config = 16,
    Data = 32
    // TODO add more type to import/export
};
Q_DECLARE_FLAGS(StoredTypes, StoredType)

enum AppsType {
    Unknown = 0,
    KMail,
    KAddressBook,
    KAlarm,
    KOrganizer,
    Akregator
};

struct importExportParameters {
    importExportParameters()
        : numberSteps(0)
        , types(None)
    {
    }

    bool isEmpty() const
    {
        return types == None;
    }

    int numberSteps;
    Utils::StoredTypes types;
};

[[nodiscard]] PIMDATAEXPORTER_EXPORT QString transportsPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString resourcesPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString identitiesPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString mailsPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString configsPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString akonadiPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString dataPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString calendarPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString addressbookPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString alarmPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString infoPath();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString notePath();

[[nodiscard]] PIMDATAEXPORTER_EXPORT QString storeMails();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString backupnote();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString storeCalendar();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString storeAlarm();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString storeAddressbook();

[[nodiscard]] PIMDATAEXPORTER_EXPORT QString resourceNoteArchiveName();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString resourceAddressbookArchiveName();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString resourceAlarmArchiveName();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString resourceCalendarArchiveName();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString resourceMailArchiveName();

[[nodiscard]] PIMDATAEXPORTER_EXPORT QString prefixAkonadiConfigFile();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString exportDataTypeFileName();
[[nodiscard]] QString akonadiAgentName(const QString &configPath);
[[nodiscard]] PIMDATAEXPORTER_EXPORT QList<Utils::AkonadiInstanceInfo> listOfResource();

KZip *openZip(const QString &filename, QString &errorMsg);
PIMDATAEXPORTER_EXPORT void storeDataExportInfo(KZip *archive, const QString &exportInfoFileName);

PIMDATAEXPORTER_EXPORT void addVersion(KZip *archive);
[[nodiscard]] PIMDATAEXPORTER_EXPORT int archiveVersion(KZip *archive);

[[nodiscard]] PIMDATAEXPORTER_EXPORT int currentArchiveVersion();
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString appTypeToI18n(AppsType type);
[[nodiscard]] PIMDATAEXPORTER_EXPORT QString storedTypeToI18n(StoredType type);
}
Q_DECLARE_TYPEINFO(Utils::AkonadiInstanceInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_METATYPE(Utils::StoredTypes)
