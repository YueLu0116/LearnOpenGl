cmake_minimum_required(VERSION 3.21)
project(hello-window)

set(CMAKE_CXX_STANDARD 17)
set(GLADPATH ${CMAKE_SOURCE_DIR}/../common/glad)
message("glad: ${GLADPATH}")

find_library(GLFW_LIBRARY glfw)


include_directories(${CMAKE_SOURCE_DIR}
        /opt/homebrew/include
        ${GLADPATH}/include)

add_executable(${PROJECT_NAME}
        main.cpp
        glad.c)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIBRARY})


