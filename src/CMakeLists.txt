cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the Qt libraries.
find_package(Qt6Network REQUIRED)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Gui)

set(This heller-control)

set(Sources
    main.cpp
    MainWindow.cpp
    MainWindow.hpp
    MainWindow.ui
    UdpNode.cpp
    UdpNode.hpp
    Definitions.hpp
    UdpPacketMaker.cpp
    UdpPacketMaker.hpp
    resource.qrc
)

set(QtLibraries
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    ${QtLibraries}
)
