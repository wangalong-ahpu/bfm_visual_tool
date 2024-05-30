#include "MainWindow.h"
#include "Functions.h"
const double SHAPE_SCALE = 0.1;
const double TEX_SCALE = 120;
const double EXPR_SCALE = 0.3;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)//, ui *(new Ui::MainWindow)//��ָ��ui��ֵ
{
	ui.setupUi(this);
	statusBar()->showMessage("loading...", 3000);
	model.init("inputs-2017_test.txt");		//ģ�ͳ�ʼ������ȡ���ݿ��ļ������أ�
	std::cout << "��������initUI����"<<endl;
	initUI();
	initSignalSlots();
	setWindowTitle(QString::fromLocal8Bit("����ҽ�Ʋ�ҵ����ά�����ؽ���������� V0.9"));
		
	statusBar()->showMessage(QString::fromLocal8Bit("����ҽ�Ʋ�ҵ����ά�����ؽ����������"));
}

//��ʼ������
void MainWindow::initUI()
{
	genRndFaceBtn = ui.centralWidget->findChild<QPushButton*>("genRndFaceBtn");
	savePlyBtn = ui.centralWidget->findChild<QPushButton*>("savePlyBtn");
	setShapePcBtn = ui.centralWidget->findChild<QPushButton*>("setShapePcBtn");
	setTexPcBtn = ui.centralWidget->findChild<QPushButton*>("setTexPcBtn");
	setExprPcBtn = ui.centralWidget->findChild<QPushButton*>("setExprPcBtn");
	pushButton_3dto3d = ui.centralWidget->findChild<QPushButton*>("pushButton_3dto3d");

	rndRangeSlider = ui.centralWidget->findChild<QSlider*>("rndRangeSlider");
	shapeSlider = ui.centralWidget->findChild<QSlider*>("shapeSlider");
	texSlider = ui.centralWidget->findChild<QSlider*>("texSlider");
	exprSlider = ui.centralWidget->findChild<QSlider*>("exprSlider");

	shapeComboBox = ui.centralWidget->findChild<QComboBox*>("shapeComboBox");
	texComboBox = ui.centralWidget->findChild<QComboBox*>("texComboBox");
	exprComboBox = ui.centralWidget->findChild<QComboBox*>("exprComboBox");
	for (int i = 1; i <= model.get_n_id_pc(); i++) {
		shapeComboBox->addItem(QString::number(i));
		texComboBox->addItem(QString::number(i));
	}
	for (int i = 1; i <= model.get_n_expr_pc(); i++)
		exprComboBox->addItem(QString::number(i));

	saveFilename = ui.centralWidget->findChild<QTextEdit*>("saveFilename");
	openGLWidget = ui.centralWidget->findChild<OpenGLWidget*>("openGLWidget");
	openGLWidget_2 = ui.centralWidget->findChild<OpenGLWidget*>("openGLWidget_2");
	
	
	//����
	ui.label_19->setStyleSheet("color: blue;");
	
	ui.openGLWidget->setWindowTitle("My OpenGL Widget");
	//ui.tabWidget->setAttribute(Qt::WA_StyledBackground);
	////��������ɫ������tabѡ����ɫ��δѡ�л�ɫ
	//ui.tabWidget->setStyleSheet("QTabWidget#tabWidget{background-color:rgb(255,255,255);}\
 //                                QTabBar::tab::selected{background-color:rgb(255,255,255);color:rgb(0,0,0);}");
	
	qDebug() << "[viewer] ui init done.\n";
}

//��ʼ���źŲ�
void MainWindow::initSignalSlots()
{
	QObject::connect(genRndFaceBtn, SIGNAL(clicked()), this, SLOT(generateRandomFace()));
	QObject::connect(setShapePcBtn, SIGNAL(clicked()), this, SLOT(setShapePc()));
	QObject::connect(setTexPcBtn, SIGNAL(clicked()), this, SLOT(setTexPc()));
	QObject::connect(setExprPcBtn, SIGNAL(clicked()), this, SLOT(setExprPc()));

	QObject::connect(shapeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(synShapeSlider()));
	QObject::connect(texComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(synTexSlider()));
	QObject::connect(exprComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(synExprSlider()));

	QObject::connect(savePlyBtn, SIGNAL(clicked()), this, SLOT(savePly()));
	//ģ��Ԥ�����ؼ�
	QObject::connect(ui.pushButton_left_1, &QPushButton::clicked, this, &MainWindow::leftModel);
	QObject::connect(ui.pushButton_left_2, &QPushButton::clicked, this, &MainWindow::leftModel);
	QObject::connect(ui.pushButton_right_1, &QPushButton::clicked, this, &MainWindow::rightModel);
	QObject::connect(ui.pushButton_right_2, &QPushButton::clicked, this, &MainWindow::rightModel);
	QObject::connect(ui.pushButton_plus_1, &QPushButton::clicked, this, &MainWindow::plusModel);
	QObject::connect(ui.pushButton_plus_2, &QPushButton::clicked, this, &MainWindow::plusModel);
	QObject::connect(ui.pushButton_minus_1, &QPushButton::clicked, this, &MainWindow::minusModel);
	QObject::connect(ui.pushButton_minus_2, &QPushButton::clicked, this, &MainWindow::minusModel);
	QObject::connect(ui.pushButton_deleteGL_1, &QPushButton::clicked, this, &MainWindow::delModel);
	QObject::connect(ui.pushButton_deleteGL_2, &QPushButton::clicked, this, &MainWindow::delModel);

	//����ͼƬ��ť
	QObject::connect(ui.pushButton_openfile, &QPushButton::clicked, this, &MainWindow::openImg);
	QObject::connect(ui.pushButton_openfile_2, &QPushButton::clicked, this, &MainWindow::openImg);
	QObject::connect(ui.pushButton_openfile_3, &QPushButton::clicked, this, &MainWindow::openImg);
	//�������ť
	QObject::connect(ui.pushButton_open2dcam, &QPushButton::clicked, this, &MainWindow::openCamera);
	QObject::connect(ui.pushButton_open2dcam_2, &QPushButton::clicked, this, &MainWindow::openCamera);
	QObject::connect(ui.pushButton_open2dcam_3, &QPushButton::clicked, this, &MainWindow::openCamera);
	//ȡ����ť
	QObject::connect(ui.pushButton_inputcancel, &QPushButton::clicked, this, &MainWindow::cancelOpen);
	QObject::connect(ui.pushButton_inputcancel_2, &QPushButton::clicked, this, &MainWindow::cancelOpen);
	QObject::connect(ui.pushButton_inputcancel_3, &QPushButton::clicked, this, &MainWindow::cancelOpen);
	//��ʼ�ؽ�
	QObject::connect(ui.pushButton_2dto3d, &QPushButton::clicked, this, &MainWindow::startReconstruct);//ͼƬ��ģ
	QObject::connect(ui.pushButton_optimizemodel, &QPushButton::clicked, this, &MainWindow::startrRegistration);//ģ����׼��ģ
	//�������ģ���ļ�
	QObject::connect(ui.pushButton_openmodel, &QPushButton::clicked, this, &MainWindow::openModel);
	//ȷ������
	QObject::connect(ui.pushButton_plastic, &QPushButton::clicked, this, &MainWindow::plasticModel);
	qDebug() << "init SignalSlots done.\n";
}

//�����ڴ�ӡ��Ϣ
void MainWindow::outputMessage(string s, bool showplainTextEdit, bool showstatusBar, bool showmsgBox , bool showpicmessage,bool showmodelmessage)
{
	QString S = QString::fromStdString(s);
	if (showplainTextEdit) {
		ui.plainTextEdit->appendPlainText(S);
	}
	if (showstatusBar) {
		statusBar()->showMessage(S);
	}
	if (showmsgBox) {
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("��Ϣ"));
		msgBox.setText(S);
		msgBox.exec();
	}
	if (showpicmessage) {
		ui.label_19->setText(S);
	}
	if (showmodelmessage) {
		ui.label_24->setText(S);
	}
	ui.plainTextEdit->update();
	ui.label_19->update();
	ui.label_24->update();
}

//�����������
void MainWindow::generateRandomFace()		//�����������
{
	statusBar()->showMessage("generating...", 3000);
	double sliderVal = ((double)rndRangeSlider->value()/100>0)? (double)rndRangeSlider->value() / 100 :0.07;
	std::cout << "sliderVal" << sliderVal << endl;
	model.generate_random_face(sliderVal / SHAPE_SCALE, sliderVal / TEX_SCALE, sliderVal / EXPR_SCALE);
	openGLWidget->theta = 0.0;
	openGLWidget->scale = 1.0;
	genface = true;
	openGLWidget->clearopengl = false;
	openGLWidget->update();
	synShapeSlider();
	synTexSlider();
	synExprSlider();
	qDebug() << "generate Random Face done.\n";
}

void MainWindow::synShapeSlider()
{
	int pc = shapeComboBox->currentIndex();
	shapeSlider->setValue(int(model.get_mutable_shape_coef()(pc) * SHAPE_SCALE));
}

void MainWindow::synTexSlider()
{
	int pc = texComboBox->currentIndex();
	texSlider->setValue(int(model.get_mutable_tex_coef()(pc) * TEX_SCALE));
}

void MainWindow::synExprSlider()
{
	int pc = exprComboBox->currentIndex();
	exprSlider->setValue(int(model.get_mutable_expr_coef()(pc) * EXPR_SCALE));
}

// ����������״���ɷ�
void MainWindow::setShapePc()
{
	int pc = shapeComboBox->currentIndex();
	double pcValue = shapeSlider->value() / SHAPE_SCALE;
	model.get_mutable_shape_coef()(pc) = pcValue;
	model.generate_face();
	openGLWidget->update();
}

// ���������������ɷ�
void MainWindow::setTexPc()
{
	int pc = texComboBox->currentIndex();
	double pcValue = texSlider->value() / TEX_SCALE;
	model.get_mutable_tex_coef()(pc) = pcValue;
	model.generate_face();
	openGLWidget->update();
}

// ���������������ɷ�
void MainWindow::setExprPc()
{
	int pc = exprComboBox->currentIndex();
	double pcValue = exprSlider->value() / EXPR_SCALE;
	model.get_mutable_expr_coef()(pc) = pcValue;
	model.generate_face();
	openGLWidget->update();
}

// ����ģ��
void MainWindow::savePly()		
{
	std::string filename = saveFilename->toPlainText().toStdString();

	/* Make sure filename is like *.ply format (default filename = rnd_face.ply) */
	int fSize = filename.size();
	if (fSize == 0)
		filename = "rnd_face.ply";
	else if (fSize <= 4 || filename.substr(fSize - 4) != ".ply")
		filename = filename + ".ply";

	model.ply_write(filename);
	QDialog* dialog = new QDialog;
	dialog->setModal(true);
	dialog->resize(300, 100);
	QLabel* label = new QLabel(dialog);
	label->setText("save success");
	dialog->exec();
}

// ģ��Ԥ�����ؼ�
void MainWindow::leftModel() //������ת
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button == ui.pushButton_left_1) {
		openGLWidget->theta += 0.1;
	}
	else {
		openGLWidget_2->theta += 0.1;
	}
	openGLWidget->update();
	openGLWidget_2->update();
}
void MainWindow::rightModel() //������ת
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button == ui.pushButton_right_1) {
		openGLWidget->theta -= 0.1;
	}
	else {
		openGLWidget_2->theta -= 0.1;
	}
	openGLWidget->update();
	openGLWidget_2->update();
}
void MainWindow::plusModel() //�Ŵ�
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button == ui.pushButton_plus_1) {
		if (!genface)
			openGLWidget->scale += 15;
		else
			openGLWidget->scale += 0.1;
	}
	else 
		openGLWidget_2->scale += 15;
	openGLWidget->update();
	openGLWidget_2->update();
}
void MainWindow::minusModel() 
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button == ui.pushButton_minus_1) {
		if (!genface)
			openGLWidget->scale -= 15;
		else
			openGLWidget->scale -= 0.1;
	}
	else
		openGLWidget_2->scale -= 15;
	openGLWidget->update();
	openGLWidget_2->update();
}
void MainWindow::delModel() 
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	
	if (button == ui.pushButton_deleteGL_1) {
		OpenModelFilePath = "";
		openGLWidget->clearopengl = true;
		openGLWidget->paintGL();
	}
	else {
		openGLWidget_2->clearopengl = true;
		openGLWidget_2->paintGL();
	}
	openGLWidget->update();
	openGLWidget_2->update();
}

// ����ͼƬ
void MainWindow::openImg()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	QString OpenFile,OpenFilePath_temp;
	QImage image;	
	// �����ļ��Ի������ô�С
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setDirectory(QDir::currentPath()+"/Input");
	dialog.setNameFilter(tr("Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)"));
	dialog.setOptions(QFileDialog::DontUseNativeDialog);
	dialog.setMinimumSize(800, 600); // ������С��С

	// ���ļ����е�ͼƬ�ļ�
	if (dialog.exec() == QDialog::Accepted) {
		OpenFile = dialog.selectedFiles().first();

		// ��ʾ��ʾͼƬ��·��
		QFileInfo OpenFileInfo;
		OpenFileInfo = QFileInfo(OpenFile);
		OpenFilePath_temp = OpenFileInfo.filePath();
		ui.plainTextEdit->appendPlainText(QString("%1%2")
			.arg(QString::fromLocal8Bit("ͼƬ·����"))
			.arg(OpenFilePath_temp)
		);//�������Ϣ��
	}


	if (OpenFile != "")
	{
		if (image.load(OpenFile))
		{
			if (button == ui.pushButton_openfile) {
				ui.label_18->setPixmap(QPixmap::fromImage(image));
				ui.label_18->setScaledContents(true);
				this->OpenFilePath = OpenFilePath_temp;
			}
			else {
				if (button == ui.pushButton_openfile_2) {
					ui.label_20->setPixmap(QPixmap::fromImage(image));
					ui.label_20->setScaledContents(true);
				}
				else {
					ui.label_21->setPixmap(QPixmap::fromImage(image));
					ui.label_21->setScaledContents(true);
				}
			}
		}
		else
		{
			ui.plainTextEdit->appendPlainText(QString("%1")
				.arg(QString::fromLocal8Bit("ͼƬ��ȡʧ�ܣ�"))
			);//�������Ϣ��
		}
	}
	else {
		ui.plainTextEdit->appendPlainText(QString("%1")
			.arg(QString::fromLocal8Bit("ͼƬ��ȡʧ�ܣ�����ͼƬλ�ã�"))
		);//�������Ϣ��
	}
}

// �����
void MainWindow::openCamera()
{
	this->openCamButton = qobject_cast<QPushButton*>(sender());
	CameraWidget* cameraWidget1 = new CameraWidget();
	cameraWidget1->setWindowFlags(Qt::Window);
	cameraWidget1->setAttribute(Qt::WA_DeleteOnClose);
	// ��������ڵ�imageCaptured�ź����ӵ��ۺ���
	connect(cameraWidget1, &CameraWidget::imageCaptured, this, &MainWindow::showCapturedImage);
	// ��ʾ�������
	cameraWidget1->show();
}

//��QLabel�ؼ�����ʾ�����ͼ��
void MainWindow::showCapturedImage(const QImage& image)
{	
	try
	{
		if (this->openCamButton == ui.pushButton_open2dcam) {
			ui.label_18->setPixmap(QPixmap::fromImage(image));
			ui.label_18->setScaledContents(true);
			// ����QImage���ض�·����ͼ���ļ�
			this->OpenFilePath = "./Input/Temp.jpg";
			image.save(this->OpenFilePath);
		}
		else {
			if (this->openCamButton == ui.pushButton_open2dcam_2) {
				ui.label_20->setPixmap(QPixmap::fromImage(image));
				ui.label_20->setScaledContents(true);
			}
			else {
				ui.label_21->setPixmap(QPixmap::fromImage(image));
				ui.label_21->setScaledContents(true);
			}
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Error reading the captured image: " << e.what() << std::endl;
		std::terminate();
	}
	
}

// ȡ��
void MainWindow::cancelOpen() 
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	QPalette palette;
	palette.setColor(QPalette::Window, Qt::white);
	
	try {
		if (button == ui.pushButton_inputcancel) {
			ui.label_18->setText(QString::fromLocal8Bit("����������"));
			ui.label_18->setStyleSheet("color: red;");
			ui.label_18->setAutoFillBackground(true);
			ui.label_18->setPalette(palette);
		}
		else {
			if (button == ui.pushButton_inputcancel_2) {
				ui.label_20->setText(QString::fromLocal8Bit("����������"));
				ui.label_20->setStyleSheet("color: red;");
				ui.label_20->setAutoFillBackground(true);
				ui.label_20->setPalette(palette);
			}
			else {
				ui.label_21->setText(QString::fromLocal8Bit("����������"));
				ui.label_21->setStyleSheet("color: red;");
				ui.label_21->setAutoFillBackground(true);
				ui.label_21->setPalette(palette);
			}
		}
		ui.progressBar->setValue(0);//��������Ϊ0
		this->OpenFilePath = "";
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Error Cancel: " << e.what() << std::endl;
		std::terminate();
	}
}

//��ʼ�ؽ�
void MainWindow::startReconstruct()
{
	//�������
	std::string imagefile,outplyfilename, modelfile;
	imagefile = this->OpenFilePath.toStdString();
	modelfile = "./Data/sfm_shape_3448.bin";//ģ��·��
	outplyfilename = "./Output/Reconstructed_Face.ply";//���.ply·��
	//�жϺ�����
	if (imagefile == "") {
		outputMessage(u8"��ģʧ�ܣ������Ƿ�����ͼƬ��", true, true,true,true);
		return;
	}
	//�ؽ�
	bool candorec = Reconstruct(imagefile, modelfile, outplyfilename);//�ؽ��������Ƿ񱨴�
	
	if (candorec != 1)
	{
		outputMessage(u8"ģ������ʧ�ܣ���������������Ҫ��", true, true, false, true);
		ui.pushButton_2dto3d->setText(QString::fromLocal8Bit("���½�ģ"));
		ui.progressBar->setValue(0);//��������Ϊ0
		ui.progressBar->update();
		ui.pushButton_2dto3d->update();
	}
	else {
		OpenModelFilePath = QString::fromStdString(outplyfilename);//����ǰģ��λ��
		//�����opengl������
		ui.progressBar->setValue(100);
		openGLWidget->loadPLYFile(outplyfilename);
		genface = false;
		openGLWidget->scale = 1500.0;
		openGLWidget->theta = 0.6;
		openGLWidget->clearopengl = false;
		openGLWidget->paintGL();
		openGLWidget->update();
		//��ʾ���ؽ���ɡ�
		outputMessage(u8"��ģ��ɣ��ɲ鿴������ģ��", true, true, false, true);
		ui.pushButton_2dto3d->setText(QString::fromLocal8Bit("���½�ģ"));
		ui.progressBar->setValue(0);
		ui.label_19->update();
		ui.pushButton_2dto3d->update();
		ui.progressBar->update();
	}
}

//�򿪱���model�ļ�
void MainWindow::openModel() {
	QString OpenFile;
	//���ļ��Ի���ѡ��model�ļ�
	
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setDirectory(QDir::currentPath() + "/Data");
	dialog.setNameFilter(tr("Image Files(*.ply *.obj *.pcd);;All(*.*)"));
	dialog.setOptions(QFileDialog::DontUseNativeDialog);
	dialog.setMinimumSize(800, 600); // ������С��С

	// ���ļ����е�ģ���ļ�
	if (dialog.exec() == QDialog::Accepted) {
		OpenFile = dialog.selectedFiles().first();

		// ��ʾ��ʾģ�͵�·��
		QFileInfo OpenFileInfo;
		OpenFileInfo = QFileInfo(OpenFile);
		this->OpenModelFilePath = OpenFileInfo.filePath();
		ui.plainTextEdit->appendPlainText(QString("%1%2")
			.arg(QString::fromLocal8Bit("ģ��·����"))
			.arg(this->OpenModelFilePath)
		);//�������Ϣ��
		outputMessage(u8"ģ�Ͷ�ȡ��...", true, true, false, false, true);
	}
	if (openGLWidget->loadPLYFile(OpenModelFilePath.toStdString())) //չʾmodel��opengl����
	{
		genface = false;
		openGLWidget->scale = 1500.0;
		openGLWidget->theta = 0.6;
		openGLWidget->clearopengl = false;
		openGLWidget->paintGL();
		openGLWidget->update();
		outputMessage(u8"ģ�ͼ�����ɣ���ʹ�ü��̺����鿴����", true, true, false ,false, true);
	}
	else 
		outputMessage(u8"ģ�ͼ���ʧ�ܣ���������ģ�͸�ʽ��", true, true, false, false, true);
}

//�Ż�ģ��
void MainWindow::startrRegistration() {
	if (OpenModelFilePath.toStdString()!="") {
		std::string src_file = "./Data/bikou_di_rgb.ply";
		std::string tar_file = OpenModelFilePath.toStdString();
		OpenModelFilePath = fastRnrr(src_file, tar_file);//����ǰģ��λ��
		//�����opengl����
		if (openGLWidget->loadPLYFile(OpenModelFilePath.toStdString()))
		{
			openGLWidget->paintGL();openGLWidget->update();
			outputMessage(u8"�Ż���ɣ���鿴��", true, true);
		}
		else
			outputMessage(u8"�Ż�ʧ�ܣ������ԡ�", true, true, false, false, true);
	}
	else
		outputMessage(u8"���ȵ��뱾��ģ�ͣ�", true, true, true);
}

//�������Σ���ʵ�֣�
void MainWindow::plasticModel() 
{
	//����ǰ��model
	//OpenModelFilePath;



	//չʾ���κ��model
	QString srcModelFilePath = "./Data/bikou_di_rgb_biliang.ply";
	if (openGLWidget_2->loadPLYFile(srcModelFilePath.toStdString()))
	{
		genface = false;
		openGLWidget_2->scale = 1500.0;
		openGLWidget_2->theta = 0.6;
		openGLWidget_2->clearopengl = false;
		openGLWidget_2->paintGL();
		openGLWidget_2->update();
		outputMessage(u8"ģ�ͱ�����ɣ���ʹ�ü��̺����Ա�ǰ��Ч��", true, true, false, false, true);
	}
	else
	{
		outputMessage(u8"ģ�ͱ���ʧ�ܣ�", true, true, false, false, true);
	}
}

//�����¼�����
void MainWindow::keyPressEvent(QKeyEvent* event) {		
	bool WS=false;
	switch (event->key()) {
		case Qt::Key_A:
			openGLWidget->theta += 0.1;
			openGLWidget_2->theta += 0.1;
			break;
		case Qt::Key_D:
			openGLWidget->theta -= 0.1;
			openGLWidget_2->theta -= 0.1;
			break;
		case Qt::Key_W:
			openGLWidget->scale += 0.1;
			openGLWidget_2->scale += 0.1;
			WS = true;
			break;
		case Qt::Key_S:
			openGLWidget->scale -= 0.1;
			openGLWidget_2->scale -= 0.1;
			WS = true;
			break;
		default:
			break;
	}
	if (!genface&&WS) {
		if (event->key() == Qt::Key_W) {
			openGLWidget->scale += 10;
			openGLWidget_2->scale += 10;
		}
		else {
			openGLWidget->scale -= 50;
			openGLWidget_2->scale -= 50;
		}
	}
	openGLWidget->update();
	openGLWidget_2->update();
}

//�������¼�������
void MainWindow::wheelEvent(QWheelEvent* event) {		
	openGLWidget->scale += event->delta() / 500.0;
	openGLWidget_2->scale += event->delta() / 500.0;
	if (openGLWidget->scale <= 0.0)
		openGLWidget->scale = 0.01;
	openGLWidget->update();
	openGLWidget_2->update();
}