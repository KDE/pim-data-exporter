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

#include "importcalendarjobinterfaceimpl.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>

#include <KLocalizedString>
#include <KConfigGroup>
#include <KArchiveDirectory>
#include <KArchiveEntry>
#include <KArchive>
#include <KZip>

#include "pimdataexportcore_debug.h"

#include <MailCommon/MailUtil>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QColor>
#include "resourceconverterimpl.h"

ImportCalendarJobInterfaceImpl::ImportCalendarJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportCalendarJobInterfaceImpl::~ImportCalendarJobInterfaceImpl()
{
}

Akonadi::Collection::Id ImportCalendarJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

QString ImportCalendarJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mCreateResource->createResource(resources, name, settings, synchronizeTree);
}

void ImportCalendarJobInterfaceImpl::synchronizeResource(const QStringList &lst)
{
    startSynchronizeResources(lst);
}
