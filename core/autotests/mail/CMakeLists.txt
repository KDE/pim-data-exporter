# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportmailjobinterfacetest_SRCS exportmailjobinterfacetest.cpp exportmailjobinterfacetestimpl.cpp)

add_executable( exportmailjobinterfacetest ${exportmailjobinterfacetest_SRCS})
add_test(NAME exportmailjobinterfacetest COMMAND exportmailjobinterfacetest)
ecm_mark_as_test(exportmailjobinterfacetest)
target_link_libraries(exportmailjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter KF5::Mime)

#####

set( importmailjobinterfacetest_SRCS importmailjobinterfacetest.cpp importmailjobinterfacetestimpl.cpp)

add_executable( importmailjobinterfacetest ${importmailjobinterfacetest_SRCS})
add_test(NAME importmailjobinterfacetest COMMAND importmailjobinterfacetest)
ecm_mark_as_test(importmailjobinterfacetest)
target_link_libraries(importmailjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore KF5::AkonadiMime test_resourceconverter KF5::IdentityManagement KF5::Mime)
