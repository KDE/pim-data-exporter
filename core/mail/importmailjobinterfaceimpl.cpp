/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailjobinterfaceimpl.h"
#include "archivestorage.h"
#include "importmailfolderattributejobimpl.h"
#include "resourceconverterimpl.h"
#include "smtpmailtransport.h"
#include <Akonadi/CollectionFetchJob>
#include <KIdentityManagement/Identity>
#include <KIdentityManagement/IdentityManager>
#include <MailCommon/FilterImporterExporter>
#include <MailCommon/FilterManager>
#include <PimCommonAkonadi/CreateResource>
using namespace Akonadi;

ImportMailJobInterfaceImpl::ImportMailJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
    , mIdentityManager(KIdentityManagement::IdentityManager::self())
{
}

ImportMailJobInterfaceImpl::~ImportMailJobInterfaceImpl() = default;

QString ImportMailJobInterfaceImpl::configLocation() const
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/');
}

void ImportMailJobInterfaceImpl::importFolderAttributes()
{
    auto job = new ImportMailFolderAttributeJobImpl(this);
    job->setArchiveDirectory(mArchiveDirectory);
    job->setExportInterface(this);
    job->setExtractPath(mTempDirName);
    connect(job, &ImportMailFolderAttributeJobImpl::successed, this, [this]() {
        emitInfo(i18n("Restoring Folder Attributes done."));
        Q_EMIT jobFinished();
    });
    connect(job, &ImportMailFolderAttributeJobImpl::failed, this, [this]() {
        Q_EMIT error(i18n("Folder Attributes cannot be restored."));
        Q_EMIT jobFinished();
    });
    job->start();
}

QString ImportMailJobInterfaceImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterImpl converter;
    return converter.adaptResourcePath(resourceConfig, storedData);
}

void ImportMailJobInterfaceImpl::registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId)
{
    auto fetch = new Akonadi::CollectionFetchJob(Akonadi::Collection(colId), Akonadi::CollectionFetchJob::Base, this);
    connect(fetch, &Akonadi::CollectionFetchJob::collectionsReceived, this, [type](const Akonadi::Collection::List &cols) {
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
    // It's maildir support. Need to add support
    startSynchronizeResources(lst);
}

void ImportMailJobInterfaceImpl::importFilters(const QString &filename)
{
    bool canceled = false;
    MailCommon::FilterImporterExporter exportFilters;
    const QVector<MailCommon::MailFilter *> lstFilter = exportFilters.importFilters(canceled, MailCommon::FilterImporterExporter::KMailFilter, filename);
    if (!canceled) {
        MailCommon::FilterManager::instance()->appendFilters(lstFilter);
    }
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
    const QString uniqueName = uniqueIdentityName(name);
    KIdentityManagement::Identity *identity = &mIdentityManager->newFromScratch(uniqueName);
    group.writeEntry(QStringLiteral("Identity"), uniqueName);
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
    int i = 1;
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

int ImportMailJobInterfaceImpl::convertEncryptionEnum(int val)
{
    switch (val) {
    case 0:
        return MailTransport::Transport::EnumEncryption::None;
    case 1:
        return MailTransport::Transport::EnumEncryption::TLS;
    case 2:
        return MailTransport::Transport::EnumEncryption::SSL;
    }
    return MailTransport::Transport::EnumEncryption::None;
}

void ImportMailJobInterfaceImpl::importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId)
{
    MailTransport::Transport *mt = MailTransport::TransportManager::self()->createTransport();
    mt->setName(smtpMailTransport.name());
    mt->setHost(smtpMailTransport.host());
    mt->setPort(smtpMailTransport.port());
    mt->setUserName(smtpMailTransport.userName());
    mt->setPrecommand(smtpMailTransport.precommand());
    mt->setRequiresAuthentication(smtpMailTransport.requiresAuthentication());
    mt->setSpecifyHostname(smtpMailTransport.specifyHostname());
    mt->setLocalHostname(smtpMailTransport.localHostname());
    mt->setSpecifySenderOverwriteAddress(smtpMailTransport.specifySenderOverwriteAddress());
    mt->setStorePassword(smtpMailTransport.storePassword());
    mt->setSenderOverwriteAddress(smtpMailTransport.senderOverwriteAddress());
    mt->setEncryption(convertEncryptionEnum(smtpMailTransport.encryption()));
    mt->setAuthenticationType(smtpMailTransport.authenticationType());
    addMailTransport(mt, defaultTransport, transportId);
}

void ImportMailJobInterfaceImpl::addMailTransport(MailTransport::Transport *mt, int defaultTransport, int transportId)
{
    mt->forceUniqueName();
    mt->save();
    MailTransport::TransportManager::self()->addTransport(mt);
    if (transportId == defaultTransport) {
        MailTransport::TransportManager::self()->setDefaultTransport(mt->id());
    }
    mHashTransport.insert(transportId, mt->id());
    // qDebug() << " void ImportMailJobInterface::addMailTransport(MailTransport::Transport *mt, int defaultTransport, int transportId)" << transportId;
}
