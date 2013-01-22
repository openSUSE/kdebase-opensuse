project(kdebase4-SuSE)

cmake_minimum_required(VERSION 2.4.5)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)
include(MacroOptionalAddSubdirectory)

macro_optional_add_subdirectory(greeter)
#macro_optional_add_subdirectory(sysinfo)
#macro_optional_add_subdirectory(krpmview)
#macro_optional_add_subdirectory(migrate)
#macro_optional_add_subdirectory(po)
