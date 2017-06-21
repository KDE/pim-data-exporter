/*
   Copyright (C) 2014-2017 Montel Laurent <montel@kde.org>

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

#include "templateselection.h"
#include "pimsettingexportcore_debug.h"
#include <QFile>
#include <QXmlStreamWriter>

TemplateSelection::TemplateSelection(const QString &path)
    : mStreamWriter(nullptr)
{
    if (!path.isEmpty()) {
        QDomDocument doc;
        QString errorMsg;
        int errorRow;
        int errorCol;
        QFile file(path);
        if (file.open(QIODevice::ReadOnly)) {
            if (!doc.setContent(&file, &errorMsg, &errorRow, &errorCol)) {
                qCDebug(PIMSETTINGEXPORTERCORE_LOG) << "Unable to load document.Parse error in line " << errorRow
                                                    << ", col " << errorCol << ": " << errorMsg;
            } else {
                mDocument = doc;
            }
        } else {
            qCDebug(PIMSETTINGEXPORTERCORE_LOG) << "Unable to load file:" << path;
        }
    }
}

TemplateSelection::~TemplateSelection()
{
    delete mStreamWriter;
}

Utils::StoredTypes TemplateSelection::loadStoredTypes(const QDomElement &element, int &numberOfStep)
{
    Utils::StoredTypes types = Utils::None;
    QDomNode n = element.firstChild();
    while (!n.isNull())  {
        QDomElement e = n.toElement();
        if (!e.isNull())  {
            const QString tagName(e.tagName());
            if (tagName == QLatin1String("mailtransport")) {
                types |= Utils::MailTransport;
                numberOfStep++;
            } else if (tagName == QLatin1String("mail")) {
                types |= Utils::Mails;
                numberOfStep++;
            } else if (tagName == QLatin1String("resources")) {
                types |= Utils::Resources;
                numberOfStep++;
            } else if (tagName == QLatin1String("identity")) {
                types |= Utils::Identity;
                numberOfStep++;
            } else if (tagName == QLatin1String("config")) {
                types |= Utils::Config;
                numberOfStep++;
            } else if (tagName == QLatin1String("data")) {
                types |= Utils::Data;
                numberOfStep++;
            }
        }
        n = n.nextSibling();
    }
    return types;
}

QHash<Utils::AppsType, Utils::importExportParameters> TemplateSelection::loadTemplate(const QDomDocument &doc)
{
    QHash<Utils::AppsType, Utils::importExportParameters> value;
    if (!doc.isNull()) {
        mDocument = doc;
    }
    QDomElement docElem = mDocument.documentElement();
    QDomNode n = docElem.firstChild();
    while (!n.isNull())  {
        QDomElement e = n.toElement();
        if (!e.isNull())  {
            const QString tagName(e.tagName());
            //qCDebug(PIMSETTINGEXPORTERCORE_LOG) << "tag :" << tagName;
            Utils::AppsType type = Utils::Unknown;
            if (tagName == QLatin1String("kmail")) {
                type = Utils::KMail;
            } else if (tagName == QLatin1String("kaddressbook")) {
                type = Utils::KAddressBook;
            } else if (tagName == QLatin1String("kalarm")) {
                type = Utils::KAlarm;
            } else if (tagName == QLatin1String("korganizer")) {
                type = Utils::KOrganizer;
            } else if (tagName == QLatin1String("knotes")) {
                type = Utils::KNotes;
            } else if (tagName == QLatin1String("akregator")) {
                type = Utils::Akregator;
            } else if (tagName == QLatin1String("blogilo")) {
                type = Utils::Blogilo;
            }
            if (type != Utils::Unknown) {
                int numberOfSteps = 0;
                Utils::StoredTypes storedType = loadStoredTypes(e, numberOfSteps);
                if (storedType != Utils::None) {
                    Utils::importExportParameters utils;
                    utils.types = storedType;
                    utils.numberSteps = numberOfSteps;
                    value.insert(type, utils);
                }
            }
        }
        n = n.nextSibling();
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

    mStreamWriter->writeStartElement(QStringLiteral("pimsettingexporter"));

    QHash<Utils::AppsType, Utils::importExportParameters>::const_iterator i = stored.constBegin();
    while (i != stored.constEnd())  {
        switch (i.key()) {
        case Utils::KMail: {
            mStreamWriter->writeStartElement(QStringLiteral("kmail"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        }
        case Utils::KAddressBook: {
            mStreamWriter->writeStartElement(QStringLiteral("kaddressbook"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        }
        case Utils::KAlarm: {
            mStreamWriter->writeStartElement(QStringLiteral("kalarm"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        }
        case Utils::KOrganizer: {
            mStreamWriter->writeStartElement(QStringLiteral("korganizer"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        }
        case Utils::KNotes: {
            mStreamWriter->writeStartElement(QStringLiteral("knotes"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        }
        case Utils::Akregator: {
            mStreamWriter->writeStartElement(QStringLiteral("akregator"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        }
        case Utils::Blogilo: {
            mStreamWriter->writeStartElement(QStringLiteral("blogilo"));
            saveParameters(i.value().types);
            mStreamWriter->writeEndElement();
            break;
        }
        case Utils::Unknown: {
            qCCritical(PIMSETTINGEXPORTERCORE_LOG) << "Code must not use this enum here";
            break;
        }
        }
        ++i;
    }
    mStreamWriter->writeEndElement();
}
