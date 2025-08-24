#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

// Qt base classes for object management and widget integration
#include <QObject>
#include <QWidget>

// Forward declarations of VTK classes to avoid including headers
class vtkImageData;              // VTK data structure for image/volume data
class vtkImageViewer2;           // VTK widget for displaying 2D image slices
class vtkRenderWindow;           // VTK window for OpenGL rendering
class vtkRenderWindowInteractor; // VTK interactor for handling user input
class vtkInteractorStyleImage;   // VTK interaction style for image viewing
class QVTKOpenGLNativeWidget;    // Qt widget that integrates VTK with Qt

/**
 * VolumeRenderer - Manages VTK-based 3D volume rendering and image display
 * 
 * This class handles:
 * - VTK widget creation and management
 * - Image data rendering and slice viewing
 * - User interaction (zoom, pan, slice navigation)
 * - Multi-planar view orientations
 */
class VolumeRenderer : public QObject
{
    Q_OBJECT

public:
    /**
     * View orientations for medical imaging
     * These correspond to standard anatomical viewing planes
     */
    enum ViewOrientation {
        AXIAL = 0,    // Top-down view (XY plane) - looking down from above
        SAGITTAL = 1, // Side view (YZ plane) - looking from the side
        CORONAL = 2   // Front view (XZ plane) - looking from the front
    };

    explicit VolumeRenderer(QObject *parent = nullptr);
    ~VolumeRenderer();

    // Rendering setup - initialize and configure VTK components
    void setImageData(vtkImageData *imageData);  // Load image data into the renderer
    QWidget* getRenderWidget();                  // Get the Qt widget for display
    
    // Slice navigation - move through the 3D volume
    void setSlice(int slice);                    // Set current slice position
    void setOrientation(ViewOrientation orientation); // Change viewing plane
    int getCurrentSlice() const;                 // Get current slice number
    int getMaxSlice() const;                     // Get maximum slice number
    int getMinSlice() const;                     // Get minimum slice number
    
    // View controls - manipulate the camera and view
    void resetView();                            // Reset camera to fit entire image
    void zoomIn();                               // Zoom into the image
    void zoomOut();                              // Zoom out from the image
    void resetZoom();                            // Reset zoom to default level
    


signals:
    void sliceChanged(int slice);                    // Emitted when slice position changes
    void orientationChanged(ViewOrientation orientation); // Emitted when view orientation changes

public slots:
    void updateRender();                             // Force a re-render of the scene

private:
    // VTK rendering components
    vtkImageViewer2 *m_imageViewer;                 // Main VTK image viewer for slice display
    QVTKOpenGLNativeWidget *m_vtkWidget;            // Qt widget that contains VTK rendering
    vtkRenderWindow *m_renderWindow;                // VTK window for OpenGL rendering
    vtkRenderWindowInteractor *m_interactor;        // Handles user input (mouse, keyboard)
    vtkInteractorStyleImage *m_interactorStyle;     // Defines how user interactions work
    
    // Current state
    vtkImageData *m_imageData;                      // Currently loaded image data
    ViewOrientation m_currentOrientation;           // Current viewing orientation
    int m_currentSlice;                             // Current slice position
    
    // Private helper methods
    void setupViewer();                              // Initialize VTK components
    void updateSliceRange();                         // Update slice range when orientation changes
};

#endif // VOLUMERENDERER_H
