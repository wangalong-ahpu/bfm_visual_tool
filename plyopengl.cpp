//#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
//#include "Functions.h"
//
//class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
//{
//public:
//    MyGLWidget(QWidget* parent = 0)
//        : QOpenGLWidget(parent)
//    {
//    }
//
//protected:
//    void initializeGL() override
//    {
//        initializeOpenGLFunctions();
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    }
//
//    void resizeGL(int w, int h) override
//    {
//        glViewport(0, 0, w, h);
//    }
//
//    void paintGL() override
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Set up the vertices of the triangle
//        GLfloat vertices[] = {
//             0.0f,  0.5f, 0.0f,
//             0.5f, -0.5f, 0.0f,
//            -0.5f, -0.5f, 0.0f
//        };
//
//        // Set up the colors of the triangle
//        GLfloat colors[] = {
//            1.0f, 0.0f, 0.0f, // red
//            0.0f, 1.0f, 0.0f, // green
//            0.0f, 0.0f, 1.0f  // blue
//        };
//
//        // Enable vertex and color arrays
//        glEnableClientState(GL_VERTEX_ARRAY);
//        glEnableClientState(GL_COLOR_ARRAY);
//
//        // Set the vertex and color pointers
//        glVertexPointer(3, GL_FLOAT, 0, vertices);
//        glColorPointer(3, GL_FLOAT, 0, colors);
//
//        // Draw the triangle
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        // Disable vertex and color arrays
//        glDisableClientState(GL_VERTEX_ARRAY);
//        glDisableClientState(GL_COLOR_ARRAY);
//    }
//};
//void test() {
//    MyGLWidget* gll = new(MyGLWidget);
//}#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
//#include "Functions.h"
//
//class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
//{
//public:
//    MyGLWidget(QWidget* parent = 0)
//        : QOpenGLWidget(parent)
//    {
//    }
//
//protected:
//    void initializeGL() override
//    {
//        initializeOpenGLFunctions();
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    }
//
//    void resizeGL(int w, int h) override
//    {
//        glViewport(0, 0, w, h);
//    }
//
//    void paintGL() override
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Set up the vertices of the triangle
//        GLfloat vertices[] = {
//             0.0f,  0.5f, 0.0f,
//             0.5f, -0.5f, 0.0f,
//            -0.5f, -0.5f, 0.0f
//        };
//
//        // Set up the colors of the triangle
//        GLfloat colors[] = {
//            1.0f, 0.0f, 0.0f, // red
//            0.0f, 1.0f, 0.0f, // green
//            0.0f, 0.0f, 1.0f  // blue
//        };
//
//        // Enable vertex and color arrays
//        glEnableClientState(GL_VERTEX_ARRAY);
//        glEnableClientState(GL_COLOR_ARRAY);
//
//        // Set the vertex and color pointers
//        glVertexPointer(3, GL_FLOAT, 0, vertices);
//        glColorPointer(3, GL_FLOAT, 0, colors);
//
//        // Draw the triangle
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        // Disable vertex and color arrays
//        glDisableClientState(GL_VERTEX_ARRAY);
//        glDisableClientState(GL_COLOR_ARRAY);
//    }
//};
//void test() {
//    MyGLWidget* gll = new(MyGLWidget);
//}