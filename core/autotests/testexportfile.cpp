/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "testexportfile.h"
#include "archivestorage.h"
#include "compareexportfile.h"
#include "abstractimportexportjob.h"
#include "utils.h"

#include <QDir>
#include <QSignalSpy>
#include <QTest>
//#define REMOVE_CREATED_ZIP_FILE 1

TestExportFile::TestExportFile(QObject *parent)
    : QObject(parent)
{
    mTemporaryFile = QDir::tempPath() + QStringLiteral("/archive.zip");
    mArchiveStorage = new ArchiveStorage(mTemporaryFile, this);
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
    Utils::addVersion(mArchiveStorage->archive());
    Utils::storeDataExportInfo(mArchiveStorage->archive());

    QSignalSpy finish(mAbstractImportExportJob, &AbstractImportExportJob::jobFinished);
    QSignalSpy error(mAbstractImportExportJob, &AbstractImportExportJob::error);
    mAbstractImportExportJob->start();
    QVERIFY(finish.wait());
    const bool hasError = error.count() > 0;
    if (hasError) {
        qDebug() << " error " << error.at(0);
    }
    QVERIFY(!hasError);
    delete mArchiveStorage;
    mArchiveStorage = nullptr;

    CompareExportFile compareExportFile;
    compareExportFile.setTempFilePath(mTemporaryFile);
    compareExportFile.setListFilePath(QLatin1String(mPathConfig));
    compareExportFile.compareFiles();
#ifdef REMOVE_CREATED_ZIP_FILE
    QVERIFY(QFile(mTemporaryFile).remove());
#endif
}

ArchiveStorage *TestExportFile::archiveStorage() const
{
    return mArchiveStorage;
}
