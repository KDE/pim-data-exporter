/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTEXPORTPROGRESSINDICATORBASE_H
#define IMPORTEXPORTPROGRESSINDICATORBASE_H

#include <QObject>
#include "pimdataexporter_export.h"
class PIMDATAEXPORTER_EXPORT ImportExportProgressIndicatorBase : public QObject
{
    Q_OBJECT
public:
    explicit ImportExportProgressIndicatorBase(QObject *parent = nullptr);
    ~ImportExportProgressIndicatorBase();

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

#endif // IMPORTEXPORTPROGRESSINDICATORBASE_H
