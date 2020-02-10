/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "pimdataexporterconfiguredialog.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QPushButton>
#include <QDialogButtonBox>
#include <kpagewidgetmodel.h>
#include "../widgets/pimdataexporterconfigurewidget.h"

PimDataExporterConfigureDialog::PimDataExporterConfigureDialog(QWidget *parent)
    : KPageDialog(parent)
{
    setFaceType(KPageDialog::List);
    setWindowTitle(i18nc("@title:window", "Configure PimDataExporter"));


    //General page
    QWidget *generalConfigureWiget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(generalConfigureWiget);
    mConfigureWidget = new PimDataExporterConfigureWidget(this);
    mConfigureWidget->setObjectName(QStringLiteral("configurewidget"));

    layout->addWidget(mConfigureWidget);

    KPageWidgetItem *generalPageWidgetPage = new KPageWidgetItem(mConfigureWidget, i18n("General"));
    generalPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(generalPageWidgetPage);


    buttonBox()->setStandardButtons(QDialogButtonBox::Ok| QDialogButtonBox::Cancel);

    connect(buttonBox(), &QDialogButtonBox::accepted, this, &PimDataExporterConfigureDialog::accept);
    connect(buttonBox(), &QDialogButtonBox::rejected, this, &PimDataExporterConfigureDialog::reject);
}

PimDataExporterConfigureDialog::~PimDataExporterConfigureDialog()
{
}

void PimDataExporterConfigureDialog::slotAccepted()
{
    mConfigureWidget->save();
    accept();
}
