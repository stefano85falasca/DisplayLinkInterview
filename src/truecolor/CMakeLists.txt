add_executable(truecolor src/main.cpp)
add_sanitizers(truecolor)
add_definitions(-std=c++11 -Wall -Werror --pedantic)

target_link_libraries(truecolor common tslib)
