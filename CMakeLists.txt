cmake_minimum_required(VERSION 3.16)
project(NiftiViewer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL)

# Find VTK
find_package(VTK REQUIRED)

# Qt6 automation
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/VolumeRenderer.cpp
    src/FileManager.cpp
)

set(HEADERS
    src/MainWindow.h
    src/VolumeRenderer.h
    src/FileManager.h
)

# Create executable
add_executable(NiftiViewer ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(NiftiViewer 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::OpenGL
    ${VTK_LIBRARIES}
)

# VTK module initialization
vtk_module_autoinit(
    TARGETS NiftiViewer
    MODULES ${VTK_LIBRARIES}
)

# Set output directory
set_target_properties(NiftiViewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Include directories
target_include_directories(NiftiViewer PRIVATE src)
