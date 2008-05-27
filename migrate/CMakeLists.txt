include_directories(
    ${QT_INCLUDES}
)

set(kde4migrate_SRCS
    main.cpp
    QuickMigrate.cpp
)

set(interactivekde4migrate_SRC
    MigrationWizard.cpp
    AppsList.cpp
)

kde4_add_executable(kde4-migrate ${kde4migrate_SRCS})
target_link_libraries(kde4-migrate ${QT_QTCORE_LIBRARY})

install(TARGETS kde4-migrate DESTINATION ${BIN_INSTALL_DIR})
