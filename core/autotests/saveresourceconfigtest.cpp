/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "saveresourceconfigtest.h"
#include "resourceconvertertest.h"
#include <Akonadi/ServerManager>
#include <QTest>

SaveResourceConfigTest::SaveResourceConfigTest() = default;

KZip *SaveResourceConfigTest::archive() const
{
    return mArchive;
}

void SaveResourceConfigTest::setArchive(KZip *archive)
{
    mArchive = archive;
}

void SaveResourceConfigTest::exportResourceToArchive(const QString &archivePath,
                                                     const QString &url,
                                                     const QString &identifier,
                                                     const QString &resourceArchiveName,
                                                     const QStringList &resourceIdentifierTypes)
{
    QVERIFY(mArchive);
    bool typeIdentifierFound = false;
    for (const QString &resourceType : resourceIdentifierTypes) {
        typeIdentifierFound = identifier.startsWith(resourceType);
        if (typeIdentifierFound) {
            break;
        }
    }

    QVERIFY(typeIdentifierFound);
    if (QFileInfo(url).isFile()) {
        QVERIFY(mArchive->addLocalFile(url, archivePath + resourceArchiveName));
    } else {
        const QString zipFile = url + identifier + QLatin1String(".zip");
        const bool success = mArchive->addLocalFile(zipFile, archivePath + resourceArchiveName);
        if (!success) {
            qDebug() << " zip file " << zipFile;
        }
        QVERIFY(success);
    }
    ResourceConverterTest converter;
    const QString errorStr = converter.storeResources(mArchive, identifier, archivePath);
    QVERIFY(errorStr.isEmpty());
    const QString urlAgentConfig = Akonadi::ServerManager::agentConfigFilePath(identifier);
    QVERIFY(!urlAgentConfig.isEmpty());
    const QFileInfo fi(urlAgentConfig);
    const QString filename = fi.fileName();
    // qDebug() << "urlAgentConfig  " << urlAgentConfig;
    QVERIFY(mArchive->addLocalFile(urlAgentConfig, archivePath + filename));
}
