file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE GTest::gtest_main GTest::gmock pyxx_lib)
include(GoogleTest)
gtest_discover_tests(unit_tests)

configure_file(lit/lit.site.cfg.py.in lit/lit.site.cfg.py @ONLY)

add_custom_target(lit 
  COMMAND lit "${CMAKE_CURRENT_BINARY_DIR}/lit" -v
  DEPENDS pyxx)