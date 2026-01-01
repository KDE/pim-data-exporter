/*
   SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "templateselection.h"
using namespace Qt::Literals::StringLiterals;

#include "pimdataexportcore_debug.h"
#include <QFile>
#include <QXmlStreamWriter>

TemplateSelection::TemplateSelection() = default;

TemplateSelection::~TemplateSelection()
{
    delete mStreamWriter;
    delete mStreamReader;
}

Utils::StoredTypes TemplateSelection::loadStoredTypes(int &numberOfStep)
{
    Utils::StoredTypes types = Utils::None;
    while (mStreamReader->readNextStartElement()) {
        if (mStreamReader->name() == QLatin1StringView("mailtransport")) {
            types |= Utils::MailTransport;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1StringView("mail")) {
            types |= Utils::Mails;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1StringView("resources")) {
            types |= Utils::Resources;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1StringView("identity")) {
            types |= Utils::Identity;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1StringView("config")) {
            types |= Utils::Config;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1StringView("data")) {
            types |= Utils::Data;
            numberOfStep++;
        }
        mStreamReader->skipCurrentElement();
    }
    return types;
}

QMap<Utils::AppsType, Utils::importExportParameters> TemplateSelection::loadTemplate(const QString &path)
{
    if (path.isEmpty()) {
        return {};
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "Unable to load file:" << path;
        return {};
    } else {
        mStreamReader = new QXmlStreamReader(&file);
    }

    QMap<Utils::AppsType, Utils::importExportParameters> value;
    if (mStreamReader->readNextStartElement()) {
        // Compatibility with old version
        if ((mStreamReader->name() == QLatin1StringView("pimsettingexporter")) || (mStreamReader->name() == QLatin1StringView("pimdataexporter"))) {
            while (mStreamReader->readNextStartElement()) {
                Utils::AppsType type = Utils::Unknown;
                if (mStreamReader->name() == QLatin1StringView("kmail")) {
                    type = Utils::KMail;
                } else if (mStreamReader->name() == QLatin1StringView("kaddressbook")) {
                    type = Utils::KAddressBook;
                } else if (mStreamReader->name() == QLatin1StringView("kalarm")) {
                    type = Utils::KAlarm;
                } else if (mStreamReader->name() == QLatin1StringView("korganizer")) {
                    type = Utils::KOrganizer;
                } else if (mStreamReader->name() == QLatin1StringView("akregator")) {
                    type = Utils::Akregator;
                }
                if (type != Utils::Unknown) {
                    int numberOfSteps = 0;
                    Utils::StoredTypes storedType = loadStoredTypes(numberOfSteps);
                    if (storedType != Utils::None) {
                        Utils::importExportParameters utils;
                        utils.types = storedType;
                        utils.numberSteps = numberOfSteps;
                        value.insert(type, utils);
                    }
                } else {
                    mStreamReader->skipCurrentElement();
                }
            }
        } else {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << "Toplevel xml is not correct";
        }
    } else {
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "Impossible to parse file";
    }

    return value;
}

void TemplateSelection::saveParameters(Utils::StoredTypes type)
{
    if (type & Utils::MailTransport) {
        mStreamWriter->writeEmptyElement(u"mailtransport"_s);
    }
    if (type & Utils::Mails) {
        mStreamWriter->writeEmptyElement(u"mail"_s);
    }
    if (type & Utils::Resources) {
        mStreamWriter->writeEmptyElement(u"resources"_s);
    }
    if (type & Utils::Identity) {
        mStreamWriter->writeEmptyElement(u"identity"_s);
    }
    if (type & Utils::Config) {
        mStreamWriter->writeEmptyElement(u"config"_s);
    }
    if (type & Utils::Data) {
        mStreamWriter->writeEmptyElement(u"data"_s);
    }
}

QString TemplateSelection::saveTemplate() const
{
    return mSaveTemplate;
}

void TemplateSelection::createTemplate(const QMap<Utils::AppsType, Utils::importExportParameters> &stored)
{
    mSaveTemplate.clear();
    delete mStreamWriter;
    mStreamWriter = new QXmlStreamWriter(&mSaveTemplate);
    mStreamWriter->setAutoFormatting(true);
    mStreamWriter->setAutoFormattingIndent(2);
    mStreamWriter->writeStartDocument();

    mStreamWriter->writeStartElement(u"pimdataexporter"_s);

    QMap<Utils::AppsType, Utils::importExportParameters>::const_iterator i = stored.constBegin();
    while (i != stored.constEnd()) {
        switch (i.key()) {
        case Utils::KMail:
            mStreamWriter->writeStartElement(u"kmail"_s);
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::KAddressBook:
            mStreamWriter->writeStartElement(u"kaddressbook"_s);
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::KAlarm:
            mStreamWriter->writeStartElement(u"kalarm"_s);
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::KOrganizer:
            mStreamWriter->writeStartElement(u"korganizer"_s);
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::Akregator:
            mStreamWriter->writeStartElement(u"akregator"_s);
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::Unknown:
            qCCritical(PIMDATAEXPORTERCORE_LOG) << "Code must not use this enum here";
            break;
        }
        ++i;
    }
    mStreamWriter->writeEndElement();
}
