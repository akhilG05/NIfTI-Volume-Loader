// Qt application framework
#include <QApplication>
#include <QStyleFactory>
#include <QDir>

// Our main application window
#include "MainWindow.h"

// VTK output handling - suppress error popups and redirect to log file
#include <vtkOutputWindow.h>
#include <vtkFileOutputWindow.h>

/**
 * Main entry point for the NifTI Volume Loader application
 * 
 * Sets up the Qt application, configures VTK error handling,
 * applies styling, and launches the main window.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application metadata for system integration
    app.setApplicationName("NifTI Volume Loader");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("NifTI Viewer");
    
        // Configure VTK error handling to avoid popup dialogs
    // Instead, redirect all VTK errors and warnings to a log file
    vtkOutputWindow::SetGlobalWarningDisplay(0);  // Disable error popups
    auto fileOutputWindow = vtkFileOutputWindow::New();
    fileOutputWindow->SetFileName("vtk_errors.log");  // Log file for VTK errors
    vtkOutputWindow::SetInstance(fileOutputWindow);
    fileOutputWindow->Delete();

    // Apply modern Fusion style for consistent cross-platform appearance
    app.setStyle(QStyleFactory::create("Fusion"));
    
        // Create and display the main application window
    MainWindow window;
    window.show();

    // Start the Qt event loop and run the application
    return app.exec();
}
