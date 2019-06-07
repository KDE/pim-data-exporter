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

#ifndef UTILS_H
#define UTILS_H
#include "pimdataexporter_export.h"
#include <QString>
#include <QUrl>
#include <KSharedConfig>
#include "pimsettingexportcore_debug.h"
class KZip;
namespace Akonadi {
class AgentInstance;
}

struct resourceFiles {
    QString akonadiConfigFile;
    QString akonadiResources;
    QString akonadiAgentConfigFile;
    void debug() const
    {
        qCDebug(PIMSETTINGEXPORTERCORE_LOG) << " akonadiconfigfile :" << akonadiConfigFile << " akonadiResources:" << akonadiResources << " akonadiAgentConfigFile:" << akonadiAgentConfigFile;
    }
};

namespace Utils {
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

QString resourcePath(const KSharedConfigPtr &resourceConfig, const QString &defaultPath = QString());
PIMDATAEXPORTER_EXPORT QString transportsPath();
PIMDATAEXPORTER_EXPORT QString resourcesPath();
PIMDATAEXPORTER_EXPORT QString identitiesPath();
PIMDATAEXPORTER_EXPORT QString mailsPath();
PIMDATAEXPORTER_EXPORT QString configsPath();
PIMDATAEXPORTER_EXPORT QString akonadiPath();
PIMDATAEXPORTER_EXPORT QString dataPath();
PIMDATAEXPORTER_EXPORT QString calendarPath();
PIMDATAEXPORTER_EXPORT QString addressbookPath();
PIMDATAEXPORTER_EXPORT QString alarmPath();
PIMDATAEXPORTER_EXPORT QString jotPath();
PIMDATAEXPORTER_EXPORT QString infoPath();
PIMDATAEXPORTER_EXPORT QString notePath();
PIMDATAEXPORTER_EXPORT QString prefixAkonadiConfigFile();
PIMDATAEXPORTER_EXPORT QString exportDataTypeFileName();
QString akonadiAgentName(const QString &configPath);

void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey);
void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey);
void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection = QString());

QString resourcePath(const Akonadi::AgentInstance &agent, const QString &defaultPath = QString());
QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData);
QString storeResources(KZip *archive, const QString &identifier, const QString &path);
KZip *openZip(const QString &filename, QString &errorMsg);
void storeDataExportInfo(const QString &filename, KZip *archive);

void addVersion(KZip *archive);
int archiveVersion(KZip *archive);

int currentArchiveVersion();
PIMDATAEXPORTER_EXPORT QString appTypeToI18n(AppsType type);
PIMDATAEXPORTER_EXPORT QString storedTypeToI18n(StoredType type);
}
#endif // UTILS_H
