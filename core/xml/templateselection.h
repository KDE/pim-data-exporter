/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include "utils.h"
#include <QHash>
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
