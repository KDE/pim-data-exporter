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

#include "importalarmjobinterfacetest.h"
#include "importalarmjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "testimportfile.h"
#include <QTest>
QTEST_MAIN(ImportAlarmJobInterfaceTest)

ImportAlarmJobInterfaceTest::ImportAlarmJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportAlarmJobInterfaceTest::importAlarmConfig_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("alarmonlyconfig") << QString::fromLatin1(pathConfig) << QStringLiteral("/alarmonlyconfig/") << options;
    options = {Utils::StoredType::Config|Utils::StoredType::Resources};
    QTest::newRow("alarmconfigresources") << QString::fromLatin1(pathConfig) << QStringLiteral("/alarmconfigresources/") << options;
}

void ImportAlarmJobInterfaceTest::importAlarmConfig()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    const QString fullTestPath = zipFilePath + testPath;
    TestImportFile *file = new TestImportFile(fullTestPath, this);
    file->setPathConfig(fullTestPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::alarmPath());
    ImportAlarmJobInterfaceTestImpl *impl = new ImportAlarmJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    impl->setPathConfig(file->pathConfig());
    impl->setExtractPath(file->extractPath());
    impl->setTempDirName(file->extractPath());
    impl->setExistingPathConfig(fullTestPath + QStringLiteral("/existingconfig/"));
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}
