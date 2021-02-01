add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportnotesjobinterfacetest_SRCS exportnotesjobinterfacetest.cpp exportnotesjobinterfacetestimpl.cpp)

add_executable( exportnotesjobinterfacetest ${exportnotesjobinterfacetest_SRCS})
add_test(NAME exportnotesjobinterfacetest COMMAND exportnotesjobinterfacetest)
ecm_mark_as_test(exportnotesjobinterfacetest)
target_link_libraries( exportnotesjobinterfacetest Qt5::Test pimdataexporterprivate test_resourceconverter KF5::ConfigCore KF5::Archive KF5::AkonadiCore)
#######

set( importnotesjobinterfacetest_SRCS importnotesjobinterfacetest.cpp importnotesjobinterfacetestimpl.cpp)

add_executable( importnotesjobinterfacetest ${importnotesjobinterfacetest_SRCS})
add_test(NAME importnotesjobinterfacetest COMMAND importnotesjobinterfacetest)
ecm_mark_as_test(importnotesjobinterfacetest)
target_link_libraries( importnotesjobinterfacetest Qt5::Test pimdataexporterprivate test_resourceconverter KF5::ConfigCore KF5::Archive)
