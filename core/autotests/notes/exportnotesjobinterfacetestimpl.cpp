/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "exportnotesjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "saveresourceconfigtest.h"
#include <KZip>

ExportNotesJobInterfaceTestImpl::ExportNotesJobInterfaceTestImpl(QObject *parent,
                                                                 Utils::StoredTypes typeSelected,
                                                                 ArchiveStorage *archiveStorage,
                                                                 int numberOfStep)
    : ExportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportNotesJobInterfaceTestImpl::~ExportNotesJobInterfaceTestImpl() = default;

void ExportNotesJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    resourceConverterTest.convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
}

QList<Utils::AkonadiInstanceInfo> ExportNotesJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

void ExportNotesJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResourceConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath, url, identifier, Utils::resourceNoteArchiveName(), {QLatin1String("akonadi_akonotes_resource_")});
    slotNoteJobTerminated();
}

Akonadi::Collection::Id ExportNotesJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ExportNotesJobInterfaceTestImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(identifier);
    return url;
}

QString ExportNotesJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString
ExportNotesJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(settings);
    Q_UNUSED(name);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}
