# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include_directories(
    ${pim-data-exporter_SOURCE_DIR}/core/
    ${pim-data-exporter_BINARY_DIR}/core/
    ${pim-data-exporter_SOURCE_DIR}/gui/
    ${pim-data-exporter_BINARY_DIR}/gui/
    )

set(selectiontreewidget_gui_SRCS selectiontreewidget_gui.cpp ../dialog/selectiontypedialog.cpp ../widgets/selectiontypetreewidget.cpp ../pimdataexportgui_debug.cpp)
add_executable(selectiontreewidget_gui ${selectiontreewidget_gui_SRCS})
target_link_libraries(selectiontreewidget_gui
    KPim${KF_MAJOR_VERSION}::AkonadiCore
    KPim${KF_MAJOR_VERSION}::AkonadiPrivate
    KPim${KF_MAJOR_VERSION}::MailCommon
    KPim${KF_MAJOR_VERSION}::PimCommon
    KF${KF_MAJOR_VERSION}::Archive
    pimdataexporterprivate
    KF${KF_MAJOR_VERSION}::ItemViews
    KF${KF_MAJOR_VERSION}::I18n
    )

set(showarchivestructuredialog_gui_SRCS showarchivestructuredialog_gui.cpp ../dialog/showarchivestructuredialog.cpp ../pimdataexportgui_debug.cpp)
add_executable(showarchivestructuredialog_gui ${showarchivestructuredialog_gui_SRCS})
target_link_libraries(showarchivestructuredialog_gui
    KPim${KF_MAJOR_VERSION}::AkonadiCore
    KPim${KF_MAJOR_VERSION}::AkonadiPrivate
    KPim${KF_MAJOR_VERSION}::MailCommon
    KPim${KF_MAJOR_VERSION}::PimCommon
    KF${KF_MAJOR_VERSION}::Archive
    pimdataexporterprivate
    KF${KF_MAJOR_VERSION}::ItemViews
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::KIOWidgets
    )
