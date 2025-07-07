add_library(time-series-tools)

list(APPEND DIRS ".")
list(TRANSFORM DIRS APPEND "/*.cpp" OUTPUT_VARIABLE SOURCE_GLOBS)
list(TRANSFORM DIRS APPEND "/*.h" OUTPUT_VARIABLE HEADER_GLOBS)

file(GLOB sources CONFIGURE_DEPENDS ${SOURCE_GLOBS})
target_sources(time-series-tools PRIVATE ${sources})

file(GLOB headers CONFIGURE_DEPENDS ${HEADER_GLOBS})
target_sources(time-series-tools PUBLIC ${headers})

target_include_directories(time-series-tools
                           PUBLIC ${PROJECT_SOURCE_DIR}/include)
