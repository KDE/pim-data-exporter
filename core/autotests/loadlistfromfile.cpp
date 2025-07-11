/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "loadlistfromfile.h"
using namespace Qt::Literals::StringLiterals;

#include <QFile>
#include <QTest>

LoadListFromFile::LoadListFromFile(const QString &filename)
{
    QFile f(filename);
    const bool openFile = f.open(QIODevice::ReadOnly);
    if (!openFile) {
        qDebug() << " Unable to open filename " << filename;
    }
    QVERIFY(openFile);
    const QByteArray content = f.readAll();
    f.close();
    const QString contextStr = QString::fromUtf8(content);
    mFileList = contextStr.split(u'\n', Qt::SkipEmptyParts);
    mFileList.sort();
}

LoadListFromFile::~LoadListFromFile() = default;

QStringList LoadListFromFile::fileList() const
{
    return mFileList;
}
