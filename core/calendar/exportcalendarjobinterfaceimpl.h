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

#ifndef EXPORTCALENDARJOBINTERFACEIMPL_H
#define EXPORTCALENDARJOBINTERFACEIMPL_H

#include "exportcalendarjobinterface.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportCalendarJobInterfaceImpl : public ExportCalendarJobInterface
{
    Q_OBJECT
public:
    explicit ExportCalendarJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportCalendarJobInterfaceImpl() override;

protected:
    void exportArchiveResource() override;

private:
    void slotCalendarJobTerminated();
    void slotWriteNextArchiveResource();
    int mIndexIdentifier = 0;
};

#endif // EXPORTCALENDARJOBINTERFACEIMPL_H