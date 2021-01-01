/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

    Q_REQUIRED_RESULT QString extractPath() const;

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
