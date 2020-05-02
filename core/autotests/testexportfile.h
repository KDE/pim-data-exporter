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

#ifndef TESTEXPORTFILE_H
#define TESTEXPORTFILE_H

#include <QObject>
class AbstractImportExportJob;
class ArchiveStorage;
class TestExportFile : public QObject
{
    Q_OBJECT
public:
    explicit TestExportFile(QObject *parent = nullptr);
    ~TestExportFile();

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
};

#endif // TESTEXPORTFILE_H
