# NifTI Volume Loader - Qt + VTK

A Qt-based medical imaging viewer for NifTI files using VTK.

## Features
- Load and display NIfTI files (.nii, .nii.gz)
- Multi-planar viewing (Axial, Sagittal, Coronal)
- Slice navigation with slider controls
- Zoom in/out and reset view
- Error handling and user feedback
- Comprehensive file metadata display

## Quick Start
1. Download `NifTIViewer.exe` from [Releases](https://github.com/akhilG05/NifTI-Volume-Loader/releases)
2. Run the executable
3. Click "Browse File" to load a NIfTI file
4. Use controls on the right panel for navigation

## Build from Source

For detailed build instructions, see [BUILD.md](BUILD.md).

## Architecture
- `MainWindow`: Qt GUI and user interface
- `VolumeRenderer`: VTK rendering and image display
- `FileManager`: NIfTI file loading and management