/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "resourceconverterimpl.h"
#include <MailCommon/MailUtil>

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

QString ResourceConverterImpl::resourcePath(const QString &agentIdentifier, const QString &defaultPath)
{
    const QString agentFileName = agentIdentifier + QStringLiteral("rc");
    const QString configFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + agentFileName;
    qDebug() << "configFileName " << configFileName;

    KSharedConfigPtr resourceConfig = KSharedConfig::openConfig(configFileName);
    const QString url = resourcePath(resourceConfig, defaultPath);
    qDebug() << " resourcePath " << url;
    return url;
}

QString ResourceConverterImpl::resourcePath(const KSharedConfigPtr &resourceConfig, const QString &defaultPath)
{
    QString url = ResourceConverterBase::resourcePath(resourceConfig, defaultPath);
    //TODO adapt path for test
    return url;
}
