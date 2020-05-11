/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#ifndef EXPORTNOTESJOBINTERFACE_H
#define EXPORTNOTESJOBINTERFACE_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportNotesJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ExportNotesJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportNotesJobInterface() override;

    void start() override;

    void slotCheckBackupConfig();
    void slotWriteNextArchiveResource();

protected:
    virtual void convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr) = 0;
    virtual void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) = 0;
    virtual Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() = 0;
    void slotNoteJobTerminated();
    virtual Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const = 0;
    Q_REQUIRED_RESULT QString resourceArchiveName() const;

private:
    void slotCheckBackupResource();

    void backupConfig();
    void backupTheme();
    int mIndexIdentifier = 0;
};

#endif // EXPORTNOTESJOBINTERFACE_H
