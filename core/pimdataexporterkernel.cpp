/*
   Copyright (C) 2012-2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "pimdataexporterkernel.h"

#include <KContacts/Addressee>
#include <KContacts/ContactGroup>

#include <kidentitymanagement/identitymanager.h>
#include <MessageComposer/AkonadiSender>
#include <MailCommon/FolderCollectionMonitor>
#include <AkonadiCore/session.h>
#include <AkonadiCore/entitytreemodel.h>
#include <AkonadiCore/entitymimetypefiltermodel.h>
#include <AkonadiCore/changerecorder.h>
#include <Akonadi/Notes/NoteUtils>
#include <KCalCore/Todo>
#include <KCalCore/Event>
#include <KSharedConfig>

//Same element as kalarmcal
const QLatin1String MIME_ACTIVE("application/x-vnd.kde.alarm.active");
const QLatin1String MIME_ARCHIVED("application/x-vnd.kde.alarm.archived");
const QLatin1String MIME_TEMPLATE("application/x-vnd.kde.alarm.template");

PimDataExporterKernel::PimDataExporterKernel(QObject *parent)
    : QObject(parent)
{
    mIdentityManager = KIdentityManagement::IdentityManager::self();
    Akonadi::Session *session = new Akonadi::Session("Backup Mail Kernel ETM", this);

    mFolderCollectionMonitor = new MailCommon::FolderCollectionMonitor(session, this);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KContacts::Addressee::mimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KContacts::ContactGroup::mimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KCalCore::Todo::todoMimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KCalCore::Event::eventMimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(Akonadi::NoteUtils::noteMimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(MIME_ACTIVE);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(MIME_ARCHIVED);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(MIME_TEMPLATE);

    mEntityTreeModel = new Akonadi::EntityTreeModel(folderCollectionMonitor(), this);
    mEntityTreeModel->setListFilter(Akonadi::CollectionFetchScope::Enabled);
    mEntityTreeModel->setItemPopulationStrategy(Akonadi::EntityTreeModel::LazyPopulation);

    mCollectionModel = new Akonadi::EntityMimeTypeFilterModel(this);
    mCollectionModel->setSourceModel(mEntityTreeModel);
    mCollectionModel->addMimeTypeInclusionFilter(Akonadi::Collection::mimeType());
    mCollectionModel->setHeaderGroup(Akonadi::EntityTreeModel::CollectionTreeHeaders);
    mCollectionModel->setDynamicSortFilter(true);
    mCollectionModel->setSortCaseSensitivity(Qt::CaseInsensitive);
}

KIdentityManagement::IdentityManager *PimDataExporterKernel::identityManager()
{
    return mIdentityManager;
}

MessageComposer::MessageSender *PimDataExporterKernel::msgSender()
{
    Q_ASSERT(false);
    return nullptr;
}

Akonadi::EntityMimeTypeFilterModel *PimDataExporterKernel::collectionModel() const
{
    return mCollectionModel;
}

KSharedConfig::Ptr PimDataExporterKernel::config()
{
    return KSharedConfig::openConfig();
}

void PimDataExporterKernel::syncConfig()
{
    Q_ASSERT(false);
}

MailCommon::JobScheduler *PimDataExporterKernel::jobScheduler() const
{
    Q_ASSERT(false);
    return nullptr;
}

Akonadi::ChangeRecorder *PimDataExporterKernel::folderCollectionMonitor() const
{
    return mFolderCollectionMonitor->monitor();
}

void PimDataExporterKernel::updateSystemTray()
{
    Q_ASSERT(false);
}

bool PimDataExporterKernel::showPopupAfterDnD()
{
    return false;
}

qreal PimDataExporterKernel::closeToQuotaThreshold()
{
    return 80;
}

QStringList PimDataExporterKernel::customTemplates()
{
    Q_ASSERT(false);
    return QStringList();
}

bool PimDataExporterKernel::excludeImportantMailFromExpiry()
{
    Q_ASSERT(false);
    return true;
}

Akonadi::Collection::Id PimDataExporterKernel::lastSelectedFolder()
{
    Q_ASSERT(false);
    return Akonadi::Collection::Id();
}

void PimDataExporterKernel::setLastSelectedFolder(Akonadi::Collection::Id col)
{
    Q_UNUSED(col);
}

void PimDataExporterKernel::expunge(Akonadi::Collection::Id col, bool sync)
{
    Q_UNUSED(col);
    Q_UNUSED(sync);
}
