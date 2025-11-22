function(add_shaders TARGET_NAME)

    set(SHADER_SOURCE_FILES ${ARGN})
    list(LENGTH SHADER_SOURCE_FILES FILE_COUNT)
    if(FILE_COUNT EQUAL 0)
        message(FATAL_ERROR "Cannot add shaders target without shader source files.")
    endif()

    set(SHADER_PRODUCTS)

    foreach(SHADER_SOURCE IN LISTS SHADER_SOURCE_FILES)
        cmake_path(ABSOLUTE_PATH SHADER_SOURCE NORMALIZE)
        cmake_path(GET SHADER_SOURCE FILENAME SHADER_NAME)
        set(SHADER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv")

        # COMMANDS
        add_custom_command(
            OUTPUT ${SHADER_OUTPUT}
            COMMAND Vulkan::glslc "${SHADER_SOURCE}" -o "${SHADER_OUTPUT}"
            DEPENDS ${SHADER_SOURCE}
            COMMENT "Compiling shader: ${SHADER_NAME}"
        )
        # PRODUCTS
        list(APPEND SHADER_PRODUCTS ${SHADER_OUTPUT})
    endforeach()

    add_custom_target(${TARGET_NAME} ALL
        DEPENDS ${SHADER_PRODUCTS}
        COMMENT "Compiling all shaders..."
    )

endfunction()
