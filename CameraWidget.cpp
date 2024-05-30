#include "CameraWidget.h"


CameraWidget::CameraWidget()
{
    setWindowTitle(QString::fromLocal8Bit("�����������"));
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    // �����������
    m_camera = new QCamera(this);

    // �������ȡ��������
    m_viewfinder = new QCameraViewfinder(this);
    m_viewfinder->setFixedSize(640, 480);

    // �������ͼ��׽����
    m_imageCapture = new QCameraImageCapture(m_camera, this);

    // �������水ť
    m_saveButton = new QPushButton(QString::fromLocal8Bit("����"), this);

    // ��������
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_viewfinder);
    layout->addWidget(m_saveButton);

    // �����źźͲ�
    connect(m_saveButton, &QPushButton::clicked, this, &CameraWidget::saveImage);
    
    // �������ȡ����
    m_camera->setViewfinder(m_viewfinder);
    // �������
    m_camera->start();

}

void CameraWidget::saveImage()
{
    // ...

    // ����ͼ��
    QImageEncoderSettings settings = m_imageCapture->encodingSettings();
    settings.setCodec("image/jpeg");
    m_imageCapture->setEncodingSettings(settings);

    // ����ͼ�񲢷��Ͳ���ͼ���ź�
    m_imageCapture->capture();
    connect(m_imageCapture, &QCameraImageCapture::imageCaptured, this, [this](int id, const QImage& image) {
        // ���䲶��ͼ���ź�
        emit imageCaptured(image);
        });
}