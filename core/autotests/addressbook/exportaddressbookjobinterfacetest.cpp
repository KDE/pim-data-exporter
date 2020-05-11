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
#include <saveresourceconfigtest.h>

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
    slotWriteNextArchiveResource();
}

Akonadi::Collection::Id ExportAddressbookJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ExportAddressbookJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResoureConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath, url, identifier, Utils::resourceAddressbookArchiveName(),
    { QLatin1String("akonadi_vcarddir_resource_"), QLatin1String("akonadi_contacts_resource_") });
    slotAddressbookJobTerminated();
}

QVector<Utils::AkonadiInstanceInfo> ExportAddressbookJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionToRealPath(group, currentKey);
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionListToRealPath(group, currentKey);
}

QString ExportAddressbookJobInterfaceTestImpl::resourcePath(const QString &agentIdentifier, const QString &defaultPath) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(agentIdentifier, defaultPath);
    return url;
}

ExportAddressbookJobInterfaceTest::ExportAddressbookJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAddressbookJobInterfaceTest::exportAddressBookConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    qDebug() << " pathConfig " << pathConfig;
    QTest::newRow("full") << pathConfig + QByteArray("full/");
}

void ExportAddressbookJobInterfaceTest::exportAddressBookConfig()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportAddressbookJobInterfaceTestImpl *exportNote = new ExportAddressbookJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
