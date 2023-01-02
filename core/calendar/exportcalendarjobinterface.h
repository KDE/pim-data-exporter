/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    void slotCalendarJobTerminated();
    void slotWriteNextArchiveResource();

protected:
    void slotCheckBackupConfig();
    Q_REQUIRED_RESULT virtual QString convertToFullCollectionPath(const qlonglong collectionValue) = 0;
    virtual void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) = 0;
    virtual void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) = 0;
    Q_REQUIRED_RESULT virtual QVector<Utils::AkonadiInstanceInfo> listOfResource() = 0;
    Q_REQUIRED_RESULT virtual QString resourcePath(const QString &identifier) const = 0;
    virtual void backupCalendarResourceFile(const QString &agentIdentifier, const QString &defaultPath) = 0;
    Q_REQUIRED_RESULT QString applicationName() const override;

private:
    void slotCheckBackupResource();
    void exportEventViewConfig();
    void exportKorganizerConfig();
    void exportKalendarConfig();
    void exportResourceColors(KConfig *config);

    void backupConfig();
    int mIndexIdentifier = 0;
};
