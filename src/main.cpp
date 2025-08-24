#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "MainWindow.h"
#include <vtkOutputWindow.h>
#include <vtkFileOutputWindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("NifTI Volume Loader");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("NifTI Viewer");
    
    // Suppress VTK error output window
    vtkOutputWindow::SetGlobalWarningDisplay(0);
    auto fileOutputWindow = vtkFileOutputWindow::New();
    fileOutputWindow->SetFileName("vtk_errors.log");
    vtkOutputWindow::SetInstance(fileOutputWindow);
    fileOutputWindow->Delete();
    
    // Set a modern style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}
