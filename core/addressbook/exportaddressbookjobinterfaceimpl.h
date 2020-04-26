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

#ifndef EXPORTADDRESSBOOKINTERFACEIMPL_H
#define EXPORTADDRESSBOOKINTERFACEIMPL_H

#include "exportaddressbookjobinterface.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportAddressbookJobInterfaceImpl : public ExportAddressbookJobInterface
{
    Q_OBJECT
public:
    explicit ExportAddressbookJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportAddressbookJobInterfaceImpl() override;

protected:
    void exportArchiveResource() override;
    void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey) override;
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) override;
private:
    void slotWriteNextArchiveResource();
    void slotAddressbookJobTerminated();

    void backupConfig();
    int mIndexIdentifier = 0;
};

#endif // EXPORTADDRESSBOOKINTERFACEIMPL_H
