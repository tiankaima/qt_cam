cmake_minimum_required(VERSION 3.26)
project(qt_example_proj)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/tiankaima/Qt/6.6.0/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(qt_example_proj main.cpp
        Widget.cpp
        Widget.h
        ToolPath-lib/ZigZag.cpp
        ToolPath-lib/ZigZag.h
        ToolPath-lib/Raster.cpp
        ToolPath-lib/Raster.h
        ToolPath-lib/Path.h
        ToolPath-lib/Path.cpp
        ToolPath-lib/Contour_Parallel.cpp
        ToolPath-lib/Contour_Parallel.h
)
target_link_libraries(qt_example_proj
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# add lib/Clipper/CPP/CMakeLists.txt
set(BUILD_SHARED_LIBS ON)
add_subdirectory(lib/Clipper/CPP)
include_directories(lib/Clipper/CPP)
target_link_libraries(qt_example_proj
        Clipper2)

