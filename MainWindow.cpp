#include "MainWindow.h"
#include "Functions.h"
const double SHAPE_SCALE = 0.1;
const double TEX_SCALE = 120;
const double EXPR_SCALE = 0.3;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)//, ui *(new Ui::MainWindow)//给指针ui赋值
{
	ui.setupUi(this);
	statusBar()->showMessage("loading...", 3000);
	model.init("inputs-2017_test.txt");		//模型初始化（读取数据库文件，加载）
	std::cout << "以下运行initUI函数"<<endl;
	initUI();
	initSignalSlots();
	setWindowTitle(QString::fromLocal8Bit("面向医疗产业的三维人脸重建及变形软件 V0.9"));
		
	statusBar()->showMessage(QString::fromLocal8Bit("面向医疗产业的三维人脸重建及变形软件"));
}

//初始化界面
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
	
	
	//美化
	ui.label_19->setStyleSheet("color: blue;");
	
	ui.openGLWidget->setWindowTitle("My OpenGL Widget");
	//ui.tabWidget->setAttribute(Qt::WA_StyledBackground);
	////标题栏红色背景，tab选中蓝色，未选中灰色
	//ui.tabWidget->setStyleSheet("QTabWidget#tabWidget{background-color:rgb(255,255,255);}\
 //                                QTabBar::tab::selected{background-color:rgb(255,255,255);color:rgb(0,0,0);}");
	
	qDebug() << "[viewer] ui init done.\n";
}

//初始化信号槽
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
	//模型预览区控件
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

	//导入图片按钮
	QObject::connect(ui.pushButton_openfile, &QPushButton::clicked, this, &MainWindow::openImg);
	QObject::connect(ui.pushButton_openfile_2, &QPushButton::clicked, this, &MainWindow::openImg);
	QObject::connect(ui.pushButton_openfile_3, &QPushButton::clicked, this, &MainWindow::openImg);
	//打开相机按钮
	QObject::connect(ui.pushButton_open2dcam, &QPushButton::clicked, this, &MainWindow::openCamera);
	QObject::connect(ui.pushButton_open2dcam_2, &QPushButton::clicked, this, &MainWindow::openCamera);
	QObject::connect(ui.pushButton_open2dcam_3, &QPushButton::clicked, this, &MainWindow::openCamera);
	//取消按钮
	QObject::connect(ui.pushButton_inputcancel, &QPushButton::clicked, this, &MainWindow::cancelOpen);
	QObject::connect(ui.pushButton_inputcancel_2, &QPushButton::clicked, this, &MainWindow::cancelOpen);
	QObject::connect(ui.pushButton_inputcancel_3, &QPushButton::clicked, this, &MainWindow::cancelOpen);
	//开始重建
	QObject::connect(ui.pushButton_2dto3d, &QPushButton::clicked, this, &MainWindow::startReconstruct);//图片建模
	QObject::connect(ui.pushButton_optimizemodel, &QPushButton::clicked, this, &MainWindow::startrRegistration);//模型配准建模
	//导入点云模型文件
	QObject::connect(ui.pushButton_openmodel, &QPushButton::clicked, this, &MainWindow::openModel);
	//确定变形
	QObject::connect(ui.pushButton_plastic, &QPushButton::clicked, this, &MainWindow::plasticModel);
	qDebug() << "init SignalSlots done.\n";
}

//主窗口打印消息
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
		msgBox.setWindowTitle(QString::fromLocal8Bit("消息"));
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

//生成随机人脸
void MainWindow::generateRandomFace()		//生成随机人脸
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

// 设置脸部形状主成分
void MainWindow::setShapePc()
{
	int pc = shapeComboBox->currentIndex();
	double pcValue = shapeSlider->value() / SHAPE_SCALE;
	model.get_mutable_shape_coef()(pc) = pcValue;
	model.generate_face();
	openGLWidget->update();
}

// 设置脸部纹理主成分
void MainWindow::setTexPc()
{
	int pc = texComboBox->currentIndex();
	double pcValue = texSlider->value() / TEX_SCALE;
	model.get_mutable_tex_coef()(pc) = pcValue;
	model.generate_face();
	openGLWidget->update();
}

// 设置脸部表情主成分
void MainWindow::setExprPc()
{
	int pc = exprComboBox->currentIndex();
	double pcValue = exprSlider->value() / EXPR_SCALE;
	model.get_mutable_expr_coef()(pc) = pcValue;
	model.generate_face();
	openGLWidget->update();
}

// 保存模型
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

// 模型预览区控件
void MainWindow::leftModel() //向左旋转
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
void MainWindow::rightModel() //向右旋转
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
void MainWindow::plusModel() //放大
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

// 导入图片
void MainWindow::openImg()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	QString OpenFile,OpenFilePath_temp;
	QImage image;	
	// 创建文件对话框并设置大小
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setDirectory(QDir::currentPath()+"/Input");
	dialog.setNameFilter(tr("Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)"));
	dialog.setOptions(QFileDialog::DontUseNativeDialog);
	dialog.setMinimumSize(800, 600); // 设置最小大小

	// 打开文件夹中的图片文件
	if (dialog.exec() == QDialog::Accepted) {
		OpenFile = dialog.selectedFiles().first();

		// 显示所示图片的路径
		QFileInfo OpenFileInfo;
		OpenFileInfo = QFileInfo(OpenFile);
		OpenFilePath_temp = OpenFileInfo.filePath();
		ui.plainTextEdit->appendPlainText(QString("%1%2")
			.arg(QString::fromLocal8Bit("图片路径："))
			.arg(OpenFilePath_temp)
		);//输出到消息框
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
				.arg(QString::fromLocal8Bit("图片读取失败！"))
			);//输出到消息框
		}
	}
	else {
		ui.plainTextEdit->appendPlainText(QString("%1")
			.arg(QString::fromLocal8Bit("图片读取失败，请检查图片位置！"))
		);//输出到消息框
	}
}

// 打开相机
void MainWindow::openCamera()
{
	this->openCamButton = qobject_cast<QPushButton*>(sender());
	CameraWidget* cameraWidget1 = new CameraWidget();
	cameraWidget1->setWindowFlags(Qt::Window);
	cameraWidget1->setAttribute(Qt::WA_DeleteOnClose);
	// 将相机窗口的imageCaptured信号连接到槽函数
	connect(cameraWidget1, &CameraWidget::imageCaptured, this, &MainWindow::showCapturedImage);
	// 显示相机窗口
	cameraWidget1->show();
}

//在QLabel控件中显示捕获的图像
void MainWindow::showCapturedImage(const QImage& image)
{	
	try
	{
		if (this->openCamButton == ui.pushButton_open2dcam) {
			ui.label_18->setPixmap(QPixmap::fromImage(image));
			ui.label_18->setScaledContents(true);
			// 保存QImage到特定路径的图像文件
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

// 取消
void MainWindow::cancelOpen() 
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	QPalette palette;
	palette.setColor(QPalette::Window, Qt::white);
	
	try {
		if (button == ui.pushButton_inputcancel) {
			ui.label_18->setText(QString::fromLocal8Bit("请重新载入"));
			ui.label_18->setStyleSheet("color: red;");
			ui.label_18->setAutoFillBackground(true);
			ui.label_18->setPalette(palette);
		}
		else {
			if (button == ui.pushButton_inputcancel_2) {
				ui.label_20->setText(QString::fromLocal8Bit("请重新载入"));
				ui.label_20->setStyleSheet("color: red;");
				ui.label_20->setAutoFillBackground(true);
				ui.label_20->setPalette(palette);
			}
			else {
				ui.label_21->setText(QString::fromLocal8Bit("请重新载入"));
				ui.label_21->setStyleSheet("color: red;");
				ui.label_21->setAutoFillBackground(true);
				ui.label_21->setPalette(palette);
			}
		}
		ui.progressBar->setValue(0);//进度条置为0
		this->OpenFilePath = "";
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Error Cancel: " << e.what() << std::endl;
		std::terminate();
	}
}

//开始重建
void MainWindow::startReconstruct()
{
	//定义变量
	std::string imagefile,outplyfilename, modelfile;
	imagefile = this->OpenFilePath.toStdString();
	modelfile = "./Data/sfm_shape_3448.bin";//模型路径
	outplyfilename = "./Output/Reconstructed_Face.ply";//输出.ply路径
	//判断合理性
	if (imagefile == "") {
		outputMessage(u8"建模失败，请检查是否输入图片！", true, true,true,true);
		return;
	}
	//重建
	bool candorec = Reconstruct(imagefile, modelfile, outplyfilename);//重建过程中是否报错
	
	if (candorec != 1)
	{
		outputMessage(u8"模型生成失败！人脸数量不满足要求！", true, true, false, true);
		ui.pushButton_2dto3d->setText(QString::fromLocal8Bit("重新建模"));
		ui.progressBar->setValue(0);//进度条置为0
		ui.progressBar->update();
		ui.pushButton_2dto3d->update();
	}
	else {
		OpenModelFilePath = QString::fromStdString(outplyfilename);//变形前模型位置
		//输出到opengl窗口中
		ui.progressBar->setValue(100);
		openGLWidget->loadPLYFile(outplyfilename);
		genface = false;
		openGLWidget->scale = 1500.0;
		openGLWidget->theta = 0.6;
		openGLWidget->clearopengl = false;
		openGLWidget->paintGL();
		openGLWidget->update();
		//显示“重建完成”
		outputMessage(u8"建模完成！可查看窗口中模型", true, true, false, true);
		ui.pushButton_2dto3d->setText(QString::fromLocal8Bit("重新建模"));
		ui.progressBar->setValue(0);
		ui.label_19->update();
		ui.pushButton_2dto3d->update();
		ui.progressBar->update();
	}
}

//打开本地model文件
void MainWindow::openModel() {
	QString OpenFile;
	//打开文件对话框，选择model文件
	
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setDirectory(QDir::currentPath() + "/Data");
	dialog.setNameFilter(tr("Image Files(*.ply *.obj *.pcd);;All(*.*)"));
	dialog.setOptions(QFileDialog::DontUseNativeDialog);
	dialog.setMinimumSize(800, 600); // 设置最小大小

	// 打开文件夹中的模型文件
	if (dialog.exec() == QDialog::Accepted) {
		OpenFile = dialog.selectedFiles().first();

		// 显示所示模型的路径
		QFileInfo OpenFileInfo;
		OpenFileInfo = QFileInfo(OpenFile);
		this->OpenModelFilePath = OpenFileInfo.filePath();
		ui.plainTextEdit->appendPlainText(QString("%1%2")
			.arg(QString::fromLocal8Bit("模型路径："))
			.arg(this->OpenModelFilePath)
		);//输出到消息框
		outputMessage(u8"模型读取中...", true, true, false, false, true);
	}
	if (openGLWidget->loadPLYFile(OpenModelFilePath.toStdString())) //展示model到opengl窗口
	{
		genface = false;
		openGLWidget->scale = 1500.0;
		openGLWidget->theta = 0.6;
		openGLWidget->clearopengl = false;
		openGLWidget->paintGL();
		openGLWidget->update();
		outputMessage(u8"模型加载完成！可使用键盘和鼠标查看窗口", true, true, false ,false, true);
	}
	else 
		outputMessage(u8"模型加载失败！请检查载入模型格式。", true, true, false, false, true);
}

//优化模型
void MainWindow::startrRegistration() {
	if (OpenModelFilePath.toStdString()!="") {
		std::string src_file = "./Data/bikou_di_rgb.ply";
		std::string tar_file = OpenModelFilePath.toStdString();
		OpenModelFilePath = fastRnrr(src_file, tar_file);//变形前模型位置
		//输出到opengl窗口
		if (openGLWidget->loadPLYFile(OpenModelFilePath.toStdString()))
		{
			openGLWidget->paintGL();openGLWidget->update();
			outputMessage(u8"优化完成，请查看！", true, true);
		}
		else
			outputMessage(u8"优化失败！请重试。", true, true, false, false, true);
	}
	else
		outputMessage(u8"请先导入本地模型！", true, true, true);
}

//人脸变形（待实现）
void MainWindow::plasticModel() 
{
	//变形前的model
	//OpenModelFilePath;



	//展示变形后的model
	QString srcModelFilePath = "./Data/bikou_di_rgb_biliang.ply";
	if (openGLWidget_2->loadPLYFile(srcModelFilePath.toStdString()))
	{
		genface = false;
		openGLWidget_2->scale = 1500.0;
		openGLWidget_2->theta = 0.6;
		openGLWidget_2->clearopengl = false;
		openGLWidget_2->paintGL();
		openGLWidget_2->update();
		outputMessage(u8"模型变形完成！可使用键盘和鼠标对比前后效果", true, true, false, false, true);
	}
	else
	{
		outputMessage(u8"模型变形失败！", true, true, false, false, true);
	}
}

//键盘事件处理
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

//鼠标滚轮事件处理函数
void MainWindow::wheelEvent(QWheelEvent* event) {		
	openGLWidget->scale += event->delta() / 500.0;
	openGLWidget_2->scale += event->delta() / 500.0;
	if (openGLWidget->scale <= 0.0)
		openGLWidget->scale = 0.01;
	openGLWidget->update();
	openGLWidget_2->update();
}