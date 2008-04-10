project(kdebase4-SuSE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)
include(MacroOptionalAddSubdirectory)

macro_optional_add_subdirectory(greeter)
macro_optional_add_subdirectory(sysinfo)
macro_optional_add_subdirectory(krpmview)
macro_optional_add_subdirectory(ksplash)
