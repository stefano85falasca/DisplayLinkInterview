add_executable(ramp src/main.cpp)
add_sanitizers(ramp)
add_definitions(-std=c++14 -Wall -Werror --pedantic)

FIND_PACKAGE(Boost 1.58 COMPONENTS program_options system filesystem REQUIRED )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

target_link_libraries(ramp common tslib display)
