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

#include "importmailjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testimportfile.h"
#include "utils.h"
#include "mail/smtpmailtransport.h"
#include <KConfigGroup>
#include <KIdentityManagement/Identity>
#include <QSignalSpy>
#include <QTest>

ImportMailJobInterfaceTestImpl::ImportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportMailJobInterfaceTestImpl::~ImportMailJobInterfaceTestImpl()
{
    //Clean up temp repo. FIXME !
    QVERIFY(QDir(extractPath()).removeRecursively());
}

void ImportMailJobInterfaceTestImpl::start()
{
    QDir().mkpath(extractPath());
    //Create temporary file for identities
    ImportMailJobInterface::start();
}

Akonadi::Collection::Id ImportMailJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ImportMailJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

QString ImportMailJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportMailJobInterfaceTestImpl::registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId)
{
    mLogCreateResource->logRegisterSpecialCollection(type, colId);
}

void ImportMailJobInterfaceTestImpl::importFilters(const QVector<MailCommon::MailFilter *> &filters)
{
    qDebug() << " void ImportMailJobInterfaceTestImpl::importFilters(const QVector<MailCommon::MailFilter *> &filters)";
}

QString ImportMailJobInterfaceTestImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptResourcePath(resourceConfig, storedData);
}

QString ImportMailJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

void ImportMailJobInterfaceTestImpl::addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid)
{
    qDebug() << " void ImportMailJobInterfaceTestImpl::addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) not implemented yet" << oldUid << " name " << name;
    KIdentityManagement::Identity identity;
    identity.setUoid(mIdentityUoid++);
    group.writeEntry(QStringLiteral("Name"), name);
    group.sync();

    identity.readConfig(group);

    if (oldUid != -1) {
        mHashIdentity.insert(oldUid, identity.uoid());
        if (oldUid == defaultIdentities) {
            //TODO
            //mIdentityManager->setAsDefault(identity->uoid());
        }
    }
//    identity.writeConfig();
    //TODO write identity ?
    //TODO log it.
    //mIdentityManager->commit();
}

void ImportMailJobInterfaceTestImpl::importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId)
{
    qDebug() << "importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) not implemented yet";
}

void ImportMailJobInterfaceTestImpl::importSmtpMailTransport(const KConfigGroup &group, int defaultTransport, int transportId)
{
    SmtpMailTransport smtpMailTransport(group);
    qDebug() << "ImportMailJobInterfaceTestImpl::importSmtpMailTransport(const KConfigGroup &group, int defaultTransport, int transportId) not implement yet";
    qDebug() << " defaultTransport " << defaultTransport << " transportId " << transportId;
}
