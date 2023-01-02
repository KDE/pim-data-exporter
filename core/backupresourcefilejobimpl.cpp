/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "backupresourcefilejobimpl.h"
#include "resourceconverterimpl.h"

BackupResourceFileJobImpl::BackupResourceFileJobImpl(QObject *parent)
    : BackupResourceFileJobBase(parent)
{
}

BackupResourceFileJobImpl::~BackupResourceFileJobImpl() = default;

QString BackupResourceFileJobImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(identifier);
    return url;
}
