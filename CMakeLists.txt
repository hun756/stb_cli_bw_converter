cmake_minimum_required(VERSION 3.15)
project(stb_cli_bw_converter VERSION 0.1 DESCRIPTION "C++ parallel black white image converter" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG v2.1.2
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
  FetchContent_Populate(stb)
  add_library(stb_image INTERFACE)
  target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11 stb_image)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Oz)
