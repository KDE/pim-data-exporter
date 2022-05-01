# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )
add_definitions( -DPIMDATAINSTALL_DIR="${CMAKE_CURRENT_BINARY_DIR}/install" )

set( exportalarmjobinterfacetest_SRCS exportalarmjobinterfacetest.cpp exportalarmjobinterfacetestimpl.cpp
    exportalarmjobinterfacetest.h exportalarmjobinterfacetestimpl.h)

add_executable( exportalarmjobinterfacetest ${exportalarmjobinterfacetest_SRCS})
add_test(NAME exportalarmjobinterfacetest COMMAND exportalarmjobinterfacetest)
ecm_mark_as_test(exportalarmjobinterfacetest)
target_link_libraries(exportalarmjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)

#####

set( importalarmjobinterfacetest_SRCS importalarmjobinterfacetest.cpp importalarmjobinterfacetestimpl.cpp
    importalarmjobinterfacetest.h importalarmjobinterfacetestimpl.h)

add_executable( importalarmjobinterfacetest ${importalarmjobinterfacetest_SRCS})
add_test(NAME importalarmjobinterfacetest COMMAND importalarmjobinterfacetest)
ecm_mark_as_test(importalarmjobinterfacetest)
target_link_libraries(importalarmjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)
