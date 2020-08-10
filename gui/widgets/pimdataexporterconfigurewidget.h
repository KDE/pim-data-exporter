/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATAEXPORTERCONFIGUREWIDGET_H
#define PIMDATAEXPORTERCONFIGUREWIDGET_H

#include <QWidget>
class QCheckBox;
class PimDataExporterConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PimDataExporterConfigureWidget(QWidget *parent = nullptr);
    ~PimDataExporterConfigureWidget();

    void save();

public Q_SLOTS:
    void resetToDefault();

private:
    void initialize();
    QCheckBox *mAlwaysOverrideFile = nullptr;
    QCheckBox *mAlwaysOverrideDirectory = nullptr;
    QCheckBox *mAlwaysMergeConfigFile = nullptr;
};

#endif // PIMDATAEXPORTERCONFIGUREWIDGET_H
