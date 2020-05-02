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


#include "testexportfile.h"
#include "archivestorage.h"
#include "compareexportfile.h"
#include "abstractimportexportjob.h"
#include "utils.h"

#include <QDir>
#include <QSignalSpy>
#include <QTest>

TestExportFile::TestExportFile(QObject *parent)
    : QObject(parent)
{
    const QString temporaryFile = QDir::tempPath() + QStringLiteral("/archive.zip");
    mArchiveStorage = new ArchiveStorage(temporaryFile, this);
    Q_ASSERT(mArchiveStorage->openArchive(true));
}

TestExportFile::~TestExportFile()
{
}

QByteArray TestExportFile::pathConfig() const
{
    return mPathConfig;
}

void TestExportFile::setPathConfig(const QByteArray &pathConfig)
{
    mPathConfig = pathConfig;
}

AbstractImportExportJob *TestExportFile::abstractImportExportJob() const
{
    return mAbstractImportExportJob;
}

void TestExportFile::setAbstractImportExportJob(AbstractImportExportJob *abstractImportExportJob)
{
    mAbstractImportExportJob = abstractImportExportJob;
}

void TestExportFile::start()
{
    //Don't use setTestModeEnabled otherwise we can set env
    //QStandardPaths::setTestModeEnabled(true);
    qputenv("XDG_DATA_HOME", mPathConfig + "/share");
    qputenv("XDG_CONFIG_HOME", mPathConfig + "/config");

    //TODO fix file name.
    const QString temporaryFile = QDir::tempPath() + QStringLiteral("/archive.zip");
    Utils::addVersion(mArchiveStorage->archive());
    //qDebug() << " temporaryFile " << temporaryFile;
    Utils::storeDataExportInfo(mArchiveStorage->archive());

    QSignalSpy finish(mAbstractImportExportJob, &AbstractImportExportJob::jobFinished);
    QSignalSpy error(mAbstractImportExportJob, &AbstractImportExportJob::error);
    mAbstractImportExportJob->start();
    QVERIFY(finish.wait());
    QCOMPARE(error.count(), 0);
    delete mArchiveStorage;
    mArchiveStorage = nullptr;

    CompareExportFile compareExportFile;
    compareExportFile.setTempFilePath(temporaryFile);
    compareExportFile.setListFilePath(QLatin1String(mPathConfig));
    compareExportFile.compareFiles();
    QVERIFY(QFile(temporaryFile).remove());
}

ArchiveStorage *TestExportFile::archiveStorage() const
{
    return mArchiveStorage;
}
