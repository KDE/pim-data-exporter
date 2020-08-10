/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATAEXPORTEDUSERFEEDBACKPROVIDER_H
#define PIMDATAEXPORTEDUSERFEEDBACKPROVIDER_H

#include <KUserFeedback/Provider>

class PimDataExportedUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit PimDataExportedUserFeedbackProvider(QObject *parent = nullptr);
    ~PimDataExportedUserFeedbackProvider();
};

#endif // PIMDATAEXPORTEDUSERFEEDBACKPROVIDER_H
