cmake_minimum_required(VERSION 3.20)

project(ClockSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_library(glad STATIC
    glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

if(UNIX)
    target_link_libraries(glad PUBLIC dl)
endif()

include(FetchContent)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

add_executable(clock_sim
    src/main.cpp
    src/helper.cpp
)

target_include_directories(clock_sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(clock_sim PRIVATE
    glad        
    glfw
)

if(WIN32)
    target_link_libraries(clock_sim PRIVATE opengl32)
endif()

if(MSVC)
    target_compile_options(clock_sim PRIVATE /W4 /permissive-)
else()
    target_compile_options(clock_sim PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS clock_sim RUNTIME DESTINATION bin)
