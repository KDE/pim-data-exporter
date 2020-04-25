add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportnotesjobinterfacetest_SRCS exportnotesjobinterfacetest.cpp resourceconvertertest.cpp )

add_executable( exportnotesjobinterfacetest ${exportnotesjobinterfacetest_SRCS})
add_test(NAME exportnotesjobinterfacetest COMMAND exportnotesjobinterfacetest)
ecm_mark_as_test(exportnotesjobinterfacetest)
target_link_libraries( exportnotesjobinterfacetest Qt5::Test pimdataexporterprivate KF5::ConfigCore)
