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

#include "importnotesjobinterfaceimpl.h"
#include "archivestorage.h"
#include <MailCommon/MailUtil>
#include <PimCommonAkonadi/CreateResource>

#include <KArchive>
#include <KLocalizedString>

#include <KZip>
#include <KConfigGroup>

#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>
#include "resourceconverterimpl.h"


ImportNotesJobInterfaceImpl::ImportNotesJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
    initializeImportJob();
}

ImportNotesJobInterfaceImpl::~ImportNotesJobInterfaceImpl()
{
}

Akonadi::Collection::Id ImportNotesJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

QString ImportNotesJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

void ImportNotesJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    //It's maildir support. Need to add support
    startSynchronizeResources(lst);
}
