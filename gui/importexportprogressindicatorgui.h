/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTEXPORTPROGRESSINDICATORGUI_H
#define IMPORTEXPORTPROGRESSINDICATORGUI_H

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
    Q_REQUIRED_RESULT bool wasCanceled() const override;
    Q_REQUIRED_RESULT int mergeConfigMessageBox(const QString &configName) const override;
    Q_REQUIRED_RESULT bool overwriteConfigMessageBox(const QString &configName) const override;
    Q_REQUIRED_RESULT bool overwriteDirectoryMessageBox(const QString &directory) const override;
    void showErrorMessage(const QString &message, const QString &title) override;

private:
    QProgressDialog *mProgressDialog = nullptr;
    QWidget *mParentWidget = nullptr;
};

#endif // IMPORTEXPORTPROGRESSINDICATORGUI_H
