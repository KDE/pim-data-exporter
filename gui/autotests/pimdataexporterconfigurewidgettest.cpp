/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfigurewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "../widgets/pimdataexporterconfigurewidget.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QTest>

PimDataExporterConfigureWidgetTest::PimDataExporterConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

PimDataExporterConfigureWidgetTest::~PimDataExporterConfigureWidgetTest() = default;

void PimDataExporterConfigureWidgetTest::shouldHaveDefaultValue()
{
    PimDataExporterConfigureWidget w;

    auto importGroupBox = w.findChild<QGroupBox *>(u"import_groupbox"_s);
    QVERIFY(importGroupBox);

    auto alwaysOverrideFile = w.findChild<QCheckBox *>(u"alwaysoverridefile"_s);
    QVERIFY(alwaysOverrideFile);

    auto alwaysOverrideDirectory = w.findChild<QCheckBox *>(u"alwaysoverridedirectory"_s);
    QVERIFY(alwaysOverrideDirectory);

    auto alwaysMergeConfigFile = w.findChild<QCheckBox *>(u"alwaysmergeconfigfile"_s);
    QVERIFY(alwaysMergeConfigFile);
}

QTEST_MAIN(PimDataExporterConfigureWidgetTest)

#include "moc_pimdataexporterconfigurewidgettest.cpp"
