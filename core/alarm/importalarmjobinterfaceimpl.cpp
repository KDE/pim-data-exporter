/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

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

#include "importalarmjobinterfaceimpl.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>
#include <MailCommon/MailUtil>

#include <KArchive>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KZip>
#include <KArchiveEntry>

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>
#include "resourceconverterimpl.h"



ImportAlarmJobInterfaceImpl::ImportAlarmJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAlarmJobInterfaceImpl::~ImportAlarmJobInterfaceImpl()
{
}


Akonadi::Collection::Id ImportAlarmJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}


QString ImportAlarmJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

void ImportAlarmJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    startSynchronizeResources(lst);
}
