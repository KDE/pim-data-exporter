/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
#include <KSharedConfig>
#include <time.h>
namespace MailCommon {
class MailFilter;
}
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportMailJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ExportMailJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportMailJobInterface() override;

    void start() override;
    void slotWriteNextArchiveResource();
    void slotMailsJobTerminated();

Q_SIGNALS:
    void taskCanceled();

protected:
    virtual void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection = QString()) = 0;
    virtual void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey) = 0;
    virtual void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) = 0;
    void backupResources();
    virtual QVector<MailCommon::MailFilter *> filters() = 0;
    virtual Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) = 0;
    void slotCheckBackupResources();
    virtual void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) = 0;
    virtual Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() = 0;
    virtual Q_REQUIRED_RESULT QString storeResources(KZip *archive, const QString &identifier, const QString &path) = 0;
    virtual Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const = 0;
    virtual void backupMailResourceFile(const QString &agentIdentifier, const QString &defaultPath) = 0;
    virtual void exportFilters() = 0;
    virtual Q_REQUIRED_RESULT QVector<uint> listIdentityUoid() const = 0;
private:
    void slotCheckBackupIdentity();
    void slotCheckBackupMailTransport();
    void slotCheckBackupConfig();
    void slotCheckBackupMails();

    Q_REQUIRED_RESULT bool checkBackupType(Utils::StoredType type) const;
    void backupTransports();
    void backupConfig();
    void backupIdentity();
private:

    int mIndexIdentifier = 0;
};

