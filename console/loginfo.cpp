/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "loginfo.h"
#include <QDebug>

LogInfo::LogInfo(QObject *parent)
    : QObject(parent)
{
}

LogInfo::~LogInfo()
{
}

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
        newMessage = QLatin1Char('\n');
        break;
    case AddInfo:
        newMessage = QStringLiteral("INFO: %1").arg(message);
        break;
    case AddError:
        newMessage = QStringLiteral("ERROR: %1").arg(message);
        break;
    case AddTitle:
        newMessage = message;
        break;
    }
    // Laurent: Don't use qCDebug here.
    qDebug() << newMessage;
}
