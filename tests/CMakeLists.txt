cmake_minimum_required(VERSION 3.5)

# Find the Qt libraries.
find_package(Qt6Network REQUIRED)

set(This tests)

set(Sources
    ExampleTest.cpp
    ../src/UdpNode.cpp
    ../src/UdpNode.hpp
)

set(QtLibraries
        Qt${QT_VERSION_MAJOR}::Network)


add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    ${QtLibraries}
    gtest_main
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

add_subdirectory(googletest)
