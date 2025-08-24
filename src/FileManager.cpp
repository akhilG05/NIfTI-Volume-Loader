#include "FileManager.h"
#include <vtkNIFTIImageReader.h>
#include <vtkImageData.h>
#include <QFileInfo>
#include <QDebug>
#include <QApplication>

FileManager::FileManager(QObject *parent)
    : QObject(parent)
    , m_reader(nullptr)
    , m_imageData(nullptr)
{
    m_reader = vtkNIFTIImageReader::New();
}

FileManager::~FileManager()
{
    if (m_reader) {
        m_reader->Delete();
    }
}

QString FileManager::selectNiftiFile(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(
        parent,
        "Select NifTI File",
        QDir::homePath(),
        "NifTI Files (*.nii *.nii.gz);;All Files (*)"
    );
    
    return fileName;
}

bool FileManager::loadNiftiFile(const QString &filePath)
{
    if (filePath.isEmpty()) {
        emit fileLoadingError("No file selected");
        return false;
    }
    
    if (!validateFile(filePath)) {
        emit fileLoadingError("Invalid NifTI file: " + filePath);
        return false;
    }
    
    emit fileLoadingStarted(QFileInfo(filePath).fileName());
    
    try {
        // Set the file to read
        m_reader->SetFileName(filePath.toStdString().c_str());
        
        // Update progress
        emit fileLoadingProgress(30);
        QApplication::processEvents();
        
        // Read the file
        m_reader->Update();
        
        emit fileLoadingProgress(70);
        QApplication::processEvents();
        
        // Get the image data
        m_imageData = m_reader->GetOutput();
        
        if (!m_imageData) {
            emit fileLoadingError("Failed to read image data from file");
            return false;
        }
        
        emit fileLoadingProgress(100);
        m_lastLoadedFile = filePath;
        
        emit fileLoadingCompleted(QFileInfo(filePath).fileName());
        return true;
        
    } catch (const std::exception &e) {
        emit fileLoadingError(QString("Error loading file: %1").arg(e.what()));
        return false;
    }
}

vtkImageData* FileManager::getImageData() const
{
    return m_imageData;
}

QString FileManager::getLastLoadedFile() const
{
    return m_lastLoadedFile;
}

QString FileManager::getFileInfo() const
{
    if (!m_imageData) {
        return "No file loaded";
    }
    
    int *dimensions = m_imageData->GetDimensions();
    double *spacing = m_imageData->GetSpacing();
    double *origin = m_imageData->GetOrigin();
    
    QString info;
    info += QString("File: %1\n").arg(QFileInfo(m_lastLoadedFile).fileName());
    info += QString("Dimensions: %1 x %2 x %3\n").arg(dimensions[0]).arg(dimensions[1]).arg(dimensions[2]);
    info += QString("Spacing: %1 x %2 x %3 mm\n").arg(spacing[0], 0, 'f', 2).arg(spacing[1], 0, 'f', 2).arg(spacing[2], 0, 'f', 2);
    info += QString("Origin: %1 x %2 x %3 mm\n").arg(origin[0], 0, 'f', 2).arg(origin[1], 0, 'f', 2).arg(origin[2], 0, 'f', 2);
    
    return info;
}

bool FileManager::isValidNiftiFile(const QString &filePath) const
{
    QFileInfo fileInfo(filePath);
    return fileInfo.exists() && 
           (fileInfo.suffix().toLower() == "nii" || 
            filePath.toLower().endsWith(".nii.gz"));
}

bool FileManager::validateFile(const QString &filePath)
{
    if (!isValidNiftiFile(filePath)) {
        return false;
    }
    
    // Additional validation can be added here
    return true;
}
