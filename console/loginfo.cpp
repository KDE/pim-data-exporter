/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "loginfo.h"
using namespace Qt::Literals::StringLiterals;

#include <QDebug>

LogInfo::LogInfo(QObject *parent)
    : QObject(parent)
{
}

LogInfo::~LogInfo() = default;

void LogInfo::addInfoLogEntry(const QString &log)
{
    addLogLine(log, AddInfo);
}

void LogInfo::addErrorLogEntry(const QString &log)
{
    addLogLine(log, AddError);
}

void LogInfo::addTitleLogEntry(const QString &log)
{
    addLogLine(log, AddTitle);
}

void LogInfo::addEndLineLogEntry()
{
    addLogLine(QString(), AddEndLine);
}

void LogInfo::addLogLine(const QString &message, LogType type)
{
    QString newMessage;
    switch (type) {
    case AddEndLine:
        newMessage = u'\n';
        break;
    case AddInfo:
        newMessage = u"INFO: %1"_s.arg(message);
        break;
    case AddError:
        newMessage = u"ERROR: %1"_s.arg(message);
        break;
    case AddTitle:
        newMessage = message;
        break;
    }
    // Laurent: Don't use qCDebug here.
    qDebug() << newMessage;
}

#include "moc_loginfo.cpp"
