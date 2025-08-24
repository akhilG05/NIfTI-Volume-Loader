#include "MainWindow.h"

// Qt Core and Widgets for application framework
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

// Qt Dialogs for user interaction
#include <QFileDialog>
#include <QMessageBox>

// Qt Layouts for organizing the interface
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>

// Qt Widgets for the user interface
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QProgressBar>
#include <QTextEdit>

// Qt Actions for menu and toolbar
#include <QAction>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // Initialize all member pointers to nullptr for safety
    , m_fileManager(nullptr)      // Will be created in setupUI()
    , m_volumeRenderer(nullptr)   // Will be created in setupUI()
    , m_browseButton(nullptr)     // Will be created in setupUI()
    , m_filePathLabel(nullptr)    // Will be created in setupUI()
    , m_renderWidget(nullptr)     // Will be created in setupUI()
    , m_controlPanel(nullptr)     // Will be created in setupUI()
    , m_orientationCombo(nullptr) // Will be created in setupUI()
    , m_sliceSlider(nullptr)      // Will be created in setupUI()
    , m_sliceSpinBox(nullptr)     // Will be created in setupUI()
    , m_sliceLabel(nullptr)       // Will be created in setupUI()
    , m_zoomInButton(nullptr)     // Will be created in setupUI()
    , m_zoomOutButton(nullptr)    // Will be created in setupUI()
    , m_resetViewButton(nullptr)  // Will be created in setupUI()
    , m_infoDisplay(nullptr)      // Will be created in setupUI()
    , m_progressBar(nullptr)      // Will be created in setupUI()
    , m_statusLabel(nullptr)      // Will be created in setupUI()
    , m_mainSplitter(nullptr)     // Will be created in setupUI()
    , m_centralWidget(nullptr)    // Will be created in setupUI()
    , m_fileLoaded(false)         // Start with no file loaded
{
    setWindowTitle("NifTI Volume Loader");
    setMinimumSize(1000, 700);  // Ensure adequate space for medical imaging interface
    
    // Initialize core components that handle file operations and rendering
    m_fileManager = new FileManager(this);      // Manages NIfTI file loading and parsing
    m_volumeRenderer = new VolumeRenderer(this); // Handles VTK-based image display
    
    // Set up the complete user interface
    setupUI();        // Create all UI elements and layouts
    connectSignals(); // Wire up all signal-slot connections
    enableControls(false); // Start with controls disabled (no file loaded)
    
    // Apply professional dark theme optimized for medical imaging
    applyDarkTheme();
}

MainWindow::~MainWindow()
{
}

/**
 * Sets up the complete user interface
 * 
 * This method orchestrates the creation of all UI components:
 * 1. Menu bar with File and About menus
 * 2. Toolbar (currently empty for clean interface)
 * 3. Status bar with progress indicator
 * 4. Main content area with render widget and control panel
 */
void MainWindow::setupUI()
{
    setupMenuBar();      // Create application menu bar
    setupToolBar();      // Create toolbar (currently empty)
    setupStatusBar();    // Create status bar with progress
    setupCentralWidget(); // Create main content area
}

/**
 * Creates the application menu bar with File and About menus
 * 
 * The File menu provides standard file operations, while the About menu
 * gives users information about the application and its capabilities.
 */
void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu - standard file operations
    QMenu *fileMenu = menuBar->addMenu("&File");
    
    // Open action with standard Ctrl+O shortcut
    QAction *openAction = new QAction("&Open NifTI File...", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::browseFile);
    fileMenu->addAction(openAction);
    
    fileMenu->addSeparator();
    
    // Exit action with standard Ctrl+Q shortcut
    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
    
    // About menu - application information
    QMenu *aboutMenu = menuBar->addMenu("&About");
    
    // About action with detailed application description
    QAction *aboutAction = new QAction("&About NifTI Volume Loader", this);
    connect(aboutAction, &QAction::triggered, this, [this](){
        QMessageBox::about(this, "About NifTI Volume Loader", 
            "NifTI Volume Loader v1.0\n\n"
            "A Qt-based medical imaging viewer for NifTI files.\n"
            "Built with Qt 6.9.1 and VTK 9.3.1\n\n"
            "📁 File Support: .nii and .nii.gz formats\n"
            "🔍 Features: Multi-planar viewing, zoom, slice navigation\n\n"
            "Use the control panel on the right for navigation and view options.");
    });
    aboutMenu->addAction(aboutAction);
}

void MainWindow::setupToolBar()
{
    // Remove toolbar completely since Browse File button handles file opening
    // This eliminates redundancy and creates a cleaner interface
}

void MainWindow::setupStatusBar()
{
    QStatusBar *statusBar = this->statusBar();
    
    m_statusLabel = new QLabel("Ready");
    statusBar->addWidget(m_statusLabel);
    
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(200);
    statusBar->addPermanentWidget(m_progressBar);
}

void MainWindow::setupCentralWidget()
{
    m_centralWidget = new QWidget();
    setCentralWidget(m_centralWidget);
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    
    // File selection area
    QHBoxLayout *fileLayout = new QHBoxLayout();
    
    m_browseButton = new QPushButton("Browse File");
    m_browseButton->setMaximumWidth(120);
    
    m_filePathLabel = new QLabel("No file selected");
    m_filePathLabel->setStyleSheet("QLabel { background-color: #2b2b2b; color: #ffffff; padding: 3px 8px; border: 1px solid #555; font-size: 12px; }");
    m_filePathLabel->setMaximumHeight(24);  // Reduced height
    m_filePathLabel->setMinimumHeight(24);
    
    QLabel *fileLabel = new QLabel("NifTI File:");
    fileLabel->setStyleSheet("QLabel { color: #ffffff; font-weight: bold; font-size: 12px; }");
    fileLabel->setMinimumWidth(80);
    
    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(m_browseButton);
    fileLayout->addWidget(m_filePathLabel, 1);
    
    mainLayout->addLayout(fileLayout);
    
    // Main content area with splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal);
    
    // VTK Render Window
    m_renderWidget = m_volumeRenderer->getRenderWidget();
    m_renderWidget->setMinimumSize(400, 400);
    m_mainSplitter->addWidget(m_renderWidget);
    
    // Control panel
    setupControlPanel();
    m_mainSplitter->addWidget(m_controlPanel);
    
    // Set splitter proportions
    m_mainSplitter->setStretchFactor(0, 3); // Render area gets more space
    m_mainSplitter->setStretchFactor(1, 1); // Control panel gets less space
    
    mainLayout->addWidget(m_mainSplitter);
}

void MainWindow::setupControlPanel()
{
    m_controlPanel = new QGroupBox("Controls");
    m_controlPanel->setMaximumWidth(300);
    m_controlPanel->setMinimumWidth(250);
    
    QVBoxLayout *controlLayout = new QVBoxLayout(m_controlPanel);
    
    // Orientation controls
    QGroupBox *orientationGroup = new QGroupBox("View Orientation");
    QVBoxLayout *orientationLayout = new QVBoxLayout(orientationGroup);
    
    m_orientationCombo = new QComboBox();
    m_orientationCombo->addItem("Axial (XY)", static_cast<int>(VolumeRenderer::AXIAL));
    m_orientationCombo->addItem("Sagittal (YZ)", static_cast<int>(VolumeRenderer::SAGITTAL));
    m_orientationCombo->addItem("Coronal (XZ)", static_cast<int>(VolumeRenderer::CORONAL));
    orientationLayout->addWidget(m_orientationCombo);
    
    controlLayout->addWidget(orientationGroup);
    
    // Slice controls
    QGroupBox *sliceGroup = new QGroupBox("Slice Navigation");
    QGridLayout *sliceLayout = new QGridLayout(sliceGroup);
    
    m_sliceLabel = new QLabel("Slice: 0 / 0");
    sliceLayout->addWidget(m_sliceLabel, 0, 0, 1, 2);
    
    m_sliceSlider = new QSlider(Qt::Horizontal);
    m_sliceSlider->setRange(0, 0);
    sliceLayout->addWidget(m_sliceSlider, 1, 0, 1, 2);
    
    m_sliceSpinBox = new QSpinBox();
    m_sliceSpinBox->setRange(0, 0);
    sliceLayout->addWidget(new QLabel("Slice:"), 2, 0);
    sliceLayout->addWidget(m_sliceSpinBox, 2, 1);
    
    controlLayout->addWidget(sliceGroup);
    

    
    // Navigation controls
    QGroupBox *navGroup = new QGroupBox("Navigation");
    QGridLayout *navLayout = new QGridLayout(navGroup);
    
    m_zoomInButton = new QPushButton("Zoom In");
    m_zoomInButton->setToolTip("Zoom into the image");
    
    m_zoomOutButton = new QPushButton("Zoom Out");
    m_zoomOutButton->setToolTip("Zoom out of the image");
    
    m_resetViewButton = new QPushButton("Reset View");
    m_resetViewButton->setToolTip("Reset camera to fit the image");
    
            navLayout->addWidget(m_zoomInButton, 0, 0);
        navLayout->addWidget(m_zoomOutButton, 0, 1);
        navLayout->addWidget(m_resetViewButton, 1, 0, 1, 2);
    
    controlLayout->addWidget(navGroup);
    
    // Information display
    QGroupBox *infoGroup = new QGroupBox("File Information");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    
    m_infoDisplay = new QTextEdit();
    m_infoDisplay->setReadOnly(true);
    m_infoDisplay->setMaximumHeight(150);
    m_infoDisplay->setText("No file loaded");
    infoLayout->addWidget(m_infoDisplay);
    
    controlLayout->addWidget(infoGroup);
    
    controlLayout->addStretch();
}

void MainWindow::connectSignals()
{
    // File management signals
    connect(m_browseButton, &QPushButton::clicked, this, &MainWindow::browseFile);
    
    connect(m_fileManager, &FileManager::fileLoadingStarted,
            this, &MainWindow::onFileLoadingStarted);
    connect(m_fileManager, &FileManager::fileLoadingProgress,
            this, &MainWindow::onFileLoadingProgress);
    connect(m_fileManager, &FileManager::fileLoadingCompleted,
            this, &MainWindow::onFileLoadingCompleted);
    connect(m_fileManager, &FileManager::fileLoadingError,
            this, &MainWindow::onFileLoadingError);
    
    // Volume renderer signals
    connect(m_volumeRenderer, &VolumeRenderer::sliceChanged,
            this, &MainWindow::onSliceChanged);
    
    // Control signals
    connect(m_orientationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onOrientationChanged);
    connect(m_sliceSlider, &QSlider::valueChanged,
            this, &MainWindow::onSliceSliderChanged);
    connect(m_sliceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onSliceSliderChanged);
    
    // Navigation signals
    connect(m_zoomInButton, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(m_zoomOutButton, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(m_resetViewButton, &QPushButton::clicked, this, &MainWindow::resetView);
}

void MainWindow::browseFile()
{
    QString fileName = m_fileManager->selectNiftiFile(this);
    if (!fileName.isEmpty()) {
        m_currentFilePath = fileName;
        m_filePathLabel->setText(fileName);
        
        // Load the file
        if (m_fileManager->loadNiftiFile(fileName)) {
            // File loading will trigger signals that handle the rest
        }
    }
}

void MainWindow::onFileLoadingStarted(const QString &fileName)
{
    m_statusLabel->setText(QString("Loading %1...").arg(fileName));
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    enableControls(false);
}

void MainWindow::onFileLoadingProgress(int percentage)
{
    m_progressBar->setValue(percentage);
}

void MainWindow::onFileLoadingCompleted(const QString &fileName)
{
    m_statusLabel->setText(QString("Loaded %1").arg(fileName));
    m_progressBar->setVisible(false);
    m_fileLoaded = true;
    
    // Set image data to volume renderer
    m_volumeRenderer->setImageData(m_fileManager->getImageData());
    
    updateSliceControls();
    updateFileInfo();
    enableControls(true);
}

void MainWindow::onFileLoadingError(const QString &errorMessage)
{
    m_statusLabel->setText("Error loading file");
    m_progressBar->setVisible(false);
    QMessageBox::critical(this, "Error", errorMessage);
    enableControls(false);
}

void MainWindow::onSliceChanged(int slice)
{
    // Update UI controls without triggering signals
    m_sliceSlider->blockSignals(true);
    m_sliceSpinBox->blockSignals(true);
    
    m_sliceSlider->setValue(slice);
    m_sliceSpinBox->setValue(slice);
    
    m_sliceLabel->setText(QString("Slice: %1 / %2")
                         .arg(slice)
                         .arg(m_volumeRenderer->getMaxSlice()));
    
    m_sliceSlider->blockSignals(false);
    m_sliceSpinBox->blockSignals(false);
}

void MainWindow::onOrientationChanged()
{
    if (!m_fileLoaded) return;
    
    int orientationIndex = m_orientationCombo->currentData().toInt();
    m_volumeRenderer->setOrientation(static_cast<VolumeRenderer::ViewOrientation>(orientationIndex));
    updateSliceControls();
}

void MainWindow::onSliceSliderChanged(int value)
{
    if (m_fileLoaded) {
        m_volumeRenderer->setSlice(value);
    }
}





void MainWindow::zoomIn()
{
    if (m_fileLoaded) {
        m_volumeRenderer->zoomIn();
    }
}

void MainWindow::zoomOut()
{
    if (m_fileLoaded) {
        m_volumeRenderer->zoomOut();
    }
}

void MainWindow::resetView()
{
    if (m_fileLoaded) {
        m_volumeRenderer->resetView();
    }
}





void MainWindow::updateSliceControls()
{
    if (!m_fileLoaded) return;
    
    int minSlice = m_volumeRenderer->getMinSlice();
    int maxSlice = m_volumeRenderer->getMaxSlice();
    int currentSlice = m_volumeRenderer->getCurrentSlice();
    
    m_sliceSlider->setRange(minSlice, maxSlice);
    m_sliceSpinBox->setRange(minSlice, maxSlice);
    
    m_sliceSlider->setValue(currentSlice);
    m_sliceSpinBox->setValue(currentSlice);
    
    m_sliceLabel->setText(QString("Slice: %1 / %2").arg(currentSlice).arg(maxSlice));
}



void MainWindow::updateFileInfo()
{
    if (m_fileLoaded) {
        QString info = m_fileManager->getFileInfo();
        m_infoDisplay->setText(info);
    } else {
        m_infoDisplay->setText("No file loaded");
    }
}

void MainWindow::enableControls(bool enabled)
{
    m_orientationCombo->setEnabled(enabled);
    m_sliceSlider->setEnabled(enabled);
    m_sliceSpinBox->setEnabled(enabled);
    m_zoomInButton->setEnabled(enabled);
    m_zoomOutButton->setEnabled(enabled);
    m_resetViewButton->setEnabled(enabled);
}

void MainWindow::applyDarkTheme()
{
    QString styleSheet = R"(
        QMainWindow {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        QMenuBar {
            background-color: #3c3c3c;
            color: #ffffff;
            border-bottom: 1px solid #555;
        }
        QMenuBar::item:selected {
            background-color: #555;
        }
        QMenu {
            background-color: #3c3c3c;
            color: #ffffff;
            border: 1px solid #555;
        }
        QMenu::item:selected {
            background-color: #555;
        }
        QStatusBar {
            background-color: #3c3c3c;
            color: #ffffff;
            border-top: 1px solid #555;
        }
        QGroupBox {
            color: #ffffff;
            border: 2px solid #555;
            border-radius: 5px;
            margin-top: 1ex;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        QPushButton {
            background-color: #404040;
            color: #ffffff;
            border: 1px solid #666;
            padding: 5px;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #505050;
        }
        QPushButton:pressed {
            background-color: #353535;
        }
        QComboBox {
            background-color: #404040;
            color: #ffffff;
            border: 1px solid #666;
            padding: 3px;
            border-radius: 3px;
        }
        QComboBox:hover {
            border: 1px solid #888;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox::down-arrow {
            border: none;
        }
        QSlider::groove:horizontal {
            background-color: #555;
            height: 6px;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background-color: #0078d4;
            width: 16px;
            border-radius: 8px;
            margin: -5px 0;
        }
        QSlider::handle:horizontal:hover {
            background-color: #106ebe;
        }
        QSpinBox {
            background-color: #404040;
            color: #ffffff;
            border: 1px solid #666;
            padding: 3px;
            border-radius: 3px;
        }
        QSpinBox:hover {
            border: 1px solid #888;
        }
        QTextEdit {
            background-color: #404040;
            color: #ffffff;
            border: 1px solid #666;
            border-radius: 3px;
        }
        QLabel {
            color: #ffffff;
        }
    )";
    
    // Update file path label for dark theme
    m_filePathLabel->setStyleSheet("QLabel { background-color: #2b2b2b; color: #ffffff; padding: 3px 8px; border: 1px solid #555; font-size: 12px; }");
    
    this->setStyleSheet(styleSheet);
}
