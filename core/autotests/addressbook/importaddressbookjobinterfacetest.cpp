/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importaddressbookjobinterfacetest.h"
using namespace Qt::Literals::StringLiterals;

#include "importaddressbookjobinterfacetestimpl.h"
#include "testimportfile.h"
#include <QTest>
QTEST_MAIN(ImportAddressbookJobInterfaceTest)

ImportAddressbookJobInterfaceTest::ImportAddressbookJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportAddressbookJobInterfaceTest::importAddressbook_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    Utils::StoredTypes options = {Utils::StoredType::Config};

    QTest::newRow("addressbookonlyconfig") << QString::fromLatin1(pathConfig) << u"/addressbookonlyconfig/"_s << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("addressbookconfigandresources") << QString::fromLatin1(pathConfig) << u"/addressbookconfigandresources/"_s << options;
}

void ImportAddressbookJobInterfaceTest::importAddressbook()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);

    auto file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::addressbookPath());
    auto impl = new ImportAddressbookJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    impl->setPathConfig(file->pathConfig());
    impl->setExtractPath(file->extractPath());
    impl->setTempDirName(file->extractPath());
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}

#include "moc_importaddressbookjobinterfacetest.cpp"
