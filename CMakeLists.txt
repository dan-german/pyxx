cmake_minimum_required(VERSION 3.30)
project(pyxx)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM targets available: ${LLVM_TARGETS_TO_BUILD}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Get required LLVM libraries
llvm_map_components_to_libnames(llvm_libs core support mc native)

file(GLOB LIB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# remove main.cpp from library resources
set(CMAKE_CXX_STANDARD 23)
list(FILTER LIB_SOURCE_FILES EXCLUDE REGEX ".*main\\.cpp$")

add_library(pyxx_lib ${LIB_SOURCE_FILES})
target_include_directories(pyxx_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(pyxx_lib PRIVATE ${llvm_libs})

add_executable(pyxx "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(pyxx PRIVATE pyxx_lib)

# setup unit testing
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_subdirectory(test)