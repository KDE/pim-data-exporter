/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATAIMPORTDATAINFOFILE_H
#define PIMDATAIMPORTDATAINFOFILE_H

#include <QObject>
#include <QString>
#include "pimdataexporter_export.h"
class QTemporaryDir;
class PIMDATAEXPORTER_EXPORT PimDataImportDataInfoFile : public QObject
{
    Q_OBJECT
public:
    explicit PimDataImportDataInfoFile(QObject *parent = nullptr);
    ~PimDataImportDataInfoFile();

    void setCurrentFileName(const QString &filename);

    Q_REQUIRED_RESULT QString importDataInfoPath();
private:
    QTemporaryDir *mTempDir = nullptr;
    QString mFilename;
};

#endif // PIMDATAIMPORTDATAINFOFILE_H
