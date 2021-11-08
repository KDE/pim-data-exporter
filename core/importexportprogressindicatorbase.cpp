/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importexportprogressindicatorbase.h"
#include <KMessageBox>
#include <QDebug>

ImportExportProgressIndicatorBase::ImportExportProgressIndicatorBase(QObject *parent)
    : QObject(parent)
{
}

ImportExportProgressIndicatorBase::~ImportExportProgressIndicatorBase()
{
}

void ImportExportProgressIndicatorBase::increaseProgressDialog()
{
    // Nothing
}

void ImportExportProgressIndicatorBase::createProgressDialog(const QString &title)
{
    // Nothing
    Q_UNUSED(title)
}

void ImportExportProgressIndicatorBase::setProgressDialogLabel(const QString &text)
{
    Q_EMIT info(text);
}

bool ImportExportProgressIndicatorBase::wasCanceled() const
{
    return false;
}

void ImportExportProgressIndicatorBase::setNumberOfStep(int numberOfStep)
{
    mNumberOfStep = numberOfStep;
}

int ImportExportProgressIndicatorBase::mergeConfigMessageBox(const QString &configName) const
{
    Q_UNUSED(configName)
    return KMessageBox::Yes;
}

bool ImportExportProgressIndicatorBase::overwriteConfigMessageBox(const QString &configName) const
{
    Q_UNUSED(configName)
    return true;
}

bool ImportExportProgressIndicatorBase::overwriteDirectoryMessageBox(const QString &directory) const
{
    Q_UNUSED(directory)
    return true;
}

void ImportExportProgressIndicatorBase::showErrorMessage(const QString &message, const QString &title)
{
    qDebug() << " message : " << message << " title " << title;
}
