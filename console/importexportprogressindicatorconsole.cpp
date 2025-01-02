/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importexportprogressindicatorconsole.h"
#include <QDebug>

ImportExportProgressIndicatorConsole::ImportExportProgressIndicatorConsole(QObject *parent)
    : ImportExportProgressIndicatorBase(parent)
{
}

ImportExportProgressIndicatorConsole::~ImportExportProgressIndicatorConsole() = default;

void ImportExportProgressIndicatorConsole::setProgressDialogLabel(const QString &text)
{
    Q_EMIT info(text);
}

void ImportExportProgressIndicatorConsole::showErrorMessage(const QString &message, const QString &title)
{
    qDebug() << " message " << message << " title " << title;
}

#include "moc_importexportprogressindicatorconsole.cpp"
