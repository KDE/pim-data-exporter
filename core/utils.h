/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

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

enum AppsType { Unknown = 0, KMail, KAddressBook, KAlarm, KOrganizer, KNotes, Akregator };

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

Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString transportsPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString resourcesPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString identitiesPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString mailsPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString configsPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString akonadiPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString dataPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString calendarPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString addressbookPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString alarmPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString infoPath();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString notePath();

Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString storeMails();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString backupnote();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString storeCalendar();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString storeAlarm();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString storeAddressbook();

Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString resourceNoteArchiveName();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString resourceAddressbookArchiveName();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString resourceAlarmArchiveName();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString resourceCalendarArchiveName();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString resourceMailArchiveName();

Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString prefixAkonadiConfigFile();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString exportDataTypeFileName();
Q_REQUIRED_RESULT QString akonadiAgentName(const QString &configPath);
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QList<Utils::AkonadiInstanceInfo> listOfResource();

KZip *openZip(const QString &filename, QString &errorMsg);
PIMDATAEXPORTER_EXPORT void storeDataExportInfo(KZip *archive, const QString &exportInfoFileName);

PIMDATAEXPORTER_EXPORT void addVersion(KZip *archive);
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT int archiveVersion(KZip *archive);

Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT int currentArchiveVersion();
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString appTypeToI18n(AppsType type);
Q_REQUIRED_RESULT PIMDATAEXPORTER_EXPORT QString storedTypeToI18n(StoredType type);
}
Q_DECLARE_TYPEINFO(Utils::AkonadiInstanceInfo, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(Utils::StoredTypes)
