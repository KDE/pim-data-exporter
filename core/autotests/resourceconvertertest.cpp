/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "resourceconvertertest.h"
using namespace Qt::Literals::StringLiterals;

#include <QDebug>
#include <QDir>

ResourceConverterTest::ResourceConverterTest() = default;

ResourceConverterTest::~ResourceConverterTest() = default;

QString ResourceConverterTest::prefixStr() const
{
    return u"foo/bla bla/bli"_s;
}

QString ResourceConverterTest::convertToFullCollectionPath(const qlonglong collectionValue)
{
    return prefixStr() + QString::number(collectionValue);
}

Akonadi::Collection::Id ResourceConverterTest::convertFolderPathToCollectionId(const QString &path)
{
    QString str = path;
    str.remove(prefixStr());
    auto result = str.toLongLong();
    // Make sure that we use another number.
    if (result != -1) {
        result += 10;
    }
    return result;
}

QString ResourceConverterTest::changeResourcePath(QString url) const
{
    // qDebug() << " need to fix " << url << " mTestPath " << mTestPath;
    if (!url.isEmpty()) {
        url.replace(QLatin1StringView("[TEST_PATH]"), mTestPath);
    }
    // qDebug() <<" FIXED " << url;
    return url;
}

void ResourceConverterTest::setTestPath(const QString &testPath)
{
    mTestPath = testPath;
}

// TODO use another one ?
QString ResourceConverterTest::installDefaultDirectory()
{
    return QDir::tempPath();
}

QString ResourceConverterTest::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    QString newUrl;
    if (overwriteResources) {
        newUrl = resourcePath(resourceConfig);
    } else {
        newUrl = adaptResourcePath(resourceConfig, storePath);
    }
    return newUrl;
}
