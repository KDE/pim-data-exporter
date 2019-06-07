add_definitions(-DPIMDATAEXPORTER_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

set( pimsettingexporter_kcfg_SRCS ../settings/pimsettingexporterglobalconfig.kcfgc )

kconfig_add_kcfg_files(pimsettingexporter_kcfg_SRCS ${pimsettingexporter_kcfg_SRCS})

add_executable( synchronizeresourcedialogtest synchronizeresourcedialogtest.cpp ../dialog/synchronizeresourcedialog.cpp)
add_test(NAME synchronizeresourcedialogtest COMMAND synchronizeresourcedialogtest)
ecm_mark_as_test(synchronizeresourcedialogtest)
target_link_libraries(synchronizeresourcedialogtest Qt5::Test Qt5::Widgets KF5::I18n KF5::ItemViews KF5::ConfigCore)

add_executable( pimsettingexporterconfiguredialogtest pimsettingexporterconfiguredialogtest.cpp ../dialog/pimsettingexporterconfiguredialog.cpp ../widgets/pimsettingexporterconfigurewidget.cpp ${pimsettingexporter_kcfg_SRCS})
add_test(NAME pimsettingexporterconfiguredialogtest COMMAND pimsettingexporterconfiguredialogtest)
ecm_mark_as_test(pimsettingexporterconfiguredialogtest)
target_link_libraries(pimsettingexporterconfiguredialogtest Qt5::Test Qt5::Widgets KF5::I18n KF5::ItemViews KF5::ConfigCore KF5::ConfigGui)


add_executable( pimsettingexporterconfigurewidgettest pimsettingexporterconfigurewidgettest.cpp ../widgets/pimsettingexporterconfigurewidget.cpp ${pimsettingexporter_kcfg_SRCS})
add_test(NAME pimsettingexporterconfigurewidgettest COMMAND pimsettingexporterconfigurewidgettest)
ecm_mark_as_test(pimsettingexporterconfigurewidgettest)
target_link_libraries(pimsettingexporterconfigurewidgettest Qt5::Test Qt5::Widgets KF5::I18n KF5::ItemViews KF5::ConfigCore KF5::ConfigGui)


add_executable( selectiontypetreewidgettest selectiontypetreewidgettest.cpp ../widgets/selectiontypetreewidget.cpp ${pimsettingexporter_kcfg_SRCS})
add_test(NAME selectiontypetreewidgettest COMMAND selectiontypetreewidgettest)
ecm_mark_as_test(selectiontypetreewidgettest)
target_link_libraries(selectiontypetreewidgettest Qt5::Test Qt5::Widgets KF5::I18n KF5::ItemViews pimdataexporterprivate KF5::ConfigCore KF5::ConfigGui KF5::PimCommonAkonadi)


add_executable( selectiontypedialogtest selectiontypedialogtest.cpp ../widgets/selectiontypetreewidget.cpp ../dialog/selectiontypedialog.cpp ${pimsettingexporter_kcfg_SRCS})
add_test(NAME selectiontypedialogtest COMMAND selectiontypedialogtest)
ecm_mark_as_test(selectiontypedialogtest)
target_link_libraries(selectiontypedialogtest Qt5::Test Qt5::Widgets KF5::I18n KF5::ItemViews pimdataexporterprivate KF5::ConfigCore KF5::ConfigGui KF5::PimCommonAkonadi)
