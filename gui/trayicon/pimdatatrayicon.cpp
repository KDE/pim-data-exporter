/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatatrayicon.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

PimDataTrayIcon::PimDataTrayIcon(QObject *parent)
    : KStatusNotifierItem(parent)
{
    setToolTipTitle(i18n("Pim Data Exporter"));
    setIconByName(u"kontact"_s);
}

PimDataTrayIcon::~PimDataTrayIcon() = default;

#include "moc_pimdatatrayicon.cpp"
