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

#include "saveresourceconfigtest.h"
#include "resourceconvertertest.h"
#include <QTest>
#include <AkonadiCore/ServerManager>

SaveResourceConfigTest::SaveResourceConfigTest()
{
}

KZip *SaveResourceConfigTest::archive() const
{
    return mArchive;
}

void SaveResourceConfigTest::setArchive(KZip *archive)
{
    mArchive = archive;
}

void SaveResourceConfigTest::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier, const QString &resourceArchiveName, const QStringList &resourceIdentifierTypes)
{
    QVERIFY(mArchive);
    bool typeIdentifierFound = false;
    for (const QString &resourceType: resourceIdentifierTypes) {
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
    qDebug() << "urlAgentConfig  " << urlAgentConfig;
    QVERIFY(mArchive->addLocalFile(urlAgentConfig, archivePath + filename));
}
