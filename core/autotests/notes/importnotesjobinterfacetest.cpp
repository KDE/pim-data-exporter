/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importnotesjobinterfacetest.h"
#include "archivestorage.h"
#include "importnotesjobinterfacetestimpl.h"
#include "testimportfile.h"
#include <QTest>
QTEST_MAIN(ImportNotesJobInterfaceTest)

ImportNotesJobInterfaceTest::ImportNotesJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportNotesJobInterfaceTest::importNote_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    QTest::newRow("test1") << QString::fromLatin1(pathConfig) << QStringLiteral("/test1/") << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("test1resource") << QString::fromLatin1(pathConfig) << QStringLiteral("/test1resource/") << options;
}

void ImportNotesJobInterfaceTest::importNote()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    const QString fullTestPath = zipFilePath + testPath;
    file->setPathConfig(fullTestPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::notePath());
    auto impl = new ImportNotesJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    impl->setPathConfig(file->pathConfig());
    impl->setExistingPathConfig(fullTestPath + QStringLiteral("/existingconfig/"));
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}
