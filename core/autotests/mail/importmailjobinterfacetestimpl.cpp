/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailjobinterfacetestimpl.h"
using namespace Qt::Literals::StringLiterals;

#include "archivestorage.h"
#include "importmailfolderattributejobtestimpl.h"
#include "mail/smtpmailtransport.h"
#include "resourceconvertertest.h"
#include "testimportfile.h"
#include "utils.h"
#include <KConfigGroup>
#include <KIdentityManagementCore/Identity>
#include <QTest>

#define REMOVE_TEMPORARY_DIRECTORIES 1

ImportMailJobInterfaceTestImpl::ImportMailJobInterfaceTestImpl(QObject *parent,
                                                               Utils::StoredTypes typeSelected,
                                                               ArchiveStorage *archiveStorage,
                                                               int numberOfStep)
    : ImportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportMailJobInterfaceTestImpl::~ImportMailJobInterfaceTestImpl()
{
#ifdef REMOVE_TEMPORARY_DIRECTORIES
    QVERIFY(QDir(extractPath()).removeRecursively());
    QVERIFY(QDir(QDir::tempPath() + u'/' + Utils::storeMails()).removeRecursively());
#endif
}

void ImportMailJobInterfaceTestImpl::start()
{
    QDir().mkpath(extractPath());
    ImportMailJobInterface::start();
}

void ImportMailJobInterfaceTestImpl::setExistingPathConfig(const QString &path)
{
    mExistingPathConfig = path;
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

QString
ImportMailJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportMailJobInterfaceTestImpl::registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId)
{
    mLogCreateResource->logRegisterSpecialCollection(type, colId);
}

void ImportMailJobInterfaceTestImpl::importFilters(const QString &filename)
{
    // qDebug() << " void ImportMailJobInterfaceTestImpl::importFilters(const QString &filename)" << filename << " extractpath " << extractPath();
    QFile f(filename);
    QDir dir(extractPath());
    QVERIFY(dir.mkpath(u"config"_s));
    QVERIFY(f.copy(extractPath() + u"/config/filters"_s));
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
    // qDebug() << " void ImportMailJobInterfaceTestImpl::addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) not
    // implemented yet" << oldUid << " name " << name;
    const QString uniqueName = uniqueIdentityName(name);
    KIdentityManagementCore::Identity identity;
    uint value = mIdentityUoid;
    mIdentityUoid++;
    identity.setUoid(value);
    group.writeEntry(u"Name"_s, uniqueName);
    group.sync();

    KConfig config(extractPath() + u"/identities/emailidentities"_s);

    identity.readConfig(group);

    if (oldUid != -1) {
        mHashIdentity.insert(oldUid, identity.uoid());
        if (oldUid == defaultIdentities) {
            KConfigGroup grpGeneral = config.group(u"General"_s);
            grpGeneral.writeEntry("Default Identity", identity.uoid());
        }
    }
    KConfigGroup grp = config.group(u"Identity #%1"_s.arg(value - 1));
    identity.writeConfig(grp);
}

QString ImportMailJobInterfaceTestImpl::uniqueIdentityName(const QString &name)
{
    QString newName(name);
    int i = 0;

    const QStringList existingIdentityNames{u"identity1"_s, u"identity2"_s};

    while (existingIdentityNames.contains(newName)) {
        newName = u"%1_%2"_s.arg(name).arg(i);
        ++i;
    }
    return newName;
}

void ImportMailJobInterfaceTestImpl::importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId)
{
    Q_UNUSED(identifierValue);
    Q_UNUSED(group);
    Q_UNUSED(defaultTransport);
    Q_UNUSED(transportId);
    qDebug()
        << "importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) not implemented yet";
}

void ImportMailJobInterfaceTestImpl::importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId)
{
    // qDebug() << " defaultTransport " << defaultTransport << " transportId " << transportId;
    KConfig config(extractPath() + u"/config/mailtransports"_s);
    const int transportValue = mMailTransportId;
    mMailTransportId++;
    KConfigGroup grp = config.group(u"Transport %1"_s.arg(transportValue));

    const auto name = smtpMailTransport.name();
    grp.writeEntry(u"name"_s, name);
    const auto host = smtpMailTransport.host();
    grp.writeEntry(u"host"_s, host);
    const auto port = smtpMailTransport.port();
    grp.writeEntry(u"port"_s, port);
    const auto userName = smtpMailTransport.userName();
    grp.writeEntry(u"userName"_s, userName);
    const auto precommand = smtpMailTransport.precommand();
    grp.writeEntry(u"precommand"_s, precommand);
    const auto requiresAuthentication = smtpMailTransport.requiresAuthentication();
    grp.writeEntry(u"requiresAuthentication"_s, requiresAuthentication);
    const auto specifyHostname = smtpMailTransport.specifyHostname();
    grp.writeEntry(u"specifyHostname"_s, specifyHostname);
    const auto localHostname = smtpMailTransport.localHostname();
    grp.writeEntry(u"localHostname"_s, localHostname);
    const auto specifySenderOverwriteAddress = smtpMailTransport.specifySenderOverwriteAddress();
    grp.writeEntry(u"specifySenderOverwriteAddress"_s, specifySenderOverwriteAddress);
    const auto storePassword = smtpMailTransport.storePassword();
    grp.writeEntry(u"storePassword"_s, storePassword);
    const auto senderOverwriteAddress = smtpMailTransport.senderOverwriteAddress();
    grp.writeEntry(u"senderOverwriteAddress"_s, senderOverwriteAddress);
    const auto encryption = smtpMailTransport.encryption();
    grp.writeEntry(u"encryption"_s, encryption);
    const auto authenticationType = smtpMailTransport.authenticationType();
    grp.writeEntry(u"authenticationType"_s, authenticationType);

    if (transportId == defaultTransport) {
        KConfigGroup generalGrp = config.group(u"General"_s);
        generalGrp.writeEntry(u"default-transport"_s, transportValue);
    }
    mHashTransport.insert(transportId, transportValue);
}

QString ImportMailJobInterfaceTestImpl::configLocation() const
{
    qDebug() << " QString ImportMailJobInterfaceTestImpl::configLocation() const " << mExistingPathConfig;
    return mExistingPathConfig;
}

void ImportMailJobInterfaceTestImpl::importFolderAttributes()
{
    auto job = new ImportMailFolderAttributeJobTestImpl(this);
    job->setArchiveDirectory(mArchiveDirectory);
    job->setExtractPath(extractPath());
    job->setExportInterface(this);
    connect(job, &ImportMailFolderAttributeJobTestImpl::successed, this, [this]() {
        emitInfo(u"Backing up Folder Attributes done."_s);
        Q_EMIT jobFinished();
    });
    connect(job, &ImportMailFolderAttributeJobTestImpl::failed, this, [this]() {
        Q_EMIT error(u"Folder Attributes cannot be exported."_s);
        Q_EMIT jobFinished();
    });
    job->start();
}

#include "moc_importmailjobinterfacetestimpl.cpp"
