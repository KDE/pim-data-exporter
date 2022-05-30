/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailjobinterfaceimpl.h"
#include "importexportmailutil.h"

#include "importexportprogressindicatorbase.h"
#include <KIdentityManagement/IdentityManager>
#include <KMime/Message>
#include <MailCommon/FilterImporterExporter>
#include <MailCommon/FilterManager>
#include <MailCommon/MailUtil>

#include <Akonadi/Collection>

#include <MailTransport/TransportManager>

#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>

#include "pimdataexportcore_debug.h"

#include "exportresourcearchivejob.h"
#include "resourceconverterimpl.h"
#include <QStandardPaths>

#include <KIdentityManagement/Identity>
#include <KIdentityManagement/IdentityManager>

ExportMailJobInterfaceImpl::ExportMailJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
    , mIdentityManager(KIdentityManagement::IdentityManager::self())
{
}

ExportMailJobInterfaceImpl::~ExportMailJobInterfaceImpl()
{
}

Akonadi::Collection::Id ExportMailJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ExportMailJobInterfaceImpl::convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection)
{
    ResourceConverterImpl converter;
    converter.convertCollectionIdsToRealPath(group, currentKey, prefixCollection);
}

void ExportMailJobInterfaceImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterImpl converter;
    converter.convertCollectionToRealPath(group, currentKey);
}

void ExportMailJobInterfaceImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterImpl converter;
    converter.convertCollectionListToRealPath(group, currentKey);
}

QVector<MailCommon::MailFilter *> ExportMailJobInterfaceImpl::filters()
{
    return MailCommon::FilterManager::instance()->filters();
}

QString ExportMailJobInterfaceImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterImpl converter;
    return converter.convertToFullCollectionPath(collectionValue);
}

QVector<Utils::AkonadiInstanceInfo> ExportMailJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
}

void ExportMailJobInterfaceImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    auto resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(Utils::resourceMailArchiveName());
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportMailJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportMailJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportMailJobInterfaceImpl::slotMailsJobTerminated);
    connect(this, &ExportMailJobInterfaceImpl::taskCanceled, resourceJob, &ExportResourceArchiveJob::slotTaskCanceled);
    resourceJob->start();
}

QString ExportMailJobInterfaceImpl::storeResources(KZip *archive, const QString &identifier, const QString &path)
{
    ResourceConverterImpl converter;
    return converter.storeResources(archive, identifier, path);
}

QString ExportMailJobInterfaceImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(identifier, QStringLiteral("$HOME/.local/share/local-mail/"));
    return url;
}

void ExportMailJobInterfaceImpl::backupMailResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    backupResourceFile(agentIdentifier, defaultPath);
}

QString ExportMailJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString ExportMailJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(name);
    Q_UNUSED(settings);
    Q_UNUSED(synchronizeTree);

    Q_UNREACHABLE();
    return {};
}

void ExportMailJobInterfaceImpl::exportFilters()
{
    const QVector<MailCommon::MailFilter *> lstFilter = filters();
    if (!lstFilter.isEmpty()) {
        QTemporaryFile tmp;
        tmp.open();
        const QUrl url = QUrl::fromLocalFile(tmp.fileName());
        MailCommon::FilterImporterExporter exportFilters;
        exportFilters.exportFilters(lstFilter, url, true);
        tmp.close();
        const bool fileAdded = archive()->addLocalFile(tmp.fileName(), Utils::configsPath() + QStringLiteral("filters"));
        if (fileAdded) {
            Q_EMIT info(i18n("Filters backup done."));
        } else {
            Q_EMIT error(i18n("Filters cannot be exported."));
        }
    }
}

QVector<uint> ExportMailJobInterfaceImpl::listIdentityUoid() const
{
    QVector<uint> listUiod;
    KIdentityManagement::IdentityManager::ConstIterator end = mIdentityManager->end();
    for (KIdentityManagement::IdentityManager::ConstIterator it = mIdentityManager->begin(); it != end; ++it) {
        const uint identityUoid = (*it).uoid();
        listUiod << identityUoid;
    }
    return listUiod;
}
