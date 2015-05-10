/*
  Copyright (c) 2012-2015 Montel Laurent <montel@kde.org>
  
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SELECTIONTYPEDIALOG_H
#define SELECTIONTYPEDIALOG_H

#include <KDialog>
#include "utils.h"
#include "pimsettingexporter_export.h"
class SelectionTypeTreeWidget;

class PIMSETTINGEXPORT_EXPORT SelectionTypeDialog : public KDialog
{
    Q_OBJECT
public:
    explicit SelectionTypeDialog(QWidget * parent=0);
    ~SelectionTypeDialog();

    QHash<Utils::AppsType, Utils::importExportParameters> storedType() const;

    void loadTemplate(const QString &fileName);

private Q_SLOTS:
    void slotSelectAll();
    void slotUnselectAll();

    void slotSaveAsTemplate();
    void slotLoadTemplate();
private:
    void readConfig();
    void writeConfig();
    SelectionTypeTreeWidget *mSelectionTreeWidget;
};

#endif // SELECTIONTYPEDIALOG_H
