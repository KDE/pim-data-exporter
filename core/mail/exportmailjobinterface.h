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

#ifndef ExportMailJobINTERFACE_H
#define ExportMailJobINTERFACE_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
#include <KSharedConfig>
#include <QDateTime>
#include <time.h>
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportMailJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ExportMailJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportMailJobInterface() override;

    void start() override;

Q_SIGNALS:
    void taskCanceled();

protected:
    virtual void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection = QString());
    virtual void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey);
    virtual void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey);
private:
    void slotCheckBackupIdentity();
    void slotCheckBackupMailTransport();
    void slotCheckBackupConfig();
    void slotCheckBackupMails();
    void slotCheckBackupResources();
    void slotMailsJobTerminated();
    void slotWriteNextArchiveResource();

    Q_REQUIRED_RESULT bool checkBackupType(Utils::StoredType type) const;
    void backupTransports();
    void backupResources();
    void backupConfig();
    void backupIdentity();
    QDateTime mArchiveTime = QDateTime::currentDateTime();
    int mIndexIdentifier = 0;
};

#endif // ExportMailJob_H