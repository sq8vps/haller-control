cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the Qt libraries.
find_package(Qt6Network REQUIRED)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Gui REQUIRED)

IF(WIN32)
set(depthai_DIR ${CMAKE_CURRENT_LIST_DIR}/../libs/depthai/build) #set path to your depthai package build folder
ENDIF()

find_package(depthai CONFIG REQUIRED)

IF(WIN32)
set(OpenCV_DIR C:/opencv/build) #set path to your OpenCV build folder
find_package(OpenCV CONFIG REQUIRED)
ENDIF()



set(This haller-control)

set(Sources
    main.cpp
    MainWindow.cpp
    MainWindow.ui
    UdpNode.cpp
    UdpPacketMaker.cpp
    Logger.cpp
    JoystickWorker.cpp
    DataSendTimer.cpp
    Allocator.cpp
    resource.qrc
    camerahandler.cpp
    grippertimer.cpp
    cameraworker.cpp
)

set(Headers
    MainWindow.hpp
    UdpNode.hpp
    Definitions.hpp
    UdpPacketMaker.hpp
    DataSendTimer.hpp
    JoystickWorker.hpp
    Logger.hpp
    camerahandler.h
    grippertimer.hpp
    cameraworker.h
    Allocator.hpp
)

set(QtLibraries
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
)

add_executable(${This} ${Sources} ${Headers}

)

target_link_libraries(${This} PUBLIC
    ${QtLibraries}
    sfml-graphics
    sfml-window
    depthai::opencv
)

IF(WIN32)
add_custom_command(TARGET ${This} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${depthai_DIR}/${CMAKE_BUILD_TYPE}/libusb-1.0d.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libusb-1.0d.dll
)
ENDIF()
