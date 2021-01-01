/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATAEXPORTERCONFIGUREDIALOG_H
#define PIMDATAEXPORTERCONFIGUREDIALOG_H

#include <KPageDialog>
#ifdef WITH_KUSERFEEDBACK
namespace KUserFeedback {
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

    PimDataExporterConfigureWidget *mConfigureWidget = nullptr;
#ifdef WITH_KUSERFEEDBACK
    KUserFeedback::FeedbackConfigWidget *mUserFeedbackWidget = nullptr;
#endif
};

#endif // PIMDATAEXPORTERCONFIGUREDIALOG_H
