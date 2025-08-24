#include "VolumeRenderer.h"
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkImageActor.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMapper3D.h>
#include <vtkLookupTable.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QDebug>

VolumeRenderer::VolumeRenderer(QObject *parent)
    : QObject(parent)
    , m_imageViewer(nullptr)
    , m_vtkWidget(nullptr)
    , m_renderWindow(nullptr)
    , m_interactor(nullptr)
    , m_interactorStyle(nullptr)
    , m_imageData(nullptr)
    , m_currentOrientation(AXIAL)
    , m_currentSlice(0)
{
    setupViewer();
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

void VolumeRenderer::setupViewer()
{
    // Create VTK widget
    m_vtkWidget = new QVTKOpenGLNativeWidget();
    
    // Create image viewer
    m_imageViewer = vtkImageViewer2::New();
    
    // Set up render window
    m_renderWindow = m_vtkWidget->renderWindow();
    m_imageViewer->SetRenderWindow(m_renderWindow);
    
    // Set up interactor
    m_interactor = m_renderWindow->GetInteractor();
    m_imageViewer->SetupInteractor(m_interactor);
    
    // Set up interaction style
    m_interactorStyle = vtkInteractorStyleImage::New();
    m_interactor->SetInteractorStyle(m_interactorStyle);
    
    // Set default orientation
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


