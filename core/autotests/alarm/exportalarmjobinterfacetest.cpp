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

#include "exportalarmjobinterfacetest.h"
#include "archivestorage.h"
#include "compareexportfile.h"
#include "resourceconvertertest.h"
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

QTEST_MAIN(ExportAlarmJobInterfaceTest)

ExportAlarmJobInterfaceTestImpl::ExportAlarmJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAlarmJobInterfaceTestImpl::~ExportAlarmJobInterfaceTestImpl()
{
}

void ExportAlarmJobInterfaceTestImpl::exportArchiveResource()
{
    qDebug() << " not implement yet";
}

ExportAlarmJobInterfaceTest::ExportAlarmJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAlarmJobInterfaceTest::exportAlarmConfigTest1()
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
    Utils::storeDataExportInfo( archiveStorage->archive());

    ExportAlarmJobInterfaceTestImpl *exportNote = new ExportAlarmJobInterfaceTestImpl(this, {Utils::StoredType::Config}, archiveStorage, 1);
    QSignalSpy finish(exportNote, &ExportAlarmJobInterfaceTestImpl::jobFinished);
    QSignalSpy error(exportNote, &ExportAlarmJobInterfaceTestImpl::error);
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


Akonadi::Collection::Id ExportAlarmJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}
