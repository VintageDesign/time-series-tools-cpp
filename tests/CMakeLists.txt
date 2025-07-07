if(BUILD_TESTS)
  message(STATUS "Time Series Tests are building")

  find_package(GTest REQUIRED)

  add_executable(time-series-tests)

  target_link_libraries(
    time-series-tests PRIVATE time-series-tools GTest::gtest GTest::gtest_main)

  list(APPEND DIRS ".")
  list(TRANSFORM DIRS APPEND "/*.cpp" OUTPUT_VARIABLE SOURCE_GLOBS)
  list(TRANSFORM DIRS APPEND "/*.h" OUTPUT_VARIABLE HEADER_GLOBS)

  file(GLOB sources CONFIGURE_DEPENDS ${SOURCE_GLOBS})
  target_sources(time-series-tests PRIVATE ${sources})

  file(GLOB headers CONFIGURE_DEPENDS ${HEADER_GLOBS})
  target_sources(time-series-tests PUBLIC ${headers})
endif()
