/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATAEXPORTERCONFIGUREDIALOGTEST_H
#define PIMDATAEXPORTERCONFIGUREDIALOGTEST_H

#include <QObject>

class PimDataExporterConfigureDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit PimDataExporterConfigureDialogTest(QObject *parent = nullptr);
    ~PimDataExporterConfigureDialogTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // PIMDATAEXPORTERCONFIGUREDIALOGTEST_H
