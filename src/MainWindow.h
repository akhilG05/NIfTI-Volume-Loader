#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt Widgets for the user interface
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QTextEdit>

// Our custom classes for file management and rendering
#include "FileManager.h"
#include "VolumeRenderer.h"

/**
 * Main application window for the NifTI Volume Loader
 * 
 * This class manages the entire user interface including:
 * - File selection and loading
 * - Image display and navigation controls
 * - Status updates and progress indication
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File management slots - handle file operations and loading states
    void browseFile();                                    // Open file dialog and initiate loading
    void onFileLoadingStarted(const QString &fileName);   // Called when file loading begins
    void onFileLoadingProgress(int percentage);           // Update progress bar during loading
    void onFileLoadingCompleted(const QString &fileName); // Handle successful file load
    void onFileLoadingError(const QString &errorMessage); // Handle loading errors
    
    // Image navigation slots - respond to user interactions
    void onSliceChanged(int slice);                      // Update UI when slice changes
    void onOrientationChanged();                         // Handle view orientation changes
    void onSliceSliderChanged(int value);                // Respond to slice slider changes
    
    // Navigation controls - manipulate the 3D view
    void zoomIn();        // Zoom into the image (closer view)
    void zoomOut();       // Zoom out from the image (wider view)
    void resetView();     // Reset camera to fit the entire image

private:
    // UI setup methods - create and organize the interface
    void setupUI();           // Main UI setup coordinator
    void setupMenuBar();      // Create application menu bar
    void setupToolBar();      // Create toolbar (currently empty)
    void setupStatusBar();    // Create status bar with progress indicator
    void setupCentralWidget(); // Create main content area
    void setupControlPanel();  // Create right-side control panel
    
    // Utility methods - handle UI updates and connections
    void connectSignals();      // Connect all signal-slot relationships
    void updateSliceControls(); // Update slice navigation controls
    void updateFileInfo();      // Update file information display
    void enableControls(bool enabled); // Enable/disable UI controls based on file state
    void applyDarkTheme();      // Apply professional dark theme styling

    // Core components - main application logic
    FileManager *m_fileManager;      // Handles NIfTI file loading and parsing
    VolumeRenderer *m_volumeRenderer; // Manages VTK rendering and image display
    
    // UI Components - main interface elements
    QPushButton *m_browseButton;    // Button to open file selection dialog
    QLabel *m_filePathLabel;        // Displays the currently loaded file path
    QWidget *m_renderWidget;        // VTK rendering widget for image display
    
    // Control panel - right-side navigation and settings
    QGroupBox *m_controlPanel;      // Container for all control widgets
    QComboBox *m_orientationCombo;  // Dropdown to select view orientation (Axial/Sagittal/Coronal)
    QSlider *m_sliceSlider;         // Slider for navigating through image slices
    QSpinBox *m_sliceSpinBox;       // Numeric input for precise slice selection
    QLabel *m_sliceLabel;           // Shows current slice position and total count
    
    // Navigation controls - image manipulation
    QPushButton *m_zoomInButton;    // Zoom into the image
    QPushButton *m_zoomOutButton;   // Zoom out from the image
    QPushButton *m_resetViewButton; // Reset camera to default view
    
    // Information display - file metadata
    QTextEdit *m_infoDisplay;       // Shows file information (dimensions, spacing, origin)
    
    // Status and progress - user feedback
    QProgressBar *m_progressBar;    // Shows file loading progress
    QLabel *m_statusLabel;          // Displays current application status
    
    // Layout - interface organization
    QSplitter *m_mainSplitter;      // Divides render area and control panel
    QWidget *m_centralWidget;       // Main content container
    
    // Current state - application data
    QString m_currentFilePath;      // Path to the currently loaded file
    bool m_fileLoaded;              // Whether a file is currently loaded
};

#endif // MAINWINDOW_H
