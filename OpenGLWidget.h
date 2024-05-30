#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <GL/gl.h>
#include <GL/glu.h>
#include <qevent.h>
#include <dlib/matrix.h>

class QMouseEvent;
class QKeyEvent;


class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit OpenGLWidget(QWidget* parent = Q_NULLPTR);

public:
	void initializeGL()			Q_DECL_OVERRIDE;
	void paintGL()				Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;

	//v0.3新增
	bool loadPLYFile(const std::string& filename);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);

public:
	double oldX;		/* last position of mouse, used for rotation */
	double theta = 1.0;		/* rotation degree */
	double scale = 0.0001;		/* zooming scale *///改
	bool has_rgb = false;//是否包含rgb颜色信息
	bool clearopengl = false;//清空
	//double theta = 0.0;		/* rotation degree */
	//double scale = 1.0;		/* zooming scale *///原

	dlib::matrix<double> shape;
	dlib::matrix<double> tex;
	dlib::matrix<double> tl;

	//重建形状和点索引
	dlib::matrix<double> reconstruct_shape;
	dlib::matrix<double> reconstruct_tex;
	dlib::matrix<double> reconstruct_tl;
};
