/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "resourceconverterimpl.h"
#include <MailCommon/MailUtil>
#include <QDir>

ResourceConverterImpl::ResourceConverterImpl()
{
}

ResourceConverterImpl::~ResourceConverterImpl()
{
}

QString ResourceConverterImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    return MailCommon::Util::fullCollectionPath(Akonadi::Collection(collectionValue));
}

Akonadi::Collection::Id ResourceConverterImpl::convertFolderPathToCollectionId(const QString &path)
{
    return MailCommon::Util::convertFolderPathToCollectionId(path);
}

QString ResourceConverterImpl::installDefaultDirectory()
{
    return QDir::homePath();
}

QString ResourceConverterImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    QString newUrl;
    if (overwriteResources) {
        // TODO fix me too
        newUrl = resourcePath(resourceConfig);
    } else {
        newUrl = adaptResourcePath(resourceConfig, storePath);
    }
    return newUrl;
}
