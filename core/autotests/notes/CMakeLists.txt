# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )
add_definitions( -DPIMDATAINSTALL_DIR="${CMAKE_CURRENT_BINARY_DIR}/install" )

set( exportnotesjobinterfacetest_SRCS exportnotesjobinterfacetest.cpp exportnotesjobinterfacetestimpl.cpp
    exportnotesjobinterfacetest.h exportnotesjobinterfacetestimpl.h)

add_executable( exportnotesjobinterfacetest ${exportnotesjobinterfacetest_SRCS})
add_test(NAME exportnotesjobinterfacetest COMMAND exportnotesjobinterfacetest)
ecm_mark_as_test(exportnotesjobinterfacetest)
target_link_libraries( exportnotesjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate test_resourceconverter KF5::ConfigCore KF5::Archive KF5::AkonadiCore)
#######

set( importnotesjobinterfacetest_SRCS importnotesjobinterfacetest.cpp importnotesjobinterfacetestimpl.cpp
    importnotesjobinterfacetest.h importnotesjobinterfacetestimpl.h)

add_executable( importnotesjobinterfacetest ${importnotesjobinterfacetest_SRCS})
add_test(NAME importnotesjobinterfacetest COMMAND importnotesjobinterfacetest)
ecm_mark_as_test(importnotesjobinterfacetest)
target_link_libraries( importnotesjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate test_resourceconverter KF5::ConfigCore KF5::Archive)
