/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATABACKUPTHREAD_H
#define PIMDATABACKUPTHREAD_H

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
    QString mUrl;
    QString mArchivePath;
    QString mArchiveName;
    KZip *mZip = nullptr;
};

#endif // PIMDATABACKUPTHREAD_H
