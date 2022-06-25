cmake_minimum_required(VERSION 3.21)
project(texture)

set(CMAKE_CXX_STANDARD 17)

set(COMMONPATH ${CMAKE_SOURCE_DIR}/../common/)
set(GLADPATH ${CMAKE_SOURCE_DIR}/../common/glad)

find_library(GLFW_LIBRARY glfw)

include_directories(${CMAKE_SOURCE_DIR}
        /opt/homebrew/include
        ${GLADPATH}/include
        ${COMMONPATH})

add_executable(${PROJECT_NAME}
        main.cpp
        glad.c)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIBRARY})


