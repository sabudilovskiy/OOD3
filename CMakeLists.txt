cmake_minimum_required(VERSION 3.20)
project(OOD3)
set(CMAKE_CXX_STANDARD 20)

include(conan_libraries/conan_paths.cmake)
find_package(GTest)
find_package(magic_enum)

add_library(${PROJECT_NAME}
        src/game.hpp
        src/game.cpp)

#Unit Tests
include(GoogleTest)
add_executable( ${PROJECT_NAME}_tests
        tests/basic.cpp)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        ${PROJECT_NAME}_objs)
#Main
add_executable(${PROJECT_NAME}_console console/main.cpp)
target_link_libraries(${PROJECT_NAME}_console PRIVATE ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_console PRIVATE magic_enum::magic_enum)
include_directories(src)