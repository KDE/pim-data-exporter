/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfigurewidget.h"
#include "pimdataexporterglobalconfig.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>

PimDataExporterConfigureWidget::PimDataExporterConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QGroupBox *groupBox = new QGroupBox(i18n("Import"));
    groupBox->setObjectName(QStringLiteral("import_groupbox"));
    layout->addWidget(groupBox);

    auto groupBoxLayout = new QVBoxLayout;
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
    groupBoxLayout->addStretch(1);

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
