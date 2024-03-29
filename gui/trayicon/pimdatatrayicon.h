/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KStatusNotifierItem>

class PimDataTrayIcon : public KStatusNotifierItem
{
    Q_OBJECT
public:
    explicit PimDataTrayIcon(QObject *parent = nullptr);
    ~PimDataTrayIcon() override;
};
