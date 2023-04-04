/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportnotesjobinterfacetest.h"
#include "archivestorage.h"
#include "exportnotesjobinterfacetestimpl.h"
#include "testexportfile.h"
#include <QTest>

QTEST_MAIN(ExportNotesJobInterfaceTest)

ExportNotesJobInterfaceTest::ExportNotesJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportNotesJobInterfaceTest::exportNote_data()
{
    QTest::addColumn<QByteArray>("configpath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    QTest::newRow("test1") << pathConfig + QByteArray("test1/") << options;
    QTest::newRow("full") << pathConfig + QByteArray("full/") << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("noteconfigandresource") << pathConfig + QByteArray("noteconfigandresource/") << options;
}

void ExportNotesJobInterfaceTest::exportNote()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    std::unique_ptr<TestExportFile> file{new TestExportFile(this)};
    file->setPathConfig(configpath);
    QList<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;
    info.identifier = QStringLiteral("akonadi_akonotes_resource_1");
    lstInfo << info;
    info.identifier = QStringLiteral("akonadi_akonotes_resource_2");
    lstInfo << info;
    // Add extra resource.
    info.identifier = QStringLiteral("akonadi_kontact_resource_2");
    lstInfo << info;

    auto exportNote = new ExportNotesJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
