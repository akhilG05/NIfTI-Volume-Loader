#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QObject>
#include <QFileDialog>
#include <QMessageBox>

class vtkNIFTIImageReader;
class vtkImageData;

class FileManager : public QObject
{
    Q_OBJECT

public:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager();

    // File operations
    QString selectNiftiFile(QWidget *parent);
    bool loadNiftiFile(const QString &filePath);
    vtkImageData* getImageData() const;
    
    // File information
    QString getLastLoadedFile() const;
    QString getFileInfo() const;
    bool isValidNiftiFile(const QString &filePath) const;

signals:
    void fileLoadingStarted(const QString &fileName);
    void fileLoadingProgress(int percentage);
    void fileLoadingCompleted(const QString &fileName);
    void fileLoadingError(const QString &errorMessage);

private:
    vtkNIFTIImageReader *m_reader;
    QString m_lastLoadedFile;
    vtkImageData *m_imageData;
    
    bool validateFile(const QString &filePath);
    void updateProgress();
};

#endif // FILEMANAGER_H
