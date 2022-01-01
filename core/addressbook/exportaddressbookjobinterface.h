/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportAddressbookJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ExportAddressbookJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportAddressbookJobInterface() override;

    void start() override;
    void slotWriteNextArchiveResource();
    void slotAddressbookJobTerminated();

protected:
    virtual void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey) = 0;
    virtual void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) = 0;
    void slotCheckBackupConfig();
    virtual void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) = 0;
    virtual Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() = 0;
    virtual Q_REQUIRED_RESULT QString resourcePath(const QString &agentIdentifier, const QString &defaultPath) const = 0;
    virtual void backupAddressBookResourceFile(const QString &agentIdentifier, const QString &defaultPath) = 0;

private:
    void slotCheckBackupResource();

    void backupConfig();
    int mIndexIdentifier = 0;
};
