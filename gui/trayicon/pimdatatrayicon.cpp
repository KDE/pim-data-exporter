/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatatrayicon.h"
#include <KLocalizedString>

PimDataTrayIcon::PimDataTrayIcon(QObject *parent)
    : KStatusNotifierItem(parent)
{
    setToolTipTitle(i18n("Pim Data Exporter"));
    setIconByName(QStringLiteral("kontact"));
}

PimDataTrayIcon::~PimDataTrayIcon()
{
}
