/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QTextStream>
class QFile;
class LogInFile : public QObject
{
    Q_OBJECT
public:
    explicit LogInFile(QObject *parent = nullptr);
    ~LogInFile() override;

    Q_REQUIRED_RESULT QString fileName() const;
    void setFileName(const QString &fileName);

public Q_SLOTS:
    void addEndLine();

    void addError(const QString &message);
    void addInfo(const QString &message);
    void addTitle(const QString &message);

private:
    enum LogType { AddEndLine = 0, AddInfo, AddError, AddTitle };
    void addLogLine(const QString &message, LogType type);
    QString mFileName;
    QTextStream mTextStream;
    QFile *mFile = nullptr;
};
