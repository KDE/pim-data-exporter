/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

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
    , mAlwaysOverrideFile(new QCheckBox(i18n("Always Override File")))
    , mAlwaysOverrideDirectory(new QCheckBox(i18n("Always Override Directory")))
    , mAlwaysMergeConfigFile(new QCheckBox(i18n("Always Merge Config File")))
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto groupBox = new QGroupBox(i18n("Import"));
    groupBox->setObjectName(QLatin1StringView("import_groupbox"));
    layout->addWidget(groupBox);

    auto groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    mAlwaysOverrideFile->setObjectName(QLatin1StringView("alwaysoverridefile"));
    groupBoxLayout->addWidget(mAlwaysOverrideFile);

    mAlwaysOverrideDirectory->setObjectName(QLatin1StringView("alwaysoverridedirectory"));
    groupBoxLayout->addWidget(mAlwaysOverrideDirectory);

    mAlwaysMergeConfigFile->setObjectName(QLatin1StringView("alwaysmergeconfigfile"));
    groupBoxLayout->addWidget(mAlwaysMergeConfigFile);
    groupBoxLayout->addStretch(1);

    initialize();
}

PimDataExporterConfigureWidget::~PimDataExporterConfigureWidget() = default;

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

#include "moc_pimdataexporterconfigurewidget.cpp"
