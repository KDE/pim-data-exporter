/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMPAREIMPORTFILE_H
#define COMPAREIMPORTFILE_H

#include <QString>

class CompareImportFile
{
public:
    CompareImportFile();
    ~CompareImportFile();

    void compareFile();

    Q_REQUIRED_RESULT QString archiveFilePath() const;
    void setArchiveFilePath(const QString &archiveFilePath);

    Q_REQUIRED_RESULT QString listFilePath() const;
    void setListFilePath(const QString &listFilePath);

    Q_REQUIRED_RESULT QString installPath() const;
    void setInstallPath(const QString &installPath);

    Q_REQUIRED_RESULT QString excludePath() const;
    void setExcludePath(const QString &excludePath);

private:
    QString mArchiveFilePath;
    QString mListFilePath;
    QString mInstallPath;
    QString mExcludePath;
};

#endif // COMPAREIMPORTFILE_H
