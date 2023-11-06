/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

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
    , mSaveTemplate(new QPushButton(i18n("Save as Template..."), this))
    , mLoadTemplate(new QPushButton(i18n("Load Template..."), this))
{
    setWindowTitle(i18nc("@title:window", "Select Type"));
    setModal(true);

    auto topLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));

    auto okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectionTypeDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectionTypeDialog::reject);
    okButton->setDefault(true);

    mSelectionTreeWidget->setObjectName(QStringLiteral("mSelectionTreeWidget"));
    topLayout->addWidget(mSelectionTreeWidget);

    mUseTemplateByDefault->setObjectName(QStringLiteral("mUseTemplateByDefault"));
    topLayout->addWidget(mUseTemplateByDefault);

    auto hbox = new QHBoxLayout;
    auto selectAll = new QPushButton(i18n("Select All"), this);
    selectAll->setObjectName(QStringLiteral("selectAll"));
    connect(selectAll, &QPushButton::clicked, this, &SelectionTypeDialog::slotSelectAll);
    hbox->addWidget(selectAll);

    auto unselectAll = new QPushButton(i18n("Unselect All"), this);
    unselectAll->setObjectName(QStringLiteral("unselectAll"));
    connect(unselectAll, &QPushButton::clicked, this, &SelectionTypeDialog::slotUnselectAll);
    hbox->addWidget(unselectAll);

    mSaveTemplate->setObjectName(QStringLiteral("mSaveTemplate"));
    connect(mSaveTemplate, &QPushButton::clicked, this, &SelectionTypeDialog::slotSaveAsTemplate);
    hbox->addWidget(mSaveTemplate);

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

void SelectionTypeDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigSelectionTypeDialogDialog));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
    loadDefaultTemplate();
}

void SelectionTypeDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigSelectionTypeDialogDialog));
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
