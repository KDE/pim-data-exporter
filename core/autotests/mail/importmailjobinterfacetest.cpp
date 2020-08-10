/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailjobinterfacetest.h"
#include "importmailjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "testimportfile.h"
#include "utils.h"
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
    options = {Utils::StoredType::Config|Utils::StoredType::Identity};
    QTest::newRow("identitiesandconfig") << QString::fromLatin1(pathConfig) << QStringLiteral("identitiesandconfig/") << options;
    options = {Utils::StoredType::Config|Utils::StoredType::Identity|Utils::StoredType::MailTransport};
    QTest::newRow("identitiesandconfigandmailtransport") << QString::fromLatin1(pathConfig) << QStringLiteral("identitiesandconfigandmailtransport/") << options;
    options = {Utils::StoredType::Config|Utils::StoredType::Identity|Utils::StoredType::MailTransport|Utils::StoredType::Resources|Utils::Mails};
    QTest::newRow("identitiesandconfigandmailtransportandresources") << QString::fromLatin1(pathConfig) << QStringLiteral("identitiesandconfigandmailtransportandresources/") << options;
}

void ImportMailJobInterfaceTest::importMail()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    const QString fullTestPath = zipFilePath + testPath;
    TestImportFile *file = new TestImportFile(fullTestPath, this);
    file->setPathConfig(fullTestPath);
    file->setExtractPath(QDir::tempPath() + QLatin1Char('/') + testPath);
    file->setExcludePath(Utils::mailsPath()); // ???
    ImportMailJobInterfaceTestImpl *impl = new ImportMailJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    impl->setPathConfig(file->pathConfig());
    impl->setExtractPath(file->extractPath());
    impl->setTempDirName(file->extractPath());
    impl->setExistingPathConfig(fullTestPath + QStringLiteral("/existingconfig/"));
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}
