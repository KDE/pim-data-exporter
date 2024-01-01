/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>
class KZip;
class PimDataBackupThread;
class ExportResourceArchiveJob : public QObject
{
    Q_OBJECT
public:
    explicit ExportResourceArchiveJob(QObject *parent = nullptr);
    ~ExportResourceArchiveJob() override;

    void setArchivePath(const QString &archivePath);

    void setUrl(const QString &url);

    void setIdentifier(const QString &identifier);

    void setArchive(KZip *zip);
    void start();
    void setArchiveName(const QString &archiveName);

Q_SIGNALS:
    void error(const QString &str);
    void info(const QString &str);
    void terminated();

public Q_SLOTS:
    void slotTaskCanceled();

private:
    void slotTerminated(bool success);
    void finished();
    QString mArchiveName;
    QString mUrl;
    QString mArchivePath;
    QString mIdentifier;
    KZip *mZip = nullptr;
    PimDataBackupThread *mThread = nullptr;
};
