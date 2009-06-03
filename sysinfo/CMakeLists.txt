add_subdirectory(kio_sysinfo)

#process and install translations
find_package(Gettext REQUIRED)
file(GLOB po_files po/*.po)
set(_gmoFiles)
foreach (_current_PO_FILE ${po_files})
   get_filename_component(_basename ${_current_PO_FILE} NAME_WE)
   set(_lang ${_basename})
   set(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.gmo)
   add_custom_command(OUTPUT ${_gmoFile}
      COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_current_PO_FILE}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      DEPENDS ${_current_PO_FILE}
   )
   list(APPEND _gmoFiles ${_gmoFile})
   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.gmo DESTINATION ${LOCALE_INSTALL_DIR}/${_lang}/LC_MESSAGES/ RENAME kio_sysinfo.mo)
endforeach (_current_PO_FILE ${po_files})
add_custom_target(pofiles ALL DEPENDS ${_gmoFiles})
