/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KPageDialog>
#ifdef WITH_KUSERFEEDBACK
namespace KUserFeedback
{
class FeedbackConfigWidget;
}
#endif
class PimDataExporterConfigureWidget;
class PimDataExporterConfigureDialog : public KPageDialog
{
    Q_OBJECT
public:
    explicit PimDataExporterConfigureDialog(QWidget *parent = nullptr);
    ~PimDataExporterConfigureDialog() override;

private:
    void slotAccepted();
    void readConfig();
    void writeConfig();

    PimDataExporterConfigureWidget *const mConfigureWidget;
#ifdef WITH_KUSERFEEDBACK
    KUserFeedback::FeedbackConfigWidget *mUserFeedbackWidget = nullptr;
#endif
};
