function(invoke_generator name generator prefix header implementation)
    cmake_parse_arguments(invoke_generator "" "" "arguments" ${ARGN})

    add_custom_command(
            OUTPUT "${header}" "${implementation}"
            COMMAND $<TARGET_FILE:${generator}> "${header}.tmp" "${implementation}.tmp" ${invoke_generator_arguments}
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${implementation}.tmp" "${implementation}"
            VERBATIM
            DEPENDS ${generator}
    )

    add_custom_target("generate_${prefix}${name}" DEPENDS "${header}")
    add_dependencies(all_generated "generate_${prefix}${name}")
endfunction()