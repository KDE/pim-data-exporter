/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class AbstractImportExportJob;
class ArchiveStorage;
class TestExportFile : public QObject
{
    Q_OBJECT
public:
    explicit TestExportFile(QObject *parent = nullptr);
    ~TestExportFile() override;

    Q_REQUIRED_RESULT QByteArray pathConfig() const;
    void setPathConfig(const QByteArray &pathConfig);

    AbstractImportExportJob *abstractImportExportJob() const;
    void setAbstractImportExportJob(AbstractImportExportJob *abstractImportExportJob);

    void start();

    Q_REQUIRED_RESULT ArchiveStorage *archiveStorage() const;

private:
    QByteArray mPathConfig;
    AbstractImportExportJob *mAbstractImportExportJob = nullptr;
    ArchiveStorage *mArchiveStorage = nullptr;
    const QString mTemporaryFile;
};
