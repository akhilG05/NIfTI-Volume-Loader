#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include "FileManager.h"
#include "VolumeRenderer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseFile();
    void onFileLoadingStarted(const QString &fileName);
    void onFileLoadingProgress(int percentage);
    void onFileLoadingCompleted(const QString &fileName);
    void onFileLoadingError(const QString &errorMessage);
    
    void onSliceChanged(int slice);
    void onOrientationChanged();
    void onSliceSliderChanged(int value);
    
    // Navigation controls
    void zoomIn();
    void zoomOut();
    void resetView();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupControlPanel();
    
    void connectSignals();
    void updateSliceControls();
    void updateFileInfo();
    void enableControls(bool enabled);
    void applyDarkTheme();

    // Core components
    FileManager *m_fileManager;
    VolumeRenderer *m_volumeRenderer;
    
    // UI Components
    QPushButton *m_browseButton;
    QLabel *m_filePathLabel;
    QWidget *m_renderWidget;
    
    // Control panel
    QGroupBox *m_controlPanel;
    QComboBox *m_orientationCombo;
    QSlider *m_sliceSlider;
    QSpinBox *m_sliceSpinBox;
    QLabel *m_sliceLabel;
    

    
    // Navigation controls
    QPushButton *m_zoomInButton;
    QPushButton *m_zoomOutButton;
    QPushButton *m_resetViewButton;
    
    // Information display
    QTextEdit *m_infoDisplay;
    
    // Status and progress
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
    
    // Layout
    QSplitter *m_mainSplitter;
    QWidget *m_centralWidget;
    
    // Current state
    QString m_currentFilePath;
    bool m_fileLoaded;
};

#endif // MAINWINDOW_H
