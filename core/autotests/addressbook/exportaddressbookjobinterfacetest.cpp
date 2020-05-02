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

#include "exportaddressbookjobinterfacetest.h"
#include "archivestorage.h"
#include "testexportfile.h"
#include "resourceconvertertest.h"
#include <QDebug>
#include <QTest>

QTEST_MAIN(ExportAddressbookJobInterfaceTest)

ExportAddressbookJobInterfaceTestImpl::ExportAddressbookJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAddressbookJobInterfaceTestImpl::~ExportAddressbookJobInterfaceTestImpl()
{
}

void ExportAddressbookJobInterfaceTestImpl::exportArchiveResource()
{
    qDebug() << " not implement yet";
}

Akonadi::Collection::Id ExportAddressbookJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.convertCollectionToRealPath(group, currentKey);
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.convertCollectionListToRealPath(group, currentKey);
}

ExportAddressbookJobInterfaceTest::ExportAddressbookJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAddressbookJobInterfaceTest::exportAddressBookConfigTest1()
{
    TestExportFile *file = new TestExportFile(this);
    const QByteArray pathConfig("/export/test1/");
    file->setPathConfig(QByteArray(PIMDATAEXPORTER_DIR) + pathConfig);
    ExportAddressbookJobInterfaceTestImpl *impl = new ExportAddressbookJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
}


