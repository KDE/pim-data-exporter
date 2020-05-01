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

#include "compareexportfile.h"
#include "generatelistfilefromarchive.h"
#include "loadlistfromfile.h"
#include <QTest>
#include <QDebug>
#include <QProcess>

CompareExportFile::CompareExportFile()
{
}

CompareExportFile::~CompareExportFile()
{
}

void CompareExportFile::compareFiles()
{
    GenerateListFileFromArchive archive(mTempFilePath);
    //qDebug() << " archive " << archive.listFile();

    LoadListFromFile f(mListFilePath + QStringLiteral("/list.txt"));
    const QStringList archiveList = archive.listFile();
    const bool equal = (f.fileList() == archiveList);
    if (!equal) {
        qDebug() << "Requested : " << f.fileList();
        qDebug() << "List File : " << archiveList;
    }
    QVERIFY(equal);
    for (const QString &file : archiveList) {
        qDebug() << " file " << file;
    }
    //TODO compare files
}

void CompareExportFile::compareFile(const QString &referenceFile, const QString &archiveFile)
{
    QProcess proc;
    const QStringList args = QStringList()
                             << QStringLiteral("-u")
                             << referenceFile
                             << archiveFile;
    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
}

QString CompareExportFile::tempFilePath() const
{
    return mTempFilePath;
}

void CompareExportFile::setTempFilePath(const QString &tempFilePath)
{
    mTempFilePath = tempFilePath;
}

QString CompareExportFile::listFilePath() const
{
    return mListFilePath;
}

void CompareExportFile::setListFilePath(const QString &listFilePath)
{
    mListFilePath = listFilePath;
}
