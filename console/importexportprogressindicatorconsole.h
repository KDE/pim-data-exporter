/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importexportprogressindicatorbase.h"

class ImportExportProgressIndicatorConsole : public ImportExportProgressIndicatorBase
{
    Q_OBJECT
public:
    explicit ImportExportProgressIndicatorConsole(QObject *parent = nullptr);
    ~ImportExportProgressIndicatorConsole() override;

private:
    void setProgressDialogLabel(const QString &text) override;
    void showErrorMessage(const QString &message, const QString &title) override;
};
