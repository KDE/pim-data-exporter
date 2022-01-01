/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importaddressbookjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include <QTest>

#include <QDir>

ImportAddressbookJobInterfaceTestImpl::ImportAddressbookJobInterfaceTestImpl(QObject *parent,
                                                                             Utils::StoredTypes typeSelected,
                                                                             ArchiveStorage *archiveStorage,
                                                                             int numberOfStep)
    : ImportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAddressbookJobInterfaceTestImpl::~ImportAddressbookJobInterfaceTestImpl()
{
    // Clean up temp repo
    QVERIFY(QDir(extractPath()).removeRecursively());
    QVERIFY(QDir(QDir::tempPath() + QLatin1Char('/') + Utils::storeAddressbook()).removeRecursively());
}

Akonadi::Collection::Id ImportAddressbookJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ImportAddressbookJobInterfaceTestImpl::createResource(const QString &resources,
                                                              const QString &name,
                                                              const QMap<QString, QVariant> &settings,
                                                              bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportAddressbookJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

QString ImportAddressbookJobInterfaceTestImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptResourcePath(resourceConfig, storedData);
}

QString ImportAddressbookJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString ImportAddressbookJobInterfaceTestImpl::configLocation() const
{
    // TODO
    return {};
}
