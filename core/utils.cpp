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

#include "utils.h"

#include <MailCommon/MailUtil>
#include <PimCommon/PimUtil>

#include <QSettings>
#include <KConfigGroup>

#include <KSharedConfig>
#include <QTemporaryFile>
#include <KLocalizedString>
#include <KZip>

#include <AkonadiCore/AgentManager>
#include <QDir>
#include <QStandardPaths>

QString Utils::storeAddressbook()
{
    return QStringLiteral("backupaddressbook/");
}

QString Utils::storeAlarm()
{
    return QStringLiteral("backupalarm/");
}

QString Utils::storeCalendar()
{
    return QStringLiteral("backupcalendar/");
}

QString Utils::backupnote()
{
    return QStringLiteral("backupnote/");
}

QString Utils::storeMails()
{
    return QStringLiteral("backupmail/");
}

QString Utils::exportDataTypeFileName()
{
    return QStringLiteral("exportdatatype.xml");
}

int Utils::currentArchiveVersion()
{
    //Increase it when we add major feature!
    return 2;
}

QString Utils::transportsPath()
{
    return QStringLiteral("transports/");
}

QString Utils::resourcesPath()
{
    return QStringLiteral("resources/");
}

QString Utils::identitiesPath()
{
    return QStringLiteral("identities/");
}

QString Utils::mailsPath()
{
    return QStringLiteral("mails/");
}

QString Utils::configsPath()
{
    return QStringLiteral("configs/");
}

QString Utils::akonadiPath()
{
    return QStringLiteral("akonadi/");
}

QString Utils::dataPath()
{
    return QStringLiteral("data/");
}

QString Utils::calendarPath()
{
    return QStringLiteral("calendar/");
}

QString Utils::addressbookPath()
{
    return QStringLiteral("addressbook/");
}

QString Utils::alarmPath()
{
    return QStringLiteral("alarm/");
}

QString Utils::notePath()
{
    return QStringLiteral("note/");
}

QString Utils::prefixAkonadiConfigFile()
{
    return QStringLiteral("agent_config_");
}

QString Utils::infoPath()
{
    return QStringLiteral("information/");
}

QString Utils::akonadiAgentName(const QString &configPath)
{
    QSettings settings(configPath, QSettings::IniFormat);
    const QString name = settings.value(QStringLiteral("Agent/Name")).toString();
    return name;
}

KZip *Utils::openZip(const QString &filename, QString &errorMsg)
{
    KZip *zip = new KZip(filename);
    const bool result = zip->open(QIODevice::ReadOnly);
    if (!result) {
        errorMsg = i18n("Archive cannot be opened in read mode.");
        delete zip;
        return nullptr;
    }
    return zip;
}

void Utils::storeDataExportInfo(KZip *archive)
{
    QTemporaryFile tmp;
    tmp.open();
    const bool fileAdded = archive->addLocalFile(tmp.fileName(), Utils::infoPath() + Utils::exportDataTypeFileName());
    if (fileAdded) {
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "storeDataExportInfo can't add to archive";
    }
}

void Utils::addVersion(KZip *archive)
{
    QTemporaryFile tmp;
    tmp.open();
    const bool fileAdded = archive->addLocalFile(tmp.fileName(), Utils::infoPath() + QStringLiteral("VERSION_%1").arg(currentArchiveVersion()));
    if (!fileAdded) {
        //TODO add i18n ?
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "version file can not add to archive";
    }
}

int Utils::archiveVersion(KZip *archive)
{
    const KArchiveEntry *informationFile = archive->directory()->entry(Utils::infoPath() + QStringLiteral("VERSION_2"));
    if (informationFile && informationFile->isFile()) {
        return 2;
    }
    informationFile = archive->directory()->entry(Utils::infoPath() + QStringLiteral("VERSION_1"));
    if (informationFile && informationFile->isFile()) {
        return 1;
    }
    //TODO add more version when new version
    return 0;
}

QString Utils::appTypeToI18n(AppsType type)
{
    switch (type) {
    case KMail:
        return i18n("KMail");
    case KAddressBook:
        return i18n("KAddressBook");
    case KAlarm:
        return i18n("KAlarm");
    case KOrganizer:
        return i18n("KOrganizer");
    case KNotes:
        return i18n("KNotes");
    case Akregator:
        return i18n("Akregator");
    case Unknown:
        break;
    }
    qCDebug(PIMDATAEXPORTERCORE_LOG) << " type unknown " << type;
    return QString();
}

QString Utils::storedTypeToI18n(StoredType type)
{
    switch (type) {
    case None:
        return QString();
    case Identity:
        return i18n("Identity");
    case Mails:
        return i18n("Mails");
    case MailTransport:
        return i18n("Mail Transport");
    case Resources:
        return i18n("Resources");
    case Config:
        return i18n("Config");
    case Data:
        return i18n("Data");
    }
    qCDebug(PIMDATAEXPORTERCORE_LOG) << " type unknown " << type;
    return QString();
}

QVector<Utils::AkonadiInstanceInfo> Utils::listOfResource()
{
    QVector<Utils::AkonadiInstanceInfo> instanceInfoList;
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    for (const Akonadi::AgentInstance &agent : list) {
        Utils::AkonadiInstanceInfo info;
        info.identifier = agent.identifier();
        info.mimeTypes = agent.type().mimeTypes();
        info.capabilities = agent.type().capabilities();
        instanceInfoList.append(info);
    }
    return instanceInfoList;
}

QString Utils::resourceNoteArchiveName()
{
    return QStringLiteral("notes.zip");
}

QString Utils::resourceAddressbookArchiveName()
{
    return QStringLiteral("addressbook.zip");
}

QString Utils::resourceAlarmArchiveName()
{
    return QStringLiteral("alarm.zip");
}

QString Utils::resourceCalendarArchiveName()
{
    return QStringLiteral("calendar.zip");
}

QString Utils::resourceMailArchiveName()
{
    return QStringLiteral("mail.zip");
}
