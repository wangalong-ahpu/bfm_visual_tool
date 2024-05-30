#include "OpenGLWidget.h"
#include "MainWindow.h"
#include <Eigen/Core>

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}


void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
    glClearColor(10.0/255.0, 34.0/255.0, 56.0/255.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glLoadIdentity();
	gluLookAt(0, 0, 300000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}


void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.0, 600000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 300000.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void OpenGLWidget::paintGL()
{    
    MainWindow* p = (MainWindow*)(parentWidget()->parentWidget());
	shape = p->getModel().get_current_blendshape();
	tex = p->getModel().get_current_tex();
	tl = p->getModel().get_tl();
	
    if (clearopengl) {//清除
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }

    /* 展示随机生成的人脸 */
    if (p->genface) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (shape.nr() != 0) {
            /* human face model */
            double sint = sin(theta), cost = cos(theta);

            for (int i = 0; i < tl.size() / 3; i++) {		// tl.size()/3 = 105694
                glBegin(GL_POINTS);

                double xIdx = tl(i * 3);
                double yIdx = tl(i * 3 + 1);
                double zIdx = tl(i * 3 + 2);

                glColor3f(tex(xIdx * 3), tex(xIdx * 3 + 1), tex(xIdx * 3 + 2));
                glVertex3f(shape(xIdx * 3) * scale * cost - shape(xIdx * 3 + 2) * scale * sint, shape(xIdx * 3 + 1) * scale, shape(xIdx * 3) * scale * sint + shape(xIdx * 3 + 2) * scale * cost);
                
                glColor3f(tex(yIdx * 3), tex(yIdx * 3 + 1), tex(yIdx * 3 + 2));
                glVertex3f(shape(yIdx * 3) * scale * cost - shape(yIdx * 3 + 2) * scale * sint, shape(yIdx * 3 + 1) * scale, shape(yIdx * 3) * scale * sint + shape(yIdx * 3 + 2) * scale * cost);
                
                glColor3f(tex(zIdx * 3), tex(zIdx * 3 + 1), tex(zIdx * 3 + 2));
                glVertex3f(shape(zIdx * 3) * scale * cost - shape(zIdx * 3 + 2) * scale * sint, shape(zIdx * 3 + 1) * scale, shape(zIdx * 3) * scale * sint + shape(zIdx * 3 + 2) * scale * cost);

                glEnd();
            }
        }
        else {
            /* 输出彩色三角形 */
            glBegin(GL_TRIANGLES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(-50000 * cos(theta) * scale, 0, 0);
            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(50000 * cos(theta) * scale, 0, 0);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 50000 * scale, 0);
            glEnd();
        }
    }
    else {
    /* 展示图片重建后的人脸 */
        
        /* human face model */
        double sint = sin(theta), cost = cos(theta);

        for (int i = 0; i < reconstruct_tl.nr(); i++) {
            if(has_rgb)
                glBegin(GL_TRIANGLES);
            else
                glBegin(GL_POINTS);
            double xIdx = reconstruct_tl(i , 0);
            double yIdx = reconstruct_tl(i , 1);
            double zIdx = reconstruct_tl(i , 2);
            
            glColor3f(float(reconstruct_tex(xIdx, 0) / 255.0), float(reconstruct_tex(xIdx, 1) / 255.0), float(reconstruct_tex(xIdx, 2) / 255.0));
            glVertex3f(reconstruct_shape(xIdx,0) * scale * cost - reconstruct_shape(xIdx ,2) * scale * sint, reconstruct_shape(xIdx, 1) * scale, reconstruct_shape(xIdx ,0) * scale * sint + reconstruct_shape(xIdx , 2) * scale * cost);
            glColor3f(float(reconstruct_tex(yIdx, 0) / 255.0), float(reconstruct_tex(yIdx, 1) / 255.0), float(reconstruct_tex(yIdx, 2) / 255.0));
            glVertex3f(reconstruct_shape(yIdx ,0) * scale * cost - reconstruct_shape(yIdx , 2) * scale * sint, reconstruct_shape(yIdx , 1) * scale, reconstruct_shape(yIdx ,0) * scale * sint + reconstruct_shape(yIdx , 2) * scale * cost);
            glColor3f(float(reconstruct_tex(zIdx, 0) / 255.0), float(reconstruct_tex(zIdx, 1) / 255.0), float(reconstruct_tex(zIdx, 2) / 255.0));
            glVertex3f(reconstruct_shape(zIdx ,0) * scale * cost - reconstruct_shape(zIdx , 2) * scale * sint, reconstruct_shape(zIdx , 1) * scale, reconstruct_shape(zIdx ,0) * scale * sint + reconstruct_shape(zIdx , 2) * scale * cost);
            glEnd();
        }
    }
}


void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		oldX = event->x();
}


void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton) {
		double clockwise;
		if (event->x() < oldX)
			clockwise = -1.0;
		else if (event->x() > oldX)
			clockwise = 1.0;
		else
			clockwise = 0;
		oldX = event->x();
		theta += clockwise * 0.1;
		update();
	}
}

//重载函数
bool OpenGLWidget::loadPLYFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return false;
    }

    std::string line;
    int num_vertices = 0;
    int num_faces = 0;

    bool read_header = false;
    std::vector<double> vertices;
    std::vector<double> colors;
    std::vector<int> faces;

    while (std::getline(file, line)) {
        if (!read_header) {
            if (line == "end_header") {
                read_header = true;
            }
            else if (line.find("element vertex") != std::string::npos) {
                num_vertices = std::stoi(line.substr(line.find_last_of(' ') + 1));
            }
            else if (line.find("element face") != std::string::npos) {
                num_faces = std::stoi(line.substr(line.find_last_of(' ') + 1));
            }
            else
                if (line.find("property uchar red") != std::string::npos ||
                    line.find("property uchar green") != std::string::npos ||
                    line.find("property uchar blue") != std::string::npos ||
                    line.find("property uchar r") != std::string::npos
                    ) {
                    has_rgb = true;
                }
        }
        else {
            if (vertices.size() < num_vertices * 3) {
                std::istringstream iss(line);
                double x, y, z, r, g, b;
                iss >> x >> y >> z >> r >> g >> b;
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                colors.push_back(r);
                colors.push_back(g);
                colors.push_back(b);
            }
            else if (faces.size() < num_faces * 3) {
                std::istringstream iss(line);
                int num_indices;
                iss >> num_indices;
                int index;
                for (int i = 0; i < num_indices; i++) {
                    iss >> index;
                    faces.push_back(index);
                }
            }
            else {
                break;
            }
        }
    }
    if (vertices.size() != num_vertices * 3 || faces.size() != num_faces * 3) {
        std::cerr << "Error: Failed to read PLY file " << filename << std::endl;
        return false;
    }

    dlib::matrix<double> recon_shape(num_vertices, 3);
    dlib::matrix<double> recon_tex(num_vertices, 3);
    dlib::matrix<double, 1, 3> row_vector = { 28.0, 144.0, 163.0 };
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < 3; j++) {
            recon_shape(i, j) = vertices[i * 3 + j];
            
        }
        if (has_rgb) {
            for (int j = 0; j < 3; j++) 
                recon_tex(i, j) = colors[i * 3 + j];
        }
        else {
            for (int j = 0; j < 3; j++) 
                recon_tex(i, j) = row_vector(j);
        }
    }

    dlib::matrix<double> recon_tl(num_faces, 3);
    for (int i = 0; i < num_faces; i++) {
        for (int j = 0; j < 3; j++) {
            recon_tl(i, j) = faces[i * 3 + j];
        }
    }
    //赋值保存
    reconstruct_shape = recon_shape;
    reconstruct_tex = recon_tex;
    reconstruct_tl = recon_tl;
    return true;
}