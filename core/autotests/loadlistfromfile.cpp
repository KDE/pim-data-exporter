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

#include "loadlistfromfile.h"

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
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    mFileList = contextStr.split(QLatin1Char('\n'), QString::SkipEmptyParts);
#else
    mFileList = contextStr.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
#endif
    mFileList.sort();
}

LoadListFromFile::~LoadListFromFile()
{
}

QStringList LoadListFromFile::fileList() const
{
    return mFileList;
}
