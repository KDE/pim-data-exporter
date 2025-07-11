/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "selectiontypetreewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "core/utils.h"
#include "core/xml/templateselection.h"

#include <PimCommon/PimUtil>

#include <QFileDialog>

#include <KLocalizedString>
#include <QTemporaryFile>

#include <QHeaderView>
#include <QPointer>
#include <QTreeWidgetItem>

SelectionTypeTreeWidget::SelectionTypeTreeWidget(bool backupData, QWidget *parent)
    : QTreeWidget(parent)
{
    initialize(backupData);
    expandAll();
    header()->hide();
}

SelectionTypeTreeWidget::~SelectionTypeTreeWidget() = default;

void SelectionTypeTreeWidget::initialize(bool backupData)
{
    mKmailItem = new QTreeWidgetItem(this);
    mKmailItem->setCheckState(0, Qt::Checked);
    mKmailItem->setText(0, Utils::appTypeToI18n(Utils::KMail));
    createSubItem(mKmailItem, Utils::Identity);
    createSubItem(mKmailItem, Utils::Mails);
    createSubItem(mKmailItem, Utils::MailTransport);
    createSubItem(mKmailItem, Utils::Resources);
    createSubItem(mKmailItem, Utils::Config);

    mKaddressbookItem = new QTreeWidgetItem(this);
    mKaddressbookItem->setText(0, Utils::appTypeToI18n(Utils::KAddressBook));
    mKaddressbookItem->setCheckState(0, Qt::Checked);
    createSubItem(mKaddressbookItem, Utils::Resources);
    createSubItem(mKaddressbookItem, Utils::Config);

    mKalarmItem = new QTreeWidgetItem(this);
    mKalarmItem->setText(0, Utils::appTypeToI18n(Utils::KAlarm));
    mKalarmItem->setCheckState(0, Qt::Checked);
    createSubItem(mKalarmItem, Utils::Resources);
    createSubItem(mKalarmItem, Utils::Config);

    mKorganizerItem = new QTreeWidgetItem(this);
    mKorganizerItem->setText(0, Utils::appTypeToI18n(Utils::KOrganizer));
    mKorganizerItem->setCheckState(0, Qt::Checked);
    createSubItem(mKorganizerItem, Utils::Resources);
    createSubItem(mKorganizerItem, Utils::Config);

    mAkregatorItem = new QTreeWidgetItem(this);
    mAkregatorItem->setText(0, Utils::appTypeToI18n(Utils::Akregator));
    mAkregatorItem->setCheckState(0, Qt::Checked);
    createSubItem(mAkregatorItem, Utils::Config);
    createSubItem(mAkregatorItem, Utils::Data);

    connect(this, &SelectionTypeTreeWidget::itemChanged, this, &SelectionTypeTreeWidget::slotItemChanged);
}

void SelectionTypeTreeWidget::removeNotSelectedItems()
{
    if (!removeNotSelectedItem(mKmailItem)) {
        delete mKmailItem;
        mKmailItem = nullptr;
    }
    if (!removeNotSelectedItem(mKalarmItem)) {
        delete mKalarmItem;
        mKalarmItem = nullptr;
    }
    if (!removeNotSelectedItem(mKaddressbookItem)) {
        delete mKaddressbookItem;
        mKaddressbookItem = nullptr;
    }
    if (!removeNotSelectedItem(mKorganizerItem)) {
        delete mKorganizerItem;
        mKorganizerItem = nullptr;
    }
    if (!removeNotSelectedItem(mAkregatorItem)) {
        delete mAkregatorItem;
        mAkregatorItem = nullptr;
    }
}

bool SelectionTypeTreeWidget::removeNotSelectedItem(QTreeWidgetItem *parent)
{
    for (int i = parent->childCount() - 1; i >= 0; --i) {
        QTreeWidgetItem *item = parent->child(i);
        if (item->checkState(0) == Qt::Unchecked) {
            delete item;
        }
    }
    bool hasChildren = (parent->childCount() != 0);
    if (hasChildren) {
        parent->setCheckState(0, Qt::Checked);
    }
    return hasChildren;
}

QMap<Utils::AppsType, Utils::importExportParameters> SelectionTypeTreeWidget::storedType() const
{
    QMap<Utils::AppsType, Utils::importExportParameters> stored;
    Utils::importExportParameters var = typeChecked(mKmailItem);
    if (!var.isEmpty()) {
        stored.insert(Utils::KMail, var);
    }
    var = typeChecked(mKalarmItem);
    if (!var.isEmpty()) {
        stored.insert(Utils::KAlarm, var);
    }
    var = typeChecked(mKaddressbookItem);
    if (!var.isEmpty()) {
        stored.insert(Utils::KAddressBook, var);
    }
    var = typeChecked(mKorganizerItem);
    if (!var.isEmpty()) {
        stored.insert(Utils::KOrganizer, var);
    }
    var = typeChecked(mAkregatorItem);
    if (!var.isEmpty()) {
        stored.insert(Utils::Akregator, var);
    }
    return stored;
}

Utils::importExportParameters SelectionTypeTreeWidget::typeChecked(QTreeWidgetItem *parent) const
{
    Utils::importExportParameters parameters;
    if (parent) {
        int numberOfStep = 0;
        Utils::StoredTypes types = Utils::None;
        for (int i = 0; i < parent->childCount(); ++i) {
            QTreeWidgetItem *item = parent->child(i);
            if (item->checkState(0) == Qt::Checked) {
                types |= static_cast<Utils::StoredType>(item->data(0, action).toInt());
                ++numberOfStep;
            }
        }
        parameters.types = types;
        parameters.numberSteps = numberOfStep;
    }
    return parameters;
}

void SelectionTypeTreeWidget::createSubItem(QTreeWidgetItem *parent, Utils::StoredType type)
{
    switch (type) {
    case Utils::None:
        break;
    case Utils::Identity: {
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, Utils::storedTypeToI18n(Utils::Identity));
        item->setCheckState(0, Qt::Checked);
        item->setData(0, action, type);
        break;
    }
    case Utils::Mails: {
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, Utils::storedTypeToI18n(Utils::Mails));
        item->setCheckState(0, Qt::Checked);
        item->setData(0, action, type);
        break;
    }
    case Utils::MailTransport: {
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, Utils::storedTypeToI18n(Utils::MailTransport));
        item->setCheckState(0, Qt::Checked);
        item->setData(0, action, type);
        break;
    }
    case Utils::Resources: {
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, Utils::storedTypeToI18n(Utils::Resources));
        item->setCheckState(0, Qt::Checked);
        item->setData(0, action, type);
        break;
    }
    case Utils::Config: {
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, Utils::storedTypeToI18n(Utils::Config));
        item->setCheckState(0, Qt::Checked);
        item->setData(0, action, type);
        break;
    }
    case Utils::Data: {
        auto item = new QTreeWidgetItem(parent);
        item->setText(0, Utils::storedTypeToI18n(Utils::Data));
        item->setCheckState(0, Qt::Checked);
        item->setData(0, action, type);
        break;
    }
    }
}

void SelectionTypeTreeWidget::selectAllItems()
{
    setSelectItems(true);
}

void SelectionTypeTreeWidget::unSelectAllItems()
{
    setSelectItems(false);
}

void SelectionTypeTreeWidget::setSelectItems(bool b)
{
    changeState(mKmailItem, b);
    changeState(mKalarmItem, b);
    changeState(mKaddressbookItem, b);
    changeState(mKorganizerItem, b);
    changeState(mAkregatorItem, b);
}

void SelectionTypeTreeWidget::changeState(QTreeWidgetItem *item, bool b)
{
    if (item) {
        blockSignals(true);
        item->setCheckState(0, b ? Qt::Checked : Qt::Unchecked);
        for (int i = 0; i < item->childCount(); ++i) {
            item->child(i)->setCheckState(0, b ? Qt::Checked : Qt::Unchecked);
        }
        blockSignals(false);
    }
}

void SelectionTypeTreeWidget::slotItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) {
        return;
    }
    // Parent
    if (item->childCount() != 0) {
        changeState(item, item->checkState(0) == Qt::Checked);
    } else { // child
        blockSignals(true);
        QTreeWidgetItem *parent = item->parent();
        Qt::CheckState state = Qt::PartiallyChecked;
        for (int i = 0; i < parent->childCount(); ++i) {
            if (i == 0) {
                state = parent->child(i)->checkState(0);
            } else {
                if (state != parent->child(i)->checkState(0)) {
                    state = Qt::PartiallyChecked;
                    break;
                }
            }
        }
        parent->setCheckState(0, state);
        blockSignals(false);
    }
}

void SelectionTypeTreeWidget::loadFileName(const QString &fileName)
{
    unSelectAllItems();
    TemplateSelection templateSelection;
    const QMap<Utils::AppsType, Utils::importExportParameters> params = templateSelection.loadTemplate(fileName);
    setParameters(params);
}

void SelectionTypeTreeWidget::loadDefaultTemplate()
{
    // Keep old name for compatibility
    QString ret = QStandardPaths::locate(QStandardPaths::GenericDataLocation, u"/pimsettingexporter/defaulttemplate.xml"_s);
    if (ret.isEmpty()) {
        ret = QStandardPaths::locate(QStandardPaths::GenericDataLocation, u"/pimdataexporter/defaulttemplate.xml"_s);
    }
    if (!ret.isEmpty()) {
        loadFileName(ret);
    }
}

void SelectionTypeTreeWidget::loadTemplate(const QString &fileName)
{
    if (fileName.isEmpty()) {
        QPointer<QFileDialog> dlg = new QFileDialog(this, QString(), QString(), u"*.xml"_s);
        dlg->setFileMode(QFileDialog::ExistingFile);
        if (dlg->exec()) {
            const QStringList file = dlg->selectedFiles();
            loadFileName(file.at(0));
        }
        delete dlg;
    } else {
        loadFileName(fileName);
    }
}

QString SelectionTypeTreeWidget::exportedFileInfo()
{
    const QString templateStr = templateSelectionToString();
    QTemporaryFile tmp;
    tmp.open();
    if (!PimCommon::Util::saveToFile(tmp.fileName(), templateStr)) {
        return {};
    } else {
        tmp.setAutoRemove(false);
    }
    return tmp.fileName();
}

QString SelectionTypeTreeWidget::templateSelectionToString()
{
    TemplateSelection templateSelection;
    templateSelection.createTemplate(storedType());
    const QString templateStr = templateSelection.saveTemplate();
    return templateStr;
}

void SelectionTypeTreeWidget::saveAsDefaultTemplate()
{
    const QString templateStr = templateSelectionToString();
    const QString ret = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/pimdataexporter/"_s;
    QDir().mkpath(ret);

    if (!PimCommon::Util::saveToFile(ret + u"defaulttemplate.xml"_s, templateStr)) {
        qWarning() << "Impossible to save as defaulttemplate.xml";
    }
}

void SelectionTypeTreeWidget::saveAsTemplate()
{
    const QString templateStr = templateSelectionToString();
    const QString filter(i18n("Template Files (*.xml)"));
    PimCommon::Util::saveTextAs(templateStr, filter, this);
}

void SelectionTypeTreeWidget::initializeSubItem(QTreeWidgetItem *item, Utils::StoredTypes types)
{
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);
        if (types & static_cast<Utils::StoredType>(child->data(0, action).toInt())) {
            child->setCheckState(0, Qt::Checked);
        } else {
            child->setCheckState(0, Qt::Unchecked);
        }
    }
}

void SelectionTypeTreeWidget::setParameters(const QMap<Utils::AppsType, Utils::importExportParameters> &params)
{
    QMap<Utils::AppsType, Utils::importExportParameters>::const_iterator i = params.constBegin();
    while (i != params.constEnd()) {
        switch (i.key()) {
        case Utils::KMail:
            initializeSubItem(mKmailItem, i.value().types);
            break;
        case Utils::KAddressBook:
            initializeSubItem(mKaddressbookItem, i.value().types);
            break;
        case Utils::KAlarm:
            initializeSubItem(mKalarmItem, i.value().types);
            break;
        case Utils::KOrganizer:
            initializeSubItem(mKorganizerItem, i.value().types);
            break;
        case Utils::Akregator:
            initializeSubItem(mAkregatorItem, i.value().types);
            break;
        case Utils::Unknown:
            break;
        }
        ++i;
    }
}

#include "moc_selectiontypetreewidget.cpp"
