# SPDX-FileCopyrightText: 2011-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_definitions(-DPIMDATAEXPORTER_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

set( pimdataexporter_kcfg_SRCS ../settings/pimdataexporterglobalconfig.kcfgc )

kconfig_add_kcfg_files(pimdataexporter_kcfg_SRCS ${pimdataexporter_kcfg_SRCS})

add_executable( synchronizeresourcedialogtest synchronizeresourcedialogtest.cpp synchronizeresourcedialogtest.h ../dialog/synchronizeresourcedialog.cpp)
add_test(NAME synchronizeresourcedialogtest COMMAND synchronizeresourcedialogtest)
ecm_mark_as_test(synchronizeresourcedialogtest)
target_link_libraries(synchronizeresourcedialogtest Qt::Test Qt::Widgets KF5::I18n KF5::ItemViews KF5::ConfigGui)

set(pimdataexporter_userfeedback_LIB)
if (TARGET KUserFeedbackWidgets)
        set(pimdataexporter_userfeedback_LIB KUserFeedbackWidgets)
endif()

#add_executable( pimdataexporterconfiguredialogtest pimdataexporterconfiguredialogtest.cpp ../dialog/pimdataexporterconfiguredialog.cpp ../widgets/pimdataexporterconfigurewidget.cpp ${pimdataexporter_kcfg_SRCS})
#add_test(NAME pimdataexporterconfiguredialogtest COMMAND pimdataexporterconfiguredialogtest)
#ecm_mark_as_test(pimdataexporterconfiguredialogtest)
#target_link_libraries(pimdataexporterconfiguredialogtest ${pimdataexporter_userfeedback_LIB} Qt::Test Qt::Widgets KF5::I18n KF5::ItemViews KF5::ConfigCore KF5::ConfigGui KF5::WidgetsAddons)


add_executable( pimdataexporterconfigurewidgettest pimdataexporterconfigurewidgettest.cpp pimdataexporterconfigurewidgettest.h
    ../widgets/pimdataexporterconfigurewidget.cpp ${pimdataexporter_kcfg_SRCS})
add_test(NAME pimdataexporterconfigurewidgettest COMMAND pimdataexporterconfigurewidgettest)
ecm_mark_as_test(pimdataexporterconfigurewidgettest)
target_link_libraries(pimdataexporterconfigurewidgettest Qt::Test Qt::Widgets KF5::I18n KF5::ItemViews KF5::ConfigCore KF5::ConfigGui)


add_executable( selectiontypetreewidgettest selectiontypetreewidgettest.cpp selectiontypetreewidgettest.h
    ../widgets/selectiontypetreewidget.cpp ${pimdataexporter_kcfg_SRCS})
add_test(NAME selectiontypetreewidgettest COMMAND selectiontypetreewidgettest)
ecm_mark_as_test(selectiontypetreewidgettest)
target_link_libraries(selectiontypetreewidgettest Qt::Test Qt::Widgets KF5::I18n KF5::ItemViews pimdataexporterprivate KF5::ConfigCore KF5::ConfigGui KF5::PimCommonAkonadi)


add_executable( selectiontypedialogtest selectiontypedialogtest.h selectiontypedialogtest.cpp
    ../widgets/selectiontypetreewidget.cpp ../dialog/selectiontypedialog.cpp ${pimdataexporter_kcfg_SRCS})
add_test(NAME selectiontypedialogtest COMMAND selectiontypedialogtest)
ecm_mark_as_test(selectiontypedialogtest)
target_link_libraries(selectiontypedialogtest Qt::Test Qt::Widgets KF5::I18n KF5::ItemViews pimdataexporterprivate KF5::ConfigCore KF5::ConfigGui KF5::PimCommonAkonadi)
