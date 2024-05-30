#pragma once
/*********************************************************************

Author: wal
Date: 2023-6-1
Description: The main widget window of Basel face model visual tool.

**********************************************************************/

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QThread>
//#include <QTimer>
#include "ui_MainWindow.h"
#include <qslider.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qstatusbar.h>
#include <qdialog.h>
#include <QFileDialog>
#include "OpenGLWidget.h"
#include "CameraWidget.h"
#include <dlib/matrix.h>
#include "bfm.h"

class QSlider;
class QPushButton;
class QLabel;
class QComboBox;
class QOpenGLWidget;
class QKeyEvent;
class QWheelEvent;
class QDialog;

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = Q_NULLPTR);
	bfm& getModel() { return model; }
	void keyPressEvent(QKeyEvent* event);
	void wheelEvent(QWheelEvent* event);
	bool Reconstruct(string& imagefile, string& modelfile, string& outplyfilename);//人脸重建
	int DlibFaceDetection(std::string& imagefile, std::string& landmarksfile);//Dlib人脸关键点检测
	QString fastRnrr(std::string& src_file, std::string& tar_file);//Fast_RNRR

	bool genface = true;
	
signals:
	void mySignal(int value);

private slots:
	void generateRandomFace();
	void synShapeSlider();
	void synTexSlider();
	void synExprSlider();
	void setShapePc();
	void setTexPc();
	void setExprPc();
	void savePly();

	void leftModel();
	void rightModel();
	void plusModel();
	void minusModel();
	void delModel();


	void openImg();
	void cancelOpen();
	void openCamera();
	void startReconstruct();
	void startrRegistration();
	void openModel();
	void plasticModel();

	void showCapturedImage(const QImage& image);

private:
	Ui::MainWindowClass ui;

	bfm model;

	OpenGLWidget* openGLWidget;
	OpenGLWidget* openGLWidget_2;
	//CameraWidget* cameraWidget;
	QPushButton* genRndFaceBtn;
	QPushButton* savePlyBtn;
	QPushButton* setShapePcBtn;
	QPushButton* setTexPcBtn;
	QPushButton* setExprPcBtn;
	QPushButton* openCamButton;
	QPushButton* pushButton_3dto3d;
	//新增
	//QImage* image;
	QString OpenFilePath;
	QString OpenModelFilePath;
	

	QSlider* shapeSlider;
	QSlider* texSlider;
	QSlider* exprSlider;
	QSlider* rndRangeSlider;

	QComboBox* shapeComboBox;
	QComboBox* texComboBox;
	QComboBox* exprComboBox;

	QTextEdit* saveFilename;
	void outputMessage(string S, bool showplainTextEdit, bool showstatusBarbool, bool showmsgBox = false, bool showpicmessage = false, bool showmodelmessage = false);
	void initUI();
	void initSignalSlots();
};
