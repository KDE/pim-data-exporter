/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportaddressbookjobinterfacetest.h"
using namespace Qt::Literals::StringLiterals;

#include "exportaddressbookjobinterfacetestimpl.h"
#include "testexportfile.h"
#include <QTest>

QTEST_MAIN(ExportAddressbookJobInterfaceTest)

ExportAddressbookJobInterfaceTest::ExportAddressbookJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAddressbookJobInterfaceTest::exportAddressBook_data()
{
    QTest::addColumn<QByteArray>("configpath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    QTest::newRow("addressbookonlyconfig") << pathConfig + QByteArray("addressbookonlyconfig/") << options;
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("addressbookconfigandresources") << pathConfig + QByteArray("addressbookconfigandresources/") << options;
}

void ExportAddressbookJobInterfaceTest::exportAddressBook()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    std::unique_ptr<TestExportFile> file{new TestExportFile(this)};
    file->setPathConfig(configpath);
    QList<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;

    info.identifier = u"akonadi_vcarddir_resource_1"_s;
    lstInfo << info;
    info.identifier = u"akonadi_contacts_resource_1"_s;
    lstInfo << info;
    info.identifier = u"akonadi_vcard_resource_1"_s;
    lstInfo << info;
    // Add extra resource.
    info.identifier = u"akonadi_kolab_resource_2"_s;
    lstInfo << info;

    auto exportNote = new ExportAddressbookJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1StringView(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

#include "moc_exportaddressbookjobinterfacetest.cpp"
