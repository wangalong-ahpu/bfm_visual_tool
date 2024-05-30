#include "bfm.h"

bfm::bfm(const std::string& filename) {
	init(filename);
}


void bfm::init(const std::string& filename) {
	if (!read_parm_from_file(filename))
		return;
	init_parm();
	std::cout << "以下运行load_data函数"<<endl;
	load_data();
}


void bfm::data_check() {
	bfm_out << "check data\n";
	bfm_out << "	(1) shape mu: \n";
	bfm_out << "		Yours:   " << shape_mu(0, 0) << "\n";
	bfm_out << "		Correct: -57239 42966 80410\n\n";
	bfm_out << "	(2) shape ev: \n";
	bfm_out << "		Yours:   " << shape_ev(0, 0) << " " << shape_ev(1, 0) << "\n";
	bfm_out << "		Correct: 884340 555880\n\n";
	bfm_out << "	(3) shape pc: \n";
	bfm_out << "		Yours:   " << shape_pc(0, 0) << "\n";
	bfm_out << "		Correct: -0.0024\n\n";
	bfm_out << "	(4) texture mu: \n";
	bfm_out << "		Yours:   " << tex_mu(0, 0) << endl;
	bfm_out << "		Correct: 182.8750 135.0400 107.1400\n" << endl;
	bfm_out << "	(5) texture ev: \n";
	bfm_out << "		Yours:   " << tex_ev(0) << " " << tex_ev(1) << "\n";
	bfm_out << "		Correct: 4103.2 2024.1\n\n";
	bfm_out << "	(6) texture pc: \n";
	bfm_out << "		Yours:   " << tex_pc(0, 0) << "\n";
	bfm_out << "		Correct: -0.0028\n\n";
	bfm_out << "	(7) expression mu: \n";
	bfm_out << "		Yours:   " << expr_mu(0, 0) << endl;
	bfm_out << "		Correct: 182.8750 135.0400 107.1400\n" << endl;
	bfm_out << "	(8) expression ev: \n";
	bfm_out << "		Yours:   " << expr_ev(0) << " " << expr_ev(1) << "\n";
	bfm_out << "		Correct: 4103.2 2024.1\n\n";
	bfm_out << "	(9) expression pc: \n";
	bfm_out << "		Yours:   " << expr_pc(0, 0) << "\n";
	bfm_out << "		Correct: -0.0028\n\n";
	bfm_out << "	(10) triangle list: \n";
	bfm_out << "		Yours:   " << tl(0) << " " << tl(1) << "\n";
	bfm_out << "		Correct: -0.0028\n\n";
}

//从txt文本中，读出bfm数据的参数
bool bfm::read_parm_from_file(const std::string& filename) {
	ifstream in(filename, std::ios::in);
	if (!in) {
		bfm_out << "[ERROR] Can't open " << filename.c_str() << ".\n";
		return false;
	}

	/*2017data*/
	in >> bfm_h5_path;
	in >> n_vertice >> n_face >> n_id_pc >> n_expr_pc;
	in >> n_landmark >> landmark_idx_path;
	in >> intrinsic_parm[0] >> intrinsic_parm[1] >> intrinsic_parm[2] >> intrinsic_parm[3];
	in >> shape_mu_h5_path >> shape_ev_h5_path >> shape_pc_h5_path;
	in >> tex_mu_h5_path >> tex_ev_h5_path >> tex_pc_h5_path;
	in >> expr_mu_h5_path >> expr_ev_h5_path >> expr_pc_h5_path;
	in >> tl_h5_path;
	
	/*2009data*/
	//in >> bfm_h5_path;
	//in >> n_vertice >> n_face >> n_id_pc;
	//in >> n_landmark >> landmark_idx_path;
	//in >> intrinsic_parm[0] >> intrinsic_parm[1] >> intrinsic_parm[2] >> intrinsic_parm[3];
	//in >> shape_mu_h5_path >> shape_ev_h5_path >> shape_pc_h5_path;
	//in >> tex_mu_h5_path >> tex_ev_h5_path >> tex_pc_h5_path;
	//in >> tl_h5_path;






	/*bfm_h5_path = "D:\\database\\bfm\\2017\\model2017-1_bfm_nomouth.h5";
	n_vertice = 53149;
	n_face = 105694;
	n_id_pc = 199;
	n_expr_pc = 100;
	n_landmark = 68;
	landmark_idx_path = "D:\\database\\bfm\\3DMM_model\\landmark.anl";
	intrinsic_parm[0] = 1744.327628674942;
	intrinsic_parm[1] = 1747.838275588676;
	intrinsic_parm[2] = 800;
	intrinsic_parm[3] = 600;
	shape_mu_h5_path = "shape\\model\\mean";
	shape_ev_h5_path = "shape\\model\\pcaVariance";
	shape_pc_h5_path = "shape\\model\\pcaBasis";
	tex_mu_h5_path = "color\\model\\mean";
	tex_ev_h5_path = "color\\model\\pcaVariance";
	tex_pc_h5_path = "color\\model\\pcaBasis";
	expr_mu_h5_path = "expression\\model\\mean";
	expr_ev_h5_path = "expression\\model\\pcaVariance";
	expr_pc_h5_path = "expression\\model\\pcaBasis";
	tl_h5_path = "shape\\representer\\cells";*/

	in.close();
	return true;
}

//初始化参数
void bfm::init_parm() {
	shape_coef.set_size(n_id_pc, 1);		//n_id_pc = 199，shape_coef为199*1
	shape_mu.set_size(n_vertice * 3, 1);	//设置形状均值矩阵，其中n_vertice = 53149，shape_mu为 159447*1 的矩阵
	shape_ev.set_size(n_id_pc, 1);		//设置形状方差矩阵，n_id_pc = 199,shape_ev 为 199*1
	shape_pc.set_size(n_vertice * 3, n_id_pc);		//159447*199
	tex_coef.set_size(n_id_pc, 1);
	tex_mu.set_size(n_vertice * 3, 1);
	tex_ev.set_size(n_id_pc, 1);
	tex_pc.set_size(n_vertice * 3, n_id_pc);

	expr_coef.set_size(n_expr_pc, 1);
	expr_mu.set_size(n_vertice * 3, 1);
	expr_ev.set_size(n_expr_pc, 1);
	expr_pc.set_size(n_vertice * 3, n_expr_pc);

	tl.set_size(n_face * 3, 1);

	current_shape.set_size(n_vertice * 3, 1);
	current_tex.set_size(n_vertice * 3, 1);
	current_expr.set_size(n_vertice * 3, 1);
	current_blendshape.set_size(n_vertice * 3, 1);

	landmark_idx.resize(n_landmark);
}


bool bfm::load_data() {
	float* shape_mu_raw = new float[n_vertice * 3];
	float* shape_ev_raw = new float[n_id_pc];
	float* shape_pc_raw = new float[n_vertice * 3 * n_id_pc];
	float* tex_mu_raw = new float[n_vertice * 3];
	float* tex_ev_raw = new float[n_id_pc];
	float* tex_pc_raw = new float[n_vertice * 3 * n_id_pc];
	float* expr_mu_raw = new float[n_vertice * 3];
	float* expr_ev_raw = new float[n_expr_pc];
	float* expr_pc_raw = new float[n_vertice * 3 * n_expr_pc];
	unsigned int* tl_raw = new unsigned int[n_face * 3];

	/* 测试读取其他文件是否成功*/
	//H5File file("../h5ex_d_rdwr.h5", H5F_ACC_RDONLY);
	//DataSet dataset = file.openDataSet("dataset/path");
	//DataSpace filespace = dataset.getSpace();
	//hsize_t shape[1];  // 此处不确定，不过长度设为 1 肯定是足够了
	//int _dims = filespace.getSimpleExtentDims(shape); // _dims 一定为 0
	//double buf[1];
	//dataset.read(buf, H5::PredType::NATIVE_DOUBLE, 1, filespace);
	//// 读出来啦
	//cout << buf[0] << std::endl;
	//return 0;

	// file 和 dataset 会被它们的析构函数关闭

	H5File file(bfm_h5_path, H5F_ACC_RDONLY);
	load_hdf5_model(shape_mu, shape_mu_h5_path, PredType::NATIVE_FLOAT);
	load_hdf5_model(shape_ev, shape_ev_h5_path, PredType::NATIVE_FLOAT);
	load_hdf5_model(shape_pc, shape_pc_h5_path, PredType::NATIVE_FLOAT);

	load_hdf5_model(tex_mu, tex_mu_h5_path, PredType::NATIVE_FLOAT);
	load_hdf5_model(tex_ev, tex_ev_h5_path, PredType::NATIVE_FLOAT);
	load_hdf5_model(tex_pc, tex_pc_h5_path, PredType::NATIVE_FLOAT);

	load_hdf5_model(expr_mu, expr_mu_h5_path, PredType::NATIVE_FLOAT);
	load_hdf5_model(expr_ev, expr_ev_h5_path, PredType::NATIVE_FLOAT);
	load_hdf5_model(expr_pc, expr_pc_h5_path, PredType::NATIVE_FLOAT);

	load_hdf5_model(tl, tl_h5_path, PredType::NATIVE_UINT32);
	file.close();
	shape_mu = shape_mu * 1000.0;

	ifstream in(landmark_idx_path, std::ios::in);
	if (!in) {
		bfm_out << "[ERROR] Can't open " << landmark_idx_path.c_str() << ".\n";
		return false;
	}
	for (int i = 0; i < n_landmark; i++) {
		int tmp_idx;
		in >> tmp_idx;
		landmark_idx[i] = tmp_idx - 1;
	}
	bfm_out << "[bfm] load data - success.\n";
	return true;
}

//产生随机人脸
void bfm::generate_random_face(double scale) {
	shape_coef = randn(n_id_pc, scale);		//n_id_pc=199
	tex_coef = randn(n_id_pc, scale);
	expr_coef = randn(n_expr_pc, scale);		//n_expr_pc=100
	std::cout << "以下运行generate_face函数"<<endl;
	generate_face();
}


void bfm::generate_random_face(double shape_scale, double tex_scale, double expr_scale) {
	shape_coef = randn(n_id_pc, shape_scale);
	std::cout << "shape_coef的类型：" << typeid (shape_coef).name() << endl;
	std::cout << "shape_coef的值：" << shape_coef << endl;
	tex_coef = randn(n_id_pc, tex_scale);
	expr_coef = randn(n_expr_pc, expr_scale);
	generate_face();
}

//生成人脸
void bfm::generate_face() {
	current_shape = coef2object(shape_coef, shape_mu, shape_pc, shape_ev);
	std::cout << "current_shape的类型：" << typeid (current_shape).name() << endl;
	current_tex = coef2object(tex_coef, tex_mu, tex_pc, tex_ev);
	current_expr = coef2object(expr_coef, expr_mu, expr_pc, expr_ev);
	current_blendshape = current_shape + current_expr;
}


dlib::matrix<double> bfm::coef2object(dlib::matrix<double>& coef, dlib::matrix<double>& mu,
	dlib::matrix<double>& pc, dlib::matrix<double>& ev) {
	return mu + pc * pointwise_multiply(coef, ev);		//原理公式
}


void bfm::ply_write(string fn, bool pick_landmark) {
	fstream out;
	/* Note: In Linux Cpp, we should use std::ios::bfm_out as flag, which is not necessary in Windows */
	out.open(fn, std::ios::out | std::ios::binary);
	if (!out) {
		bfm_out << "cannot open " << fn.c_str() << endl;
		return;
	}
	out << "ply\n";
	out << "format binary_little_endian 1.0\n";
	out << "comment Made from the 3D Morphable Face Model of the Univeristy of Basel, Switzerland.\n";
	out << "element vertex " << n_vertice << "\n";
	out << "property float x\n";
	out << "property float y\n";
	out << "property float z\n";
	out << "property uchar red\n";
	out << "property uchar green\n";
	out << "property uchar blue\n";
	out << "element face " << n_face << "\n";
	out << "property list uchar int vertex_indices\n";
	out << "end_header\n";

	int cnt = 0;
	for (int i = 0; i < 68; i++)
		std::cout << " " << landmark_idx[i] << std::endl;

	for (int i = 0; i < n_vertice; i++) {
		float x = float(current_blendshape(i * 3));
		float y = float(current_blendshape(i * 3 + 1));
		float z = float(current_blendshape(i * 3 + 2));
		unsigned char r, g, b;
		if (find(landmark_idx.begin(), landmark_idx.end(), i) != landmark_idx.end()) {
			r = 255;
			g = 0;
			b = 0;
			cnt++;
		}
		else {
			r = current_tex(i * 3);
			g = current_tex(i * 3 + 1);
			b = current_tex(i * 3 + 2);
		}
		out << x <<" " << y <<" " << z << " " << r << " " << g << " " << b << " " << endl;
		/*out.write((char*)&x, sizeof(x));
		out.write((char*)&y, sizeof(y));
		out.write((char*)&z, sizeof(z));
		out.write((char*)&r, sizeof(r));
		out.write((char*)&g, sizeof(g));
		out.write((char*)&b, sizeof(b));*/
	}
	if (pick_landmark && cnt != n_landmark) {
		bfm_out << "[ERROR] Pick too less landmarks.\n";
		bfm_out << "Number of picked points is " << cnt << ".\n";
	}

	unsigned char N_VER_PER_FACE = 3;
	for (int i = 0; i < n_face; i++) {		//面
		out.write((char*)&N_VER_PER_FACE, sizeof(N_VER_PER_FACE));
		//out << N_VER_PER_FACE << " ";
		//int x = tl(i * 3) - 1;
		//int y = tl(i * 3 + 1) - 1;
		//int z = tl(i * 3 + 2) - 1;
		int x = tl(i * 3);
		int y = tl(i * 3 + 1);
		int z = tl(i * 3 + 2);
		/*out.write((char*)&y, sizeof(y));
		out.write((char*)&x, sizeof(x));
		out.write((char*)&z, sizeof(z));*/
		out << x << " " << y << " " << z << endl;
	}
	out.close();
}