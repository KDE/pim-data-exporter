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
#include "testexportfile.h"
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

QVector<Utils::AkonadiInstanceInfo> ExportAlarmJobInterfaceTestImpl::listOfResource()
{
    qDebug() << " QVector<Utils::AkonadiInstanceInfo> ExportAlarmJobInterfaceTestImpl::listOfResource()";
    return mListAkonadiInstanceInfo;
}

void ExportAlarmJobInterfaceTestImpl::setListOfResource(const QVector<Utils::AkonadiInstanceInfo> &instanceInfoList)
{
    mListAkonadiInstanceInfo = instanceInfoList;
}

Akonadi::Collection::Id ExportAlarmJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ExportAlarmJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    //TODO implement me
}

QString ExportAlarmJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    const QString url = converter.resourcePath(identifier);
    return url;
}

ExportAlarmJobInterfaceTest::ExportAlarmJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAlarmJobInterfaceTest::exportAlarmConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("test1") << pathConfig + QByteArray("test1/");
}

void ExportAlarmJobInterfaceTest::exportAlarmConfig()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportAlarmJobInterfaceTestImpl *exportNote = new ExportAlarmJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
