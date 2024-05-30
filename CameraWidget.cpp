#include "CameraWidget.h"


CameraWidget::CameraWidget()
{
    setWindowTitle(QString::fromLocal8Bit("本地相机拍照"));
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    // 创建相机对象
    m_camera = new QCamera(this);

    // 创建相机取景器对象
    m_viewfinder = new QCameraViewfinder(this);
    m_viewfinder->setFixedSize(640, 480);

    // 创建相机图像捕捉对象
    m_imageCapture = new QCameraImageCapture(m_camera, this);

    // 创建保存按钮
    m_saveButton = new QPushButton(QString::fromLocal8Bit("拍照"), this);

    // 创建布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_viewfinder);
    layout->addWidget(m_saveButton);

    // 连接信号和槽
    connect(m_saveButton, &QPushButton::clicked, this, &CameraWidget::saveImage);
    
    // 设置相机取景器
    m_camera->setViewfinder(m_viewfinder);
    // 开启相机
    m_camera->start();

}

void CameraWidget::saveImage()
{
    // ...

    // 捕获图像
    QImageEncoderSettings settings = m_imageCapture->encodingSettings();
    settings.setCodec("image/jpeg");
    m_imageCapture->setEncodingSettings(settings);

    // 捕获图像并发送捕获图像信号
    m_imageCapture->capture();
    connect(m_imageCapture, &QCameraImageCapture::imageCaptured, this, [this](int id, const QImage& image) {
        // 发射捕获图像信号
        emit imageCaptured(image);
        });
}