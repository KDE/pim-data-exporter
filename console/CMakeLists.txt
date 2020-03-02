
include_directories(${AKONADI_INCLUDE_DIR} 
    ${pim-data-exporter_SOURCE_DIR}/core/
    )


set(pimdataexporterconsole_SRCS
    importexportprogressindicatorconsole.cpp
    pimdataexporterconsole.cpp
    loginfo.cpp
    loginfile.cpp
    main.cpp)


if (ECM_VERSION VERSION_LESS "5.68.0")
    ecm_qt_declare_logging_category(pimdataexporterconsole_SRCS HEADER pimdataexportconsole_debug.h IDENTIFIER PIMDATAEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimdataexporterconsole)
else()
    ecm_qt_declare_logging_category(pimdataexporterconsole_SRCS HEADER pimdataexportconsole_debug.h IDENTIFIER PIMDATAEXPORTERCONSOLE_LOG CATEGORY_NAME org.kde.pim.pimdataexporterconsole
        DESCRIPTION "pimdataexporter (pimdataexporter console)"
        OLD_CATEGORY_NAMES log_pimsettingexporterconsole org.kde.pim.pimsettingexporterconsole
        EXPORT PIMDATAEXPORTER
    )
endif()

add_executable(pimdataexporterconsole ${pimdataexporterconsole_SRCS})

target_link_libraries(pimdataexporterconsole KF5::ConfigCore KF5::CoreAddons KF5::I18n KF5::Archive KF5::AkonadiCore   KF5::MailCommon
    pimdataexporterprivate
    )
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
install(TARGETS pimdataexporterconsole ${INSTALL_TARGETS_DEFAULT_ARGS} )

