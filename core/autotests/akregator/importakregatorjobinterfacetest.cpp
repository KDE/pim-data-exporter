/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importakregatorjobinterfacetest.h"
using namespace Qt::Literals::StringLiterals;

#include "archivestorage.h"
#include "importakregatorjobinterfacetestimpl.h"
#include "testimportfile.h"
#include <QTest>
QTEST_MAIN(ImportAkregatorJobInterfaceTest)

ImportAkregatorJobInterfaceTest::ImportAkregatorJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportAkregatorJobInterfaceTest::importAkegator_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    QTest::newRow("test1") << QString::fromLatin1(pathConfig) << u"/akregatorconfig/"_s << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("test1resource") << QString::fromLatin1(pathConfig) << u"/akregatorresource/"_s << options;
}

void ImportAkregatorJobInterfaceTest::importAkegator()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    auto file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    auto impl = new ImportAkregatorJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->start();
    delete impl;
    delete file;
}

#include "moc_importakregatorjobinterfacetest.cpp"
