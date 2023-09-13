cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(libKDU)

# multi-processor compilation
if(MSVC)
    include(ProcessorCount)
    ProcessorCount(PROCESSOR_COUNT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${PROCESSOR_COUNT}")
    set(CMAKE_VS_MSBUILD_COMMAND "${CMAKE_VS_MSBUILD_COMMAND} /p:CL_MPCount=${PROCESSOR_COUNT} /m")
endif()

enable_language(ASM_MASM)

# prepare files
file(COPY "KDU/Source" DESTINATION ".")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze/global.h" "\n#include <kdu_override.h>")

# generate resource files
add_executable(ResourceGenerator ResourceGenerator/ResourceGenerator.cpp)
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
add_library(Hamakaze STATIC ${HAMAKAZE_SOURCES})
target_include_directories(Hamakaze 
  PRIVATE ${HAMAKAZE_INCLUDE_DIRS}
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/Source/Hamakaze"
)
target_compile_definitions(Hamakaze PRIVATE UNICODE _UNICODE)
set_target_properties(Hamakaze PROPERTIES OUTPUT_NAME "kdu")

# export library
install(TARGETS Hamakaze)