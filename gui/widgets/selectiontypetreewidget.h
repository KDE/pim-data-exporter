/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "utils.h"
#include <QMap>
#include <QTreeWidget>

class QTreeWidgetItem;

class SelectionTypeTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SelectionTypeTreeWidget(bool backupData, QWidget *parent = nullptr);
    ~SelectionTypeTreeWidget() override;

    [[nodiscard]] QMap<Utils::AppsType, Utils::importExportParameters> storedType() const;

    void selectAllItems();
    void unSelectAllItems();

    void saveAsTemplate();

    void loadTemplate(const QString &fileName = QString());
    void saveAsDefaultTemplate();
    void loadDefaultTemplate();
    [[nodiscard]] QString exportedFileInfo();
    void removeNotSelectedItems();

private:
    void slotItemChanged(QTreeWidgetItem *, int);

    enum ActionType { action = Qt::UserRole + 1 };

    void initialize(bool backupData);
    void setSelectItems(bool b);
    void changeState(QTreeWidgetItem *item, bool b);
    void createSubItem(QTreeWidgetItem *parent, Utils::StoredType type);
    void setParameters(const QMap<Utils::AppsType, Utils::importExportParameters> &params);

    [[nodiscard]] Utils::importExportParameters typeChecked(QTreeWidgetItem *parent) const;
    void initializeSubItem(QTreeWidgetItem *item, Utils::StoredTypes types);
    void loadFileName(const QString &fileName);
    [[nodiscard]] QString templateSelectionToString();
    bool removeNotSelectedItem(QTreeWidgetItem *parent);

    QTreeWidgetItem *mKmailItem = nullptr;
    QTreeWidgetItem *mKalarmItem = nullptr;
    QTreeWidgetItem *mKaddressbookItem = nullptr;
    QTreeWidgetItem *mKorganizerItem = nullptr;
    QTreeWidgetItem *mKNotesItem = nullptr;
    QTreeWidgetItem *mAkregatorItem = nullptr;
};
