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
#include "exportnotesjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "saveresourceconfigtest.h"
#include <KZip>
#include <QDebug>
#include <QTest>

ExportNotesJobInterfaceTestImpl::ExportNotesJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportNotesJobInterfaceTestImpl::~ExportNotesJobInterfaceTestImpl()
{
}

void ExportNotesJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    resourceConverterTest.convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
}

QVector<Utils::AkonadiInstanceInfo> ExportNotesJobInterfaceTestImpl::listOfResource()
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

QString ExportNotesJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNREACHABLE();
    return {};
}
