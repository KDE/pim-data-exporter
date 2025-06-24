/*
   SPDX-FileCopyrightText: 2012-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "utils.h"
using namespace Qt::Literals::StringLiterals;

#include <MailCommon/MailUtil>
#include <PimCommon/PimUtil>

#include <QSettings>

#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>

#include <Akonadi/AgentManager>
#include <QStandardPaths>

QString Utils::storeAddressbook()
{
    return u"backupaddressbook/"_s;
}

QString Utils::storeAlarm()
{
    return u"backupalarm/"_s;
}

QString Utils::storeCalendar()
{
    return u"backupcalendar/"_s;
}

QString Utils::backupnote()
{
    return u"backupnote/"_s;
}

QString Utils::storeMails()
{
    return u"backupmail/"_s;
}

QString Utils::exportDataTypeFileName()
{
    return u"exportdatatype.xml"_s;
}

int Utils::currentArchiveVersion()
{
    // Increase it when we add major feature!
    return 2;
}

QString Utils::transportsPath()
{
    return u"transports/"_s;
}

QString Utils::resourcesPath()
{
    return u"resources/"_s;
}

QString Utils::identitiesPath()
{
    return u"identities/"_s;
}

QString Utils::mailsPath()
{
    return u"mails/"_s;
}

QString Utils::configsPath()
{
    return u"configs/"_s;
}

QString Utils::akonadiPath()
{
    return u"akonadi/"_s;
}

QString Utils::dataPath()
{
    return u"data/"_s;
}

QString Utils::calendarPath()
{
    return u"calendar/"_s;
}

QString Utils::addressbookPath()
{
    return u"addressbook/"_s;
}

QString Utils::alarmPath()
{
    return u"alarm/"_s;
}

QString Utils::notePath()
{
    return u"note/"_s;
}

QString Utils::prefixAkonadiConfigFile()
{
    return u"agent_config_"_s;
}

QString Utils::infoPath()
{
    return u"information/"_s;
}

QString Utils::akonadiAgentName(const QString &configPath)
{
    QSettings settings(configPath, QSettings::IniFormat);
    const QString name = settings.value(u"Agent/Name"_s).toString();
    return name;
}

KZip *Utils::openZip(const QString &filename, QString &errorMsg)
{
    auto zip = new KZip(filename);
    const bool result = zip->open(QIODevice::ReadOnly);
    if (!result) {
        errorMsg = i18n("Archive cannot be opened in read mode.");
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "Impossible to open archive: " << filename << "zip error : " << zip->errorString();
        delete zip;
        return nullptr;
    }
    return zip;
}

void Utils::storeDataExportInfo(KZip *archive, const QString &exportInfoFileName)
{
    if (!exportInfoFileName.isEmpty()) {
        const bool fileAdded = archive->addLocalFile(exportInfoFileName, Utils::infoPath() + Utils::exportDataTypeFileName());
        if (!fileAdded) {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << "storeDataExportInfo can't add to archive" << Utils::infoPath() + Utils::exportDataTypeFileName();
        }
    }
}

void Utils::addVersion(KZip *archive)
{
    QTemporaryFile tmp;
    tmp.open();
    const bool fileAdded = archive->addLocalFile(tmp.fileName(), Utils::infoPath() + u"VERSION_%1"_s.arg(currentArchiveVersion()));
    if (!fileAdded) {
        // TODO add i18n ?
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "version file can not add to archive";
    }
}

int Utils::archiveVersion(KZip *archive)
{
    const KArchiveEntry *informationFile = archive->directory()->entry(Utils::infoPath() + u"VERSION_2"_s);
    if (informationFile && informationFile->isFile()) {
        return 2;
    }
    informationFile = archive->directory()->entry(Utils::infoPath() + u"VERSION_1"_s);
    if (informationFile && informationFile->isFile()) {
        return 1;
    }
    // TODO add more version when new version
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
    case Akregator:
        return i18n("Akregator");
    case Unknown:
        break;
    }
    qCDebug(PIMDATAEXPORTERCORE_LOG) << " type unknown " << type;
    return {};
}

QString Utils::storedTypeToI18n(StoredType type)
{
    switch (type) {
    case None:
        return {};
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
    return {};
}

QList<Utils::AkonadiInstanceInfo> Utils::listOfResource()
{
    QList<Utils::AkonadiInstanceInfo> instanceInfoList;
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    instanceInfoList.reserve(list.count());
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
    return u"notes.zip"_s;
}

QString Utils::resourceAddressbookArchiveName()
{
    return u"addressbook.zip"_s;
}

QString Utils::resourceAlarmArchiveName()
{
    return u"alarm.zip"_s;
}

QString Utils::resourceCalendarArchiveName()
{
    return u"calendar.zip"_s;
}

QString Utils::resourceMailArchiveName()
{
    return u"mail.zip"_s;
}
