/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATAEXPORTERCONFIGUREWIDGETTEST_H
#define PIMDATAEXPORTERCONFIGUREWIDGETTEST_H

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

#endif // PIMDATAEXPORTERCONFIGUREWIDGETTEST_H
