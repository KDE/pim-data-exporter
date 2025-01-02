/*
   SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include "utils.h"
#include <QMap>
class QXmlStreamWriter;
class QXmlStreamReader;
class PIMDATAEXPORTER_EXPORT TemplateSelection
{
public:
    TemplateSelection();
    ~TemplateSelection();

    void createTemplate(const QMap<Utils::AppsType, Utils::importExportParameters> &stored);

    [[nodiscard]] QMap<Utils::AppsType, Utils::importExportParameters> loadTemplate(const QString &path);

    [[nodiscard]] QString saveTemplate() const;

private:
    [[nodiscard]] Utils::StoredTypes loadStoredTypes(int &numberOfStep);
    void saveParameters(Utils::StoredTypes type);
    QXmlStreamWriter *mStreamWriter = nullptr;
    QXmlStreamReader *mStreamReader = nullptr;
    QString mSaveTemplate;
};
