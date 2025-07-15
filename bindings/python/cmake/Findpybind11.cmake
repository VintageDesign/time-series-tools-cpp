# FindPython3 sets Python3_EXECUTABLE but pybind11 uses PYTHON_EXECUTABLE to
# find Python Libs, and it's liable to find python2 if both versions are
# installed. Setting PYTHON_EXECUTABLE prevents linking against a different
# python version
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE})

# Try to find pybind11 in the host environment
if(NOT pybind11_FOUND)
  # When pybind11 is installed on a system, it adds a pybind11Config.cmake file
  # under site-packages or dist-packages. Unfortunately, these aren't standard
  # locations for cmake to look for things. This will add all the package
  # directories that python is aware of to the prefix path when searching
  execute_process(COMMAND ${PYTHON_EXECUTABLE} -m site
                  OUTPUT_VARIABLE PYTHON_SITE_OUTPUT)

  if(PYTHON_SITE_OUTPUT)
    # Convert the output from python to a cmake list, one line per item
    #
    # CMake regex won't match [] literals properly, so we have to replace them
    # separately :(
    string(REPLACE "[" " " PYTHON_SITE_OUTPUT ${PYTHON_SITE_OUTPUT})
    string(REPLACE "]" " " PYTHON_SITE_OUTPUT ${PYTHON_SITE_OUTPUT})
    string(REGEX REPLACE "[\t\r\n ]+" ";" PYTHON_SITE_OUTPUT
                         ${PYTHON_SITE_OUTPUT})

    set(FIND_HINTS "")

    list(LENGTH PYTHON_SITE_OUTPUT LENGTH)
    # Figure out which lines match */site-packages or */dist-packages
    foreach(LINE IN LISTS PYTHON_SITE_OUTPUT)
      if(LINE)
        string(REGEX MATCH "'(.*-packages)',?" FIND_HINT ${LINE})
        if(FIND_HINT)
          list(APPEND FIND_HINTS ${CMAKE_MATCH_1})
        endif()
      endif()
    endforeach()
  endif()

  message(STATUS "Searching for Pybind with find hints ${FIND_HINTS}")

  if(${pybind11_FIND_VERSION_EXACT})
    set(pybind11_exact EXACT)
  endif()
  find_package(
    pybind11
    ${pybind11_FIND_VERSION}
    ${pybind11_exact}
    CONFIG
    QUIET
    HINTS
    ${FIND_HINTS})

  if(pybind11_FOUND)
    message(STATUS "pybind11 found in host environment")
  endif()
endif()

# If not found in the host environment, then add the subdirectory for it
if(NOT TARGET pybind11::pybind11)
  if(NOT pybind11_ROOT)
    set(pybind11_ROOT ${PROJECT_SOURCE_DIR}/submodules/pybind11)
  endif()
  message(STATUS "Adding pybind11 submodule ${pybind11_ROOT}")

  # Add pybind
  set(PYBIND11_INSTALL OFF)
  add_subdirectory(${pybind11_ROOT} pybind11)
endif()
