/*
   SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "utils.h"
#include <QDialog>
class SelectionTypeTreeWidget;
class QCheckBox;
class SelectionTypeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectionTypeDialog(bool backupData, QWidget *parent = nullptr);
    ~SelectionTypeDialog() override;

    [[nodiscard]] QMap<Utils::AppsType, Utils::importExportParameters> storedType() const;

    void loadTemplate(const QString &fileName);

    [[nodiscard]] QString exportedFileInfo() const;
    void removeNotSelectedItems();

private:
    void slotSelectAll();
    void slotUnselectAll();

    void slotSaveAsTemplate();
    void slotLoadTemplate();
    void readConfig();
    void writeConfig();
    void loadDefaultTemplate();
    void saveDefaultTemplate();
    SelectionTypeTreeWidget *const mSelectionTreeWidget;
    QCheckBox *const mUseTemplateByDefault;
    QPushButton *const mSaveTemplate;
    QPushButton *const mLoadTemplate;
};
