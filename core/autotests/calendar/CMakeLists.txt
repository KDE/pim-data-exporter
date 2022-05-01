# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )
add_definitions( -DPIMDATAINSTALL_DIR="${CMAKE_CURRENT_BINARY_DIR}/install" )

set( exportcalendarsjobinterfacetest_SRCS exportcalendarsjobinterfacetest.cpp exportcalendarsjobinterfacetestimpl.cpp
    exportcalendarsjobinterfacetest.h exportcalendarsjobinterfacetestimpl.h)

add_executable( exportcalendarsjobinterfacetest ${exportcalendarsjobinterfacetest_SRCS})
add_test(NAME exportcalendarsjobinterfacetest COMMAND exportcalendarsjobinterfacetest)
ecm_mark_as_test(exportcalendarsjobinterfacetest)
target_link_libraries(exportcalendarsjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)

#####

set( importcalendarjobinterfacetest_SRCS importcalendarjobinterfacetest.cpp importcalendarjobinterfacetestimpl.cpp
    importcalendarjobinterfacetest.h importcalendarjobinterfacetestimpl.h)

add_executable( importcalendarjobinterfacetest ${importcalendarjobinterfacetest_SRCS})
add_test(NAME importcalendarjobinterfacetest COMMAND importcalendarjobinterfacetest)
ecm_mark_as_test(importcalendarjobinterfacetest)
target_link_libraries(importcalendarjobinterfacetest Qt${QT_MAJOR_VERSION}::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)
