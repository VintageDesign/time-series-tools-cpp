cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build lists of all the files we need for this binding
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB HEADER_FILES CONFIGURE_DEPENDS "include/time_series_tools/*.h")
file(GLOB PYTHON_FILES CONFIGURE_DEPENDS "bindings/python/*.cpp"
     "bindings/python/*.h")
file(GLOB PYTHON_TEST_FILES "bindings/python/test/*.py")

# Set up such that XCode organizes the files
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES}
                                         ${PYTHON_FILES} ${PYTHON_TEST_FILES})

find_package(pybind11 REQUIRED)

# Add the time_series_tools Python module
pybind11_add_module(time_series_tools_python ${SOURCE_FILES})

target_include_directories(
  time_series_tools_python
  PRIVATE ${PROJECT_SOURCE_DIR}/bindings/python/include)

# If pybind11 is installed in the system, but we don't use it because it's tool
# old of a version, then we need to make sure it's in the include paths before
# the python site-packages directory because otherwise #include
# <pybind11/pybind11.h> finds the version that's installed in the system instead
# of the submodule, and if it's too old of a version of pybind, it may not build
if(TARGET pybind11::headers)
  target_include_directories(
    time_series_tools_python
    PRIVATE SYSTEM BEFORE ${PROJECT_SOURCE_DIR}/submodules/pybind11/include)
endif()

# pybind11 code internally forces hidden visibility on all internal code, but if
# non-hidden (and thus exported) code attempts to include a pybind type (for
# example, py::object or py::list) you can run into the warning that ‘SomeClass’
# declared with greater visibility than the type of its field
# ‘SomeClass::member’ [-Wattributes]”.
target_compile_options(time_series_tools_python PRIVATE -fvisibility=hidden)

# The pybind11_add_module sets both the output name and the suffix. In order for
# python to find this library, the filename must simply be time_series_tools and
# then one of the recognized file extensions.
set_target_properties(time_series_tools_python
                      PROPERTIES OUTPUT_NAME "time_series_tools" SUFFIX ".so")

target_link_libraries(time_series_tools_python PRIVATE time-series-tools)

if(TIME_SERIES_TOOLS_TESTS)
  add_subdirectory(test)
endif()

# We don't set the platform name when we call setup.py, so it's always a
# `none-any` wheel. That could probably be improved, but for now we only provide
# the wheel target as a convenience for developers - the production build
# invokes setup.py manually
set(TIME_SERIES_TOOLS_WHEEL_VERSION ${TIME_SERIES_TOOLS_VERSION})
set(WHEEL_OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/dist/time_series_tools-${TIME_SERIES_TOOLS_WHEEL_VERSION}-py3-none-any.whl
)

set(SETUP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/setup.py)
set(INIT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py)

set(SETUP_FILE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
set(INIT_FILE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)

# Build two lists - files that are needed to build the wheel, and files that
# belong in the wheel. The contents of the wheel do not need to be absolute
# paths; they are expected to all end up directly in CMAKE_CURRENT_BINARY_DIR
list(APPEND WHEEL_CONTENTS "time_series_tools.so" ${INIT_FILE_OUTPUT})
list(APPEND WHEEL_DEPENDENCIES $<TARGET_FILE:time_series_tools_python>
     ${INIT_FILE_OUTPUT} ${SETUP_FILE_OUTPUT})

# Convert the list of wheel files (which are full paths), to a list of
# comma-separated filenames that can be substituted into setup.py. These paths
# are relative to ${CMAKE_CURRENT_BINARY_DIR}, and since all the files that go
# in the wheel are placed directly in that directory, we can just strip the
# paths entirely
set(TIME_SERIES_TOOLS_WHEEL_DATA_FILES "")
foreach(FILE IN LISTS WHEEL_CONTENTS)
  get_filename_component(FILE_NAME ${FILE} NAME)
  list(APPEND TIME_SERIES_TOOLS_WHEEL_DATA_FILES \"${FILE_NAME}\")
endforeach()
string(REPLACE ";" ", " TIME_SERIES_TOOLS_WHEEL_DATA_FILES
               "${TIME_SERIES_TOOLS_WHEEL_DATA_FILES}")

configure_file(${SETUP_FILE} ${SETUP_FILE_OUTPUT} @ONLY)
configure_file(${INIT_FILE} ${INIT_FILE_OUTPUT} COPYONLY)

# This custom target generates a python wheel that can be installed using pip
# for integration with python-based projects
add_custom_command(
  OUTPUT ${WHEEL_OUTPUT}
  COMMAND ${Python3_EXECUTABLE} ${SETUP_FILE_OUTPUT} bdist_wheel
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating Python Wheel"
  DEPENDS ${WHEEL_DEPENDENCIES}
  USES_TERMINAL)

add_custom_target(time_series_tools-python-pip-package DEPENDS ${WHEEL_OUTPUT})

# File-level dependencies with DEPENDS in add_custom_target should handle this
# dependency, but for some reason that doesn't work when building with make
# (Ninja is fine)
add_dependencies(time_series_tools-python-pip-package time_series_tools_python)

if(TIME_SERIES_TOOLS_IS_MAIN_PROJECT)
  add_custom_target(pip-package)
  add_dependencies(pip-package time_series_tools-python-pip-package)
endif()
