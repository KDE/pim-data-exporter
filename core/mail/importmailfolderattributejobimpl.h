/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importmailfolderattributejob.h"
#include <Akonadi/Collection>
class KJob;
class ImportMailFolderAttributeJobImpl : public ImportMailFolderAttributeJob
{
    Q_OBJECT
public:
    explicit ImportMailFolderAttributeJobImpl(QObject *parent = nullptr);
    ~ImportMailFolderAttributeJobImpl() override;

protected:
    void applyAttributes(const QMap<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo> &map) override final;

private:
    void nextAttribute();
    void slotCollectionModifyDone(KJob *job);
    void collectionFetchResult(KJob *job);
    QScopedPointer<QMapIterator<Akonadi::Collection::Id, ImportExportMailUtil::AttributeInfo>> mIndexMap;
};
