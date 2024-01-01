/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfiguredialogtest.h"
#include "../dialog/pimdataexporterconfiguredialog.h"
#include "../widgets/pimdataexporterconfigurewidget.h"
#include <QDialogButtonBox>
#include <QTest>

PimDataExporterConfigureDialogTest::PimDataExporterConfigureDialogTest(QObject *parent)
    : QObject(parent)
{
}

PimDataExporterConfigureDialogTest::~PimDataExporterConfigureDialogTest()
{
}

void PimDataExporterConfigureDialogTest::shouldHaveDefaultValue()
{
    PimDataExporterConfigureDialog dlg;
    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);

    PimDataExporterConfigureWidget *configureWidget = dlg.findChild<PimDataExporterConfigureWidget *>(QStringLiteral("configurewidget"));
    QVERIFY(configureWidget);
}

QTEST_MAIN(PimDataExporterConfigureDialogTest)

#include "moc_pimdataexporterconfiguredialogtest.cpp"
