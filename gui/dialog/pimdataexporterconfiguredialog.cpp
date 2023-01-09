/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfiguredialog.h"
#include "gui/widgets/pimdataexporterconfigurewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/FeedbackConfigWidget>
#include <KUserFeedback/Provider>
#endif

#include <KWindowConfig>
#include <QWindow>

namespace
{
static const char myPimDataExporterConfigureDialogGroupName[] = "PimDataExporterConfigureDialog";
}
PimDataExporterConfigureDialog::PimDataExporterConfigureDialog(QWidget *parent)
    : KPageDialog(parent)
    , mConfigureWidget(new PimDataExporterConfigureWidget(this))
{
    setFaceType(KPageDialog::List);
    setWindowTitle(i18nc("@title:window", "Configure PimDataExporter"));

    // General page
    auto generalConfigureWiget = new QWidget(this);
    auto layout = new QVBoxLayout(generalConfigureWiget);
    mConfigureWidget->setObjectName(QStringLiteral("configurewidget"));

    layout->addWidget(mConfigureWidget);

    auto generalPageWidgetPage = new KPageWidgetItem(mConfigureWidget, i18n("General"));
    generalPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(generalPageWidgetPage);

#ifdef WITH_KUSERFEEDBACK
    auto userFeedBackWidget = new QWidget;
    userFeedBackWidget->setObjectName(QStringLiteral("userFeedBackWidget"));

    mUserFeedbackWidget = new KUserFeedback::FeedbackConfigWidget(this);

    auto userFeedBackLayout = new QHBoxLayout(userFeedBackWidget);
    userFeedBackLayout->setContentsMargins(0, 0, 0, 0);
    userFeedBackLayout->addWidget(mUserFeedbackWidget);

    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
    auto userFeedBackPageWidgetPage = new KPageWidgetItem(userFeedBackWidget, i18n("User Feedback"));
    userFeedBackPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-other")));
    addPage(userFeedBackPageWidgetPage);
#endif

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox(), &QDialogButtonBox::accepted, this, &PimDataExporterConfigureDialog::accept);
    connect(buttonBox(), &QDialogButtonBox::rejected, this, &PimDataExporterConfigureDialog::reject);
    readConfig();
}

PimDataExporterConfigureDialog::~PimDataExporterConfigureDialog()
{
    writeConfig();
}

void PimDataExporterConfigureDialog::slotAccepted()
{
    mConfigureWidget->save();
#ifdef WITH_KUSERFEEDBACK
    // set current active mode + write back the config for future starts
    UserFeedBackManager::self()->userFeedbackProvider()->setTelemetryMode(mUserFeedbackWidget->telemetryMode());
    UserFeedBackManager::self()->userFeedbackProvider()->setSurveyInterval(mUserFeedbackWidget->surveyInterval());
#endif
    accept();
}

void PimDataExporterConfigureDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), myPimDataExporterConfigureDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void PimDataExporterConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myPimDataExporterConfigureDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}
