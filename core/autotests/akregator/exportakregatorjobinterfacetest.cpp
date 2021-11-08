/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportakregatorjobinterfacetest.h"
#include "archivestorage.h"
#include "exportakregatorjobinterfacetestimpl.h"
#include "testexportfile.h"
#include <QTest>

QTEST_MAIN(ExportAkregatorJobInterfaceTest)

ExportAkregatorJobInterfaceTest::ExportAkregatorJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAkregatorJobInterfaceTest::exportAkregator_data()
{
    QTest::addColumn<QByteArray>("configpath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    // TODO
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("fullresource") << pathConfig + QByteArray("fullresource/") << options;
}

void ExportAkregatorJobInterfaceTest::exportAkregator()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    std::unique_ptr<TestExportFile> file{new TestExportFile(this)};
    file->setPathConfig(configpath);

    auto exportAkregator = new ExportAkregatorJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(exportAkregator);
    file->start();
    delete exportAkregator;
}
