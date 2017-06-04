/*
   Copyright (C) 2012-2017 Montel Laurent <montel@kde.org>

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

#ifndef IMPORTALARMJOB_H
#define IMPORTALARMJOB_H

#include "abstractimportexportjob.h"

class ArchiveStorage;

class ImportAlarmJob : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportAlarmJob(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAlarmJob();

    void start() override;

protected Q_SLOTS:
    void slotNextStep() override;

private:
    bool isAConfigFile(const QString &name) const  override;
    void importkalarmConfig(const KArchiveFile *kalarmFile, const QString &kalarmrc, const QString &filename, const QString &prefix);
    void restoreResources();
    void restoreConfig();
};

#endif // IMPORTALARMJOB_H
