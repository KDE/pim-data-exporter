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

#include "exportmailjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "compareexportfile.h"
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

QTEST_MAIN(ExportMailJobInterfaceTest)

ExportMailJobInterfaceTestImpl::ExportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{

}

ExportMailJobInterfaceTestImpl::~ExportMailJobInterfaceTestImpl()
{
}

void ExportMailJobInterfaceTestImpl::exportArchiveResource()
{
    qDebug() << " not implement yet";
}

void ExportMailJobInterfaceTestImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.convertCollectionToRealPath(group, currentKey);
}

void ExportMailJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.convertCollectionListToRealPath(group, currentKey);
}

void ExportMailJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.convertCollectionIdsToRealPath(group, currentKey, prefixCollection);
}

void ExportMailJobInterfaceTestImpl::backupResources()
{
    qDebug() << " not implement yet";
}

ExportMailJobInterfaceTest::ExportMailJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportMailJobInterfaceTest::exportMailConfigTest1()
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

    ExportMailJobInterfaceTestImpl *exportNote = new ExportMailJobInterfaceTestImpl(this, {Utils::StoredType::Config}, archiveStorage, 1);
    QSignalSpy finish(exportNote, &ExportMailJobInterfaceTestImpl::jobFinished);
    QSignalSpy error(exportNote, &ExportMailJobInterfaceTestImpl::error);
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
