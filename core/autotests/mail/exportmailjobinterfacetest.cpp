/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailjobinterfacetest.h"
using namespace Qt::Literals::StringLiterals;

#include "archivestorage.h"
#include "exportmailjobinterfacetestimpl.h"
#include "testexportfile.h"
#include <KMime/Message>
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

    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("mailconfigandresource") << pathConfig + QByteArray("mailconfigandresource/") << options;

    options = {Utils::StoredType::MailTransport};
    QTest::newRow("mailtransport") << pathConfig + QByteArray("mailtransport/") << options;

    options = {Utils::StoredType::MailTransport};
    QTest::newRow("mailtransport2") << pathConfig + QByteArray("mailtransport2/") << options;

    options = {Utils::StoredType::Identity};
    QTest::newRow("identities") << pathConfig + QByteArray("identities/") << options;

    options = {Utils::StoredType::Identity | Utils::StoredType::Config};
    QTest::newRow("identitiesandconfig") << pathConfig + QByteArray("identitiesandconfig/") << options;

    options = {Utils::StoredType::Config | Utils::StoredType::Identity | Utils::StoredType::MailTransport};
    QTest::newRow("identitiesandconfigandmailtransport") << pathConfig + QByteArray("identitiesandconfigandmailtransport/") << options;

    options = {Utils::StoredType::Config | Utils::StoredType::Identity | Utils::StoredType::MailTransport | Utils::StoredType::Resources
               | Utils::StoredType::Mails};
    QTest::newRow("identitiesandconfigandmailtransportandresources") << pathConfig + QByteArray("identitiesandconfigandmailtransportandresources/") << options;
}

void ExportMailJobInterfaceTest::exportMail()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    std::unique_ptr<TestExportFile> file{new TestExportFile(this)};
    file->setPathConfig(configpath);
    QList<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;

    info.identifier = u"akonadi_mbox_resource_1"_s;
    info.mimeTypes = QStringList() << KMime::Message::mimeType();
    info.capabilities = QStringList() << u"Resource"_s;
    lstInfo << info;
    info.identifier = u"akonadi_maildir_resource_1"_s;
    info.mimeTypes = QStringList() << KMime::Message::mimeType();
    info.capabilities = QStringList() << u"Resource"_s;
    lstInfo << info;
    info.identifier = u"akonadi_mixedmaildir_resource_1"_s;
    info.mimeTypes = QStringList() << KMime::Message::mimeType();
    info.capabilities = QStringList() << u"Resource"_s;
    lstInfo << info;

    // Pop3 support
    info.identifier = u"akonadi_pop3_resource_1"_s;
    info.mimeTypes = QStringList() << KMime::Message::mimeType();
    info.capabilities = QStringList() << u"Resource"_s;
    lstInfo << info;

    // Imap support
    info.identifier = u"akonadi_imap_resource_1"_s;
    info.mimeTypes = QStringList() << KMime::Message::mimeType();
    info.capabilities = QStringList() << u"Resource"_s;
    lstInfo << info;

    // Kolab support
    info.identifier = u"akonadi_kolab_resource_1"_s;
    info.mimeTypes = QStringList() << KMime::Message::mimeType();
    info.capabilities = QStringList() << u"Resource"_s;
    lstInfo << info;

    // Add extra resource.
    info.identifier = u"akonadi_kalarm_dir_resource_2"_s;
    lstInfo << info;

    auto exportMail = new ExportMailJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportMail->setListOfResource(lstInfo);
    exportMail->setPathConfig(QLatin1StringView(configpath));
    file->setAbstractImportExportJob(exportMail);
    file->start();
    delete exportMail;
}

#include "moc_exportmailjobinterfacetest.cpp"
