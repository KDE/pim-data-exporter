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

#include "importmailjobinterfaceimpl.h"
#include "resourceconverterimpl.h"
#include "archivestorage.h"
#include <AkonadiCore/CollectionFetchJob>
#include <PimCommonAkonadi/CreateResource>
#include <MailCommon/FilterManager>
#include <KIdentityManagement/Identity>
#include <KIdentityManagement/IdentityManager>
#include <MailTransport/TransportManager>
using namespace Akonadi;

ImportMailJobInterfaceImpl::ImportMailJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportMailJobInterfaceImpl::~ImportMailJobInterfaceImpl()
{
}

QString ImportMailJobInterfaceImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterImpl converter;
    return converter.adaptResourcePath(resourceConfig, storedData);
}

void ImportMailJobInterfaceImpl::registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId)
{
    auto fetch = new Akonadi::CollectionFetchJob(Akonadi::Collection(colId), Akonadi::CollectionFetchJob::Base, this);
    connect(fetch, &Akonadi::CollectionFetchJob::collectionsReceived,
            this, [ type](const Akonadi::Collection::List &cols) {
        if (cols.count() != 1) {
            return;
        }
        Akonadi::SpecialMailCollections::self()->registerCollection(type, cols.first());
    });
}

QString ImportMailJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

void ImportMailJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    //It's maildir support. Need to add support
    startSynchronizeResources(lst);
}

void ImportMailJobInterfaceImpl::importFilters(const QVector<MailCommon::MailFilter *> &filters)
{
    MailCommon::FilterManager::instance()->appendFilters(filters);
}

Collection::Id ImportMailJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

QString ImportMailJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

void ImportMailJobInterfaceImpl::addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentity, int oldUid)
{
    KIdentityManagement::Identity *identity = &mIdentityManager->newFromScratch(uniqueIdentityName(name));
    group.writeEntry(QStringLiteral("Name"), name);
    group.sync();

    identity->readConfig(group);

    if (oldUid != -1) {
        mHashIdentity.insert(oldUid, identity->uoid());
        if (oldUid == defaultIdentity) {
            mIdentityManager->setAsDefault(identity->uoid());
        }
    }
    mIdentityManager->commit();
}

QString ImportMailJobInterfaceImpl::uniqueIdentityName(const QString &name)
{
    QString newName(name);
    int i = 0;
    while (!mIdentityManager->isUnique(newName)) {
        newName = QStringLiteral("%1_%2").arg(name).arg(i);
        ++i;
    }
    return newName;
}


void ImportMailJobInterfaceImpl::importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId)
{
    if (!identifierValue.isEmpty()) {
        if (identifierValue == QLatin1String("sendmail") || identifierValue == QLatin1String("akonadi_ewsmta_resource")) {
            MailTransport::Transport *mt = MailTransport::TransportManager::self()->createTransport();
            mt->setName(group.readEntry(QStringLiteral("name")));
            const QString hostStr(QStringLiteral("host"));
            if (group.hasKey(hostStr)) {
                mt->setHost(group.readEntry(hostStr));
            }
            mt->setIdentifier(identifierValue);
            addMailTransport(mt, defaultTransport, transportId);
        } else {
            qCWarning(PIMDATAEXPORTERCORE_LOG) << "Unknown identifier type " << identifierValue;
        }
    } else {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "identifier value is empty";
    }
}

void ImportMailJobInterfaceImpl::importSmtpMailTransport(const KConfigGroup &group, int defaultTransport, int transportId)
{

    MailTransport::Transport *mt = MailTransport::TransportManager::self()->createTransport();
    mt->setName(group.readEntry(QStringLiteral("name")));
    const QString hostStr(QStringLiteral("host"));
    if (group.hasKey(hostStr)) {
        mt->setHost(group.readEntry(hostStr));
    }
    const QString portStr(QStringLiteral("port"));
    if (group.hasKey(portStr)) {
        mt->setPort(group.readEntry(portStr, -1));
    }
    const QString userNameStr(QStringLiteral("user"));
    if (group.hasKey(userNameStr)) {
        mt->setUserName(group.readEntry(userNameStr));
    }
    const QString precommandStr(QStringLiteral("precommand"));
    if (group.hasKey(precommandStr)) {
        mt->setPrecommand(group.readEntry(precommandStr));
    }
    const QString requiresAuthenticationStr(QStringLiteral("auth"));
    if (group.hasKey(requiresAuthenticationStr)) {
        mt->setRequiresAuthentication(group.readEntry(requiresAuthenticationStr, false));
    }
    const QString specifyHostnameStr(QStringLiteral("specifyHostname"));
    if (group.hasKey(specifyHostnameStr)) {
        mt->setSpecifyHostname(group.readEntry(specifyHostnameStr, false));
    }
    const QString localHostnameStr(QStringLiteral("localHostname"));
    if (group.hasKey(localHostnameStr)) {
        mt->setLocalHostname(group.readEntry(localHostnameStr));
    }
    const QString specifySenderOverwriteAddressStr(QStringLiteral("specifySenderOverwriteAddress"));
    if (group.hasKey(specifySenderOverwriteAddressStr)) {
        mt->setSpecifySenderOverwriteAddress(group.readEntry(specifySenderOverwriteAddressStr, false));
    }
    const QString storePasswordStr(QStringLiteral("storepass"));
    if (group.hasKey(storePasswordStr)) {
        mt->setStorePassword(group.readEntry(storePasswordStr, false));
    }
    const QString senderOverwriteAddressStr(QStringLiteral("senderOverwriteAddress"));
    if (group.hasKey(senderOverwriteAddressStr)) {
        mt->setSenderOverwriteAddress(group.readEntry(senderOverwriteAddressStr));
    }
    const QString encryptionStr(QStringLiteral("encryption"));
    if (group.hasKey(encryptionStr)) {
        const QString encryptionType = group.readEntry(encryptionStr, QString());
        if (!encryptionType.isEmpty()) {
            if (encryptionType == QLatin1String("TLS")) {
                mt->setEncryption(static_cast<int>(MailTransport::TransportBase::EnumEncryption::TLS));
            } else if (encryptionType == QLatin1String("SSL")) {
                mt->setEncryption(static_cast<int>(MailTransport::TransportBase::EnumEncryption::SSL));
            } else if (encryptionType == QLatin1String("None")) {
                mt->setEncryption(static_cast<int>(MailTransport::TransportBase::EnumEncryption::None));
            } else {
                qCWarning(PIMDATAEXPORTERCORE_LOG) << "Unknown encryption type " << encryptionType;
            }
        } else {
            qCWarning(PIMDATAEXPORTERCORE_LOG) << "Encryption type is empty. It's a bug";
        }
        mt->setEncryption(group.readEntry(encryptionStr, 1)); //TODO verify
    }
    const QString authenticationTypeStr(QStringLiteral("authtype"));
    if (group.hasKey(authenticationTypeStr)) {
        mt->setAuthenticationType(group.readEntry(authenticationTypeStr, 1)); //TODO verify
    }
    addMailTransport(mt, defaultTransport, transportId);
}
