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

#include "exportmailjobinterfacetest.h"
#include "exportmailjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "testexportfile.h"
#include <QDebug>
#include <QTest>

QTEST_MAIN(ExportMailJobInterfaceTest)

ExportMailJobInterfaceTest::ExportMailJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportMailJobInterfaceTest::exportMail_data()
{
    QTest::addColumn<QByteArray>("configpath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    QTest::newRow("mailonlyconfig") << pathConfig + QByteArray("mailonlyconfig/") << options;
    options = {Utils::StoredType::Config|Utils::StoredType::Resources};
    QTest::newRow("mailconfigandresource") << pathConfig + QByteArray("mailconfigandresource/") << options;
    options = {Utils::StoredType::MailTransport};
    QTest::newRow("mailtransport") << pathConfig + QByteArray("mailtransport/") << options;
    options = {Utils::StoredType::Identity};
    QTest::newRow("identities") << pathConfig + QByteArray("identities/") << options;
}

void ExportMailJobInterfaceTest::exportMail()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    QVector<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;

    info.identifier = QLatin1String("akonadi_mbox_resource_1");
    lstInfo << info;
    info.identifier = QLatin1String("akonadi_maildir_resource_1");
    lstInfo << info;
    info.identifier = QLatin1String("akonadi_mixedmaildir_resource_1");
    lstInfo << info;
    //Add extra resource.
    info.identifier = QStringLiteral("akonadi_kolab_resource_2");
    lstInfo << info;

    ExportMailJobInterfaceTestImpl *exportNote = new ExportMailJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
