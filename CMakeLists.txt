cmake_minimum_required(VERSION 3.2)

project(roguelike)

add_custom_target(all_generated)

add_subdirectory(extlibs)
add_subdirectory(src)

include(cmake/invoke_generator.cmake)

invoke_generator(
        "EntityPrecompiler"
        entity-precompiler
        "Entity_"
        "${CMAKE_CURRENT_LIST_DIR}/src/game/entities.hh"
        "${CMAKE_BINARY_DIR}/entities_meta.hh"
)