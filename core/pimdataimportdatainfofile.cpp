/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataimportdatainfofile.h"
using namespace Qt::Literals::StringLiterals;

#include "pimdataexportcore_debug.h"
#include "utils.h"
#include <KZip>
#include <QTemporaryDir>

PimDataImportDataInfoFile::PimDataImportDataInfoFile(QObject *parent)
    : QObject(parent)
{
}

PimDataImportDataInfoFile::~PimDataImportDataInfoFile()
{
    delete mTempDir;
}

void PimDataImportDataInfoFile::setCurrentFileName(const QString &filename)
{
    mFilename = filename;
}

QString PimDataImportDataInfoFile::importDataInfoPath()
{
    QString temporaryFilePath;
    if (mFilename.isEmpty()) {
        return temporaryFilePath;
    }
    mTempDir = new QTemporaryDir;
    QString errorMsg;
    KZip *archive = Utils::openZip(mFilename, errorMsg);
    if (!archive) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "unable to open zip file " << errorMsg;
    } else {
        const KArchiveEntry *informationFile = archive->directory()->entry(Utils::infoPath() + Utils::exportDataTypeFileName());
        if (informationFile && informationFile->isFile()) {
            const auto file = static_cast<const KArchiveFile *>(informationFile);
            if (file->size() == 0) {
                qCWarning(PIMDATAEXPORTERCORE_LOG) << "Empty exporteddata information file, skipping it";
            } else {
                temporaryFilePath = mTempDir->path() + u'/' + Utils::exportDataTypeFileName();
                if (file->copyTo(mTempDir->path())) {
                    temporaryFilePath = mTempDir->path() + u'/' + Utils::exportDataTypeFileName();
                } else {
                    qCWarning(PIMDATAEXPORTERCORE_LOG) << "Impossible to copy to temporary file" << temporaryFilePath;
                }
            }
        } else {
            qCWarning(PIMDATAEXPORTERCORE_LOG) << "Old archive without exporteddata information";
        }
    }
    delete archive;
    return temporaryFilePath;
}

#include "moc_pimdataimportdatainfofile.cpp"
