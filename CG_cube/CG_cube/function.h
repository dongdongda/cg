#pragma once



#include <iostream>
using namespace std;

#include<stdlib.h>
#include<math.h>
#include <algorithm>
#include<gl/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct Vertex;
struct Vector3D;
struct Camera;
class Triangle;
struct matrix_4x4;
Vector3D vector_matrix(Vector3D v, matrix_4x4 m);
void LiangBarsky(int &x1, int &y1, int &x2, int &y2, int XL, int XR, int YT, int YB);


//键盘旋转角度设置
float xRot = -30.001f;
float yRot = 0.001f;

float xRot_c = 0.001f;
float yRot_c = 0.001f;

//-------------------------------------定义RGB颜色数据格式----------------------------------------
struct color_RGB
{
	float red;
	float green;
	float blue;
};



//定义三维向量
struct Vector3D
{
	float x;
	float y;
	float z;

	Vector3D()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector3D(float p1, float p2, float p3)
	{
		x = p1;
		y = p2;
		z = p3;
	}

	//重载+实现向量加法运算
	Vector3D operator+(const Vector3D &others)
	{
		Vector3D res;
		res.x = this->x + others.x;
		res.y = this->y + others.y;
		res.z = this->z + others.z;

		return  res;
	}
	//重载-实现向量减法运算
	Vector3D operator-(const Vector3D &others)
	{
		Vector3D res;
		res.x = this->x - others.x;
		res.y = this->y - others.y;
		res.z = this->z - others.z;

		return  res;
	}

	//向量点乘
	float DotProduct(Vector3D &others)
	{
		float res = this->x * others.x + this->y * others.y + this->z * others.z;
		return res;
	}

	//向量叉乘
	Vector3D Crossproduct(Vector3D &others)
	{
		Vector3D res;
		res.x = this->y * others.z - this->z * others.y;
		res.y = this->z * others.x - this->x * others.z;
		res.z = this->x * others.y - this->y * others.x;

		return res;
	}


	//向量归一化（变成同方向的单位向量）
	void normalization()
	{
		float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);

		this->x = this->x * (1 / length);
		this->y = this->y * (1 / length);
		this->z = this->z * (1 / length);

	}

};
//-------------------------------------三维顶点绕X,Y轴旋转----------------------------------------
#define PI acos(-1)
//绕x轴的旋转变换
Vector3D Rotation_x(Vector3D p, float Angle)
{
	Angle = PI * (Angle / 180);
	Vector3D res;
	res.x = p.x;
	res.y = p.y * cos(Angle) - p.z * sin(Angle);
	res.z = p.y * sin(Angle) + p.z * cos(Angle);
	return res;
}

//绕y轴的旋转变换
Vector3D Rotation_y(Vector3D p, float Angle)
{
	Angle = PI * (Angle / 180);
	Vector3D res;
	res.x = p.x * cos(Angle) + p.z * sin(Angle);
	res.y = p.y;
	res.z = -p.x * sin(Angle) + p.z * cos(Angle);
	return res;
}

// 综合旋转变换
Vector3D Rotation_xy(Vector3D p, float Angle_x, float Angle_y)
{
	Vector3D res0 = Rotation_x(p, Angle_x);
	Vector3D res = Rotation_y(res0, Angle_y);
	return res;
}



//--------------------------------------------------------------纹理映射关系-------------------------------------------------------------


//--------------------------------------------------------------定义纹理映射算法---------------------------------------------------------
//定义纹理空间（u，v）
color_RGB texture_uv[1024][1024];

//--------------------------------------------------------------自定义简单纹理----------------------------------------------------------
color_RGB color_texture(Vector3D p)
{
	if (p.y <= -0.3 && p.y >= -0.51)
		return { 1.0f, 1.0f, 1.0f };

	if (p.y <= -0.1)
		return { 1.0f, 0.0f, 0.0f };

	if (p.y <= 0.1)
		return { 0.0f, 1.0f, 0.0f };

	if (p.y <= 0.3)
		return { 0.0f, 0.0f, 1.0f };

	if (p.y >= 0.3 && p.y <= 0.51)
		return { 0.0f, 1.0f, 1.0f };
}



//--------------------------------------------------------------BMP图片生成的复杂纹理-------------------------------------------------------------
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;

typedef struct tagBMP {
	DWORD biSize;
	long biWidth;
	long biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}BMP;

int ReadBmp(const char* szFileName);
int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b);
BMP bih;
BYTE buf[512];
BYTE *Buffer = buf;
BYTE p;
long LineByteWidth;


BYTE r, g, b;

int ReadBmp(const char* szFileName)
{
	FILE *file;
	WORD bfh[7];
	long dpixeladd;
	if (NULL == (file = fopen(szFileName, "rb")))
	{
		return 0;
	}
	printf("%s\n", szFileName);
	fread(&bfh, sizeof(WORD), 7, file);
	if (bfh[0] != (WORD)(((WORD)'B') | ('M' << 8)))
	{
		fclose(file);
		return 0;
	}
	fread(&bih, sizeof(BMP), 1, file);
	if (bih.biBitCount < 24)
	{
		fclose(file);
		return 0;
	}
	dpixeladd = bih.biBitCount / 8;
	LineByteWidth = bih.biWidth * (dpixeladd);
	if ((LineByteWidth % 4) != 0)
		LineByteWidth += 4 - (LineByteWidth % 4);
	if ((Buffer = (BYTE*)malloc(sizeof(BYTE)* LineByteWidth * bih.biHeight)) != NULL)
	{
		fread(Buffer, LineByteWidth * bih.biHeight, 1, file);
		fclose(file);
		return 1;
	}
	fclose(file);
	return 0;
}
int GetDIBColor(int X, int Y, BYTE *red, BYTE *green, BYTE *blue)
{
	int dpixeladd;
	BYTE *ptr;
	if (X < 0 || X >= 512 || Y < 0 || Y >= 512)
	{
		return 0;
	}
	dpixeladd = bih.biBitCount / 8;
	ptr = Buffer + X * dpixeladd + Y * LineByteWidth;
	*blue = *ptr;
	*green = *(ptr + 1);
	*red = *(ptr + 2);
	return 1;
}

void texture_init()
{

	float red;
	float green;
	float blue;
	color_RGB tmp;
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			if (GetDIBColor(i, j, &r, &g, &b) == 1)
			{
				red = r / 255.0;
				green = g / 255.0;
				blue = b / 255.0;
				tmp = { red, green, blue };
//				cout << tmp.red << "  " << tmp.blue << "  "<<tmp.green << endl;
				texture_uv[i][j] = tmp;
			}
//			else
//				cout << "Error!!!" << endl;
		}	
	}

}

color_RGB color_texture_bmp(Vector3D p)
{
	if (p.z < -0.495)
	{
		p.x += 0.5;
		if (p.x > 1) p.x = 1;
		p.y += 0.5;
		if (p.y > 1) p.y = 1;
		int x = abs((int)round(p.x * 511));
		int y = abs((int)round(p.y * 511));
		return texture_uv[x][y];
	}

	if (p.z > 0.495)
	{
		p.x += 0.5;
		if (p.x > 1) p.x = 1;
		p.y += 0.5;
		if (p.y > 1) p.y = 1;
		int x = abs((int)round(p.x * 511));
		int y = abs((int)round(p.y * 511));
		return texture_uv[x][y];
	}

	if (p.x < -0.495)
	{
		p.z += 0.5;
		if (p.z > 1) p.z = 1;
		p.y += 0.5;
		if (p.x > 1) p.x = 1;
		int z = abs((int)round(p.z * 511));
		int y = abs((int)round(p.y * 511));
		return texture_uv[z][y];
	}

	if (p.x > 0.495)
	{
		p.z += 0.5;
		if (p.z > 1) p.z = 1;
		p.y += 0.5;
		if (p.y > 1) p.y = 1;
		int z = abs((int)round(p.z * 511));
		int y = abs((int)round(p.y * 511));
		return texture_uv[z][y];
	}

	if (p.y < -0.495)
	{
		p.x += 0.5;
		if (p.x > 1) p.x = 1;
		p.z += 0.5;
		if (p.z > 1) p.z = 1;
		int x = abs((int)round(p.x * 511));
		int z = abs((int)round(p.z * 511));
		return texture_uv[x][z];
	}

	if (p.y > 0.495)
	{
		p.x += 0.5;
		if (p.x > 1) p.x = 1;
		p.z += 0.5;
		if (p.z > 1) p.z = 1;
		int x = abs((int)round(p.x * 511));
		int z = abs((int)round(p.z * 511));
		return texture_uv[x][z];
	}
}



//-------------------------------------定义三角形（Triangle）-------------------------------------
class Triangle
{
public:
	Vector3D a;
	Vector3D b;
	Vector3D c;
	Vector3D normal;

	Triangle()
	{
		a = { 0,0,0 };
		b = { 1,0,0 };
		c = { 0,1,0 };

		normal = { 0,0,1 };
	}

	Triangle(Vector3D a, Vector3D b, Vector3D c)
	{
		this->a = a;
		this->b = b;
		this->c = c;

		Vector3D l1 = b - a;
		Vector3D l2 = c - a;

		this->normal = l1.Crossproduct(l2);
	}

};



//-------------------------------------定义矩阵及向量和矩阵乘法----------------------------------------
//4乘4矩阵
struct matrix_4x4 {

	float matrix[4][4];
};

//矢量和矩阵相乘
Vector3D vector_matrix(Vector3D v, matrix_4x4 m)
{
	Vector3D res;
	res.x = (m.matrix[0][0] * v.x) + (m.matrix[0][1] * v.y) + (m.matrix[0][2] * v.z) + m.matrix[0][3];
	res.y = (m.matrix[1][0] * v.x) + (m.matrix[1][1] * v.y) + (m.matrix[1][2] * v.z) + m.matrix[1][3];
	res.z = (m.matrix[2][0] * v.x) + (m.matrix[2][1] * v.y) + (m.matrix[2][2] * v.z) + m.matrix[2][3];
	//	cout << "原始Z值：" << res.z << endl;
	float s = (m.matrix[3][0] * v.x) + (m.matrix[3][1] * v.y) + (m.matrix[3][2] * v.z) + m.matrix[3][3];
	res.x = res.x / s;
	res.y = res.y / s;
	res.z = res.z / s;
	//	cout << "转化后Z值：" << res.z << endl;
	return res;
}




//-------------------------------------定义摄像机（Camera）----------------------------------------
struct Camera
{
	//摄像机所在的位置
	struct Vector3D pos;

	//摄像机指向方向
	struct Vector3D look;
	//摄像机右轴方向
	struct Vector3D right;
	//摄像机上轴方向
	struct Vector3D up;
};




//------------------------------------------------------------------MVP矩阵坐标变换------------------------------------------------------------------------
//世界坐标变换(World Transform)
Vector3D World_Transform(Vector3D model_coord, matrix_4x4 t)
{
	return vector_matrix(model_coord, t);
}


//相机视图变换(View Transform)
Vector3D View_Transform(Vector3D world_coord, Camera c)
{


	matrix_4x4  t{
					{ {c.right.x,                    c.right.y,                    c.right.z,                     -(c.right.DotProduct(c.pos))},
					  {c.up.x,                       c.up.y,                       c.up.z,                        -(c.up.DotProduct(c.pos))   },
					  {c.look.x,                     c.look.y,                     c.look.z,                      -(c.look.DotProduct(c.pos)) },
					  {0,                            0,                            0,                             1} }
	};
	return vector_matrix(world_coord, t);
}


//投影视图变换（Project Transform，包括正交投影和透视投影）
//（1）通过角度计算(左手系)
Vector3D Project_Transform(Vector3D view_coord, float n, float f, float tan_angle, float Aspect)
{
	matrix_4x4 t{
				   {  {(1 / tan_angle) / Aspect,         0,                       0,                 0              },
					  {0,                                 1 / tan_angle,          0,                 0              },
					  {0,                                 0,                      -f / (n - f),     f * n / (n - f) },
					  {0,                                 0,                      1,                 0              }  }
	};
	return vector_matrix(view_coord, t);
}

//(2)通过平面坐标计算(左手系)
Vector3D Project_Transform(Vector3D view_coord, float n, float f, float r, float l, float t, float b)
{
	matrix_4x4 mat{
				   {  {(2 * n) / (r - l),         0,                    -(r + l) / (r - l),          0                     },
					  {0,                         (2 * n) / (t - b),    -(t + b) / (t - b),          0                     },
					  {0,                         0,                    (f + n) / (f - n),          (2 * f * n) / (n - f) },
					  {0,                         0,                    1,                          0                     }  }
	};
	return vector_matrix(view_coord, mat);
}


float cam_pos_z = -3.0;
float tan_angle = 0.5;
//使用左手坐标系，初始将摄像机放在（0，0，-3）的位置，正方向为Z正轴，右轴向平行于X正轴，上轴向平行于Y正轴；
//NDC空间为标准立方体空间，xyz范围均为-1到1；
//世界坐标系下某点经过MVP变换后的坐标为：
Vector3D MVP_Transform(Vector3D p, Camera c)
{
	Vector3D v = View_Transform(p, c);
	//	return Project_Transform(v, 2.0, 4.0, 1.0, -1.0, 1.0, -1.0);
	//	cout << "摄像机位置：" << cam_pos_z << endl;
	return Project_Transform(v, -1.0 - cam_pos_z, 1.0 - cam_pos_z, 0.5, 1.0);
}










//-------------------------------------定义绘制直线的2DDA算法----------------------------------------
void DDALine(float xa, float ya, color_RGB ca, float xb, float yb, color_RGB cb)
{
	int steps = 267;

	float dx = (xb - xa) / steps;
	float dy = (yb - ya) / steps;

	float d_red = (cb.red - ca.red) / steps;
	float d_blue = (cb.blue - ca.blue) / steps;
	float d_green = (cb.green - ca.green) / steps;

	float x = xa;
	float y = ya;

	color_RGB c = ca;

	for (int i = 0; i <= steps; i++)
	{
		x += dx;
		y += dy;
		c.red += d_red;
		c.blue += d_blue;
		c.green += d_green;
		glColor3f(c.red, c.green, c.blue);
		glBegin(GL_POINTS);
		glVertex3f(x, y, 0);
		glEnd();
	}

}



//--------------------------------------------------绘制三角形（通过叉积判断点是否在三角形内）-----------------------------------------------------------
struct Point2D {
	int x;
	int y;
	Point2D(int a, int b) :x(a), y(b) { }
};

int cross(const Point2D &a, const Point2D &b, const Point2D &p)
{
	return (b.x - a.x)*(p.y - a.y) - (b.y - a.y)*(p.x - a.x);
}

bool toLeft(const Point2D &a, const Point2D &b, const Point2D &p)
{
	return cross(a, b, p) > 0;
}

bool inTriangle(const Point2D &p, const Point2D &a, const Point2D &b, const Point2D &c)
{
	bool res = toLeft(a, b, p);
	if (res != toLeft(b, c, p))
		return false;
	if (res != toLeft(c, a, p))
		return false;
	if (cross(a, b, c) == 0)    //ABC is in one line
		return false;
	return true;
}


void Draw_Triangle(float xa, float ya, float xb, float yb, float xc, float yc)
{
	//确定三角形所在的矩形框
	float xmin = xa;
	float xmax = xa;
	if (xb < xmin) xmin = xb;
	if (xc < xmin) xmin = xc;
	if (xb > xmax) xmax = xb;
	if (xc > xmax) xmax = xc;
	float ymin = ya;
	float ymax = ya;
	if (yb < ymin) ymin = yb;
	if (yc < ymin) ymin = yc;
	if (yb > ymax) ymax = yb;
	if (yc > ymax) ymax = yc;

	//利用向量叉积确定是否在三角形内部
	Point2D a(xa, ya);
	Point2D b(xb, yb);
	Point2D c(xc, yc);
	Point2D tmp(0.0f, 0.0f);
	float frame = 1.0 / 400.0;
	for (float i = xmin; i < xmax; i += frame)
	{
		for (float j = ymin; j < ymax; j += frame)
		{
			tmp.x = i;
			tmp.y = j;
			if (inTriangle(tmp, a, b, c) == true)
			{
				glVertex2f(i, j);
			}

		}
	}
}


//-------------------------------------定义Gouraud光照模式下的光照算法----------------------------------------
//假设直射光源为白光

const int n = 100;

color_RGB shador(float ka, float kd[3], float ks, Vector3D L, Vector3D N, Vector3D V)
{
	L.normalization();
	N.normalization();
	V.normalization();
	color_RGB res{ 0.0f, 0.0f, 0.0f };
	Vector3D H = L + V;
	H.normalization();
	res.red = ka + kd[0] * L.DotProduct(N) + ks * pow(H.DotProduct(N), n);
	res.blue = ka + kd[1] * L.DotProduct(N) + ks * pow(H.DotProduct(N), n);
	res.green = ka + kd[2] * L.DotProduct(N) + ks * pow(H.DotProduct(N), n);
	return res;
}

//-----------------------------------------------------------定义深度缓存矩阵(以及透射矫正差值)----------------------------------------
float zbuffer[800][800];

inline void Z_buffer_init()
{
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 800; j++)
		{
			zbuffer[i][j] = 10000.0;
		}
	}
}

float depth(Point2D p, const Point2D & p1, const Point2D & p2, const Point2D & p3, float ZA, float ZB, float ZC)
{
	float x = p.x;
	float i = p.y;
	/** 计算重心系数 */
	float alph = (-(x - p2.x)*(p3.y - p2.y) + (i - p2.y)*(p3.x - p2.x)) / (-(p1.x - p2.x)*(p3.y - p2.y) + (p1.y - p2.y)*(p3.x - p2.x));
	float beta = (-(x - p3.x)*(p1.y - p3.y) + (i - p3.y)*(p1.x - p3.x)) / (-(p2.x - p3.x)*(p1.y - p3.y) + (p2.y - p3.y)*(p1.x - p3.x));

	//做了透射矫正差值
	float res = 1 / ((alph / ZA) + (beta / ZB) + ((1 - alph - beta) / ZC));

	return res;
}

float depth_2D(Point2D p, const Point2D & p1, const Point2D & p2, const Point2D & p3, float ZA, float ZB, float ZC)
{
	float x = p.x;
	float i = p.y;
	/** 计算重心系数 */
	float alph = (-(x - p2.x)*(p3.y - p2.y) + (i - p2.y)*(p3.x - p2.x)) / (-(p1.x - p2.x)*(p3.y - p2.y) + (p1.y - p2.y)*(p3.x - p2.x));
	float beta = (-(x - p3.x)*(p1.y - p3.y) + (i - p3.y)*(p1.x - p3.x)) / (-(p2.x - p3.x)*(p1.y - p3.y) + (p2.y - p3.y)*(p1.x - p3.x));

	//没有做透射矫正差值
	float res = ZA * alph + ZB * beta + ZC * (1 - alph - beta);
	return res;
}



//--------------------------------------------------------------显示窗口设置及初始化---------------------------------------------------------
//初始化窗口大
GLsizei winWidth = 800, winHeight = 800;

//设置世界坐标系的显示范围
GLfloat xwcMin = -400.0, xwcMax = 400.0;
GLfloat ywcMin = -400.0, ywcMax = 400.0;

//背景颜色设置
void init(void)
{
	//设置显示窗口的背景颜色为黑色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Vector3D pos0(0.0f, 0.0f, cam_pos_z);
Vector3D look0(0.0f, 0.0f, 1.0f);
Vector3D rig0(1.0f, 0.0f, 0.0f);
Vector3D up0(0.0f, 1.0f, 0.0f);
Camera cam = { pos0, look0, rig0, up0 };
void camera_rotate(float xRot_c, float yRot_c)
{
	pos0.z = cam_pos_z;
	cam.pos = Rotation_xy(pos0, xRot_c, yRot_c);
	cam.look = { -cam.pos.x, -cam.pos.y, -cam.pos.z };
	Vector3D r_point0 = { 1.0, 0.0, cam_pos_z };
	Vector3D r_point = Rotation_xy(r_point0, xRot_c, yRot_c);
	cam.right = { r_point.x - cam.pos.x, r_point.y - cam.pos.y, r_point.z - cam.pos.z };
	cam.up = cam.look.Crossproduct(cam.right);
	cam.look.normalization();
	cam.right.normalization();
	cam.up.normalization();
	//	cout << "摄像机属性：                  " << endl;
	//	cout << cam.pos.x << "          " << cam.pos.y << "          " << cam.pos.z << endl;
	//	cout << cam.look.x << "          " << cam.look.y << "          " << cam.look.z << endl;
	//	cout << cam.right.x << "          " << cam.right.y << "          " << cam.right.z << endl;
	//	cout << cam.up.x << "          " << cam.up.y << "          " << cam.up.z << endl;
}



//--------------------------------------------------------------定义三角形着色算法（重心插值算法+SacnLine扫描算法）---------------------------------------------------------

//三角形重心插值算法
color_RGB GetInterpolationColor(Point2D p, const Point2D & p1, const Point2D & p2, const Point2D & p3, const color_RGB & c1, const color_RGB & c2, const color_RGB & c3)
{
	float x = p.x;
	float i = p.y;
	/** 进行颜色插值 */
	float alph = (-(x - p2.x)*(p3.y - p2.y) + (i - p2.y)*(p3.x - p2.x)) / (-(p1.x - p2.x)*(p3.y - p2.y) + (p1.y - p2.y)*(p3.x - p2.x));
	float beta = (-(x - p3.x)*(p1.y - p3.y) + (i - p3.y)*(p1.x - p3.x)) / (-(p2.x - p3.x)*(p1.y - p3.y) + (p2.y - p3.y)*(p1.x - p3.x));
	/** 注意RGB是三通道，在计算插值颜色时需要分别计算三个通道的插值分量，最后再合并为的插值颜色 */
	float  pixel_r = c1.red * alph + c2.red * beta + c3.red *(1 - alph - beta);
	float  pixel_g = c1.green * alph + c2.green * beta + c3.green *(1 - alph - beta);
	float  pixel_b = c1.blue * alph + c2.blue * beta + c3.blue *(1 - alph - beta);

	color_RGB res{ pixel_r, pixel_g, pixel_b };
	return res;
}

//--------------------------------------------------------------屏幕二维坐标转换回原始三维坐标--------------------------------------------------------
Vector3D Point2D_to_Vector3D(Point2D p, const Point2D & p1, const Point2D & p2, const Point2D & p3, float ZA, float ZB, float ZC)
{
	float x = p.x / 400.0;
	float y = p.y / 400.0;
	float z = depth_2D(p, p1, p2, p3, ZA, ZB, ZC);
	//	cout << y << "    " << z << endl;
	float z0 = (1 - cam_pos_z * cam_pos_z) / (2 * z + cam_pos_z - 1);
	float x0 = 0.5 * z0 * x;
	float y0 = 0.5 * z0 * y;
	Vector3D res(x0, y0, z0);
	res.z += cam_pos_z;
	res = Rotation_xy(res, xRot_c, yRot_c);

	/*
	Camera c = cam;
	c.pos = {0, 0, 3};
	c.look = { -cam.look.x, -cam.look.y, -cam.look.z };
	c.right = { -cam.right.x, -cam.right.y, -cam.right.z };
	c.up = { -cam.up.x, -cam.up.y, -cam.up.z };
	res = View_Transform(res, c);*/

	res = Rotation_y(res, -yRot);
	res = Rotation_x(res, -xRot);
	return res;

}

//绘制一条扫描线
void drawSpanLine(int minX, int maxX, int y, const Point2D & p1, const Point2D & p2, const Point2D & p3, const color_RGB & c1, const color_RGB & c2, const color_RGB & c3, float ZA, float ZB, float ZC)
{
	if (minX > maxX)   swap(minX, maxX);
	//	minX = max(-200, minX);
	//	maxX = min(200, maxX);
	Point2D t{ minX, y };
	color_RGB c = c1;
	for (int i = minX; i <= maxX; ++i)
	{
		t.x = i;
		c = GetInterpolationColor(t, p1, p2, p3, c1, c2, c3);
		Vector3D v3 = Point2D_to_Vector3D(t, p1, p2, p3, ZA, ZB, ZC);
//		cout << "原始坐标:" << v3.x << "  " << v3.y << "  " << v3.z << endl;
		color_RGB tc = color_texture_bmp(v3);
//		cout << tc.red << "  " << tc.green << "  " << tc.blue << endl;
		c.red *= tc.red;
		c.green *= tc.green;
		c.blue *= tc.blue;
		//	    cout << depth(t, p1, p2, p3, ZA, ZB, ZC) << endl;
		//		cout << zbuffer[i + 400][y + 400] << endl;
		if (depth(t, p1, p2, p3, ZA, ZB, ZC) < zbuffer[i + 400][y + 400])
		{
			zbuffer[i + 400][y + 400] = depth(t, p1, p2, p3, ZA, ZB, ZC);
			glColor3f(c.red, c.green, c.blue);
			//	glBegin(GL_POINTS);
			glVertex3f(t.x / 400.0, y / 400.0, 0);
			//	glEnd();

		}
	}
}

//绘制平底三角形
void drawFlatBottomTriangle(const Point2D& mid, const Point2D & p1, const Point2D & p2, const Point2D & p3, const color_RGB & c1, const color_RGB & c2, const color_RGB & c3, float ZA, float ZB, float ZC)
{
	if (p1.y == mid.y)  return;
	for (int y = p1.y; y >= p2.y; y--)
	{
		int left_x = p1.x - (int)((p1.y - y)*(p1.x - p2.x) / (p1.y - p2.y));
		int righ_x = p1.x - (int)((p1.y - y)*(p1.x - mid.x) / (p1.y - mid.y));

		drawSpanLine(left_x, righ_x, y, p1, p2, p3, c1, c2, c3, ZA, ZB, ZC);
	}
}
//绘制平顶三角形
void drawFlatTopTriangle(const Point2D& mid, const Point2D & p1, const Point2D & p2, const Point2D & p3, const color_RGB & c1, const color_RGB & c2, const color_RGB & c3, float ZA, float ZB, float ZC)
{
	if (p3.y == mid.y)  return;
	for (int y = p3.y; y <= p2.y; y++)
	{
		int left_x = p3.x - (int)((p3.y - y)*(p3.x - p2.x) / (p3.y - p2.y));
		int righ_x = p3.x - (int)((p3.y - y)*(p3.x - mid.x) / (p3.y - mid.y));

		drawSpanLine(left_x, righ_x, y, p1, p2, p3, c1, c2, c3, ZA, ZB, ZC);
	}
}

//绘制整个三角形
void  drawTriangle(const Point2D & p1, const Point2D & p2, const Point2D & p3, const color_RGB & c1, const color_RGB & c2, const color_RGB & c3, float ZA, float ZB, float ZC)
{
	Point2D p[3] = { p1, p2, p3 };
	color_RGB c[3] = { c1, c2, c3 };
	//将三角形三个点按y值大小排序
	Point2D tmp1 = p1;
	color_RGB tmp2 = c1;
	if (p[1].y > p[0].y)
	{
		p[0] = p2;
		p[1] = p1;
		c[0] = c2;
		c[1] = c1;
		swap(ZA, ZB);
	}
	if (p[2].y > p[0].y)
	{
		tmp1 = p[0];
		p[0] = p[2];
		p[2] = tmp1;

		tmp2 = c[0];
		c[0] = c[2];
		c[2] = tmp2;
		swap(ZA, ZC);
	}
	if (p[1].y < p[2].y)
	{
		tmp1 = p[1];
		p[1] = p[2];
		p[2] = tmp1;

		tmp2 = c[1];
		c[1] = c[2];
		c[2] = tmp2;
		swap(ZB, ZC);
	}

	if (p[0].y == p[2].y)
		return;

	int px = (int)roundf(float(p[1].y - p[0].y) / (p[2].y - p[0].y) * (p[2].x - p[0].x) + p[0].x);

	Point2D mid(px, p[1].y);


	drawFlatBottomTriangle(mid, p[0], p[1], p[2], c[0], c[1], c[2], ZA, ZB, ZC);
	drawFlatTopTriangle(mid, p[0], p[1], p[2], c[0], c[1], c[2], ZA, ZB, ZC);
}


//--------------------------------------------------------------定义Liang-Barsky直线裁剪算法---------------------------------------------------------
void LiangBarsky(int &x1, int &y1, int &x2, int &y2, int XL, int XR, int YT, int YB)
{
	int ansx1, ansx2, ansy1, ansy2;
	//三种类型
		//平行于y轴
	if (x1 - x2 == 0)
	{
		if (x1<XL || x1>XR)
		{
			return;
		}
		else
		{
			int ymin = max(YB, min(y1, y2));
			int ymax = min(YT, max(y1, y2));
			if (ymin <= ymax)
			{
				ansx1 = ansx2 = x1;
				ansy1 = ymin;
				ansy2 = ymax;
			}
			else
			{
				return;
			}
		}
	}
	//平行于x轴
	else if (y1 - y2 == 0)
	{
		if (y1<YB || y1>YT)
		{
			return;
		}
		else
		{
			int xmin = max(XL, min(x1, x2));
			int xmax = min(XR, max(x1, x2));
			if (xmin <= xmax)
			{
				ansy1 = ansy2 = y1;
				ansx1 = xmin;
				ansx2 = xmax;
			}
			else
			{
				return;
			}
		}
	}
	//不平行于坐标轴
	else
	{
		float p1, p2, p3, p4;
		float q1, q2, q3, q4;
		p1 = -(x2 - x1);
		p2 = -p1;
		p3 = -(y2 - y1);
		p4 = -p3;
		q1 = x1 - XL;
		q2 = XR - x1;
		q3 = y1 - YB;
		q4 = YT - y1;
		float u1, u2, u3, u4;
		u1 = q1 / p1;
		u2 = q2 / p2;
		u3 = q3 / p3;
		u4 = q4 / p4;
		float umin, umax;
		if (p1 < 0)
		{
			if (p3 < 0)
			{
				umin = max(0.0f, max(u1, u3));
				umax = min(1.0f, min(u2, u4));
			}
			else
			{
				umin = max(0.0f, max(u1, u4));
				umax = min(1.0f, min(u2, u3));
			}

		}
		else
		{
			if (p3 < 0)
			{
				umin = max(0.0f, max(u2, u3));
				umax = min(1.0f, min(u1, u4));
			}
			else
			{
				umin = max(0.0f, max(u2, u4));
				umax = min(1.0f, min(u1, u3));

			}

		}
		if (umin <= umax)
		{
			x1 = x1 + umin * (x2 - x1);
			x2 = x1 + umax * (x2 - x1);
			y1 = y1 + umin * (y2 - y1);
			y2 = y1 + umax * (y2 - y1);
		}
		else
			return;
	}
	//调用函数重绘直线
	cout << ansx1 << " " << ansy1 << endl;
	cout << ansx1 << " " << ansy1 << endl;
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(ansx1 / 400.0, ansy1 / 400.0); // clipped line startpoint 
	glVertex2f(ansx2 / 400.0, ansx2 / 400.0); // clipped line endpoint 
	glEnd();

	return;
}





//-----------------------------------------------------光照和纹理叠加渲染-------------------------------------------------------------
void Triangle_display(Triangle t, float xrot, float yrot, Camera cam, Vector3D light)
{
	Vector3D a = Rotation_xy(t.a, xrot, yrot);
	Vector3D b = Rotation_xy(t.b, xrot, yrot);
	Vector3D c = Rotation_xy(t.c, xrot, yrot);

	camera_rotate(xRot_c, yRot_c);
	//	cout << "摄像机位置" <<cam.pos.x << "  " << cam.pos.y << "  " << cam.pos.z << endl;

	Vector3D a_s = MVP_Transform(a, cam);
	Vector3D b_s = MVP_Transform(b, cam);
	Vector3D c_s = MVP_Transform(c, cam);

	//	cout << a_s.x << " " << a_s.y << " " << a_s.z << endl;

	Point2D a_s_2D{ (int)round(a_s.x * 400), (int)round(a_s.y * 400) };
	Point2D b_s_2D{ (int)round(b_s.x * 400), (int)round(b_s.y * 400) };
	Point2D c_s_2D{ (int)round(c_s.x * 400), (int)round(c_s.y * 400) };

	float ka = 0.6f;
	float kd_a[3] = { 0.5f, 0.5f, 0.5f };
	float kd_b[3] = { 0.5f, 0.5f, 0.5f };
	float kd_c[3] = { 0.5f, 0.5f, 0.5f };
	float ks = 0.4f;

	Vector3D L_a(light.x - a.x, light.y - a.y, light.z - a.z);
	Vector3D L_b(light.x - b.x, light.y - b.y, light.z - b.z);
	Vector3D L_c(light.x - c.x, light.y - c.y, light.z - c.z);

	Vector3D N_a(a.x, a.y, a.z);
	Vector3D N_b(b.x, b.y, b.z);
	Vector3D N_c(c.x, c.y, c.z);

	Vector3D V_a(cam.pos.x - a.x, cam.pos.y - a.y, cam.pos.z - a.z);
	Vector3D V_b(cam.pos.x - b.x, cam.pos.y - b.y, cam.pos.z - b.z);
	Vector3D V_c(cam.pos.x - c.x, cam.pos.y - c.y, cam.pos.z - c.z);

	//	cout << L_a.x << "   " << L_a.y << "   " << L_a.z << endl;
	//	cout << N_a.x << "   " << N_a.y << "   " << N_a.z << endl;
	//	cout << V_a.x << "   " << V_a.y << "   " << V_a.z << endl;
	color_RGB ca = shador(ka, kd_a, ks, L_a, N_a, V_a);
	color_RGB cb = shador(ka, kd_b, ks, L_b, N_b, V_b);
	color_RGB cc = shador(ka, kd_c, ks, L_c, N_c, V_c);
	//	cout << ca.blue << ca.green << ca.blue << endl;

	glPointSize(1.0f);
	glBegin(GL_POINTS);
	drawTriangle(a_s_2D, b_s_2D, c_s_2D, ca, cb, cc, a_s.z, b_s.z, c_s.z);
	glEnd();

	return;

}