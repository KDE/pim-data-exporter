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
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testexportfile.h"
#include <QDebug>
#include <QTest>
#include <saveresourceconfigtest.h>
#include <testbackupresourcefilejob.h>

QTEST_MAIN(ExportMailJobInterfaceTest)

ExportMailJobInterfaceTestImpl::ExportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportMailJobInterfaceTestImpl::~ExportMailJobInterfaceTestImpl()
{
}

void ExportMailJobInterfaceTestImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    resourceConverter.convertCollectionToRealPath(group, currentKey);
}

void ExportMailJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    resourceConverter.convertCollectionListToRealPath(group, currentKey);
}

void ExportMailJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    resourceConverter.convertCollectionIdsToRealPath(group, currentKey, prefixCollection);
}

Akonadi::Collection::Id ExportMailJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QVector<MailCommon::MailFilter *> ExportMailJobInterfaceTestImpl::filters()
{
    qDebug() << " not implement yet ExportMailJobInterfaceTestImpl::filters()";
    //TODO implement it
    return {};
}

void ExportMailJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResoureConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath, url, identifier, Utils::resourceMailArchiveName(),
                                               { QLatin1String("akonadi_maildir_resource_"), QLatin1String("akonadi_mixedmaildir_resource_")});
    slotMailsJobTerminated();
}

QVector<Utils::AkonadiInstanceInfo> ExportMailJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

QString ExportMailJobInterfaceTestImpl::storeResources(KZip *archive, const QString &identifier, const QString &path)
{
    ResourceConverterTest converter;
    return converter.storeResources(archive, identifier, path);
}

QString ExportMailJobInterfaceTestImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterTest converter;
    converter.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    return converter.convertToFullCollectionPath(collectionValue);
}

QString ExportMailJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(QLatin1String(PIMDATAEXPORTER_DIR));
    const QString url = converter.resourcePath(identifier);
    return url;
}

void ExportMailJobInterfaceTestImpl::backupMailResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    TestBackupResourceFileJob *job = new TestBackupResourceFileJob(this);
    job->setDefaultPath(defaultPath);
    job->setIdentifier(agentIdentifier);
    job->setZip(archive());
    connect(job, &TestBackupResourceFileJob::error, this, &ExportMailJobInterfaceTestImpl::error);
    connect(job, &TestBackupResourceFileJob::info, this, &ExportMailJobInterfaceTestImpl::info);
    job->start();
}

ExportMailJobInterfaceTest::ExportMailJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportMailJobInterfaceTest::exportMailConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("mailonlyconfig") << pathConfig + QByteArray("mailonlyconfig/");
}

void ExportMailJobInterfaceTest::exportMailConfig()
{
    QFETCH(QByteArray, configpath);
    qDebug() << " configpath" << configpath;
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportMailJobInterfaceTestImpl *exportNote = new ExportMailJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

void ExportMailJobInterfaceTest::exportMailConfigAndResource_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("mailconfigandconfig") << pathConfig + QByteArray("mailconfigandconfig/");
}

void ExportMailJobInterfaceTest::exportMailConfigAndResource()
{
    QFETCH(QByteArray, configpath);
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

    ExportMailJobInterfaceTestImpl *exportNote = new ExportMailJobInterfaceTestImpl(this, {Utils::StoredType::Config|Utils::StoredType::Resources}, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
