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

#include "exportakregatorjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testexportfile.h"
#include "compareexportfile.h"
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

QTEST_MAIN(ExportAkregatorJobInterfaceTest)

ExportAkregatorJobInterfaceTestImpl::ExportAkregatorJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAkregatorJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAkregatorJobInterfaceTestImpl::~ExportAkregatorJobInterfaceTestImpl()
{
}

QString ExportAkregatorJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    Q_UNREACHABLE();
    return {};
}

QString ExportAkregatorJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNREACHABLE();
    return {};
}

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
    //TODO
    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("fullresource") << pathConfig + QByteArray("fullresource/") << options;
}

void ExportAkregatorJobInterfaceTest::exportAkregator()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);

    ExportAkregatorJobInterfaceTestImpl *exportAkregator = new ExportAkregatorJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(exportAkregator);
    file->start();
    delete exportAkregator;
}
