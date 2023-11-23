#
# Specify minimum CMake version and project name
#
cmake_minimum_required (VERSION 3.0)
project (opengl)

#
# CMake setup
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_VERBOSE_MAKEFILE 0) # 1 should be used for debugging
set (CMAKE_SUPPRESS_REGENERATION TRUE) # Suppresses ZERO_CHECK
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -std=c++11") # -Wextra 
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
endif()

#
# GLFW options
#
option (GLFW_INSTALL OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)
add_subdirectory (opengl/lib/glfw)

option (ASSIMP_INSTALL OFF)
option (ASSIMP_WARNINGS_AS_ERRORS OFF)
option (ASSIMP_BUILD_TESTS OFF)
add_subdirectory (opengl/lib/assimp)
#
# Set include paths
#
include_directories (opengl/src/
                     opengl/lib/glad/include/
                     opengl/lib/glfw/include/
                     opengl/lib/assimp/include
                     opengl/lib/glm/
                     opengl/lib/stb/)

#
# Add files
#
file (GLOB         LIB_SOURCES opengl/lib/glad/src/glad.c)
file (GLOB_RECURSE PROJECT_HEADERS opengl/src/*.hpp
                                   opengl/src/*.h)
file (GLOB_RECURSE PROJECT_SOURCES opengl/src/*.cpp
                                   opengl/src/*.cxx
                                   opengl/src/*.cc
                                   opengl/src/*.c)
file (GLOB_RECURSE PROJECT_SHADERS opengl/shaders/*.comp
                                   opengl/shaders/*.frag
                                   opengl/shaders/*.geom
                                   opengl/shaders/*.vert)
file (GLOB         PROJECT_CONFIGS CMakeLists.txt
                                   README.rst
                                  .gitignore
                                  .gitmodules)

#
# Organizing files
#
source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("shaders" FILES ${PROJECT_SHADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})
source_group ("lib" FILES ${LIB_SOURCES})

#
# Set executable and target link libraries
#
add_definitions (-DGLFW_INCLUDE_NONE
                 -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                                ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                                ${LIB_SOURCES})

target_link_libraries (${PROJECT_NAME}
                       glfw
                       assimp
                       ${GLFW_LIBRARIES}
                       ${GLAD_LIBRARIES})

set_target_properties (${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
