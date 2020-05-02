/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "exportcalendarsjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testexportfile.h"
#include <QDebug>
#include <QTest>

QTEST_MAIN(ExportCalendarsJobInterfaceTest)

ExportCalendarsJobInterfaceTestImpl::ExportCalendarsJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportCalendarsJobInterfaceTestImpl::~ExportCalendarsJobInterfaceTestImpl()
{
}

void ExportCalendarsJobInterfaceTestImpl::exportArchiveResource()
{
    qDebug() << " not implement yet";
}

Akonadi::Collection::Id ExportCalendarsJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

ExportCalendarsJobInterfaceTest::ExportCalendarsJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportCalendarsJobInterfaceTest::exportCalendarConfigTest1()
{
    TestExportFile *file = new TestExportFile(this);
    const QByteArray pathConfig("/export/test1/");
    file->setPathConfig(QByteArray(PIMDATAEXPORTER_DIR) + pathConfig);
    ExportCalendarsJobInterfaceTestImpl *impl = new ExportCalendarsJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
}
