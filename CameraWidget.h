#pragma once
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include "MainWindow.h"

class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    CameraWidget();
    //QString filename;
public slots:
    void saveImage();
signals:
    void imageCaptured(const QImage& image);
private:
    QCamera* m_camera;
    QCameraViewfinder* m_viewfinder;
    QCameraImageCapture* m_imageCapture;
    QPushButton* m_saveButton;
    QPushButton* m_putButton;
};