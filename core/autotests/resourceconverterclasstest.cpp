/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "resourceconverterclasstest.h"
#include "resourceconvertertest.h"
#include "../utils.h"
#include <QTest>
QTEST_MAIN(ResourceConverterClassTest)

ResourceConverterClassTest::ResourceConverterClassTest(QObject *parent)
    : QObject(parent)
{
}

void ResourceConverterClassTest::shouldGetAgentFileName()
{
    ResourceConverterTest w;
    const QString filename = Utils::resourcesPath() + QStringLiteral("akonadi_imap_resource_0rc");
    QCOMPARE(w.agentFileName(filename), QStringLiteral("resources/agent_config_akonadi_imap_resource_0"));
}