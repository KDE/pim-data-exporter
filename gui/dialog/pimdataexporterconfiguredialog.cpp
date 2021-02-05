/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterconfiguredialog.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QPushButton>
#include <QDialogButtonBox>
#include <KPageWidgetModel>
#include "../widgets/pimdataexporterconfigurewidget.h"
#include <KConfigGroup>
#include <KSharedConfig>

#ifdef WITH_KUSERFEEDBACK
#include <KUserFeedback/Provider>
#include <KUserFeedback/FeedbackConfigWidget>
#include "userfeedback/userfeedbackmanager.h"
#endif

PimDataExporterConfigureDialog::PimDataExporterConfigureDialog(QWidget *parent)
    : KPageDialog(parent)
{
    setFaceType(KPageDialog::List);
    setWindowTitle(i18nc("@title:window", "Configure PimDataExporter"));

    //General page
    QWidget *generalConfigureWiget = new QWidget;
    auto layout = new QVBoxLayout(generalConfigureWiget);
    mConfigureWidget = new PimDataExporterConfigureWidget(this);
    mConfigureWidget->setObjectName(QStringLiteral("configurewidget"));

    layout->addWidget(mConfigureWidget);

    KPageWidgetItem *generalPageWidgetPage = new KPageWidgetItem(mConfigureWidget, i18n("General"));
    generalPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(generalPageWidgetPage);

#ifdef WITH_KUSERFEEDBACK
    QWidget *userFeedBackWidget = new QWidget;
    userFeedBackWidget->setObjectName(QStringLiteral("userFeedBackWidget"));

    mUserFeedbackWidget = new KUserFeedback::FeedbackConfigWidget(this);

    auto userFeedBackLayout = new QHBoxLayout(userFeedBackWidget);
    userFeedBackLayout->setContentsMargins(0, 0, 0, 0);
    userFeedBackLayout->addWidget(mUserFeedbackWidget);

    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
    KPageWidgetItem *userFeedBackPageWidgetPage = new KPageWidgetItem(userFeedBackWidget, i18n("User Feedback"));
    userFeedBackPageWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-other")));
    addPage(userFeedBackPageWidgetPage);
#endif

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok| QDialogButtonBox::Cancel);

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
    KConfigGroup group(KSharedConfig::openConfig(), "PimDataExporterConfigureDialog");
    const QSize size = group.readEntry("Size", QSize(600, 400));
    if (size.isValid()) {
        resize(size);
    }
}

void PimDataExporterConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "PimDataExporterConfigureDialog");
    group.writeEntry("Size", size());
    group.sync();
}
