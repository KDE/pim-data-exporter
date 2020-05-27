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

#include "importmailjobinterfacetest.h"
#include "importmailjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "testimportfile.h"
#include "utils.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(ImportMailJobInterfaceTest)

ImportMailJobInterfaceTest::ImportMailJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportMailJobInterfaceTest::importMail_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");

    QTest::newRow("mailonlyconfig") << QString::fromLatin1(pathConfig) << QStringLiteral("/mailonlyconfig/") << options;
    options = {Utils::StoredType::Config|Utils::StoredType::Resources};
    QTest::newRow("mailconfigandresource") << QString::fromLatin1(pathConfig) <<  QStringLiteral("mailconfigandresource/") << options;
    options = {Utils::StoredType::MailTransport};
    QTest::newRow("mailtransport") << QString::fromLatin1(pathConfig) << QStringLiteral("mailtransport/") << options;
    options = {Utils::StoredType::Identity};
    QTest::newRow("identities") << QString::fromLatin1(pathConfig) << QStringLiteral("identities/") << options;
}

void ImportMailJobInterfaceTest::importMail()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + QLatin1Char('/') + testPath);
    file->setExcludePath(Utils::mailsPath()); // ???
    ImportMailJobInterfaceTestImpl *impl = new ImportMailJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    impl->setPathConfig(file->pathConfig());
    impl->setExtractPath(file->extractPath());
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}
