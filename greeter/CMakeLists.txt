add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories ( ${KDE4_INCLUDES})

set(greeter_SRCS main.cpp greetings.cpp)

kde4_add_executable(SUSEgreeter ${greeter_SRCS})

target_link_libraries(SUSEgreeter ${KDE4_KDEUI_LIBS})

install(TARGETS SUSEgreeter DESTINATION ${BIN_INSTALL_DIR})

install( FILES greet_header.html greet_section.html greet_footer.html greet.xml
         DESTINATION ${DATA_INSTALL_DIR}/SUSEgreeter )

install(FILES SUSEgreeter.desktop DESTINATION ${AUTOSTART_INSTALL_DIR})

install( FILES SUSEgreeter.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )

add_subdirectory( pics )
