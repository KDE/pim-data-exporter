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


#ifndef BACKUPRESOURCEFILEJOB_H
#define BACKUPRESOURCEFILEJOB_H

#include <QObject>
#include <KZip>

#include "pimdataexporter_export.h"
class PIMDATAEXPORTER_EXPORT BackupResourceFileJob : public QObject
{
    Q_OBJECT
public:
    explicit BackupResourceFileJob(QObject *parent = nullptr);
    ~BackupResourceFileJob();

    void start();

    Q_REQUIRED_RESULT bool canStart() const;


    Q_REQUIRED_RESULT QString identifier() const;
    void setIdentifier(const QString &identifier);

    Q_REQUIRED_RESULT KZip *zip() const;
    void setZip(KZip *zip);

    Q_REQUIRED_RESULT QString defaultPath() const;
    void setDefaultPath(const QString &defaultPath);

Q_SIGNALS:
    void error(const QString &);
    void info(const QString &);

private:
    QString mDefaultPath;
    QString mIdentifier;
    KZip *mZip = nullptr;
};

#endif // BACKUPRESOURCEFILEJOB_H
