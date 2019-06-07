/*
   Copyright (C) 2015-2019 Montel Laurent <montel@kde.org>

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

#include "pimdataexporterconfigurewidget.h"
#include "pimdataexporterglobalconfig.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <KLocalizedString>
#include <QGroupBox>

PimDataExporterConfigureWidget::PimDataExporterConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QGroupBox *groupBox = new QGroupBox(i18n("Import"));
    groupBox->setObjectName(QStringLiteral("import_groupbox"));
    layout->addWidget(groupBox);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    mAlwaysOverrideFile = new QCheckBox(i18n("Always Override File"));
    mAlwaysOverrideFile->setObjectName(QStringLiteral("alwaysoverridefile"));
    groupBoxLayout->addWidget(mAlwaysOverrideFile);

    mAlwaysOverrideDirectory = new QCheckBox(i18n("Always Override Directory"));
    mAlwaysOverrideDirectory->setObjectName(QStringLiteral("alwaysoverridedirectory"));
    groupBoxLayout->addWidget(mAlwaysOverrideDirectory);

    mAlwaysMergeConfigFile = new QCheckBox(i18n("Always Merge Config File"));
    mAlwaysMergeConfigFile->setObjectName(QStringLiteral("alwaysmergeconfigfile"));
    groupBoxLayout->addWidget(mAlwaysMergeConfigFile);

    initialize();
}

PimDataExporterConfigureWidget::~PimDataExporterConfigureWidget()
{
}

void PimDataExporterConfigureWidget::initialize()
{
    mAlwaysOverrideFile->setChecked(PimDataExportGlobalConfig::self()->alwaysOverrideFile());
    mAlwaysMergeConfigFile->setChecked(PimDataExportGlobalConfig::self()->alwaysMergeConfigFile());
    mAlwaysOverrideDirectory->setChecked(PimDataExportGlobalConfig::self()->alwaysOverrideDirectory());
}

void PimDataExporterConfigureWidget::save()
{
    PimDataExportGlobalConfig::self()->setAlwaysOverrideFile(mAlwaysOverrideFile->isChecked());
    PimDataExportGlobalConfig::self()->setAlwaysMergeConfigFile(mAlwaysMergeConfigFile->isChecked());
    PimDataExportGlobalConfig::self()->setAlwaysOverrideDirectory(mAlwaysOverrideDirectory->isChecked());
}

void PimDataExporterConfigureWidget::resetToDefault()
{
    const bool bUseDefaults = PimDataExportGlobalConfig::self()->useDefaults(true);
    initialize();

    PimDataExportGlobalConfig::self()->useDefaults(bUseDefaults);
}