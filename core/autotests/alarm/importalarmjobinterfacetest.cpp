/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importalarmjobinterfacetest.h"
#include "archivestorage.h"
#include "importalarmjobinterfacetestimpl.h"
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
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("alarmconfigresources") << QString::fromLatin1(pathConfig) << QStringLiteral("/alarmconfigresources/") << options;
}

void ImportAlarmJobInterfaceTest::importAlarmConfig()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    const QString fullTestPath = zipFilePath + testPath;
    auto file = new TestImportFile(fullTestPath, this);
    file->setPathConfig(fullTestPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::alarmPath());
    auto impl = new ImportAlarmJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
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
