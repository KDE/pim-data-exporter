/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importalarmjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"

#include <QDir>
#include <QTest>

ImportAlarmJobInterfaceTestImpl::ImportAlarmJobInterfaceTestImpl(QObject *parent,
                                                                 Utils::StoredTypes typeSelected,
                                                                 ArchiveStorage *archiveStorage,
                                                                 int numberOfStep)
    : ImportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAlarmJobInterfaceTestImpl::~ImportAlarmJobInterfaceTestImpl()
{
    // Clean up temp repo
    QVERIFY(QDir(extractPath()).removeRecursively());
    QVERIFY(QDir(QDir::tempPath() + QLatin1Char('/') + Utils::storeAlarm()).removeRecursively());
}

Akonadi::Collection::Id ImportAlarmJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString
ImportAlarmJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportAlarmJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

QString ImportAlarmJobInterfaceTestImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptResourcePath(resourceConfig, storedData);
}

QString ImportAlarmJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

void ImportAlarmJobInterfaceTestImpl::setExistingPathConfig(const QString &path)
{
    mExistingPathConfig = path;
}

QString ImportAlarmJobInterfaceTestImpl::configLocation() const
{
    return mExistingPathConfig;
}

#include "moc_importalarmjobinterfacetestimpl.cpp"
