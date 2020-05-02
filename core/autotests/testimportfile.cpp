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

#include "abstractimportexportjob.h"
#include "compareexportfile.h"

#include <QSignalSpy>
#include <QTest>

TestImportFile::TestImportFile(const QString &archivePath, QObject *parent)
    : QObject(parent)
    , mArchivePath(archivePath)
{
    mArchiveStorage = new ArchiveStorage(archivePath, this);
    Q_ASSERT(mArchiveStorage->openArchive(false));
}

TestImportFile::~TestImportFile()
{

}

void TestImportFile::start()
{
    QSignalSpy finish(mAbstractImportExportJob, &AbstractImportExportJob::jobFinished);
    QSignalSpy error(mAbstractImportExportJob, &AbstractImportExportJob::error);
    mAbstractImportExportJob->start();
    QVERIFY(finish.wait());
    QCOMPARE(error.count(), 0);
    delete mArchiveStorage;
    mArchiveStorage = nullptr;

    CompareExportFile compareExportFile;
    compareExportFile.setTempFilePath(mArchivePath);
    compareExportFile.setListFilePath(mPathConfig);
    compareExportFile.compareFiles();
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
