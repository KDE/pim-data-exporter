/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfiguredialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(u"buttonbox"_s);
    QVERIFY(buttonBox);

    PimDataExporterConfigureWidget *configureWidget = dlg.findChild<PimDataExporterConfigureWidget *>(u"configurewidget"_s);
    QVERIFY(configureWidget);
}

QTEST_MAIN(PimDataExporterConfigureDialogTest)

#include "moc_pimdataexporterconfiguredialogtest.cpp"
