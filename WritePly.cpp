#include "eos/core/Mesh.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include "Functions.h"
using namespace std;
using namespace eos;
void write_ply(const core::Mesh& mesh, std::string filename)
{
    std::ofstream ply_file(filename);
    if (ply_file.is_open())
    {
        //信息头部分
        ply_file << "ply\n";
        ply_file << "format ascii 1.0\n";
        ply_file << "element vertex " << mesh.vertices.size() << "\n"; // 每个点占用3个float元素
        ply_file << "property float x\n";
        ply_file << "property float y\n";
        ply_file << "property float z\n";
        ply_file << "element face " << mesh.tvi.size() << "\n";
        ply_file << "property list uchar int vertex_index\n";
        ply_file << "end_header\n";
        //xyz
        for (std::size_t i = 0; i < mesh.vertices.size(); ++i)
            ply_file << mesh.vertices[i][0] << " " << mesh.vertices[i][1] << " " << mesh.vertices[i][2] << "\n";
        //三角网格列表
        for (std::size_t i = 0; i < mesh.tvi.size(); ++i)
            ply_file << "3 " << mesh.tvi[i][0] << " " << mesh.tvi[i][1] << " " << mesh.tvi[i][2] << std::endl;
        std::cout << "Reconstructed face had saved at " << filename << std::endl;
    }
    else
    {
        std::cout << "Error opening " << filename << std::endl;
    }
}