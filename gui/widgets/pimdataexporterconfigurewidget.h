/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QCheckBox;
class PimDataExporterConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PimDataExporterConfigureWidget(QWidget *parent = nullptr);
    ~PimDataExporterConfigureWidget() override;

    void save();

public Q_SLOTS:
    void resetToDefault();

private:
    void initialize();
    QCheckBox *mAlwaysOverrideFile = nullptr;
    QCheckBox *mAlwaysOverrideDirectory = nullptr;
    QCheckBox *mAlwaysMergeConfigFile = nullptr;
};

