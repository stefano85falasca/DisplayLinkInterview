project(display VERSION 0.1 LANGUAGES CXX)

add_library(display src/display.cpp)
target_link_libraries(display common)
add_sanitizers(display)
add_definitions(-std=c++11 -Wall --pedantic -Werror)
 
target_include_directories(display PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

install(TARGETS display
  ARCHIVE DESTINATION display
  LIBRARY DESTINATION display
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

export(TARGETS display FILE TslibLibraryConfig.cmake)
