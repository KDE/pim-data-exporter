/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include <QObject>
class PIMDATAEXPORTER_EXPORT ImportExportProgressIndicatorBase : public QObject
{
    Q_OBJECT
public:
    explicit ImportExportProgressIndicatorBase(QObject *parent = nullptr);
    ~ImportExportProgressIndicatorBase() override;

    virtual void increaseProgressDialog();
    virtual void createProgressDialog(const QString &title);

    virtual void setProgressDialogLabel(const QString &text);
    virtual bool wasCanceled() const;
    virtual int mergeConfigMessageBox(const QString &configName) const;
    virtual bool overwriteConfigMessageBox(const QString &configName) const;
    virtual bool overwriteDirectoryMessageBox(const QString &directory) const;
    virtual void showErrorMessage(const QString &message, const QString &title);

    void setNumberOfStep(int numberOfStep);

Q_SIGNALS:
    void info(const QString &text);
    void canceled();

protected:
    int mNumberOfStep = 0;
};
