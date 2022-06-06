cmake_minimum_required(VERSION 3.1)
project(n-body)
set(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
set(PROJECT_LIBRARIES_DIR lib)
set(PROJECT_LIBRARIES_COMP glfw fmt glm) # Compiled libs

file(GLOB PROJECT_SOURCES src/*.cpp src/render/*.cpp src/model/*.cpp)
file(GLOB PROJECT_HEADERS src/*.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(${PROJECT_LIBRARIES_DIR}/stb)
include_directories(src/shaders src/textures)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

foreach(LIBRARY ${PROJECT_LIBRARIES_COMP})
    add_subdirectory(${PROJECT_LIBRARIES_DIR}/${LIBRARY})
endforeach(LIBRARY)

# Add glad
add_subdirectory(${PROJECT_LIBRARIES_DIR}/glad/cmake glad_cmake)
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} lib/stb)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${PROJECT_LIBRARIES_COMP} glad_gl_core_33)
