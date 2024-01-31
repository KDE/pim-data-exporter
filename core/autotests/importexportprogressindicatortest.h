/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importexportprogressindicatorbase.h"

class ImportExportProgressIndicatorTest : public ImportExportProgressIndicatorBase
{
public:
    explicit ImportExportProgressIndicatorTest(QObject *parent);

    [[nodiscard]] int mergeConfigMessageBox(const QString &configName) const override;
    [[nodiscard]] bool overwriteConfigMessageBox(const QString &configName) const override;
    [[nodiscard]] bool overwriteDirectoryMessageBox(const QString &directory) const override;
};
