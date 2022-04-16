cmake_minimum_required(VERSION 3.16)
project(fractualiser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

file(
  GLOB_RECURSE SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/*.c
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)

file(
  GLOB_RECURSE HEADER_FILES
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/src/*.hpp
)

add_library(GLAD "thirdparty/glad.c")

include_directories(
  "${CMAKE_SOURCE_DIR}/include/glad"
  "${CMAKE_SOURCE_DIR}/include"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(fractualiser glfw OpenGL::GL GLAD ${CMAKE_DL_LIBS})
