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

#ifndef EXPORTCALENDARJOBINTERFACE_H
#define EXPORTCALENDARJOBINTERFACE_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportCalendarJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ExportCalendarJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportCalendarJobInterface() override;

    void start() override;

protected:
    virtual void exportArchiveResource() = 0;
    void slotCheckBackupConfig();
    virtual Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) = 0;
    virtual void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) = 0;
private:
    void slotCheckBackupResource();

    void backupConfig();
    int mIndexIdentifier = 0;
};

#endif // EXPORTCALENDARJOBINTERFACE_H
