/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "templateselection.h"
#include "pimdataexportcore_debug.h"
#include <QFile>
#include <QXmlStreamWriter>

TemplateSelection::TemplateSelection()
{
}

TemplateSelection::~TemplateSelection()
{
    delete mStreamWriter;
    delete mStreamReader;
}

Utils::StoredTypes TemplateSelection::loadStoredTypes(int &numberOfStep)
{
    Utils::StoredTypes types = Utils::None;
    while (mStreamReader->readNextStartElement()) {
        if (mStreamReader->name() == QLatin1String("mailtransport")) {
            types |= Utils::MailTransport;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1String("mail")) {
            types |= Utils::Mails;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1String("resources")) {
            types |= Utils::Resources;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1String("identity")) {
            types |= Utils::Identity;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1String("config")) {
            types |= Utils::Config;
            numberOfStep++;
        } else if (mStreamReader->name() == QLatin1String("data")) {
            types |= Utils::Data;
            numberOfStep++;
        }
        mStreamReader->skipCurrentElement();
    }
    return types;
}

QHash<Utils::AppsType, Utils::importExportParameters> TemplateSelection::loadTemplate(const QString &path)
{
    if (path.isEmpty()) {
        return {};
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qCDebug(PIMDATAEXPORTERCORE_LOG) << "Unable to load file:" << path;
        return {};
    } else {
        mStreamReader = new QXmlStreamReader(&file);
    }

    QHash<Utils::AppsType, Utils::importExportParameters> value;
    if (mStreamReader->readNextStartElement()) {
        //Compatibility with old version
        if ((mStreamReader->name() == QLatin1String("pimsettingexporter")) || (mStreamReader->name() == QLatin1String("pimdataexporter"))) {
            while (mStreamReader->readNextStartElement()) {
                Utils::AppsType type = Utils::Unknown;
                if (mStreamReader->name() == QLatin1String("kmail")) {
                    type = Utils::KMail;
                } else if (mStreamReader->name() == QLatin1String("kaddressbook")) {
                    type = Utils::KAddressBook;
                } else if (mStreamReader->name() == QLatin1String("kalarm")) {
                    type = Utils::KAlarm;
                } else if (mStreamReader->name() == QLatin1String("korganizer")) {
                    type = Utils::KOrganizer;
                } else if (mStreamReader->name() == QLatin1String("knotes")) {
                    type = Utils::KNotes;
                } else if (mStreamReader->name() == QLatin1String("akregator")) {
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
        mStreamWriter->writeEmptyElement(QStringLiteral("mailtransport"));
    }
    if (type & Utils::Mails) {
        mStreamWriter->writeEmptyElement(QStringLiteral("mail"));
    }
    if (type & Utils::Resources) {
        mStreamWriter->writeEmptyElement(QStringLiteral("resources"));
    }
    if (type & Utils::Identity) {
        mStreamWriter->writeEmptyElement(QStringLiteral("identity"));
    }
    if (type & Utils::Config) {
        mStreamWriter->writeEmptyElement(QStringLiteral("config"));
    }
    if (type & Utils::Data) {
        mStreamWriter->writeEmptyElement(QStringLiteral("data"));
    }
}

QString TemplateSelection::saveTemplate() const
{
    return mSaveTemplate;
}

void TemplateSelection::createTemplate(const QHash<Utils::AppsType, Utils::importExportParameters> &stored)
{
    mSaveTemplate.clear();
    delete mStreamWriter;
    mStreamWriter = new QXmlStreamWriter(&mSaveTemplate);
    mStreamWriter->setAutoFormatting(true);
    mStreamWriter->setAutoFormattingIndent(2);
    mStreamWriter->writeStartDocument();

    mStreamWriter->writeStartElement(QStringLiteral("pimdataexporter"));

    QHash<Utils::AppsType, Utils::importExportParameters>::const_iterator i = stored.constBegin();
    while (i != stored.constEnd()) {
        switch (i.key()) {
        case Utils::KMail:
            mStreamWriter->writeStartElement(QStringLiteral("kmail"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::KAddressBook:
            mStreamWriter->writeStartElement(QStringLiteral("kaddressbook"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::KAlarm:
            mStreamWriter->writeStartElement(QStringLiteral("kalarm"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::KOrganizer:
            mStreamWriter->writeStartElement(QStringLiteral("korganizer"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::KNotes:
            mStreamWriter->writeStartElement(QStringLiteral("knotes"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        case Utils::Akregator:
            mStreamWriter->writeStartElement(QStringLiteral("akregator"));
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
