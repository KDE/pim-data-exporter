/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
        lstAttributeInfo.insert(mInterface->convertToFullCollectionPath(i), att);
    }
    storeFileFolderAttribute(lstAttributeInfo);
}
