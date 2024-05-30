#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "eos/core/Mesh.hpp"
#include <string>
#include <iostream>


//图像特征点提取模块 声明
//int DlibFaceDetection(std::string& imagefile, std::string& landmarksfile);
//输出为ply格式模块 声明
void write_ply(const eos::core::Mesh& mesh, std::string filename);
//重建模块 声明
//bool Reconstruct(std::string& imagefile, std::string& modelfile, std::string& outplyfilename);

//void test();
#endif