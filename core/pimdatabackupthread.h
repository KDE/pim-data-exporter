/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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
