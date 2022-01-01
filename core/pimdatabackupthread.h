/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QThread>
class KZip;
class PimDataBackupThread : public QThread
{
    Q_OBJECT
public:
    explicit PimDataBackupThread(KZip *zip, const QString &url, const QString &archivePath, const QString &archivename, QObject *parent = nullptr);
    ~PimDataBackupThread() override;

Q_SIGNALS:
    void error(const QString &str);
    void info(const QString &str);
    void terminated(bool success);

protected:
    void run() override;

private:
    const QString mUrl;
    const QString mArchivePath;
    const QString mArchiveName;
    KZip *const mZip;
};
