/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMPARELOGGINGFILE_H
#define COMPARELOGGINGFILE_H

#include <QString>

class CompareLoggingFile
{
public:
    CompareLoggingFile();
    ~CompareLoggingFile() = default;

    void compare();

    Q_REQUIRED_RESULT QString loggingFilePath() const;
    void setLoggingFilePath(const QString &loggingFilePath);

    Q_REQUIRED_RESULT QString listFilePath() const;
    void setListFilePath(const QString &listFilePath);

private:
    QString mLoggingFilePath;
    QString mListFilePath;
};

#endif // COMPARELOGGINGFILE_H
