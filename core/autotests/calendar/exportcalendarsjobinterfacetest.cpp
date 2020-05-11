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
#include <saveresourceconfigtest.h>

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
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ExportCalendarsJobInterfaceTestImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertToFullCollectionPath(collectionValue);
}

void ExportCalendarsJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResoureConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath, url, identifier, Utils::resourceCalendarArchiveName(), { QLatin1String("akonadi_icaldir_resource_")});
    slotCalendarJobTerminated();
}

QVector<Utils::AkonadiInstanceInfo> ExportCalendarsJobInterfaceTestImpl::listOfResource()
{
    qDebug() << " QVector<Utils::AkonadiInstanceInfo> ExportNotesJobInterfaceTestImpl::listOfResource()";
    return mListAkonadiInstanceInfo;
}

void ExportCalendarsJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    converter.convertCollectionListToRealPath(group, currentKey);
}

QString ExportCalendarsJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(identifier);
    return url;
}

ExportCalendarsJobInterfaceTest::ExportCalendarsJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportCalendarsJobInterfaceTest::exportCalendarConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("full") << pathConfig + QByteArray("full/");
}

void ExportCalendarsJobInterfaceTest::exportCalendarConfig()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportCalendarsJobInterfaceTestImpl *impl = new ExportCalendarsJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    impl->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
}
