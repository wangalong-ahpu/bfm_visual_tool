/**
*  @Reconstruct����
*  ����eos��ʵ���ˣ�
*  1 ��ȡ�α�ģ�ͣ�����2D-3D�����Ӧ��ϵ�Ĵ���
*  2 �����������(Orthographic Camera)�������ƵĹ��ƺ����
*  3 ��ȡ����������
*  ���յ������������ģ���ļ�.obj��.ply, ����ͼƬ.png�Ͷ�Ӧ�Ĳ��Ͽ��ļ�.mtl(ָ��ģ�Ͷ�Ӧ�������ļ�����)
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


	//������ز���
	string landmarksfile, mappingsfile, outputbasename;// ,imgDir;
	mappingsfile = "Data/dlib_to_sfm.txt";//������-ģ��ӳ���ļ�·��
	outputbasename = "Output/out";//�����

	//��ʼ�ؽ�30%
	ui.progressBar->setValue(30);	ui.label_19->setText(QString::fromLocal8Bit("ģ�������У����Ժ�..."));//��Ϣ����ʾ���ؽ��С�
	ui.label_19->setStyleSheet("font-size: 20pt;color: black;");// ���������СΪ20
	ui.label_19->setAlignment(Qt::AlignCenter);// �����ı����ж���
	ui.label_19->update();
	ui.progressBar->update();

	//�ؽ���ʱ��ʼ
	using namespace std::chrono;
	auto start = steady_clock::now();

	// ������������㣬���洢
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

	//��ȡ����ͼƬ
	Mat image = cv::imread(imagefile);
	LandmarkCollection<Eigen::Vector2f> landmarks;

	// ��ȡpts�ļ�
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
	// ��ȡ�α�ģ��
	morphablemodel::MorphableModel morphable_model;
	morphable_model = morphablemodel::load_model(modelfile);
	// LandmarkMapper����󣬽�dlib�Ķ�ά�����ת��Ϊ����id( vertex ids)
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
		// ���ƶ���������
		Mat outimg = image.clone();
		for (auto&& lm : landmarks)
		{
			//�����������pt1��pt2Ϊ�Խ��� �����α߿�
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

	// ��������������ϵ����ݵ�
	std::vector<Vector4f> model_points; // ��ά�α�ģ�͵ĵ�
	std::vector<int> vertex_indices;    // ��������
	std::vector<Vector2f> image_points; // ��Ӧ�Ķ�ά�����

	try
	{
		// ��ÿһ�����������жϣ�����dlib_to_sfm.txt�涨��2D-3D��Ӧ��ϵ����ѯģ���϶�Ӧ����ά����㣨���ù�ϣ��
		for (int i = 0; i < landmarks.size(); ++i)
		{
			// ��ģ�����68���㣬name��Ϊ1-68��converted_nameΪdlib_to_sfm.txt��ֵ.����������Ϊ����ؼ��㣬������Ӧ
			auto converted_name = landmark_mapper.convert(landmarks[i].name);
			if (!converted_name)
			{ // ��û�ж�Ӧ�ĵ㣬�����
				continue;
			}
			int vertex_idx = std::stoi(converted_name.value());
			// ����һ��3x1��������������x-y-z����
			auto vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
			model_points.emplace_back(Vector4f(vertex.x(), vertex.y(), vertex.z(), 1.0f));
			vertex_indices.emplace_back(vertex_idx);
			image_points.emplace_back(landmarks[i].coordinates);
		}

		// ����2D - 3D��Ӧ��ϵ������ͶӰ����ģ�͵����ƣ�camera/pose��OpenCV����Ϊtrue
		fitting::ScaledOrthoProjectionParameters pose =
			fitting::estimate_orthographic_projection_linear(image_points, model_points, true, image.rows);
		fitting::RenderingParameters rendering_params(pose, image.cols, image.rows);

		// ��ȡ��ά�����Ƕ�
		const float yaw_angle = glm::degrees(glm::yaw(rendering_params.get_rotation()));
		// ��ȡ����任����
		const Eigen::Matrix<float, 3, 4> affine_from_ortho =
			fitting::get_3x4_affine_camera_matrix(rendering_params, image.cols, image.rows);
		// ͨ����ģ������������ϣ�����ģ�͵Ĳ���

		const std::vector<float> fitted_coeffs = fitting::fit_shape_to_landmarks_linear(
			morphable_model.get_shape_model(), affine_from_ortho, image_points, vertex_indices);

		// ���ݻ�õĲ�������ȡ����������ģ��
		const core::Mesh mesh = morphable_model.draw_sample(fitted_coeffs, std::vector<float>());

		// ������ģ�ͱ���Ϊ.ply(xyz+���������б�)
		write_ply(mesh, outplyfilename);

		// ��������ģ�ͺ����Ʋ�������ԭͼ����ȡ����(����ע��)
		const core::Image4u isomap = render::extract_texture(mesh, rendering_params.get_modelview(), rendering_params.get_projection(),
			render::ProjectionType::Orthographic, core::from_mat_with_alpha(image));//��
		// ������ģ�ͱ���Ϊ.obj(����ע��)�������ͷ�ļ���
		string outputfile = outputbasename + ".obj";
		core::write_textured_obj(mesh, outputfile);
		std::cout << "Reconstructed face had saved at " << outputfile << std::endl;

		//����isomap�����ļ�(����ע��)
		string isomapoutputfile = outputbasename + ".texture.png";
		cv::imwrite(isomapoutputfile, core::to_mat(isomap));
		auto end = steady_clock::now();
		
		//�����ؽ�����ʱ��
		ui.plainTextEdit->appendPlainText(QString("%1%2%3s")
			.arg(QString::fromLocal8Bit("��ģ��ɣ�"))
			.arg(QString::fromLocal8Bit("��ʱ:"))
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