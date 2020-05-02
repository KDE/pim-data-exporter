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
#include <KZip>

//#define KEEP_TEMPORARY_FILES 1

CompareExportFile::CompareExportFile()
{
}

CompareExportFile::~CompareExportFile()
{
    delete mZip;
    mZip = nullptr;
    delete mTempDir;
    mTempDir = nullptr;
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
    mZip = new KZip(mTempFilePath);
    //qDebug() << " mFileName" << mFileName;
    const bool result = mZip->open(QIODevice::ReadOnly);
    QVERIFY(result);
    const KArchiveDirectory *topDirectory = mZip->directory();
    for (const QString &file : archiveList) {
        if (file == QLatin1String("information/VERSION_2") || file == QLatin1String("information/exportdatatype.xml")) {
            continue;
        }
        const KArchiveEntry *currentEntry = topDirectory->entry(file);
        if (currentEntry && currentEntry->isFile()) {
            if (!mTempDir) {
                mTempDir = new QTemporaryDir;
#ifdef KEEP_TEMPORARY_FILES
                mTempDir->setAutoRemove(false);
#endif
            }
            const KArchiveFile *currentFile = static_cast<const KArchiveFile *>(currentEntry);

            QString adaptFile = file;
            //We store in zip as configs, but we extract in config/
            adaptFile.replace(QStringLiteral("configs/"), QStringLiteral("config/"));
            const QString fileName = mTempDir->path() + QLatin1Char('/') + adaptFile;
            //create path
            const QFileInfo fileInfo(fileName);
            QDir().mkpath(fileInfo.dir().path());

            QFile f(fileName);
            //qDebug() << " fileName" << fileName;
            QVERIFY(f.open(QIODevice::WriteOnly));

            //Store data.
            const QByteArray data = currentFile->data();
            QCOMPARE(f.write(data), data.length());
            f.close();

            compareFile(mListFilePath + QStringLiteral("/references/") + adaptFile, fileName);
        }
    }
}

void CompareExportFile::compareFile(const QString &referenceFile, const QString &archiveFile)
{
    //qDebug() << "referenceFile " << referenceFile << " archiveFile " << archiveFile;
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
