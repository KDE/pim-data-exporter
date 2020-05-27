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

#include "importnotesjobinterfacetest.h"
#include "importnotesjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "testimportfile.h"
#include <QTest>
QTEST_MAIN(ImportNotesJobInterfaceTest)

ImportNotesJobInterfaceTest::ImportNotesJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportNotesJobInterfaceTest::importNote_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    QTest::newRow("test1") << QString::fromLatin1(pathConfig) << QStringLiteral("/test1/") << options;
    options = {Utils::StoredType::Config|Utils::StoredType::Resources};
    QTest::newRow("test1resource") << QString::fromLatin1(pathConfig) << QStringLiteral("/test1resource/") << options;
}

void ImportNotesJobInterfaceTest::importNote()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::notePath());
    ImportNotesJobInterfaceTestImpl *impl = new ImportNotesJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    impl->setPathConfig(file->pathConfig());
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}
