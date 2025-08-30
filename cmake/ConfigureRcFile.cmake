function(configure_rc_file)
  string(CONCAT RC_VERSION
          "${PROJECT_VERSION_MAJOR}, "
          "${PROJECT_VERSION_MINOR}, "
          "${PROJECT_VERSION_PATCH}, "
          "0"
  )
  set(RC_NAME "${PACKAGE_TARNAME}.rc")
  set(RC_INPUT_FILE "${WIN_DIR}/${RC_NAME}.in")
  set(RC_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${RC_NAME}")

  configure_file(${RC_INPUT_FILE} ${RC_OUTPUT_FILE})
  target_sources(${PACKAGE_TARNAME} PUBLIC ${RC_OUTPUT_FILE})
endfunction()
