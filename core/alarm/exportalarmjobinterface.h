/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

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

#ifndef EXPORTALARMJOBINTERFACE_H
#define EXPORTALARMJOBINTERFACE_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportAlarmJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ExportAlarmJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportAlarmJobInterface() override;

    void start() override;
    void slotAlarmJobTerminated();
    void slotWriteNextArchiveResource();
protected:
    virtual void exportArchiveResource() = 0;
    void slotCheckBackupConfig();
    virtual void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) = 0;
    virtual Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() = 0;
private:
    void slotCheckBackupResource();

    void backupConfig();
private:

    int mIndexIdentifier = 0;
};

#endif // EXPORTALARMJOBINTERFACE_H
