cmake_minimum_required(VERSION 3.10)

set(project_name                  "arabica")
set(dir_app                       "app")
set(dir_emulator                  "arabica")
set(dir_test                      "test")
set(dir_vcpkg                     "~/vcpkg")
set(CMAKE_TOOLCHAIN_FILE          "${dir_vcpkg}/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_STANDARD            17)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${project_name})

find_package(fmt   CONFIG REQUIRED)
find_package(SDL2  CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

include_directories("${CMAKE_CURRENT_LIST_DIR}")

file(GLOB_RECURSE src_app      "${dir_app}/*.cpp")
file(GLOB_RECURSE src_emulator "${dir_emulator}/*.cpp")
file(GLOB_RECURSE src_test     "${dir_test}/*.cpp")

OPTION(BUILD_APP  "Build App"  OFF)
OPTION(BUILD_TEST "Build Test" OFF)
OPTION(BUILD_WASM "Build WASM" OFF)

IF(BUILD_APP)
  add_library(${dir_emulator} ${src_emulator})

  target_compile_options(${dir_emulator} PUBLIC -g)
  target_compile_options(${dir_emulator} PUBLIC -O0)

  target_link_libraries(${dir_emulator} PUBLIC -lm)
  target_link_libraries(${dir_emulator} PUBLIC fmt::fmt)
  target_link_libraries(${dir_emulator} PUBLIC SDL2::SDL2)

  add_executable(${project_name}.out ${src_app})

  target_compile_options(${project_name}.out PRIVATE -g)
  target_compile_options(${project_name}.out PRIVATE -O0)

  target_link_libraries(${project_name}.out PUBLIC ${dir_emulator})
ENDIF(BUILD_APP)

IF(BUILD_TEST)
  add_library(${dir_emulator} ${src_emulator})

  target_compile_options(${dir_emulator} PUBLIC -g)
  target_compile_options(${dir_emulator} PUBLIC -O0)

  target_link_libraries(${dir_emulator} PUBLIC -lm)
  target_link_libraries(${dir_emulator} PUBLIC fmt::fmt)
  target_link_libraries(${dir_emulator} PUBLIC SDL2::SDL2)

  add_executable(test_${project_name}.out ${src_test})

  target_compile_options(test_${project_name}.out PRIVATE -g)
  target_compile_options(test_${project_name}.out PRIVATE -O0)
  
  target_link_libraries(test_${project_name}.out PUBLIC ${dir_emulator}
                                                        GTest::gtest 
                                                        GTest::gtest_main 
                                                        GTest::gmock 
                                                        GTest::gmock_main)
ENDIF(BUILD_TEST)

IF(BUILD_WASM)
  set(CMAKE_CXX_COMPILER em++)
  set(CMAKE_C_COMPILER   emcc)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  add_library(${dir_emulator} ${src_emulator})

  target_compile_options(${dir_emulator} PUBLIC -g)
  target_compile_options(${dir_emulator} PUBLIC -O0)

  target_link_libraries(${dir_emulator} PUBLIC "-s WASM=1 -s USE_SDL=2 -s -O3 --preload-file ../roms@/roms")

  add_executable(${project_name}_wasm ${src_app})

  target_compile_options(${project_name}_wasm PRIVATE -O3)
  
  target_link_libraries(${project_name}_wasm PUBLIC ${dir_emulator})

  set_target_properties(${project_name}_wasm PROPERTIES LINK_FLAGS "-s WASM=1 -s USE_SDL=2 -s -O3 --preload-file ../roms@/roms")
ENDIF(BUILD_WASM)