set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(entity-precompiler
    entity_precompiler.cc
)

target_link_libraries(entity-precompiler PUBLIC cpp-parser)