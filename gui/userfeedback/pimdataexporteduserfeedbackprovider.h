/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KUserFeedbackQt6/Provider>
class PimDataExportedUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit PimDataExportedUserFeedbackProvider(QObject *parent = nullptr);
    ~PimDataExportedUserFeedbackProvider() override;
};
