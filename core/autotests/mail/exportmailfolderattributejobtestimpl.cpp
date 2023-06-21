/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejobtestimpl.h"
#include "mail/exportmailjobinterface.h"
#include <QMap>

ExportMailFolderAttributeJobTestImpl::ExportMailFolderAttributeJobTestImpl(QObject *parent)
    : ExportMailFolderAttributeJob{parent}
{
}

void ExportMailFolderAttributeJobTestImpl::fetchAttributes()
{
    QMap<QString, ImportExportMailUtil::AttributeInfo> lstAttributeInfo;
    for (int i = 0; i < 10; ++i) {
        ImportExportMailUtil::AttributeInfo att;
        att.displayAttribute = QByteArrayLiteral("foo") + QByteArray::number(i);
        att.expireAttribute = QByteArrayLiteral("bla") + QByteArray::number(i);
        att.favoriteAttribute = QByteArrayLiteral("blu") + QByteArray::number(i);
        att.folderAttribute = QByteArrayLiteral("bli") + QByteArray::number(i);
        lstAttributeInfo.insert(mInterface->convertToFullCollectionPath(i), att);
    }
    // Only folderAttribute
    {
        const int index = 12;
        ImportExportMailUtil::AttributeInfo att;
        att.folderAttribute = QByteArrayLiteral("kul") + QByteArray::number(index);
        lstAttributeInfo.insert(mInterface->convertToFullCollectionPath(index), att);
    }
    // Only favoriteAttribute
    {
        const int index = 13;
        ImportExportMailUtil::AttributeInfo att;
        att.favoriteAttribute = QByteArrayLiteral("klo") + QByteArray::number(index);
        lstAttributeInfo.insert(mInterface->convertToFullCollectionPath(index), att);
    }
    // Only display
    {
        const int index = 14;
        ImportExportMailUtil::AttributeInfo att;
        att.displayAttribute = QByteArrayLiteral("osd") + QByteArray::number(index);
        lstAttributeInfo.insert(mInterface->convertToFullCollectionPath(index), att);
    }
    // Only expire
    {
        const int index = 15;
        ImportExportMailUtil::AttributeInfo att;
        att.expireAttribute = QByteArrayLiteral("plo") + QByteArray::number(index);
        lstAttributeInfo.insert(mInterface->convertToFullCollectionPath(index), att);
    }

    storeFileFolderAttribute(lstAttributeInfo);
}

#include "moc_exportmailfolderattributejobtestimpl.cpp"
