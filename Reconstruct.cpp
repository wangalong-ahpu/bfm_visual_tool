/**
*  @Reconstruct函数
*  利用eos库实现了：
*  1 读取形变模型，进行2D-3D坐标对应关系的处理
*  2 利用正交相机(Orthographic Camera)进行姿势的估计和拟合
*  3 提取并保存纹理
*  最终的输出，包含了模型文件.obj及.ply, 纹理图片.png和对应的材料库文件.mtl(指出模型对应的纹理文件名称)
*/
#include "eos/core/Image.hpp"
#include "eos/core/image/opencv_interop.hpp"
#include "eos/core/Landmark.hpp"
#include "eos/core/LandmarkMapper.hpp"
#include "eos/core/read_pts_landmarks.hpp"
#include "eos/core/write_obj.hpp"
#include "eos/fitting/RenderingParameters.hpp"
#include "eos/fitting/linear_shape_fitting.hpp"
#include "eos/fitting/orthographic_camera_estimation_linear.hpp"
#include "eos/fitting/blendshape_fitting.hpp"
#include "eos/fitting/contour_correspondence.hpp"
#include "eos/fitting/closest_edge_fitting.hpp"
#include "eos/fitting/nonlinear_camera_estimation.hpp"
#include "eos/morphablemodel/MorphableModel.hpp"
#include "eos/render/texture_extraction.hpp"
#include "eos/render/detail/utils.hpp"

#include "Eigen/Core"
#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utils/logger.hpp>

#include "Functions.h"
#include "MainWindow.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace eos;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using eos::core::Landmark;
using eos::core::LandmarkCollection;
using Eigen::Vector2f;
using Eigen::Vector4f;
using cv::Mat;
//using std::cout;
//using std::endl;
using std::string;
using std::vector;


bool MainWindow::Reconstruct(string& imagefile, string& modelfile, string& outplyfilename)
{
	ui.progressBar->setValue(10);
	ui.progressBar->update();


	//定义相关参数
	string landmarksfile, mappingsfile, outputbasename;// ,imgDir;
	mappingsfile = "Data/dlib_to_sfm.txt";//特征点-模型映射文件路径
	outputbasename = "Output/out";//输出名

	//开始重建30%
	ui.progressBar->setValue(30);	ui.label_19->setText(QString::fromLocal8Bit("模型生成中，请稍后..."));//消息栏显示“重建中”
	ui.label_19->setStyleSheet("font-size: 20pt;color: black;");// 设置字体大小为20
	ui.label_19->setAlignment(Qt::AlignCenter);// 设置文本居中对齐
	ui.label_19->update();
	ui.progressBar->update();

	//重建计时开始
	using namespace std::chrono;
	auto start = steady_clock::now();

	// 检测人脸特征点，并存储
	try
	{
		landmarksfile = "Input/out_dlib.pts";
		int facenum = DlibFaceDetection(imagefile, landmarksfile);
		if (facenum != 1) {
			return 0;
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Error processing face landmark detection: " << e.what() << std::endl;
		std::terminate();
	}
	ui.progressBar->setValue(50);
	ui.progressBar->update();

	//读取人脸图片
	Mat image = cv::imread(imagefile);
	LandmarkCollection<Eigen::Vector2f> landmarks;

	// 读取pts文件
	try
	{
		landmarks = core::read_pts_landmarks(landmarksfile);
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Error reading the landmarks: " << e.what() << std::endl;
		std::terminate();

	}
	ui.progressBar->setValue(60);
	ui.progressBar->update();
	// 读取形变模型
	morphablemodel::MorphableModel morphable_model;
	morphable_model = morphablemodel::load_model(modelfile);
	// LandmarkMapper类对象，将dlib的二维坐标点转换为顶点id( vertex ids)
	core::LandmarkMapper landmark_mapper;
	try
	{
		landmark_mapper = core::LandmarkMapper(mappingsfile);
	}
	catch (const std::exception& e)
	{
		std::cout << "Error loading the landmark mappings: " << e.what() << std::endl;
		std::terminate();
	}

	ui.progressBar->setValue(70);
	ui.progressBar->update();

	try
	{
		// 绘制读入的坐标点
		Mat outimg = image.clone();
		for (auto&& lm : landmarks)
		{
			//以输入的两点pt1和pt2为对角线 描绘矩形边框
			cv::rectangle(outimg, cv::Point2f(lm.coordinates[0] - 2.0f, lm.coordinates[1] - 2.0f),
				cv::Point2f(lm.coordinates[0] + 2.0f, lm.coordinates[1] + 2.0f), { 255, 0, 0 });
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Error drawing the landmarks: " << e.what() << std::endl;
		std::terminate();
	}

	ui.progressBar->setValue(80);
	ui.progressBar->update();

	// 定义最终用于拟合的数据点
	std::vector<Vector4f> model_points; // 三维形变模型的点
	std::vector<int> vertex_indices;    // 顶点索引
	std::vector<Vector2f> image_points; // 对应的二维坐标点

	try
	{
		// 对每一个坐标点进行判断，依据dlib_to_sfm.txt规定的2D-3D对应关系，查询模型上对应的三维坐标点（利用哈希表）
		for (int i = 0; i < landmarks.size(); ++i)
		{
			// 本模块采用68个点，name即为1-68，converted_name为dlib_to_sfm.txt右值.若后续更改为更多关键点，则自适应
			auto converted_name = landmark_mapper.convert(landmarks[i].name);
			if (!converted_name)
			{ // 若没有对应的点，则忽略
				continue;
			}
			int vertex_idx = std::stoi(converted_name.value());
			// 返回一个3x1的容器，包含了x-y-z坐标
			auto vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
			model_points.emplace_back(Vector4f(vertex.x(), vertex.y(), vertex.z(), 1.0f));
			vertex_indices.emplace_back(vertex_idx);
			image_points.emplace_back(landmarks[i].coordinates);
		}

		// 根据2D - 3D对应关系，正交投影估计模型的姿势（camera/pose）OpenCV参数为true
		fitting::ScaledOrthoProjectionParameters pose =
			fitting::estimate_orthographic_projection_linear(image_points, model_points, true, image.rows);
		fitting::RenderingParameters rendering_params(pose, image.cols, image.rows);

		// 获取三维人脸角度
		const float yaw_angle = glm::degrees(glm::yaw(rendering_params.get_rotation()));
		// 获取仿射变换矩阵
		const Eigen::Matrix<float, 3, 4> affine_from_ortho =
			fitting::get_3x4_affine_camera_matrix(rendering_params, image.cols, image.rows);
		// 通过将模型与特征点拟合，估计模型的参数

		const std::vector<float> fitted_coeffs = fitting::fit_shape_to_landmarks_linear(
			morphable_model.get_shape_model(), affine_from_ortho, image_points, vertex_indices);

		// 依据获得的参数，获取完整的网格模型
		const core::Mesh mesh = morphable_model.draw_sample(fitted_coeffs, std::vector<float>());

		// 将网格模型保存为.ply(xyz+三角网格列表)
		write_ply(mesh, outplyfilename);

		// 依据网格模型和姿势参数，从原图中提取纹理(报错注释)
		const core::Image4u isomap = render::extract_texture(mesh, rendering_params.get_modelview(), rendering_params.get_projection(),
			render::ProjectionType::Orthographic, core::from_mat_with_alpha(image));//改
		// 将网格模型保存为.obj(报错注释)（需添加头文件）
		string outputfile = outputbasename + ".obj";
		core::write_textured_obj(mesh, outputfile);
		std::cout << "Reconstructed face had saved at " << outputfile << std::endl;

		//保存isomap纹理文件(报错注释)
		string isomapoutputfile = outputbasename + ".texture.png";
		cv::imwrite(isomapoutputfile, core::to_mat(isomap));
		auto end = steady_clock::now();
		
		//整个重建过程时长
		ui.plainTextEdit->appendPlainText(QString("%1%2%3s")
			.arg(QString::fromLocal8Bit("建模完成！"))
			.arg(QString::fromLocal8Bit("用时:"))
			.arg(duration_cast<milliseconds>(end - start).count()/1000.0)
			);
		
		/*ofstream c("count.txt");
		c << "Reconstruction time: " << duration_cast<milliseconds>(end - start).count() << "milliseconds";
		c.close();*/
	}
	catch (const std::exception& e)
	{
		std::cout << "Error processing the mesh and texture: " << e.what() << std::endl;
		std::terminate();
	}
	return 1;
}