add_definitions( -DPIMDATAEXPORTER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

set( exportakregatorjobinterfacetest_SRCS exportakregatorjobinterfacetest.cpp ../resourceconvertertest.cpp )

add_executable( exportakregatorjobinterfacetest ${exportakregatorjobinterfacetest_SRCS})
add_test(NAME exportakregatorjobinterfacetest COMMAND exportakregatorjobinterfacetest)
ecm_mark_as_test(exportakregatorjobinterfacetest)
target_link_libraries(exportakregatorjobinterfacetest Qt5::Test pimdataexporterprivate KF5::ConfigCore)
