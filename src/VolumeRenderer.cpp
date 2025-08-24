#include "VolumeRenderer.h"

// VTK core rendering classes
#include <vtkImageData.h>              // Image/volume data structure
#include <vtkImageViewer2.h>           // Main image viewer for slice display
#include <vtkRenderWindow.h>           // OpenGL rendering window
#include <vtkRenderWindowInteractor.h> // User input handling
#include <vtkInteractorStyleImage.h>   // Image-specific interaction style
#include <vtkRenderer.h>               // Scene renderer
#include <vtkCamera.h>                 // Camera for view manipulation

// VTK image processing classes
#include <vtkImageActor.h>             // Actor for displaying images
#include <vtkImageMapToColors.h>       // Base class for color mapping
#include <vtkImageMapToWindowLevelColors.h> // Color mapping for contrast adjustment
#include <vtkImageMapper3D.h>          // 3D image mapping
#include <vtkLookupTable.h>            // Color lookup table

// Qt VTK integration
#include <QVTKOpenGLNativeWidget.h>   // Qt widget for VTK rendering

// Qt debugging support
#include <QDebug>                      // For debug output

/**
 * Constructor - Initializes the VolumeRenderer with default settings
 * 
 * Sets up all VTK components and starts with an axial view orientation.
 * The viewer is ready to display images once image data is provided.
 */
VolumeRenderer::VolumeRenderer(QObject *parent)
    : QObject(parent)
    // Initialize all VTK components to nullptr for safety
    , m_imageViewer(nullptr)      // Main image viewer
    , m_vtkWidget(nullptr)        // Qt widget container
    , m_renderWindow(nullptr)     // OpenGL rendering window
    , m_interactor(nullptr)       // User input handler
    , m_interactorStyle(nullptr)  // Interaction style
    , m_imageData(nullptr)        // Image data (none loaded initially)
    , m_currentOrientation(AXIAL) // Start with axial view (top-down)
    , m_currentSlice(0)           // Start at first slice
{
    setupViewer();  // Initialize all VTK components
}

VolumeRenderer::~VolumeRenderer()
{
    if (m_imageViewer) {
        m_imageViewer->Delete();
    }
    if (m_interactorStyle) {
        m_interactorStyle->Delete();
    }
}

/**
 * Sets up all VTK components for image viewing
 * 
 * This method creates and configures:
 * 1. Qt widget for VTK integration
 * 2. VTK image viewer for slice display
 * 3. Render window for OpenGL rendering
 * 4. Interactor for user input handling
 * 5. Interaction style for image-specific controls
 */
void VolumeRenderer::setupViewer()
{
    // Create Qt widget that will contain VTK rendering
    m_vtkWidget = new QVTKOpenGLNativeWidget();
    
    // Create VTK image viewer for displaying 2D slices
    m_imageViewer = vtkImageViewer2::New();
    
    // Connect the render window from the Qt widget to the VTK viewer
    m_renderWindow = m_vtkWidget->renderWindow();
    m_imageViewer->SetRenderWindow(m_renderWindow);
    
    // Set up the interactor for handling mouse and keyboard input
    m_interactor = m_renderWindow->GetInteractor();
    m_imageViewer->SetupInteractor(m_interactor);
    
    // Configure interaction style for medical image viewing
    // This enables standard operations like zoom, pan, and slice navigation
    m_interactorStyle = vtkInteractorStyleImage::New();
    m_interactor->SetInteractorStyle(m_interactorStyle);
    
    // Set default orientation to axial view (top-down, standard for medical imaging)
    m_imageViewer->SetSliceOrientationToXY(); // Axial view
}

void VolumeRenderer::setImageData(vtkImageData *imageData)
{
    if (!imageData) {
        qWarning() << "Null image data provided to VolumeRenderer";
        return;
    }
    
    m_imageData = imageData;
    m_imageViewer->SetInputData(imageData);
    
    // Update the pipeline before proceeding
    m_imageViewer->GetInput()->Modified();
    m_imageViewer->UpdateDisplayExtent();
    
    updateSliceRange();
    
    // Set to middle slice
    int middleSlice = (getMinSlice() + getMaxSlice()) / 2;
    setSlice(middleSlice);
    
    // Reset view
    resetView();
    
    updateRender();
}

QWidget* VolumeRenderer::getRenderWidget()
{
    return m_vtkWidget;
}

void VolumeRenderer::setSlice(int slice)
{
    if (!m_imageViewer || !m_imageData) {
        return;
    }
    
    int minSlice = getMinSlice();
    int maxSlice = getMaxSlice();
    
    // Clamp slice to valid range
    slice = qMax(minSlice, qMin(maxSlice, slice));
    
    if (slice != m_currentSlice) {
        m_currentSlice = slice;
        m_imageViewer->SetSlice(slice);
        updateRender();
        emit sliceChanged(slice);
    }
}

void VolumeRenderer::setOrientation(ViewOrientation orientation)
{
    if (!m_imageViewer) {
        return;
    }
    
    m_currentOrientation = orientation;
    
    switch (orientation) {
        case AXIAL:
            m_imageViewer->SetSliceOrientationToXY();
            break;
        case SAGITTAL:
            m_imageViewer->SetSliceOrientationToYZ();
            break;
        case CORONAL:
            m_imageViewer->SetSliceOrientationToXZ();
            break;
    }
    
    updateSliceRange();
    
    // Reset to middle slice for new orientation
    int middleSlice = (getMinSlice() + getMaxSlice()) / 2;
    setSlice(middleSlice);
    
    emit orientationChanged(orientation);
}

int VolumeRenderer::getCurrentSlice() const
{
    return m_currentSlice;
}

int VolumeRenderer::getMaxSlice() const
{
    if (!m_imageViewer) {
        return 0;
    }
    return m_imageViewer->GetSliceMax();
}

int VolumeRenderer::getMinSlice() const
{
    if (!m_imageViewer) {
        return 0;
    }
    return m_imageViewer->GetSliceMin();
}

void VolumeRenderer::resetView()
{
    if (m_imageViewer) {
        m_imageViewer->GetRenderer()->ResetCamera();
        updateRender();
    }
}

void VolumeRenderer::zoomIn()
{
    if (m_imageViewer) {
        vtkRenderer* renderer = m_imageViewer->GetRenderer();
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->Zoom(1.2);
        updateRender();
    }
}

void VolumeRenderer::zoomOut()
{
    if (m_imageViewer) {
        vtkRenderer* renderer = m_imageViewer->GetRenderer();
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->Zoom(0.8);
        updateRender();
    }
}

void VolumeRenderer::resetZoom()
{
    resetView();
}







void VolumeRenderer::updateRender()
{
    if (m_renderWindow) {
        m_renderWindow->Render();
    }
}

void VolumeRenderer::updateSliceRange()
{
    if (m_imageViewer && m_imageData) {
        m_imageViewer->UpdateDisplayExtent();
        
        // Get the current slice range
        int minSlice = getMinSlice();
        int maxSlice = getMaxSlice();
        
        qDebug() << "Slice range updated:" << minSlice << "to" << maxSlice;
    }
}


