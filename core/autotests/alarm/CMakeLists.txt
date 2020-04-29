add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportalarmjobinterfacetest_SRCS exportalarmjobinterfacetest.cpp)

add_executable( exportalarmjobinterfacetest ${exportalarmjobinterfacetest_SRCS})
add_test(NAME exportalarmjobinterfacetest COMMAND exportalarmjobinterfacetest)
ecm_mark_as_test(exportalarmjobinterfacetest)
target_link_libraries(exportalarmjobinterfacetest Qt5::Test pimdataexporterprivate KF5::ConfigCore test_resourceconverter)
