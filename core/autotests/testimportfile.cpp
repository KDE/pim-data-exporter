/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "testimportfile.h"
using namespace Qt::Literals::StringLiterals;

#include "abstractimportexportjob.h"
#include "archivestorage.h"
#include "compareimportfile.h"
#include "compareloggingfile.h"
#include "utils.h"

#include <QSignalSpy>
#include <QTest>
#define REMOVE_TEMPORARY_DIR 1

TestImportFile::TestImportFile(const QString &archivePath, QObject *parent)
    : QObject(parent)
    , mArchivePath(archivePath + u"archive.zip"_s)
{
    mArchiveStorage = new ArchiveStorage(mArchivePath, this);
    const bool archiveOpened = mArchiveStorage->openArchive(false);
    if (!archiveOpened) {
        qDebug() << " mArchivePath can't be opened :" << mArchivePath;
    }
    Q_ASSERT(archiveOpened);
}

TestImportFile::~TestImportFile() = default;

void TestImportFile::start()
{
    // Don't use setTestModeEnabled otherwise we can set env
    // QStandardPaths::setTestModeEnabled(true);
    // TODO define extract path
    QVERIFY(!mExtractPath.isEmpty());

    qputenv("XDG_DATA_HOME", QByteArray(mExtractPath.toLatin1() + "/share"));
    qputenv("XDG_CONFIG_HOME", QByteArray(mExtractPath.toLatin1() + "/config"));

    const int version = Utils::archiveVersion(mArchiveStorage->archive());
    QVERIFY(version <= Utils::currentArchiveVersion());
    mAbstractImportExportJob->setArchiveVersion(version);

    QSignalSpy finish(mAbstractImportExportJob, &AbstractImportExportJob::jobFinished);
    QSignalSpy error(mAbstractImportExportJob, &AbstractImportExportJob::error);
    mAbstractImportExportJob->start();
    QVERIFY(finish.wait());
    delete mArchiveStorage;
    mArchiveStorage = nullptr;

    CompareImportFile compareExportFile;
    compareExportFile.setArchiveFilePath(mArchivePath);
    compareExportFile.setListFilePath(mPathConfig);
    compareExportFile.setInstallPath(mExtractPath);
    compareExportFile.setExcludePath(mExcludePath);
    compareExportFile.compareFile();

    CompareLoggingFile file;
    file.setLoggingFilePath(mLoggingFilePath);
    file.setListFilePath(mPathConfig);
    file.compare();

    if (!error.isEmpty()) {
        qDebug() << error.at(0);
    }
    QCOMPARE(error.count(), 0);

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

QString TestImportFile::extractPath() const
{
    return mExtractPath;
}

#include "moc_testimportfile.cpp"
