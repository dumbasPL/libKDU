cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(libKDU)

enable_language(ASM_MASM)

# prepare files
file(COPY "KDU/Source" DESTINATION ".")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze/global.h" "\n#include <kdu_override.h>")

# generate resource files
add_executable(ResourceGenerator ResourceGenerator/ResourceGenerator.cpp)
set_target_properties(ResourceGenerator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Resources/Hamakaze.cpp"
  COMMAND ResourceGenerator "${CMAKE_CURRENT_BINARY_DIR}/Resources/Hamakaze.cpp" "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze/resource.rc" "Hamakaze"
  DEPENDS ResourceGenerator
)
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Resources/Tanikaze.cpp"
  COMMAND ResourceGenerator "${CMAKE_CURRENT_BINARY_DIR}/Resources/Tanikaze.cpp" "${CMAKE_CURRENT_BINARY_DIR}/Source/Tanikaze/resource.rc" "Tanikaze"
  DEPENDS ResourceGenerator
)

# build kdu
file(GLOB_RECURSE HAMAKAZE_SOURCES
  "override/*.cpp"
  "public/*.cpp"
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Shared/*.cpp" 
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Shared/*.c"
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze/*.cpp" 
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze/*.c" 
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze/*.asm"
)
list(APPEND HAMAKAZE_SOURCES
  "${CMAKE_CURRENT_BINARY_DIR}/Resources/Hamakaze.cpp"
  "${CMAKE_CURRENT_BINARY_DIR}/Resources/Tanikaze.cpp"
)
list(REMOVE_ITEM HAMAKAZE_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze/main.cpp")
set(HAMAKAZE_INCLUDE_DIRS 
  "override"
  "${CMAKE_CURRENT_BINARY_DIR}/Source" 
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Shared" 
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze"
  "${CMAKE_CURRENT_BINARY_DIR}/Source/Tanikaze"
)
add_library(${PROJECT_NAME} STATIC ${HAMAKAZE_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
target_include_directories(${PROJECT_NAME} 
  PRIVATE ${HAMAKAZE_INCLUDE_DIRS}
  PUBLIC "public"
)

# export library
install(TARGETS ${PROJECT_NAME})