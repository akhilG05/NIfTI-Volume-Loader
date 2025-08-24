#ifndef FILEMANAGER_H
#define FILEMANAGER_H

// Qt base classes for object management and string handling
#include <QString>
#include <QObject>

// Qt dialogs for user interaction
#include <QFileDialog>
#include <QMessageBox>

// Forward declarations of VTK classes to avoid including headers
class vtkNIFTIImageReader;  // VTK reader for NIfTI file format
class vtkImageData;         // VTK data structure for image/volume data

/**
 * FileManager - Handles NIfTI file operations and validation
 * 
 * This class manages:
 * - File selection through dialogs
 * - NIfTI file loading and parsing
 * - File validation and error handling
 * - Progress reporting during file operations
 * - Access to loaded image data
 */
class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager();

    // File operations - core functionality
    QString selectNiftiFile(QWidget *parent);           // Open file dialog for NIfTI selection
    bool loadNiftiFile(const QString &filePath);        // Load and parse NIfTI file
    vtkImageData* getImageData() const;                 // Get loaded image data for rendering
    
    // File information - metadata and validation
    QString getLastLoadedFile() const;                  // Get path of last successfully loaded file
    QString getFileInfo() const;                        // Get formatted file information (dimensions, spacing, etc.)
    bool isValidNiftiFile(const QString &filePath) const; // Validate if file is a valid NIfTI format

signals:
    void fileLoadingStarted(const QString &fileName);    // Emitted when file loading begins
    void fileLoadingProgress(int percentage);            // Emitted during loading to update progress bar
    void fileLoadingCompleted(const QString &fileName);  // Emitted when file successfully loads
    void fileLoadingError(const QString &errorMessage);  // Emitted when file loading fails

private:
    // VTK components for file reading
    vtkNIFTIImageReader *m_reader;     // VTK reader for NIfTI format files
    QString m_lastLoadedFile;          // Path to the most recently loaded file
    vtkImageData *m_imageData;         // Currently loaded image data
    
    // Private helper methods
    bool validateFile(const QString &filePath);  // Internal file validation
    void updateProgress();                       // Update loading progress
};

#endif // FILEMANAGER_H
