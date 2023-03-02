# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )



add_executable( exportmailjobinterfacetest)
target_sources( exportmailjobinterfacetest PRIVATE exportmailjobinterfacetest.cpp exportmailjobinterfacetestimpl.cpp
    exportmailjobinterfacetest.h exportmailjobinterfacetestimpl.h
    exportmailfolderattributejobtestimpl.h exportmailfolderattributejobtestimpl.cpp)

add_test(NAME exportmailjobinterfacetest COMMAND exportmailjobinterfacetest)
ecm_mark_as_test(exportmailjobinterfacetest)
target_link_libraries(exportmailjobinterfacetest Qt::Test pimdataexporterprivate KF${KF_MAJOR_VERSION}::ConfigCore test_resourceconverter KPim${KF_MAJOR_VERSION}::Mime)

#####


add_executable( importmailjobinterfacetest )
target_sources( importmailjobinterfacetest PRIVATE importmailjobinterfacetest.cpp
    importmailjobinterfacetestimpl.cpp
    importmailjobinterfacetest.h importmailjobinterfacetestimpl.h
    importmailfolderattributejobtestimpl.h importmailfolderattributejobtestimpl.cpp)

add_test(NAME importmailjobinterfacetest COMMAND importmailjobinterfacetest)
ecm_mark_as_test(importmailjobinterfacetest)
target_link_libraries(importmailjobinterfacetest Qt::Test pimdataexporterprivate KF${KF_MAJOR_VERSION}::ConfigCore KPim${KF_MAJOR_VERSION}::AkonadiCore KPim${KF_MAJOR_VERSION}::AkonadiMime test_resourceconverter KPim${KF_MAJOR_VERSION}::IdentityManagement KPim${KF_MAJOR_VERSION}::Mime)

#####
add_executable( exportmailfolderattributejobtest)
target_sources( exportmailfolderattributejobtest PRIVATE exportmailfolderattributejobtest.cpp exportmailfolderattributejobtest.h)
add_test(NAME exportmailfolderattributejobtest COMMAND exportmailfolderattributejobtest)
ecm_mark_as_test(exportmailfolderattributejobtest)
target_link_libraries(exportmailfolderattributejobtest Qt::Test pimdataexporterprivate KF${KF_MAJOR_VERSION}::ConfigCore)

#####
add_executable( importmailfolderattributejobtest)
target_sources( importmailfolderattributejobtest PRIVATE importmailfolderattributejobtest.cpp importmailfolderattributejobtest.h)
add_test(NAME importmailfolderattributejobtest COMMAND importmailfolderattributejobtest)
ecm_mark_as_test(importmailfolderattributejobtest)
target_link_libraries(importmailfolderattributejobtest Qt::Test pimdataexporterprivate KPim${KF_MAJOR_VERSION}::AkonadiCore KF${KF_MAJOR_VERSION}::ConfigCore)

