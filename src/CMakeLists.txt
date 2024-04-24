cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the Qt libraries.
find_package(Qt6Network REQUIRED)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Gui)

set(This haller-control)

set(Sources
    main.cpp
    MainWindow.cpp
    MainWindow.ui
    UdpNode.cpp
    UdpPacketMaker.cpp
    Logger.cpp

    JoystickWorker.cpp
    resource.qrc
)

set(Headers
    MainWindow.hpp
    UdpNode.hpp
    Definitions.hpp
    UdpPacketMaker.hpp
    DataSendWorker.hpp
    JoystickWorker.hpp
    Logger.hpp
)

set(QtLibraries
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui)

add_executable(${This} ${Sources} ${Headers}
    DataSendWorker.hpp
    DataSendTimer.hpp DataSendTimer.cpp)

target_link_libraries(${This} PUBLIC
    ${QtLibraries}
    sfml-graphics
    sfml-window
)
