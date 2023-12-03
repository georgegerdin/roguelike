set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(game
        game.cc
        entities.hh
        )

add_dependencies(game all_generated)

target_include_directories(game PUBLIC ${CMAKE_BINARY_DIR})
