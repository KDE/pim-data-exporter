/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

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
    QFile data(mExtractPath + u"/config/mailfolderattributes"_s);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        for (const auto &[key, value] : map.asKeyValueRange()) {
            out << "Collection " << key << " display " << value.displayAttribute << " expire " << value.expireAttribute << " favorite "
                << value.favoriteAttribute << " folder " << value.folderAttribute << "\n";
        }
    }
    restoreFileFolderAttribute();
}

#include "moc_importmailfolderattributejobtestimpl.cpp"
