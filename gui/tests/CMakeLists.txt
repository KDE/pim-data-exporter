# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include_directories(
    ${pim-data-exporter_SOURCE_DIR}/core/
    ${pim-data-exporter_BINARY_DIR}/core/
    ${pim-data-exporter_SOURCE_DIR}/gui/
    ${pim-data-exporter_BINARY_DIR}/gui/
)

set(selectiontreewidget_gui_SRCS
    selectiontreewidget_gui.cpp
    ../dialog/selectiontypedialog.cpp
    ../widgets/selectiontypetreewidget.cpp
    ../pimdataexportgui_debug.cpp
)
add_executable(selectiontreewidget_gui ${selectiontreewidget_gui_SRCS})
target_link_libraries(
    selectiontreewidget_gui
    KPim6::AkonadiCore
    KPim6::AkonadiPrivate
    KPim6::MailCommon
    KPim6::PimCommon
    KF6::Archive
    pimdataexporterprivate
    KF6::ItemViews
    KF6::I18n
)

set(showarchivestructuredialog_gui_SRCS
    showarchivestructuredialog_gui.cpp
    ../dialog/showarchivestructuredialog.cpp
    ../pimdataexportgui_debug.cpp
)
add_executable(showarchivestructuredialog_gui ${showarchivestructuredialog_gui_SRCS})
target_link_libraries(
    showarchivestructuredialog_gui
    KPim6::AkonadiCore
    KPim6::AkonadiPrivate
    KPim6::MailCommon
    KPim6::PimCommon
    KF6::Archive
    pimdataexporterprivate
    KF6::ItemViews
    KF6::I18n
    KF6::KIOWidgets
)
