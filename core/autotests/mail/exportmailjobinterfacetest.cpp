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
#include "testexportfile.h"
#include <QDebug>
#include <QTest>

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

Akonadi::Collection::Id ExportMailJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QVector<MailCommon::MailFilter *> ExportMailJobInterfaceTestImpl::filters()
{
    qDebug() << " not implement yet ExportMailJobInterfaceTestImpl::filters()";
    //TODO implement it
    return {};
}

void ExportMailJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    qDebug() << " not implement yet";
}

QVector<Utils::AkonadiInstanceInfo> ExportMailJobInterfaceTestImpl::listOfResource()
{
    //TODO
    return {};
}

ExportMailJobInterfaceTest::ExportMailJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportMailJobInterfaceTest::exportMailConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("test1") << pathConfig + QByteArray("test1/");
}

void ExportMailJobInterfaceTest::exportMailConfig()
{
    QFETCH(QByteArray, configpath);
    qDebug() << " configpath" << configpath;
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportMailJobInterfaceTestImpl *exportNote = new ExportMailJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

QString ExportMailJobInterfaceTestImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterTest converter;
    return converter.convertToFullCollectionPath(collectionValue);
}
