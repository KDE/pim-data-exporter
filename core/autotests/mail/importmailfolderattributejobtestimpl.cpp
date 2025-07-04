/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailfolderattributejobtestimpl.h"
using namespace Qt::Literals::StringLiterals;

#include <QFile>
ImportMailFolderAttributeJobTestImpl::ImportMailFolderAttributeJobTestImpl(QObject *parent)
    : ImportMailFolderAttributeJob{parent}
{
}

ImportMailFolderAttributeJobTestImpl::~ImportMailFolderAttributeJobTestImpl()
{
}

void ImportMailFolderAttributeJobTestImpl::applyAttributes(const QMap<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo> &map)
{
    QMapIterator<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo> indexDisplayMap(map);
    QFile data(mExtractPath + u"/config/mailfolderattributes"_s);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        while (indexDisplayMap.hasNext()) {
            indexDisplayMap.next();
            out << "Collection " << indexDisplayMap.key() << " display " << indexDisplayMap.value().displayAttribute << " expire "
                << indexDisplayMap.value().expireAttribute << " favorite " << indexDisplayMap.value().favoriteAttribute << " folder "
                << indexDisplayMap.value().folderAttribute << "\n";
        }
    }
    restoreFileFolderAttribute();
}

#include "moc_importmailfolderattributejobtestimpl.cpp"
