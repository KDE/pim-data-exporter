/*
   Copyright (C) 2012-2017 Montel Laurent <montel@kde.org>

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

SelectionTypeDialog::SelectionTypeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Select Type"));
    setModal(true);

    QVBoxLayout *topLayout = new QVBoxLayout(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectionTypeDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectionTypeDialog::reject);
    okButton->setDefault(true);

    mSelectionTreeWidget = new SelectionTypeTreeWidget(this);
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

QString SelectionTypeDialog::exportedFileInfo()
{
    return mSelectionTreeWidget->exportedFileInfo();
}

void SelectionTypeDialog::removeNotSelectedItems()
{
    mSelectionTreeWidget->removeNotSelectedItems();
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
