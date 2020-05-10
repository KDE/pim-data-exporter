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

#ifndef STORERESOURCEJOB_H
#define STORERESOURCEJOB_H

#include <QObject>
class KZip;
class StoreResourceJob : public QObject
{
    Q_OBJECT
public:
    explicit StoreResourceJob(QObject *parent = nullptr);
    ~StoreResourceJob();

    void start();

    void setArchivePath(const QString &archivePath);

    void setIdentifier(const QString &identifier);

    void setZip(KZip *zip);
Q_SIGNALS:
    void error(const QString &);
    void info(const QString &);
private:
    QString mArchivePath;
    QString mIdentifier;
    KZip *mZip = nullptr;
};

#endif // STORERESOURCEJOB_H
