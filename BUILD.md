# NifTI Volume Loader - Build Instructions

Complete step-by-step guide to build the NifTI Volume Loader from source.

## Prerequisites

1. **Visual Studio 2022** with C++ development tools
2. **Qt 6.9.1** installed at `C:\Qt\6.9.1\msvc2022_64`
3. **VTK** library with Qt support
4. **CMake** (version 3.16 or higher)

## VTK Installation

```powershell
# Download VTK source
git clone https://github.com/Kitware/VTK.git
cd VTK
git checkout v9.3.0

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 `
    -DCMAKE_INSTALL_PREFIX="C:\VTK" `
    -DVTK_GROUP_ENABLE_Qt=YES `
    -DVTK_MODULE_ENABLE_VTK_GUISupportQt=YES `
    -DVTK_MODULE_ENABLE_VTK_GUISupportQtOpenGL=YES `
    -DVTK_MODULE_ENABLE_VTK_IONIFTI=YES `
    -DQt6_DIR="C:\Qt\6.9.1\msvc2022_64\lib\cmake\Qt6"

# Build and install
cmake --build . --config Release --target install
```

## Building the NifTI Viewer

### Build Steps
```powershell
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 `
    -DQt6_DIR="C:\Qt\6.9.1\msvc2022_64\lib\cmake\Qt6" `
    -DVTK_DIR="C:\VTK\lib\cmake\vtk-9.3"

# Build
cmake --build . --config Release

# Run
.\build\bin\Release\NiftiViewer.exe
```

## Environment Variables
If VTK is installed in a different location, set these environment variables:
```powershell
$env:VTK_DIR = "C:\path\to\your\VTK\lib\cmake\vtk-9.3"
$env:Qt6_DIR = "C:\Qt\6.9.1\msvc2022_64\lib\cmake\Qt6"
```

## Troubleshooting

### Common Issues:
1. **Qt not found**: Ensure Qt6_DIR points to the correct CMake directory
2. **VTK not found**: Check VTK_DIR path and ensure VTK was built with Qt support
3. **Missing DLLs**: Make sure Qt and VTK bin directories are in your PATH

### Runtime Dependencies:
The application requires these DLLs to be available:
- Qt6Core.dll, Qt6Widgets.dll, Qt6OpenGL.dll
- VTK DLLs (various vtk*.dll files)
- Visual C++ Redistributable

## Testing
Place sample NifTI files (.nii or .nii.gz) in a test directory and use the "Browse File" button to load them.