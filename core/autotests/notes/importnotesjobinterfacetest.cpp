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

void ImportNotesJobInterfaceTestImpl::restoreResources()
{
    qDebug() << " not implement yet";
}

Akonadi::Collection::Id ImportNotesJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

ImportNotesJobInterfaceTest::ImportNotesJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportNotesJobInterfaceTest::importNoteConfig_data()
{
    QTest::addColumn<QString>("zipFilePath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("test1") << QLatin1String(pathConfig) + QStringLiteral("test1/test1.zip");
}

void ImportNotesJobInterfaceTest::importNoteConfig()
{
    QFETCH(QString, zipFilePath);
    const QString path = QLatin1String(PIMDATAEXPORTER_DIR) + QStringLiteral("/import/test1/");

    TestImportFile *file = new TestImportFile(zipFilePath, this);
    file->setPathConfig(path);
    ImportNotesJobInterfaceTestImpl *impl = new ImportNotesJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
}
