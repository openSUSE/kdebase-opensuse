project(krpmview)

# search packages used by KDE
find_package(KDE4 REQUIRED)
include (UsePkgConfig)
include (KDE4Defaults)
include (MacroLibrary)
include(MacroOptionalAddSubdirectory)

add_definitions (${QT_DEFINITIONS}${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} /usr/include/rpm)

set(krpmview_SRCS
   krpmview.cpp krpmview_factory.cpp
)

kde4_add_plugin(krpmview WITH_PREFIX ${krpmview_SRCS})

target_link_libraries(krpmview -lrpm -lrpmio ${KDE4_KHTML_LIBS} ${QT_QTGUI_LIBRARY})

install(TARGETS krpmview DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES krpmview.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )

install( FILES konqfilemgr_rpm.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
