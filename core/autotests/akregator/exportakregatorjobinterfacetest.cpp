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

#include "exportakregatorjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "compareexportfile.h"
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

QTEST_MAIN(ExportAkregatorJobInterfaceTest)

ExportAkregatorJobInterfaceTestImpl::ExportAkregatorJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAkregatorJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{

}

ExportAkregatorJobInterfaceTestImpl::~ExportAkregatorJobInterfaceTestImpl()
{
}

ExportAkregatorJobInterfaceTest::ExportAkregatorJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAkregatorJobInterfaceTest::exportAkregatorConfigTest1()
{
    //Don't use setTestModeEnabled otherwise we can set env
    //QStandardPaths::setTestModeEnabled(true);
    qputenv("XDG_DATA_HOME", PIMDATAEXPORTER_DIR "/test1/share");
    qputenv("XDG_CONFIG_HOME", PIMDATAEXPORTER_DIR "/test1/config");

    //TODO fix file name.
    const QString temporaryFile = QStringLiteral("/tmp/foo.zip");
    const QString storeArchivePath(temporaryFile);
    ArchiveStorage *archiveStorage = new ArchiveStorage(storeArchivePath, this);
    Q_ASSERT(archiveStorage->openArchive(true));
    Utils::addVersion(archiveStorage->archive());
    Utils::storeDataExportInfo(storeArchivePath, archiveStorage->archive());

    ExportAkregatorJobInterfaceTestImpl *exportNote = new ExportAkregatorJobInterfaceTestImpl(this, {Utils::StoredType::Config}, archiveStorage, 1);
    QSignalSpy finish(exportNote, &ExportAkregatorJobInterfaceTestImpl::jobFinished);
    QSignalSpy error(exportNote, &ExportAkregatorJobInterfaceTestImpl::error);
    exportNote->start();
    QVERIFY(finish.wait());
    QCOMPARE(error.count(), 0);
    delete exportNote;
    delete archiveStorage;

    CompareExportFile compareExportFile;
    compareExportFile.setTempFilePath(temporaryFile);
    compareExportFile.setListFilePath(QStringLiteral(PIMDATAEXPORTER_DIR "/test1/list.txt"));
    compareExportFile.compareFiles();
}




