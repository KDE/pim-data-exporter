/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "logcreatingressource.h"
#include "utils.h"
#include <QVector>

class ExportImportUtil
{
public:
    ExportImportUtil();
    ~ExportImportUtil();
    void setListOfResource(const QVector<Utils::AkonadiInstanceInfo> &instanceInfoList);
    void setPathConfig(const QString &pathConfig);
    void addLogCreateResource(const QString &str);
    Q_REQUIRED_RESULT QString loggingFilePath() const;

    QVector<Utils::AkonadiInstanceInfo> mListAkonadiInstanceInfo;
    QString mPathConfig;
    void setExtractPath(const QString &extractPath);

    Q_REQUIRED_RESULT QString extractPath() const;

protected:
    LogCreatingResource *mLogCreateResource = nullptr;

private:
    QString mExtractPath;
};
