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


	// ��������
	std::vector<glm::vec3> vectexPosition;
	std::vector<glm::vec2> vertexTexcoord;
	std::vector<glm::vec3> vectexNormal;

	// ��Ƭ������Ϣ
	std::vector<glm::ivec3> positionIndex;
	std::vector<glm::ivec3> texcoordIndex;
	std::vector<glm::ivec3> normalIndex;

	// ���ļ���
	std::ifstream fin(filepath);
	std::string line;
	if (!fin.is_open())
	{


		std::cout << "�ļ� " << filepath << " ��ʧ��" << std::endl;
		exit(-1);
	}

	// ���ж�ȡ
	while (std::getline(fin, line))
	{


		std::istringstream sin(line);   // ��һ�е�������Ϊ string stream �������Ҷ�ȡ
		std::string type;
		GLfloat x, y, z;
		int v0, vt0, vn0;   // ��Ƭ�� 1 ������ġ�λ�ã��������꣬���ߡ�����
		int v1, vt1, vn1;   // 2
		int v2, vt2, vn2;   // 3
		char slash;

		// ��ȡobj�ļ�
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

	// ������Ƭ��Ϣ�������մ��붥����ɫ���Ķ�������
	for (int i = 0; i < positionIndex.size(); i++)
	{


		// ����λ��
		points.push_back(vectexPosition[positionIndex[i].x]);
		points.push_back(vectexPosition[positionIndex[i].y]);
		points.push_back(vectexPosition[positionIndex[i].z]);

		// ������������
		texcoords.push_back(vertexTexcoord[texcoordIndex[i].x]);
		texcoords.push_back(vertexTexcoord[texcoordIndex[i].y]);
		texcoords.push_back(vertexTexcoord[texcoordIndex[i].z]);

		// ���㷨��
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