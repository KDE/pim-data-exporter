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

#include "exportnotesjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testexportfile.h"
#include <AkonadiCore/ServerManager>
#include <KZip>
#include <QDebug>
#include <QTest>

QTEST_MAIN(ExportNotesJobInterfaceTest)

ExportNotesJobInterfaceTestImpl::ExportNotesJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportNotesJobInterfaceTestImpl::~ExportNotesJobInterfaceTestImpl()
{
}

void ExportNotesJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    resourceConverterTest.convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
}

void ExportNotesJobInterfaceTestImpl::exportArchiveResource()
{
    qDebug() << " not implement yet";
    slotWriteNextArchiveResource();
}

QVector<Utils::AkonadiInstanceInfo> ExportNotesJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

void ExportNotesJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    qDebug() << " void ExportNotesJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)" << archivePath << " url " << url << " identifier " << identifier;
    QVERIFY(identifier.startsWith(QLatin1String("akonadi_akonotes_resource_")));
    QVERIFY(mArchiveStorage->archive()->addLocalFile(url + identifier + QLatin1String(".zip"), archivePath + Utils::resourceNoteArchiveName()));
    ResourceConverterTest converter;
    const QString errorStr = converter.storeResources(archive(), identifier, archivePath);
    QVERIFY(!errorStr.isEmpty());
    const QString urlAgentConfig = Akonadi::ServerManager::agentConfigFilePath(identifier);
    QVERIFY(!urlAgentConfig.isEmpty());
    const QFileInfo fi(urlAgentConfig);
    const QString filename = fi.fileName();
    QVERIFY(mArchiveStorage->archive()->addLocalFile(urlAgentConfig, archivePath + filename));
    slotNoteJobTerminated();
}

Akonadi::Collection::Id ExportNotesJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ExportNotesJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(identifier);
    return url;
}

ExportNotesJobInterfaceTest::ExportNotesJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportNotesJobInterfaceTest::exportNoteConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("test1") << pathConfig + QByteArray("test1/");
    QTest::newRow("full") << pathConfig + QByteArray("full/");
}

void ExportNotesJobInterfaceTest::exportNoteConfig()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportNotesJobInterfaceTestImpl *exportNote = new ExportNotesJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

void ExportNotesJobInterfaceTest::exportNoteConfigAndResource_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    //QTest::newRow("test1resource") << pathConfig + QByteArray("test1resource/");
    QTest::newRow("fullresource") << pathConfig + QByteArray("fullresource/");
}

void ExportNotesJobInterfaceTest::exportNoteConfigAndResource()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    QVector<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;
    info.identifier = QStringLiteral("akonadi_akonotes_resource_1");
    lstInfo << info;
    info.identifier = QStringLiteral("akonadi_akonotes_resource_2");
    lstInfo << info;
    //Add extra resource.
    info.identifier = QStringLiteral("akonadi_kontact_resource_2");
    lstInfo << info;

    ExportNotesJobInterfaceTestImpl *exportNote = new ExportNotesJobInterfaceTestImpl(this, {Utils::StoredType::Config|Utils::StoredType::Resources}, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
