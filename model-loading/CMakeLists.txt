cmake_minimum_required(VERSION 3.22)
project(model-loading)

set(CMAKE_CXX_STANDARD 17)

set(COMMONPATH ${CMAKE_SOURCE_DIR}/../common/)
set(GLADPATH ${CMAKE_SOURCE_DIR}/../common/glad)
set(GLMPATH ${CMAKE_SOURCE_DIR}/../common/glm)
set(ASSIMPPATH ${CMAKE_SOURCE_DIR}/../common/assimp)

find_library(GLFW_LIBRARY glfw)

include_directories(${CMAKE_SOURCE_DIR}
        /opt/homebrew/include
        ${GLADPATH}/include
        ${COMMONPATH}
        ${GLMPATH}
        ${ASSIMPPATH}/include)

add_executable(${PROJECT_NAME}
        main.cpp glad.c shader.h mesh.h model.h)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMPPATH}/bin/libassimp.dylib)