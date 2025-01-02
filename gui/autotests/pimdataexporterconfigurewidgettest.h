/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PimDataExporterConfigureWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit PimDataExporterConfigureWidgetTest(QObject *parent = nullptr);
    ~PimDataExporterConfigureWidgetTest() override;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
