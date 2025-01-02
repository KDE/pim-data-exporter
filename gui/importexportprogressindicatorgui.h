/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importexportprogressindicatorbase.h"
class QProgressDialog;
class ImportExportProgressIndicatorGui : public ImportExportProgressIndicatorBase
{
    Q_OBJECT
public:
    explicit ImportExportProgressIndicatorGui(QWidget *parentWidget, QObject *parent = nullptr);
    ~ImportExportProgressIndicatorGui() override;

    void increaseProgressDialog() override;
    void createProgressDialog(const QString &title) override;

    void setProgressDialogLabel(const QString &text) override;
    [[nodiscard]] bool wasCanceled() const override;
    [[nodiscard]] int mergeConfigMessageBox(const QString &configName) const override;
    [[nodiscard]] bool overwriteConfigMessageBox(const QString &configName) const override;
    [[nodiscard]] bool overwriteDirectoryMessageBox(const QString &directory) const override;
    void showErrorMessage(const QString &message, const QString &title) override;

private:
    QProgressDialog *mProgressDialog = nullptr;
    QWidget *const mParentWidget;
};
