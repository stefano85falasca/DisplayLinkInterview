cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(ramp LANGUAGES CXX)

IF(PEDANTIC MATCHES True)
    message("USING Wall and pedantic")
ENDIF(PEDANTIC MATCHES True)

IF(ASSERT_LEVEL MATCHES SAFE)
    message("ASSERT_LEVEL SAFE")
    add_definitions(-DPADS_ASSERT_SAFE_ENABLED)
ENDIF(ASSERT_LEVEL MATCHES SAFE)

IF(ASSERT_LEVEL MATCHES NORMAL)
    message("ASSERT_LEVEL NORMAL")
    add_definitions(-DPADS_ASSERT_ENABLED)
ENDIF(ASSERT_LEVEL MATCHES NORMAL)

IF(ASSERT_LEVEL MATCHES OPT)
    message("ASSERT_LEVEL OPT")
    add_definitions(-DPADS_ASSERT_OPT_ENABLED)
ENDIF(ASSERT_LEVEL MATCHES OPT)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("DEBUG MODE SELECTED")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

IF(CMAKE_BUILD_TYPE MATCHES DEVEL)
    message("DEVEL MODE SELECTED")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEVEL)

IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
    message("RELEASE MODE SELECTED")
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

IF(OPTIMIZE MATCHES True)
    message("OPTIMIZED BUILD")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3" )
ENDIF(OPTIMIZE MATCHES True)

IF(SANITIZE MATCHES ADDRESS)
    message("SANITIZE ADDRESS")
    add_definitions(-DSANITIZE_ADDRESS=On)
    add_definitions(-DSANITIZE_UNDEFINED=Off)
    add_definitions(-DSANITIZE_THREAD=Off)
    add_definitions(-DSANITIZE_MEMORY=Off)
ENDIF(SANITIZE MATCHES ADDRESS)

IF(SANITIZE MATCHES UNDEFINED)
    message("SANITIZE UNDEFINED")
    add_definitions(-DSANITIZE_ADDRESS=Off)
    add_definitions(-DSANITIZE_UNDEFINED=On)
    add_definitions(-DSANITIZE_THREAD=Off)
    add_definitions(-DSANITIZE_MEMORY=Off)
ENDIF(SANITIZE MATCHES UNDEFINED)

IF(SANITIZE MATCHES THREAD)
    message("SANITIZE THREAD")
    add_definitions(-DSANITIZE_ADDRESS=Off)
    add_definitions(-DSANITIZE_UNDEFINED=Off)
    add_definitions(-DSANITIZE_THREAD=On)
    add_definitions(-DSANITIZE_MEMORY=Off)
ENDIF(SANITIZE MATCHES THREAD)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tools/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

find_package(Sanitizers)

# Include Boost as an imported target
find_package(Boost COMPONENTS program_options REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
  INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
add_library(boost-program_options SHARED IMPORTED)
set_property(TARGET boost-program_options PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET boost-program_options PROPERTY IMPORTED_LOCATION ${Boost_PROGRAM_OPTIONS_LIBRARY})

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
  string(FIND ${SOURCE_FILE} libs PROJECT_TRDPARTY_DIR_FOUND)
  if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
    list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
  endif ()
endforeach ()

add_custom_target(
  cppcheck
  COMMAND /usr/bin/cppcheck
  --inline-suppr
  --enable=warning,performance,portability
  --std=c++11
  --library=qt.cfg
  --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
  --verbose
  --quiet
  ${ALL_SOURCE_FILES}
)

add_custom_target( clangformat COMMAND /usr/bin/clang-format -i ${ALL_SOURCE_FILES} )

enable_testing()
add_subdirectory(libs/googletest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

IF(PEDANTIC MATCHES True)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic" )
ENDIF(PEDANTIC MATCHES True)

add_subdirectory(common)
add_subdirectory(testscreens)
add_subdirectory(display)
add_subdirectory(ramp)
