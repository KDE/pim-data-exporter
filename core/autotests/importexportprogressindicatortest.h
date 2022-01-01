/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <importexportprogressindicatorbase.h>

class ImportExportProgressIndicatorTest : public ImportExportProgressIndicatorBase
{
public:
    explicit ImportExportProgressIndicatorTest(QObject *parent);

    Q_REQUIRED_RESULT int mergeConfigMessageBox(const QString &configName) const override;
    Q_REQUIRED_RESULT bool overwriteConfigMessageBox(const QString &configName) const override;
    Q_REQUIRED_RESULT bool overwriteDirectoryMessageBox(const QString &directory) const override;
};
