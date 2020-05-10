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
        qCDebug(PIMDATAEXPORTERCORE_LOG) << " akonadiconfigfile :" << akonadiConfigFile << " akonadiResources:" << akonadiResources << " akonadiAgentConfigFile:" << akonadiAgentConfigFile;
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

PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceNoteArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceAddressbookArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceAlarmArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceCalendarArchiveName();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString resourceMailArchiveName();


PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString prefixAkonadiConfigFile();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString exportDataTypeFileName();
Q_REQUIRED_RESULT QString akonadiAgentName(const QString &configPath);
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource();

Q_REQUIRED_RESULT QString storeResources(KZip *archive, const QString &identifier, const QString &path);
KZip *openZip(const QString &filename, QString &errorMsg);
PIMDATAEXPORTER_EXPORT void storeDataExportInfo(KZip *archive);

PIMDATAEXPORTER_EXPORT void addVersion(KZip *archive);
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT int archiveVersion(KZip *archive);

PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT int currentArchiveVersion();
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString appTypeToI18n(AppsType type);
PIMDATAEXPORTER_EXPORT Q_REQUIRED_RESULT QString storedTypeToI18n(StoredType type);
}
Q_DECLARE_TYPEINFO(Utils::AkonadiInstanceInfo, Q_MOVABLE_TYPE);
#endif // UTILS_H
