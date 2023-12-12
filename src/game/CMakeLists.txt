cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(game
        game.cc
        entities.hh
        imgui_app_glfw.cpp
        )

add_dependencies(game all_generated)

target_include_directories(game PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(game PUBLIC imgui OpenGL::GL glfw GLEW::GLEW)
