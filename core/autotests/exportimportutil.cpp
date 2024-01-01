/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportimportutil.h"

ExportImportUtil::ExportImportUtil()
{
    mLogCreateResource = new LogCreatingResource(nullptr);
}

ExportImportUtil::~ExportImportUtil()
{
    delete mLogCreateResource;
}

void ExportImportUtil::setListOfResource(const QList<Utils::AkonadiInstanceInfo> &instanceInfoList)
{
    mListAkonadiInstanceInfo = instanceInfoList;
}

void ExportImportUtil::setPathConfig(const QString &pathConfig)
{
    mPathConfig = pathConfig;
}

void ExportImportUtil::addLogCreateResource(const QString &str)
{
    mLogCreateResource->appendText(str);
}

QString ExportImportUtil::loggingFilePath() const
{
    return mLogCreateResource->logPath();
}

void ExportImportUtil::setExtractPath(const QString &extractPath)
{
    mExtractPath = extractPath;
}

QString ExportImportUtil::extractPath() const
{
    return mExtractPath;
}
