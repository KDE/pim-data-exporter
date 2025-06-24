/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "synchronizeresourcedialog.h"
using namespace Qt::Literals::StringLiterals;

#include <KListWidgetSearchLine>
#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char mySynchronizeResourceDialogGroupName[] = "SynchronizeResourceDialog";
}
SynchronizeResourceDialog::SynchronizeResourceDialog(QWidget *parent)
    : QDialog(parent)
    , mListResourceWidget(new QListWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Synchronize Resources"));
    auto topLayout = new QVBoxLayout(this);

    auto lab = new QLabel(i18nc("@label:textbox", "Some resources were added but data were not sync. Select resources that you want to sync:"), this);
    lab->setWordWrap(true);
    lab->setObjectName(QLatin1StringView("label"));

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    mListResourceWidget->setObjectName(QLatin1StringView("listresourcewidget"));
    auto listWidgetSearchLine = new KListWidgetSearchLine(this, mListResourceWidget);
    listWidgetSearchLine->setObjectName(QLatin1StringView("listwidgetsearchline"));

    auto hbox = new QHBoxLayout;
    auto selectAll = new QPushButton(i18nc("@action:button", "Select All"));
    selectAll->setObjectName(QLatin1StringView("selectall_button"));
    connect(selectAll, &QPushButton::clicked, this, &SynchronizeResourceDialog::slotSelectAll);
    hbox->addWidget(selectAll);

    auto unselectAll = new QPushButton(i18nc("@action:button", "Unselect All"));
    unselectAll->setObjectName(QLatin1StringView("unselectall_button"));
    connect(unselectAll, &QPushButton::clicked, this, &SynchronizeResourceDialog::slotUnselectAll);
    hbox->addWidget(unselectAll);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SynchronizeResourceDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SynchronizeResourceDialog::reject);
    setModal(true);

    topLayout->addWidget(lab);
    topLayout->addWidget(listWidgetSearchLine);
    topLayout->addWidget(mListResourceWidget);
    topLayout->addLayout(hbox);
    topLayout->addWidget(buttonBox);
    readConfig();
}

SynchronizeResourceDialog::~SynchronizeResourceDialog()
{
    writeConfig();
}

void SynchronizeResourceDialog::slotSelectAll()
{
    selectItem(true);
}

void SynchronizeResourceDialog::slotUnselectAll()
{
    selectItem(false);
}

void SynchronizeResourceDialog::selectItem(bool state)
{
    for (int i = 0; i < mListResourceWidget->count(); ++i) {
        QListWidgetItem *item = mListResourceWidget->item(i);
        item->setCheckState(state ? Qt::Checked : Qt::Unchecked);
    }
}

void SynchronizeResourceDialog::setResources(const QHash<QString, QString> &resources)
{
    QHashIterator<QString, QString> i(resources);
    while (i.hasNext()) {
        i.next();
        auto item = new QListWidgetItem(mListResourceWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setText(i.key());
        item->setData(ResourceIdentifier, i.value());
    }
}

QStringList SynchronizeResourceDialog::resources() const
{
    QStringList lst;
    for (int i = 0; i < mListResourceWidget->count(); ++i) {
        QListWidgetItem *item = mListResourceWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            lst << item->data(ResourceIdentifier).toString();
        }
    }
    return lst;
}

void SynchronizeResourceDialog::slotAccepted()
{
    accept();
}

void SynchronizeResourceDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySynchronizeResourceDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SynchronizeResourceDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySynchronizeResourceDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_synchronizeresourcedialog.cpp"
