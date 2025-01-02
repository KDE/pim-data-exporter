/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importexportprogressindicatortest.h"
#include <KMessageBox>

ImportExportProgressIndicatorTest::ImportExportProgressIndicatorTest(QObject *parent)
    : ImportExportProgressIndicatorBase(parent)
{
}

int ImportExportProgressIndicatorTest::mergeConfigMessageBox(const QString &configName) const
{
    Q_UNUSED(configName)
    return KMessageBox::ButtonCode::PrimaryAction;
}

bool ImportExportProgressIndicatorTest::overwriteConfigMessageBox(const QString &configName) const
{
    Q_UNUSED(configName)
    return true;
}

bool ImportExportProgressIndicatorTest::overwriteDirectoryMessageBox(const QString &directory) const
{
    Q_UNUSED(directory)
    return true;
}
