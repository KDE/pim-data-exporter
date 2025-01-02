/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>

class CompareImportFile
{
public:
    CompareImportFile();
    ~CompareImportFile();

    void compareFile();

    [[nodiscard]] QString archiveFilePath() const;
    void setArchiveFilePath(const QString &archiveFilePath);

    [[nodiscard]] QString listFilePath() const;
    void setListFilePath(const QString &listFilePath);

    [[nodiscard]] QString installPath() const;
    void setInstallPath(const QString &installPath);

    [[nodiscard]] QString excludePath() const;
    void setExcludePath(const QString &excludePath);

private:
    QString mArchiveFilePath;
    QString mListFilePath;
    QString mInstallPath;
    QString mExcludePath;
};
