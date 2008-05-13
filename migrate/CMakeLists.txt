include_directories(
    ${KDE4_INCLUDES}
)

set(kde4migrate_SRCS
    MigrationWizard.cpp
    AppsList.cpp
    main.cpp
)

kde4_add_executable(kde4-migrate ${kde4migrate_SRCS})
target_link_libraries(kde4-migrate ${KDE4_KDEUI_LIBS})

install(TARGETS kde4-migrate DESTINATION ${BIN_INSTALL_DIR})
