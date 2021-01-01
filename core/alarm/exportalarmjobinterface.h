/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    void slotCheckBackupConfig();
    virtual void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) = 0;
    virtual Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() = 0;
    virtual Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const = 0;
    virtual void backupAlarmResourceFile(const QString &agentIdentifier, const QString &defaultPath) = 0;
    virtual void convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr) = 0;
private:
    void slotCheckBackupResource();

    void backupConfig();
private:

    int mIndexIdentifier = 0;
};

#endif // EXPORTALARMJOBINTERFACE_H
