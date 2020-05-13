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

#include "testimportfile.h"
#include "archivestorage.h"
#include "utils.h"
#include "abstractimportexportjob.h"
#include "compareimportfile.h"

#include <QSignalSpy>
#include <QTest>
//#define REMOVE_TEMPORARY_DIR 1

TestImportFile::TestImportFile(const QString &archivePath, QObject *parent)
    : QObject(parent)
    , mArchivePath(archivePath + QStringLiteral("archive.zip"))
{
    mArchiveStorage = new ArchiveStorage(mArchivePath, this);
    const bool archiveOpened = mArchiveStorage->openArchive(false);
    if (!archiveOpened) {
        qDebug() << " mArchivePath " << mArchivePath;
    }
    Q_ASSERT(archiveOpened);
}

TestImportFile::~TestImportFile()
{
}

void TestImportFile::start()
{
    //Don't use setTestModeEnabled otherwise we can set env
    //QStandardPaths::setTestModeEnabled(true);
    //TODO define extract path
    QVERIFY(!mExtractPath.isEmpty());

    qputenv("XDG_DATA_HOME", mExtractPath.toLatin1() + "/share");
    qputenv("XDG_CONFIG_HOME", mExtractPath.toLatin1() + "/config");
    qDebug() << " mExtractPath.toLatin1()  " << mExtractPath.toLatin1();

    const int version = Utils::archiveVersion(mArchiveStorage->archive());
    QVERIFY(version <= Utils::currentArchiveVersion());
    mAbstractImportExportJob->setArchiveVersion(version);

    QSignalSpy finish(mAbstractImportExportJob, &AbstractImportExportJob::jobFinished);
    QSignalSpy error(mAbstractImportExportJob, &AbstractImportExportJob::error);
    mAbstractImportExportJob->start();
    QVERIFY(finish.wait());
    if (error.count()) {
        qDebug() << error.at(0);
    }
    QCOMPARE(error.count(), 0);
    delete mArchiveStorage;
    mArchiveStorage = nullptr;

    CompareImportFile compareExportFile;
    compareExportFile.setArchiveFilePath(mArchivePath);
    compareExportFile.setListFilePath(mPathConfig);
    compareExportFile.setInstallPath(mExtractPath);
    compareExportFile.setExcludePath(mExcludePath);
    compareExportFile.compareFile();

#ifdef REMOVE_TEMPORARY_DIR
    QVERIFY(QDir(mExtractPath).removeRecursively());
#endif
}

AbstractImportExportJob *TestImportFile::abstractImportExportJob() const
{
    return mAbstractImportExportJob;
}

void TestImportFile::setAbstractImportExportJob(AbstractImportExportJob *abstractImportExportJob)
{
    mAbstractImportExportJob = abstractImportExportJob;
}

ArchiveStorage *TestImportFile::archiveStorage() const
{
    return mArchiveStorage;
}

void TestImportFile::setArchiveStorage(ArchiveStorage *archiveStorage)
{
    mArchiveStorage = archiveStorage;
}

QString TestImportFile::pathConfig() const
{
    return mPathConfig;
}

void TestImportFile::setPathConfig(const QString &pathConfig)
{
    mPathConfig = pathConfig;
}

void TestImportFile::setExtractPath(const QString &extractPath)
{
    mExtractPath = extractPath;
}

QString TestImportFile::excludePath() const
{
    return mExcludePath;
}

void TestImportFile::setExcludePath(const QString &excludePath)
{
    mExcludePath = excludePath;
}

QString TestImportFile::loggingFilePath() const
{
    return mLoggingFilePath;
}

void TestImportFile::setLoggingFilePath(const QString &loggingFilePath)
{
    mLoggingFilePath = loggingFilePath;
}
