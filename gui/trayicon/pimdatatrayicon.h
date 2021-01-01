/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATATRAYICON_H
#define PIMDATATRAYICON_H

#include <KStatusNotifierItem>

class PimDataTrayIcon : public KStatusNotifierItem
{
    Q_OBJECT
public:
    explicit PimDataTrayIcon(QObject *parent = nullptr);
    ~PimDataTrayIcon() override;
};

#endif // PIMDATATRAYICON_H
