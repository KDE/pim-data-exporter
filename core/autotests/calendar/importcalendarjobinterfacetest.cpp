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

#include "importcalendarjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testimportfile.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(ImportCalendarJobInterfaceTest)

ImportCalendarJobInterfaceTestImpl::ImportCalendarJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportCalendarJobInterfaceTestImpl::~ImportCalendarJobInterfaceTestImpl()
{
}

Akonadi::Collection::Id ImportCalendarJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ImportCalendarJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
    //TODO
    return {};
}

void ImportCalendarJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    //TODO
}

ImportCalendarJobInterfaceTest::ImportCalendarJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportCalendarJobInterfaceTest::importCalendarConfig_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("test1") << QString::fromLatin1(pathConfig) << QStringLiteral("/test1/");
}

void ImportCalendarJobInterfaceTest::importCalendarConfig()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    TestImportFile *file = new TestImportFile(zipFilePath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    ImportCalendarJobInterfaceTestImpl *impl = new ImportCalendarJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
    delete file;
}
