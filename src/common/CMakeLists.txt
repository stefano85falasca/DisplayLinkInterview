project(commonlib VERSION 0.1 LANGUAGES CXX)

add_library(common src/assert.cpp src/command.cpp)
add_sanitizers(common)
add_definitions(-std=c++14 -Wall -Werror -pedantic)

target_include_directories(common PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

install(TARGETS common
  ARCHIVE DESTINATION common
  LIBRARY DESTINATION common
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

export(TARGETS common FILE CommonLibraryConfig.cmake)

add_executable(testcommonlib test/main.cpp test/assert_normal.cpp test/assert_opt.cpp test/assert_safe.cpp test/assert_none.cpp test/command.cpp test/pimpl.cpp test/pimpl_source.cpp)
add_sanitizers(testcommonlib)

target_include_directories(testcommonlib PRIVATE $gtest_SOURCE_DIR/include)
target_link_libraries(testcommonlib boost-program_options common gtest)

add_test(testcommonlib testcommonlib)
