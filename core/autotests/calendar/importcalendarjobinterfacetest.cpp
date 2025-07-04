/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importcalendarjobinterfacetest.h"
using namespace Qt::Literals::StringLiterals;

#include "archivestorage.h"
#include "importcalendarjobinterfacetestimpl.h"
#include "testimportfile.h"
#include <QTest>
QTEST_MAIN(ImportCalendarJobInterfaceTest)

ImportCalendarJobInterfaceTest::ImportCalendarJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportCalendarJobInterfaceTest::importCalendar_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    Utils::StoredTypes options = {Utils::StoredType::Config};

    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("calendaronlyconfig") << QString::fromLatin1(pathConfig) << u"/calendaronlyconfig/"_s << options;

    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("calendarconfigandresource") << QString::fromLatin1(pathConfig) << u"/calendarconfigandresource/"_s << options;

    options = {Utils::StoredType::Config | Utils::StoredType::Resources};
    QTest::newRow("calendarconfigandresourceskalendar") << QString::fromLatin1(pathConfig) << u"/calendarconfigandresourceskalendar/"_s << options;
}

void ImportCalendarJobInterfaceTest::importCalendar()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    const QString fullTestPath = zipFilePath + testPath;

    auto file = new TestImportFile(fullTestPath, this);
    file->setPathConfig(fullTestPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::calendarPath());
    auto impl = new ImportCalendarJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    impl->setPathConfig(file->pathConfig());
    impl->setExtractPath(file->extractPath());
    impl->setTempDirName(file->extractPath());
    impl->setExistingPathConfig(fullTestPath + u"/existingconfig/"_s);
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}

#include "moc_importcalendarjobinterfacetest.cpp"
