/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportmailfolderattributejobtestimpl.h"

#include <QMap>

ExportMailFolderAttributeJobTestImpl::ExportMailFolderAttributeJobTestImpl(QObject *parent)
    : ExportMailFolderAttributeJob{parent}
{
}

void ExportMailFolderAttributeJobTestImpl::fetchAttributes()
{
    QMap<QString, AttributeInfo> lstAttributeInfo;
    for (int i = 0; i < 10; ++i) {
        AttributeInfo att;
        att.displayAttribute = QByteArrayLiteral("foo") + QByteArray::number(i);
        att.expireAttribute = QByteArrayLiteral("bla") + QByteArray::number(i);
        lstAttributeInfo.insert(QString::number(i), att);
    }
    storeFileFolderAttribute(lstAttributeInfo);
}
