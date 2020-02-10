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
#include <kpagewidgetmodel.h>
#include "../widgets/pimdataexporterconfigurewidget.h"

#ifdef WITH_KUSERFEEDBACK
#include <KUserFeedback/Provider>
#include <KUserFeedback/FeedbackConfigWidget>
#include "userfeedback/userfeedbackmanager.h"
#include <KConfigGroup>
#include <KSharedConfig>
#endif

PimDataExporterConfigureDialog::PimDataExporterConfigureDialog(QWidget *parent)
    : KPageDialog(parent)
{
    setFaceType(KPageDialog::List);
    setWindowTitle(i18nc("@title:window", "Configure PimDataExporter"));


    //General page
    QWidget *generalConfigureWiget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(generalConfigureWiget);
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

    QHBoxLayout *userFeedBackLayout = new QHBoxLayout(userFeedBackWidget);
    userFeedBackLayout->setContentsMargins(0, 0, 0, 0);
    userFeedBackLayout->addWidget(mUserFeedbackWidget);

    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
    KPageWidgetItem *userFeedBackPageWidgetPage = new KPageWidgetItem(userFeedBackWidget, i18n("User Feedback"));
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
