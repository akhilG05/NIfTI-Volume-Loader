# NifTI Volume Loader - CMake Configuration
# 
# This project builds a Qt-based medical imaging viewer using VTK
# for NIfTI file format support.

cmake_minimum_required(VERSION 3.16)
project(NiftiViewer VERSION 1.0.0 LANGUAGES CXX)

# C++ standard requirements
set(CMAKE_CXX_STANDARD 17)           # Use C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Require C++17 support

# Find required Qt6 components for GUI and OpenGL support
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL)

# Find VTK library for medical imaging and 3D rendering
find_package(VTK REQUIRED)

# Qt6 automation - enable Qt-specific build tools
set(CMAKE_AUTOMOC ON)                # Enable Qt MOC (Meta-Object Compiler)
set(CMAKE_AUTOUIC ON)                # Enable Qt UI compilation
set(CMAKE_AUTORCC ON)                # Enable Qt resource compilation

# Source files - C++ implementation files
set(SOURCES
    src/main.cpp           # Application entry point
    src/MainWindow.cpp     # Main window implementation
    src/VolumeRenderer.cpp # VTK rendering implementation
    src/FileManager.cpp    # File handling implementation
)

# Header files - C++ class declarations
set(HEADERS
    src/MainWindow.h       # Main window class definition
    src/VolumeRenderer.h   # Volume renderer class definition
    src/FileManager.h      # File manager class definition
)

# Create the main executable
add_executable(NiftiViewer ${SOURCES} ${HEADERS})

# Link required libraries
target_link_libraries(NiftiViewer 
    Qt6::Core      # Qt core functionality
    Qt6::Widgets   # Qt GUI widgets
    Qt6::OpenGL    # Qt OpenGL support
    ${VTK_LIBRARIES} # VTK libraries for medical imaging
)

# VTK module initialization - required for VTK to work properly
vtk_module_autoinit(
    TARGETS NiftiViewer
    MODULES ${VTK_LIBRARIES}
)

# Set output directory - place executable in build/bin folder
set_target_properties(NiftiViewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Include directories - add src folder to include path
target_include_directories(NiftiViewer PRIVATE src)
