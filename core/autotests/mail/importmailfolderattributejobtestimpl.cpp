/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejobtestimpl.h"
#include <QDebug>
ImportMailFolderAttributeJobTestImpl::ImportMailFolderAttributeJobTestImpl(QObject *parent)
    : ImportMailFolderAttributeJob{parent}
{
}

ImportMailFolderAttributeJobTestImpl::~ImportMailFolderAttributeJobTestImpl()
{
}

void ImportMailFolderAttributeJobTestImpl::applyAttributes(const QMap<Akonadi::Collection::Id, AttributeInfo> &)
{
    // TODO
    restoreFileFolderAttribute();
}
