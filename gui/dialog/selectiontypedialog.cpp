/*
   SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectiontypedialog.h"
#include "gui/widgets/selectiontypetreewidget.h"

#include <KLocalizedString>
#include <QHBoxLayout>
#include <QPushButton>
#include <KSharedConfig>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QCheckBox>

SelectionTypeDialog::SelectionTypeDialog(bool backupData, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Select Type"));
    setModal(true);

    QVBoxLayout *topLayout = new QVBoxLayout(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectionTypeDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectionTypeDialog::reject);
    okButton->setDefault(true);

    mSelectionTreeWidget = new SelectionTypeTreeWidget(backupData, this);
    mSelectionTreeWidget->setObjectName(QStringLiteral("mSelectionTreeWidget"));
    topLayout->addWidget(mSelectionTreeWidget);

    mUseTemplateByDefault = new QCheckBox(i18n("Use this template by default"), this);
    mUseTemplateByDefault->setObjectName(QStringLiteral("mUseTemplateByDefault"));
    topLayout->addWidget(mUseTemplateByDefault);

    QHBoxLayout *hbox = new QHBoxLayout;
    QPushButton *selectAll = new QPushButton(i18n("Select All"), this);
    selectAll->setObjectName(QStringLiteral("selectAll"));
    connect(selectAll, &QPushButton::clicked, this, &SelectionTypeDialog::slotSelectAll);
    hbox->addWidget(selectAll);

    QPushButton *unselectAll = new QPushButton(i18n("Unselect All"), this);
    unselectAll->setObjectName(QStringLiteral("unselectAll"));
    connect(unselectAll, &QPushButton::clicked, this, &SelectionTypeDialog::slotUnselectAll);
    hbox->addWidget(unselectAll);

    mSaveTemplate = new QPushButton(i18n("Save as Template..."), this);
    mSaveTemplate->setObjectName(QStringLiteral("mSaveTemplate"));
    connect(mSaveTemplate, &QPushButton::clicked, this, &SelectionTypeDialog::slotSaveAsTemplate);
    hbox->addWidget(mSaveTemplate);

    mLoadTemplate = new QPushButton(i18n("Load Template..."), this);
    mLoadTemplate->setObjectName(QStringLiteral("mLoadTemplate"));
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

void SelectionTypeDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SelectionTypeDialog");
    group.writeEntry("Size", size());
}

void SelectionTypeDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SelectionTypeDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(600, 400));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
    loadDefaultTemplate();
}

QHash<Utils::AppsType, Utils::importExportParameters> SelectionTypeDialog::storedType() const
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
