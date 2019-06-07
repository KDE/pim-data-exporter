
include_directories(${AKONADI_INCLUDE_DIR} 
    ${pim-data-exporter_SOURCE_DIR}/core/
    )


set(pimdataexporterconsole_SRCS
    importexportprogressindicatorconsole.cpp
    pimdataexporterconsole.cpp
    loginfo.cpp
    loginfile.cpp
    main.cpp)

ecm_qt_declare_logging_category(pimdataexporterconsole_SRCS HEADER pimdataexportconsole_debug.h IDENTIFIER PIMDATAEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimdataexporterconsole)

add_executable(pimsettingexporterconsole ${pimdataexporterconsole_SRCS})

target_link_libraries(pimsettingexporterconsole KF5::ConfigCore KF5::CoreAddons KF5::I18n KF5::Archive KF5::AkonadiCore   KF5::MailCommon
    pimdataexporterprivate
    )
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
install(TARGETS pimsettingexporterconsole ${INSTALL_TARGETS_DEFAULT_ARGS} )

