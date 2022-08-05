#include <fstream>
#include <sstream>
#include <iostream>
#include<vector>
#include <algorithm>
using namespace std;

#include <glm/glm.hpp>
#include<gl/glut.h>

void readObj(
	std::string filepath,
	std::vector<glm::vec3>& points,
	std::vector<glm::vec2>& texcoords,
	std::vector<glm::vec3>& normals
)
{


	// 顶点属性
	std::vector<glm::vec3> vectexPosition;
	std::vector<glm::vec2> vertexTexcoord;
	std::vector<glm::vec3> vectexNormal;

	// 面片索引信息
	std::vector<glm::ivec3> positionIndex;
	std::vector<glm::ivec3> texcoordIndex;
	std::vector<glm::ivec3> normalIndex;

	// 打开文件流
	std::ifstream fin(filepath);
	std::string line;
	if (!fin.is_open())
	{


		std::cout << "文件 " << filepath << " 打开失败" << std::endl;
		exit(-1);
	}

	// 按行读取
	while (std::getline(fin, line))
	{


		std::istringstream sin(line);   // 以一行的数据作为 string stream 解析并且读取
		std::string type;
		GLfloat x, y, z;
		int v0, vt0, vn0;   // 面片第 1 个顶点的【位置，纹理坐标，法线】索引
		int v1, vt1, vn1;   // 2
		int v2, vt2, vn2;   // 3
		char slash;

		// 读取obj文件
		sin >> type;
		if (type == "v") {


			sin >> x >> y >> z;
			vectexPosition.push_back(glm::vec3(x, y, z));
		//	cout << x << y << z << endl;
		}
		if (type == "vt") {


			sin >> x >> y;
			vertexTexcoord.push_back(glm::vec2(x, y));
		}
		if (type == "vn") {


			sin >> x >> y >> z;
			vectexNormal.push_back(glm::vec3(x, y, z));
		}
		if (type == "f") {


			sin >> v0 >> slash >> vt0 >> slash >> vn0;
			sin >> v1 >> slash >> vt1 >> slash >> vn1;
			sin >> v2 >> slash >> vt2 >> slash >> vn2;
			positionIndex.push_back(glm::ivec3(v0 - 1, v1 - 1, v2 - 1));
			texcoordIndex.push_back(glm::ivec3(vt0 - 1, vt1 - 1, vt2 - 1));
			normalIndex.push_back(glm::ivec3(vn0 - 1, vn1 - 1, vn2 - 1));
		}
	}

	// 根据面片信息生成最终传入顶点着色器的顶点数据
	for (int i = 0; i < positionIndex.size(); i++)
	{


		// 顶点位置
		points.push_back(vectexPosition[positionIndex[i].x]);
		points.push_back(vectexPosition[positionIndex[i].y]);
		points.push_back(vectexPosition[positionIndex[i].z]);

		// 顶点纹理坐标
		texcoords.push_back(vertexTexcoord[texcoordIndex[i].x]);
		texcoords.push_back(vertexTexcoord[texcoordIndex[i].y]);
		texcoords.push_back(vertexTexcoord[texcoordIndex[i].z]);

		// 顶点法线
		normals.push_back(vectexNormal[normalIndex[i].x]);
		normals.push_back(vectexNormal[normalIndex[i].y]);
		normals.push_back(vectexNormal[normalIndex[i].z]);
	}

}

std::vector<glm::vec3> points;
std::vector<glm::vec2> texcoords;
std::vector<glm::vec3> normals;

int main()
{
	readObj("models/res/diablo3_pose.obj", points, texcoords, normals);
	std::cout << points[0].x << "    " << points[0].y << "    " << points[0].z << endl;
	std::cout << points[1].x << "    " << points[1].y << "    " << points[1].z << endl;
	std::cout << points[2].x << "    " << points[2].y << "    " << points[2].z << endl;
	std::cout << texcoords[0].x << "    " << texcoords[0].y << "    " << endl;
	std::cout << texcoords[1].x << "    " << texcoords[1].y << "    " << endl;
	std::cout << texcoords[2].x << "    " << texcoords[2].y << "    " << endl;
	system("pause");
}