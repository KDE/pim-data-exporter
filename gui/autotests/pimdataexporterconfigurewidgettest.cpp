/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfigurewidgettest.h"
#include "../widgets/pimdataexporterconfigurewidget.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QTest>

PimDataExporterConfigureWidgetTest::PimDataExporterConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

PimDataExporterConfigureWidgetTest::~PimDataExporterConfigureWidgetTest()
{
}

void PimDataExporterConfigureWidgetTest::shouldHaveDefaultValue()
{
    PimDataExporterConfigureWidget w;

    QGroupBox *importGroupBox = w.findChild<QGroupBox *>(QStringLiteral("import_groupbox"));
    QVERIFY(importGroupBox);

    QCheckBox *alwaysOverrideFile = w.findChild<QCheckBox *>(QStringLiteral("alwaysoverridefile"));
    QVERIFY(alwaysOverrideFile);

    QCheckBox *alwaysOverrideDirectory = w.findChild<QCheckBox *>(QStringLiteral("alwaysoverridedirectory"));
    QVERIFY(alwaysOverrideDirectory);

    QCheckBox *alwaysMergeConfigFile = w.findChild<QCheckBox *>(QStringLiteral("alwaysmergeconfigfile"));
    QVERIFY(alwaysMergeConfigFile);
}

QTEST_MAIN(PimDataExporterConfigureWidgetTest)
