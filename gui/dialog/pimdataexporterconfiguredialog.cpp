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
#include "../widgets/pimdataexporterconfigurewidget.h"

PimDataExporterConfigureDialog::PimDataExporterConfigureDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Configure PimDataExporter"));
    QVBoxLayout *layout = new QVBoxLayout(this);
    mConfigureWidget = new PimDataExporterConfigureWidget(this);
    mConfigureWidget->setObjectName(QStringLiteral("configurewidget"));
    layout->addWidget(mConfigureWidget);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults, this);
    button->setObjectName(QStringLiteral("buttonbox"));
    connect(button, &QDialogButtonBox::accepted, this, &PimDataExporterConfigureDialog::slotAccepted);
    connect(button, &QDialogButtonBox::rejected, this, &PimDataExporterConfigureDialog::reject);
    connect(button->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, mConfigureWidget, &PimDataExporterConfigureWidget::resetToDefault);
    layout->addWidget(button);
}

PimDataExporterConfigureDialog::~PimDataExporterConfigureDialog()
{
}

void PimDataExporterConfigureDialog::slotAccepted()
{
    mConfigureWidget->save();
    accept();
}
