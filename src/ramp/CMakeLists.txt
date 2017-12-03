add_executable(ramp src/main.cpp)
add_sanitizers(ramp)
add_definitions(-std=c++14 -Wall -Werror --pedantic)

target_link_libraries(ramp common tslib display)
