project(krpmview)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(CMAKE_USE_RELATIVE_PATHS OFF)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(KDE_INSTALL_USE_QT_SYS_PATHS ON CACHE BOOL "Install mkspecs files, Plugins and Imports to the Qt 5 install dir" FORCE)

set(QT_MIN_VERSION "5.4.1")
set(VERSION_MAJOR "2")
set(VERSION_MINOR "0")
set(VERSION_RELEASE "0")
set(VERSION_SUFFIX "")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}${VERSION_SUFFIX})
add_definitions(-D'VERSION="${VERSION}"') #"

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})
include(ECMInstallIcons)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(GenerateExportHeader)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
  Gui
)

# Load the frameworks we need
find_package(KF5 5.15 REQUIRED
  KHtml
  KDELibs4Support
  I18n
  Parts
)


include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}  /usr/include/rpm)

set(krpmview_SRCS
   krpmview.cpp krpmview_factory.cpp
)

add_library(krpmview MODULE ${krpmview_SRCS})

target_link_libraries(krpmview -lrpm -lrpmio KF5::KHtml KF5::I18n KF5::KDELibs4Support KF5::Parts Qt5::Gui)

install(TARGETS krpmview DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES krpmview.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )

install( FILES konqfilemgr_rpm.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
