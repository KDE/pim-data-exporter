/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LOGINFO_H
#define LOGINFO_H

#include <QObject>

class LogInfo : public QObject
{
    Q_OBJECT
public:
    explicit LogInfo(QObject *parent = nullptr);
    ~LogInfo();

    void addInfoLogEntry(const QString &log);
    void addErrorLogEntry(const QString &log);
    void addTitleLogEntry(const QString &log);
    void addEndLineLogEntry();
private:
    enum LogType {
        AddEndLine = 0,
        AddInfo,
        AddError,
        AddTitle
    };
    void addLogLine(const QString &message, LogType type);
};

#endif // LOGINFO_H
