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
#include "compareexportfile.h"
#include "resourceconvertertest.h"
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

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
    //Don't use setTestModeEnabled otherwise we can set env
    //QStandardPaths::setTestModeEnabled(true);
    qputenv("XDG_DATA_HOME", PIMDATAEXPORTER_DIR "/export/test1/share");
    qputenv("XDG_CONFIG_HOME", PIMDATAEXPORTER_DIR "/export/test1/config");

    //TODO fix file name.
    const QString temporaryFile = QStringLiteral("/tmp/foo.zip");
    const QString storeArchivePath(temporaryFile);
    ArchiveStorage *archiveStorage = new ArchiveStorage(storeArchivePath, this);
    Q_ASSERT(archiveStorage->openArchive(true));
    Utils::addVersion(archiveStorage->archive());
    Utils::storeDataExportInfo(storeArchivePath, archiveStorage->archive());

    ExportAddressbookJobInterfaceTestImpl *exportNote = new ExportAddressbookJobInterfaceTestImpl(this, {Utils::StoredType::Config}, archiveStorage, 1);
    QSignalSpy finish(exportNote, &ExportAddressbookJobInterfaceTestImpl::jobFinished);
    QSignalSpy error(exportNote, &ExportAddressbookJobInterfaceTestImpl::error);
    exportNote->start();
    QVERIFY(finish.wait());
    QCOMPARE(error.count(), 0);
    delete exportNote;
    delete archiveStorage;
    CompareExportFile compareExportFile;
    compareExportFile.setTempFilePath(temporaryFile);
    compareExportFile.setListFilePath(QStringLiteral(PIMDATAEXPORTER_DIR "/export/test1"));
    compareExportFile.compareFiles();
}


Akonadi::Collection::Id ExportAddressbookJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    //TODO
    return -1;
}
