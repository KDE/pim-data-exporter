/*
   SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectiontypedialog.h"
#include "gui/widgets/selectiontypetreewidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char myConfigSelectionTypeDialogDialog[] = "SelectionTypeDialog";
}
SelectionTypeDialog::SelectionTypeDialog(bool backupData, QWidget *parent)
    : QDialog(parent)
    , mSelectionTreeWidget(new SelectionTypeTreeWidget(backupData, this))
    , mUseTemplateByDefault(new QCheckBox(i18n("Use this template by default"), this))
    , mSaveTemplate(new QPushButton(i18nc("@action:button", "Save as Template..."), this))
    , mLoadTemplate(new QPushButton(i18nc("@action:button", "Load Template..."), this))
{
    setWindowTitle(i18nc("@title:window", "Select Type"));
    setModal(true);

    auto topLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QLatin1StringView("buttonbox"));

    auto okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectionTypeDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectionTypeDialog::reject);
    okButton->setDefault(true);

    mSelectionTreeWidget->setObjectName(QLatin1StringView("mSelectionTreeWidget"));
    topLayout->addWidget(mSelectionTreeWidget);

    mUseTemplateByDefault->setObjectName(QLatin1StringView("mUseTemplateByDefault"));
    topLayout->addWidget(mUseTemplateByDefault);

    auto hbox = new QHBoxLayout;
    auto selectAll = new QPushButton(i18nc("@action:button", "Select All"), this);
    selectAll->setObjectName(QLatin1StringView("selectAll"));
    connect(selectAll, &QPushButton::clicked, this, &SelectionTypeDialog::slotSelectAll);
    hbox->addWidget(selectAll);

    auto unselectAll = new QPushButton(i18nc("@action:button", "Unselect All"), this);
    unselectAll->setObjectName(QLatin1StringView("unselectAll"));
    connect(unselectAll, &QPushButton::clicked, this, &SelectionTypeDialog::slotUnselectAll);
    hbox->addWidget(unselectAll);

    mSaveTemplate->setObjectName(QLatin1StringView("mSaveTemplate"));
    connect(mSaveTemplate, &QPushButton::clicked, this, &SelectionTypeDialog::slotSaveAsTemplate);
    hbox->addWidget(mSaveTemplate);

    mLoadTemplate->setObjectName(QLatin1StringView("mLoadTemplate"));
    connect(mLoadTemplate, &QPushButton::clicked, this, &SelectionTypeDialog::slotLoadTemplate);
    hbox->addWidget(mLoadTemplate);

    topLayout->addLayout(hbox);
    topLayout->addWidget(buttonBox);
    readConfig();
}

SelectionTypeDialog::~SelectionTypeDialog()
{
    saveDefaultTemplate();
    writeConfig();
}

void SelectionTypeDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigSelectionTypeDialogDialog));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
    loadDefaultTemplate();
}

void SelectionTypeDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigSelectionTypeDialogDialog));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

QMap<Utils::AppsType, Utils::importExportParameters> SelectionTypeDialog::storedType() const
{
    return mSelectionTreeWidget->storedType();
}

QString SelectionTypeDialog::exportedFileInfo() const
{
    return mSelectionTreeWidget->exportedFileInfo();
}

void SelectionTypeDialog::removeNotSelectedItems()
{
    mSelectionTreeWidget->removeNotSelectedItems();
    mSaveTemplate->hide();
    mLoadTemplate->hide();
    mUseTemplateByDefault->hide();
}

void SelectionTypeDialog::loadTemplate(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        mSelectionTreeWidget->loadTemplate(fileName);
    }
}

void SelectionTypeDialog::slotSelectAll()
{
    mSelectionTreeWidget->selectAllItems();
}

void SelectionTypeDialog::slotUnselectAll()
{
    mSelectionTreeWidget->unSelectAllItems();
}

void SelectionTypeDialog::slotSaveAsTemplate()
{
    mSelectionTreeWidget->saveAsTemplate();
}

void SelectionTypeDialog::slotLoadTemplate()
{
    mSelectionTreeWidget->loadTemplate();
}

void SelectionTypeDialog::saveDefaultTemplate()
{
    if (mUseTemplateByDefault->isChecked()) {
        mSelectionTreeWidget->saveAsDefaultTemplate();
    }
}

void SelectionTypeDialog::loadDefaultTemplate()
{
    mSelectionTreeWidget->loadDefaultTemplate();
}

#include "moc_selectiontypedialog.cpp"
