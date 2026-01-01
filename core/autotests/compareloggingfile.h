/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>

class CompareLoggingFile
{
public:
    CompareLoggingFile();
    ~CompareLoggingFile() = default;

    void compare();

    [[nodiscard]] QString loggingFilePath() const;
    void setLoggingFilePath(const QString &loggingFilePath);

    [[nodiscard]] QString listFilePath() const;
    void setListFilePath(const QString &listFilePath);

private:
    QString mLoggingFilePath;
    QString mListFilePath;
};
