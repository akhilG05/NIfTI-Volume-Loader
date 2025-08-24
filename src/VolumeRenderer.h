#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include <QObject>
#include <QWidget>

class vtkImageData;
class vtkImageViewer2;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkInteractorStyleImage;
class QVTKOpenGLNativeWidget;

class VolumeRenderer : public QObject
{
    Q_OBJECT

public:
    enum ViewOrientation {
        AXIAL = 0,
        SAGITTAL = 1,
        CORONAL = 2
    };

    explicit VolumeRenderer(QObject *parent = nullptr);
    ~VolumeRenderer();

    // Rendering setup
    void setImageData(vtkImageData *imageData);
    QWidget* getRenderWidget();
    
    // Slice navigation
    void setSlice(int slice);
    void setOrientation(ViewOrientation orientation);
    int getCurrentSlice() const;
    int getMaxSlice() const;
    int getMinSlice() const;
    
    // View controls
    void resetView();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    


signals:
    void sliceChanged(int slice);
    void orientationChanged(ViewOrientation orientation);

public slots:
    void updateRender();

private:
    vtkImageViewer2 *m_imageViewer;
    QVTKOpenGLNativeWidget *m_vtkWidget;
    vtkRenderWindow *m_renderWindow;
    vtkRenderWindowInteractor *m_interactor;
    vtkInteractorStyleImage *m_interactorStyle;
    
    vtkImageData *m_imageData;
    ViewOrientation m_currentOrientation;
    int m_currentSlice;
    
    void setupViewer();
    void updateSliceRange();
};

#endif // VOLUMERENDERER_H
