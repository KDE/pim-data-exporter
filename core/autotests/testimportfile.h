/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class ArchiveStorage;
class AbstractImportExportJob;
class TestImportFile : public QObject
{
    Q_OBJECT
public:
    explicit TestImportFile(const QString &archivePath, QObject *parent = nullptr);
    ~TestImportFile() override;

    void start();
    [[nodiscard]] AbstractImportExportJob *abstractImportExportJob() const;
    void setAbstractImportExportJob(AbstractImportExportJob *abstractImportExportJob);

    [[nodiscard]] ArchiveStorage *archiveStorage() const;
    void setArchiveStorage(ArchiveStorage *archiveStorage);

    [[nodiscard]] QString pathConfig() const;
    void setPathConfig(const QString &pathConfig);

    void setExtractPath(const QString &extractPath);

    [[nodiscard]] QString excludePath() const;
    void setExcludePath(const QString &excludePath);

    [[nodiscard]] QString loggingFilePath() const;
    void setLoggingFilePath(const QString &loggingFilePath);

    [[nodiscard]] QString extractPath() const;

private:
    AbstractImportExportJob *mAbstractImportExportJob = nullptr;
    ArchiveStorage *mArchiveStorage = nullptr;
    const QString mArchivePath;
    QString mPathConfig;
    QString mExtractPath;
    QString mExcludePath;
    QString mLoggingFilePath;
};
