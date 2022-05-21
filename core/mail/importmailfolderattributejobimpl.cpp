/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejobimpl.h"
#include "pimdataexportcore_debug.h"
#include <Akonadi/CollectionModifyJob>
#include <Akonadi/EntityDisplayAttribute>
#include <MailCommon/ExpireCollectionAttribute>

ImportMailFolderAttributeJobImpl::ImportMailFolderAttributeJobImpl(QObject *parent)
    : ImportMailFolderAttributeJob{parent}
{
}

ImportMailFolderAttributeJobImpl::~ImportMailFolderAttributeJobImpl() = default;

void ImportMailFolderAttributeJobImpl::applyAttributes(const QMap<Akonadi::Collection::Id, AttributeInfo> &map)
{
    QMapIterator<Akonadi::Collection::Id, AttributeInfo> indexMap(map);
    while (indexMap.hasNext()) {
        indexMap.next();
        // TODO
    }
    // Call it when all is finished!
    restoreFileFolderAttribute();
}
