/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importcalendarjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include <QTest>

ImportCalendarJobInterfaceTestImpl::ImportCalendarJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportCalendarJobInterfaceTestImpl::~ImportCalendarJobInterfaceTestImpl()
{
    //Clean up temp repo
    QVERIFY(QDir(extractPath()).removeRecursively());
    QVERIFY(QDir(QDir::tempPath() + QLatin1Char('/') + Utils::storeCalendar()).removeRecursively());
}

Akonadi::Collection::Id ImportCalendarJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ImportCalendarJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportCalendarJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

QString ImportCalendarJobInterfaceTestImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptResourcePath(resourceConfig, storedData);
}

QString ImportCalendarJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString ImportCalendarJobInterfaceTestImpl::configLocation() const
{
    return mExistingPathConfig;
}

void ImportCalendarJobInterfaceTestImpl::setExistingPathConfig(const QString &path)
{
    mExistingPathConfig = path;
}
