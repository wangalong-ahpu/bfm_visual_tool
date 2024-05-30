#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>  

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Functions.h"
#include "MainWindow.h"
using namespace std;
using namespace dlib;
using namespace cv;

//QImage Mat2QImage(const cv::Mat& src)
//{
//    cv::Mat temp; // make the same cv::Mat
//    cvtColor(src, temp, COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
//    QImage dest((uchar*)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
//    dest.bits(); // enforce deep copy, see documentation 
//    return dest;
//}

int MainWindow::DlibFaceDetection(std::string& imagefile, std::string& landmarksfile)
{
    try
    {
        // 读取人脸检测器和关键点检测器
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor sp;
        deserialize("Data/shape_predictor_68_face_landmarks.dat") >> sp;

        // 读取图像
        Mat img = imread(imagefile);
        if (img.empty())
        {
            std::cout << "Error: Failed to read image." << endl;
            return 0;
        }

        // 将OpenCV的Mat转换为dlib的array2d
        array2d<rgb_pixel> dlib_img;
        assign_image(dlib_img, cv_image<rgb_pixel>(img));

        // 检测人脸
        std::vector<dlib::rectangle> dets = detector(dlib_img);
        if (dets.size() != 1)
        {
            std::cout << "Error: Detected " << dets.size() << " faces." << endl;
            return dets.size();
        }
        std::vector<full_object_detection> shapes;
        // 提取关键点
        full_object_detection shape = sp(dlib_img, dets[0]);

        // 将关键点信息保存为.pts文件
        ofstream outfile(landmarksfile);
        outfile << "version: 1" << endl;
        outfile << "n_points: 68" << endl;
        outfile << "{" << endl;
        for (int i = 0; i < 68; ++i)
        {
            outfile << shape.part(i).x() << " " << shape.part(i).y() << endl;
            //std::codut << shape.part(i).x()<<" "<< shape.part(i).y()<<endl;
        }
        outfile << "}" << endl;
        std::cout << "Dlib facelankmarks had saved at " << landmarksfile << std::endl;
        //加
        // 在图像上画出关键点
        for (unsigned long j = 0; j < shape.num_parts(); ++j)
        {
            int x = shape.part(j).x();
            int y = shape.part(j).y();
            cv::circle(img, Point(x, y), 2, Scalar(0, 255, 0), -1);
        }
        // 在图像上画出人脸框
        //dlib::rectangle r = dets[0];
        //cv::rectangle(img, cv::Point(r.left(), r.top()), cv::Point(r.right(), r.bottom()), cv::Scalar(0, 0, 255), 2);
        // 显示结果(独立窗口)
        /*namedWindow("result", WINDOW_NORMAL);
        resizeWindow("result", img.cols / 2, img.rows / 2);
        imshow("result", img);
        waitKey(0);*/

        // 将cv::Mat转换为QImage
        //QImage qtImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        ////QImage qimg = Mat2QImage(img);
        //ui.label_18->setPixmap(QPixmap::fromImage(qtImage));
        //ui.label_18->setScaledContents(true);

    }
    catch (exception& e)
    {
        std::cout << "Exception: " << e.what() << endl;
    }
    return 1;
}
