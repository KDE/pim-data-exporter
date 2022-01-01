/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PimDataExporterConfigureDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit PimDataExporterConfigureDialogTest(QObject *parent = nullptr);
    ~PimDataExporterConfigureDialogTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
