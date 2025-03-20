cmake_minimum_required(VERSION 3.30)
project(ccpp)
file(GLOB LIB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# remove main.cpp from library resources
set(CMAKE_CXX_STANDARD 23)
list(FILTER LIB_SOURCE_FILES EXCLUDE REGEX ".*main\\.cpp$")
add_library(ccpp_lib ${LIB_SOURCE_FILES})
target_include_directories(ccpp_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
add_executable(ccpp "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
target_link_libraries(ccpp PRIVATE ccpp_lib)

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