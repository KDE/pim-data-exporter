/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    [[nodiscard]] virtual QList<Utils::AkonadiInstanceInfo> listOfResource() = 0;
    void slotNoteJobTerminated();
    [[nodiscard]] virtual QString resourcePath(const QString &identifier) const = 0;
    [[nodiscard]] QString resourceArchiveName() const;
    [[nodiscard]] QString applicationName() const override;

private:
    void slotCheckBackupResource();

    void backupConfig();
    void backupTheme();
    int mIndexIdentifier = 0;
};
