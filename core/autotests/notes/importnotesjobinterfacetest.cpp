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

#include "importnotesjobinterfacetest.h"
#include "archivestorage.h"
#include "notes/importnotesjobinterface.h"
#include "resourceconvertertest.h"
#include "testimportfile.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(ImportNotesJobInterfaceTest)

ImportNotesJobInterfaceTestImpl::ImportNotesJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportNotesJobInterfaceTestImpl::~ImportNotesJobInterfaceTestImpl()
{
}

Akonadi::Collection::Id ImportNotesJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ImportNotesJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
    qDebug() << " QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings)" << resources << " name " << name << " settings " << settings;
    //TODO
    return {};
}

void ImportNotesJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

ImportNotesJobInterfaceTest::ImportNotesJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportNotesJobInterfaceTest::importNoteConfig_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("test1") << QString::fromLatin1(pathConfig) << QStringLiteral("/test1/");
}

void ImportNotesJobInterfaceTest::importNoteConfig()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    ImportNotesJobInterfaceTestImpl *impl = new ImportNotesJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
    delete file;
}

void ImportNotesJobInterfaceTest::importNoteConfigAndResources_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("test1") << QString::fromLatin1(pathConfig) << QStringLiteral("/test1/");
}

void ImportNotesJobInterfaceTest::importNoteConfigAndResources()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    ImportNotesJobInterfaceTestImpl *impl = new ImportNotesJobInterfaceTestImpl(this, {Utils::StoredType::Config|Utils::StoredType::Resources}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
    delete file;
}
