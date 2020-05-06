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

#include "compareimportfile.h"
#include "generatelistfilefromarchive.h"
#include "loadlistfromfile.h"
#include "comparefilehelper.h"
#include <QProcess>
#include <QTest>

CompareImportFile::CompareImportFile()
{
}

CompareImportFile::~CompareImportFile()
{
}

void CompareImportFile::compareFile()
{
    GenerateListFileFromArchive archive(mArchiveFilePath);
    //qDebug() << " archive " << archive.listFile();

    LoadListFromFile f(mListFilePath + QStringLiteral("/list.txt"));
    const QStringList archiveList = archive.listFile();
    const bool equal = (f.fileList() == archiveList);
    if (!equal) {
        qDebug() << "Requested : " << f.fileList();
        qDebug() << "List File : " << archiveList;
    }
    QVERIFY(equal);
    for (QString file : archiveList) {
        file.replace(QStringLiteral("configs/"), QStringLiteral("config/"));
        if (file == QLatin1String("information/exportdatatype.xml") || file == QLatin1String("information/VERSION_2")) {
            continue;
        }
        CompareFileHelper::compareFile(mListFilePath + QStringLiteral("/references/") + file, mInstallPath + QLatin1Char('/') + file);
    }
}

QString CompareImportFile::archiveFilePath() const
{
    return mArchiveFilePath;
}

void CompareImportFile::setArchiveFilePath(const QString &archiveFilePath)
{
    mArchiveFilePath = archiveFilePath;
}

QString CompareImportFile::listFilePath() const
{
    return mListFilePath;
}

void CompareImportFile::setListFilePath(const QString &listFilePath)
{
    mListFilePath = listFilePath;
}

QString CompareImportFile::installPath() const
{
    return mInstallPath;
}

void CompareImportFile::setInstallPath(const QString &installPath)
{
    mInstallPath = installPath;
}

