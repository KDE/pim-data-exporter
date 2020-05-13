/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#ifndef TESTIMPORTFILE_H
#define TESTIMPORTFILE_H

#include <QObject>
class ArchiveStorage;
class AbstractImportExportJob;
class TestImportFile : public QObject
{
    Q_OBJECT
public:
    explicit TestImportFile(const QString &archivePath, QObject *parent = nullptr);
    ~TestImportFile();

    void start();
    Q_REQUIRED_RESULT AbstractImportExportJob *abstractImportExportJob() const;
    void setAbstractImportExportJob(AbstractImportExportJob *abstractImportExportJob);

    Q_REQUIRED_RESULT ArchiveStorage *archiveStorage() const;
    void setArchiveStorage(ArchiveStorage *archiveStorage);

    Q_REQUIRED_RESULT QString pathConfig() const;
    void setPathConfig(const QString &pathConfig);

    void setExtractPath(const QString &extractPath);

    Q_REQUIRED_RESULT QString excludePath() const;
    void setExcludePath(const QString &excludePath);

    Q_REQUIRED_RESULT QString loggingFilePath() const;
    void setLoggingFilePath(const QString &loggingFilePath);

private:
    AbstractImportExportJob *mAbstractImportExportJob = nullptr;
    ArchiveStorage *mArchiveStorage = nullptr;
    QString mArchivePath;
    QString mPathConfig;
    QString mExtractPath;
    QString mExcludePath;
    QString mLoggingFilePath;
};

#endif // TESTIMPORTFILE_H
