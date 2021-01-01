/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SELECTIONTYPETREEWIDGET_H
#define SELECTIONTYPETREEWIDGET_H

#include <QTreeWidget>
#include <QHash>
#include "utils.h"

class QTreeWidgetItem;

class SelectionTypeTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SelectionTypeTreeWidget(bool backupData, QWidget *parent = nullptr);
    ~SelectionTypeTreeWidget();

    Q_REQUIRED_RESULT QHash<Utils::AppsType, Utils::importExportParameters> storedType() const;

    void selectAllItems();
    void unSelectAllItems();

    void saveAsTemplate();

    void loadTemplate(const QString &fileName = QString());
    void saveAsDefaultTemplate();
    void loadDefaultTemplate();
    Q_REQUIRED_RESULT QString exportedFileInfo();
    void removeNotSelectedItems();

private:
    void slotItemChanged(QTreeWidgetItem *, int);

    enum ActionType {
        action = Qt::UserRole + 1
    };

    void initialize(bool backupData);
    void setSelectItems(bool b);
    void changeState(QTreeWidgetItem *item, bool b);
    void createSubItem(QTreeWidgetItem *parent, Utils::StoredType type);
    void setParameters(const QHash<Utils::AppsType, Utils::importExportParameters> &params);

    Q_REQUIRED_RESULT Utils::importExportParameters typeChecked(QTreeWidgetItem *parent) const;
    void initializeSubItem(QTreeWidgetItem *item, Utils::StoredTypes types);
    void loadFileName(const QString &fileName);
    Q_REQUIRED_RESULT QString templateSelectionToString();
    bool removeNotSelectedItem(QTreeWidgetItem *parent);

    QTreeWidgetItem *mKmailItem = nullptr;
    QTreeWidgetItem *mKalarmItem = nullptr;
    QTreeWidgetItem *mKaddressbookItem = nullptr;
    QTreeWidgetItem *mKorganizerItem = nullptr;
    QTreeWidgetItem *mKNotesItem = nullptr;
    QTreeWidgetItem *mAkregatorItem = nullptr;
};

#endif // SELECTIONTYPETREEWIDGET_H
