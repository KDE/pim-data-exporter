/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfiguredialogtest.h"
#include "../dialog/pimdataexporterconfiguredialog.h"
#include <QDialogButtonBox>
#include <QTest>
#include "../widgets/pimdataexporterconfigurewidget.h"

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
