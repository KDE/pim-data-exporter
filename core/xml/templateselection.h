/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TEMPLATESELECTION_H
#define TEMPLATESELECTION_H

#include "pimdataexporter_export.h"
#include <QHash>
#include "utils.h"
class QXmlStreamWriter;
class QXmlStreamReader;
class PIMDATAEXPORTER_EXPORT TemplateSelection
{
public:
    TemplateSelection();
    ~TemplateSelection();

    void createTemplate(const QHash<Utils::AppsType, Utils::importExportParameters> &stored);

    Q_REQUIRED_RESULT QHash<Utils::AppsType, Utils::importExportParameters> loadTemplate(const QString &path);

    Q_REQUIRED_RESULT QString saveTemplate() const;

private:
    Q_REQUIRED_RESULT Utils::StoredTypes loadStoredTypes(int &numberOfStep);
    void saveParameters(Utils::StoredTypes type);
    QXmlStreamWriter *mStreamWriter = nullptr;
    QXmlStreamReader *mStreamReader = nullptr;
    QString mSaveTemplate;
};

#endif // TEMPLATESELECTION_H
