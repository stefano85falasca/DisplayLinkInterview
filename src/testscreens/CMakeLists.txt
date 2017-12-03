project(tslib VERSION 0.1 LANGUAGES CXX)

add_library(tslib src/ramp.cpp)
target_link_libraries(tslib common display)
add_sanitizers(tslib)
add_definitions(-std=c++14 -Wall --pedantic -Werror)

target_include_directories(
  tslib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

install(TARGETS tslib ARCHIVE DESTINATION tslib LIBRARY
  DESTINATION tslib RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

export(TARGETS tslib FILE TslibLibraryConfig.cmake)

add_executable(testtslib test/main.cpp test/ramp.cpp)
add_sanitizers(testtslib)

target_include_directories(
  testtslib PRIVATE $gtest_SOURCWE_DIR /include)
target_link_libraries(testtslib tslib common display gtest)
add_test(testtslib testtslib)
