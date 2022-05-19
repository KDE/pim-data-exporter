# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )


set( exportmailjobinterfacetest_SRCS exportmailjobinterfacetest.cpp exportmailjobinterfacetestimpl.cpp
    exportmailjobinterfacetest.h exportmailjobinterfacetestimpl.h)

add_executable( exportmailjobinterfacetest ${exportmailjobinterfacetest_SRCS})
add_test(NAME exportmailjobinterfacetest COMMAND exportmailjobinterfacetest)
ecm_mark_as_test(exportmailjobinterfacetest)
target_link_libraries(exportmailjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter KF5::Mime)

#####

set( importmailjobinterfacetest_SRCS importmailjobinterfacetest.cpp importmailjobinterfacetestimpl.cpp
    importmailjobinterfacetest.h importmailjobinterfacetestimpl.h)

add_executable( importmailjobinterfacetest ${importmailjobinterfacetest_SRCS})
add_test(NAME importmailjobinterfacetest COMMAND importmailjobinterfacetest)
ecm_mark_as_test(importmailjobinterfacetest)
target_link_libraries(importmailjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore KF5::AkonadiMime test_resourceconverter KF5::IdentityManagement KF5::Mime)

#####
set( exportmailfolderattributejobtest_SRCS exportmailfolderattributejobtest.cpp exportmailfolderattributejobtest.h)

add_executable( exportmailfolderattributejobtest ${exportmailfolderattributejobtest_SRCS})
add_test(NAME exportmailfolderattributejobtest COMMAND exportmailfolderattributejobtest)
ecm_mark_as_test(exportmailfolderattributejobtest)
target_link_libraries(exportmailfolderattributejobtest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate)
