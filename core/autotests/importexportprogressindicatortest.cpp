/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importexportprogressindicatortest.h"
#include <KMessageBox>
#include <kwidgetsaddons_version.h>

ImportExportProgressIndicatorTest::ImportExportProgressIndicatorTest(QObject *parent)
    : ImportExportProgressIndicatorBase(parent)
{
}

int ImportExportProgressIndicatorTest::mergeConfigMessageBox(const QString &configName) const
{
    Q_UNUSED(configName)
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    return KMessageBox::ButtonCode::PrimaryAction;
#else
    return KMessageBox::Yes;
#endif
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
