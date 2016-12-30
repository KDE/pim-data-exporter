/*
   Copyright (C) 2016 Montel Laurent <montel@kde.org>

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


#include "pimsettingimportdatainfofile.h"
#include "pimsettingexportcore_debug.h"
#include "utils.h"
#include <KZip>

PimSettingImportDataInfoFile::PimSettingImportDataInfoFile(QObject *parent)
    : QObject(parent)
{

}

PimSettingImportDataInfoFile::~PimSettingImportDataInfoFile()
{

}

void PimSettingImportDataInfoFile::setCurrentFileName(const QString &filename)
{
    mFilename = filename;
}

QString PimSettingImportDataInfoFile::importDataInfoPath()
{
    QString temporaryFilePath;
    if (mFilename.isEmpty()) {
        return temporaryFilePath;
    }
    mTempFile.open();
    const QString tmpFileName = mTempFile.fileName();
    QString errorMsg;
    KZip *archive = Utils::openZip(mFilename, errorMsg);
    if (!archive) {
        qCWarning(PIMSETTINGEXPORTERCORE_LOG) << "unable to open zip file " << errorMsg;
    } else {
        const KArchiveEntry *informationFile = archive->directory()->entry(Utils::infoPath() + Utils::exportDataTypeFileName());
        if (informationFile && informationFile->isFile()) {
            const KArchiveFile *file = static_cast<const KArchiveFile *>(informationFile);
            if (file->copyTo(tmpFileName)) {
                temporaryFilePath = tmpFileName;
            } else {
                qCWarning(PIMSETTINGEXPORTERCORE_LOG) << "Impossible to copy to temporary file" << tmpFileName;
            }
        }
    }
    delete archive;
    return tmpFileName;
}
