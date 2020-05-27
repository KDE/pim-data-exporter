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

#include "exportnotesjobinterfacetest.h"
#include "exportnotesjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "testexportfile.h"
#include <KZip>
#include <QDebug>
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
    options = {Utils::StoredType::Config|Utils::StoredType::Resources};
    QTest::newRow("noteconfigandresource") << pathConfig + QByteArray("noteconfigandresource/") << options;
}

void ExportNotesJobInterfaceTest::exportNote()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    QVector<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;
    info.identifier = QStringLiteral("akonadi_akonotes_resource_1");
    lstInfo << info;
    info.identifier = QStringLiteral("akonadi_akonotes_resource_2");
    lstInfo << info;
    //Add extra resource.
    info.identifier = QStringLiteral("akonadi_kontact_resource_2");
    lstInfo << info;

    ExportNotesJobInterfaceTestImpl *exportNote = new ExportNotesJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
