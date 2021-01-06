/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef UTILS_H
#define UTILS_H
#include "pimdataexporter_export.h"
#include <QString>
#include <KSharedConfig>
#include "pimdataexportcore_debug.h"
class KZip;

struct ResourceFiles {
    QString akonadiConfigFile;
    QString akonadiResources;
    QString akonadiAgentConfigFile;
    void debug() const
    {
        qDebug() << " akonadiconfigfile :" << akonadiConfigFile << " akonadiResources:" << akonadiResources << " akonadiAgentConfigFile:" << akonadiAgentConfigFile;
    }

    bool operator<(const ResourceFiles &other) const
    {
        return other.akonadiConfigFile < akonadiConfigFile;
    }
};

namespace Utils {
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
           //TODO add more type to import/export
};
Q_DECLARE_FLAGS(StoredTypes, StoredType)

enum AppsType {
    Unknown = 0,
    KMail,
    KAddressBook,
    KAlarm,
    KOrganizer,
    KNotes,
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

PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString transportsPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourcesPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString identitiesPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString mailsPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString configsPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString akonadiPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString dataPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString calendarPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString addressbookPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString alarmPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString infoPath();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString notePath();

PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString storeMails();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString backupnote();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString storeCalendar();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString storeAlarm();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString storeAddressbook();

PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceNoteArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceAddressbookArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceAlarmArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceCalendarArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceMailArchiveName();

PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString prefixAkonadiConfigFile();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString exportDataTypeFileName();
Q_REQUIRED_RESULT QString akonadiAgentName(const QString &configPath);
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource();

KZip *openZip(const QString &filename, QString &errorMsg);
PIMDATAEXPORTER_EXPORT void storeDataExportInfo(KZip *archive, const QString &exportInfoFileName);

PIMDATAEXPORTER_EXPORT void addVersion(KZip *archive);
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT int archiveVersion(KZip *archive);

PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT int currentArchiveVersion();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString appTypeToI18n(AppsType type);
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString storedTypeToI18n(StoredType type);
}
Q_DECLARE_TYPEINFO(Utils::AkonadiInstanceInfo, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(Utils::StoredTypes)
#endif // UTILS_H
