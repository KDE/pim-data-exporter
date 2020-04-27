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

#ifndef IMPORTADDRESSBOOKJOBINTERFACE_H
#define IMPORTADDRESSBOOKJOBINTERFACE_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;
class KArchiveFile;

class PIMDATAEXPORTER_TESTS_EXPORT ImportAddressbookJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportAddressbookJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAddressbookJobInterface() override;

    void start() override;

protected:
    void slotNextStep() override;

private:
    Q_REQUIRED_RESULT bool isAConfigFile(const QString &name) const override;
    void importkaddressBookConfig(const KArchiveFile *file, const QString &config, const QString &filename, const QString &prefix);
    void restoreResources();
    void restoreConfig();
    void addSpecificResourceSettings(const KSharedConfig::Ptr &resourceConfig, const QString &resourceName, QMap<QString, QVariant> &settings) override;
};

#endif // IMPORTADDRESSBOOKJOBINTERFACE_H
