/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTEXPORTPROGRESSINDICATORCONSOLE_H
#define IMPORTEXPORTPROGRESSINDICATORCONSOLE_H

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

#endif // IMPORTEXPORTPROGRESSINDICATORCONSOLE_H
