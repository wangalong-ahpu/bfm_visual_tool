#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "eos/core/Mesh.hpp"
#include <string>
#include <iostream>


//ͼ����������ȡģ�� ����
//int DlibFaceDetection(std::string& imagefile, std::string& landmarksfile);
//���Ϊply��ʽģ�� ����
void write_ply(const eos::core::Mesh& mesh, std::string filename);
//�ؽ�ģ�� ����
//bool Reconstruct(std::string& imagefile, std::string& modelfile, std::string& outplyfilename);

//void test();
#endif