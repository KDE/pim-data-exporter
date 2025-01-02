/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailjobinterfaceimpl.h"
#include "exportmailfolderattributejobimpl.h"
#include <KIdentityManagementCore/IdentityManager>
#include <MailCommon/FilterImporterExporter>
#include <MailCommon/FilterManager>
#include <MailCommon/MailUtil>

#include <Akonadi/Collection>

#include <MailTransport/TransportManager>

#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>

#include "exportresourcearchivejob.h"
#include "resourceconverterimpl.h"
#include <QStandardPaths>

#include <KIdentityManagementCore/Identity>
#include <KIdentityManagementCore/IdentityManager>

ExportMailJobInterfaceImpl::ExportMailJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
    , mIdentityManager(KIdentityManagementCore::IdentityManager::self())
{
}

ExportMailJobInterfaceImpl::~ExportMailJobInterfaceImpl() = default;

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

QList<MailCommon::MailFilter *> ExportMailJobInterfaceImpl::filters()
{
    return MailCommon::FilterManager::instance()->filters();
}

QString ExportMailJobInterfaceImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterImpl converter;
    return converter.convertToFullCollectionPath(collectionValue);
}

QList<Utils::AkonadiInstanceInfo> ExportMailJobInterfaceImpl::listOfResource()
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
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportMailJobInterfaceImpl::emitError);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportMailJobInterfaceImpl::emitInfo);
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
    const QList<MailCommon::MailFilter *> lstFilter = filters();
    if (!lstFilter.isEmpty()) {
        QTemporaryFile tmp;
        tmp.open();
        const QUrl url = QUrl::fromLocalFile(tmp.fileName());
        MailCommon::FilterImporterExporter mailCommonExportFilters;
        mailCommonExportFilters.exportFilters(lstFilter, url, true);
        tmp.close();
        const bool fileAdded = archive()->addLocalFile(tmp.fileName(), Utils::configsPath() + QStringLiteral("filters"));
        if (fileAdded) {
            emitInfo(i18n("Filters backup done."));
        } else {
            emitError(i18n("Filters cannot be exported."));
        }
    }
}

QList<uint> ExportMailJobInterfaceImpl::listIdentityUoid() const
{
    QList<uint> listUiod;
    KIdentityManagementCore::IdentityManager::ConstIterator end = mIdentityManager->end();
    for (KIdentityManagementCore::IdentityManager::ConstIterator it = mIdentityManager->begin(); it != end; ++it) {
        const uint identityUoid = (*it).uoid();
        listUiod << identityUoid;
    }
    return listUiod;
}

void ExportMailJobInterfaceImpl::exportFolderAttributes()
{
    auto job = new ExportMailFolderAttributeJobImpl(this);
    job->setArchive(archive());
    job->setExportInterface(this);
    connect(job, &ExportMailFolderAttributeJobImpl::successed, this, [this]() {
        emitInfo(i18n("Backing up Folder Attributes done."));
        Q_EMIT exportAttributeDone();
    });
    connect(job, &ExportMailFolderAttributeJobImpl::failed, this, [this]() {
        emitError(i18n("Folder Attributes cannot be exported."));
        Q_EMIT exportAttributeDone();
    });
    job->start();
}

#include "moc_exportmailjobinterfaceimpl.cpp"
