#pragma once

#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <cmath>
#include<math.h>
#include <algorithm>

#include<gl/glut.h>
#include <GL/freeglut_ext.h>

struct color_RGB;
struct Point2D;
struct Vector3D;
struct Camera;
class Triangle;
struct matrix_4x4;
Vector3D vector_matrix(Vector3D v, matrix_4x4 m);

//模型和摄像机旋转角度设置
float xRot = 0.001f;
float yRot = 180.001f;

float xRot_c = 0.001f;
float yRot_c = 0.001f;

struct color_RGB
{
	float red;
	float green;
	float blue;
	color_RGB color_add(color_RGB &others)
	{
		color_RGB res;
		res.red = this->red * others.red;
		res.green = this->green * others.green;
		res.blue = this->blue * others.blue;
		return res;
	}
};


// 定义二维坐标
struct Point2D {
	int x;
	int y;

	Point2D()
	{
		x = 0;
		y = 0;
	}
	Point2D(int a, int b) :x(a), y(b) { }
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
const int dpi = 1024;
BYTE buf[dpi];
BYTE *Buffer = buf;
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
	if (X < 0 || X >= dpi || Y < 0 || Y >= dpi)
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

//定义纹理空间（u，v）
color_RGB texture_uv[1024][1024];


void texture_init()
{
	float red;
	float green;
	float blue;
	color_RGB tmp;
	for (int i = 0; i < dpi; i++)
	{
		for (int j = 0; j < dpi; j++)
		{
			if (GetDIBColor(i, j, &r, &g, &b) == 1)
			{
				red = r / 255.0;
				green = g / 255.0;
				blue = b / 255.0;
				tmp = { red, green, blue };
				texture_uv[i][j] = tmp;
			}
		}
	}

}


//-------------------------------------定义三角形（Triangle）-------------------------------------
class Triangle
{
public:
	Vector3D a;
	Vector3D b;
	Vector3D c;
	Vector3D normal_a;
	Vector3D normal_b;
	Vector3D normal_c;
	Point2D a_uv;
	Point2D b_uv;
	Point2D c_uv;

	Triangle()
	{
		a = { 0,0,0 };
		b = { 1,0,0 };
		c = { 0,1,0 };

		normal_a = { 0,0,1 };
		normal_b = { 0,0,1 };
		normal_c = { 0,0,1 };

		a_uv = { 0, 0 };
		b_uv = { 0, 1 };
		b_uv = { 1, 0 };
	}

	Triangle(Vector3D a, Vector3D b, Vector3D c, Vector3D normal1, Vector3D normal2, Vector3D normal3, Point2D a_tex, Point2D b_tex, Point2D c_tex)
	{
		this->a = a;
		this->b = b;
		this->c = c;

		this->normal_a = normal1;
		this->normal_b = normal2;
		this->normal_c = normal3;

		this->a_uv = a_tex;
		this->b_uv = b_tex;
		this->c_uv = c_tex;
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
float scale = 1.0;
Vector3D World_Transform_scale(Vector3D model_coord, float s)
{
	Vector3D res;
	res.x = model_coord.x * s;
	res.y = model_coord.y * s;
	res.z = model_coord.z * s;
	return res;

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
				   {  {(1 / tan_angle) / Aspect,          0,                      0,                  0               },
					  {0,                                 1 / tan_angle,          0,                  0               },
					  {0,                                 0,                      -f / (n - f),       f * n / (n - f) },
					  {0,                                 0,                      1,                  0               }  }
	};
	return vector_matrix(view_coord, t);
}

//(2)通过平面坐标计算(左手系)
Vector3D Project_Transform(Vector3D view_coord, float n, float f, float r, float l, float t, float b)
{
	matrix_4x4 mat{
				   {  {(2 * n) / (r - l),         0,                    -(r + l) / (r - l),          0                     },
					  {0,                         (2 * n) / (t - b),    -(t + b) / (t - b),          0                     },
					  {0,                         0,                    (f + n) / (f - n),           (2 * f * n) / (n - f) },
					  {0,                         0,                    1,                           0                     }  }
	};
	return vector_matrix(view_coord, mat);
}


float cam_pos_z = -5;
float tan_angle = 0.5;

//使用左手坐标系，初始将摄像机放在（0，0，-3）的位置，正方向为Z正轴，右轴向平行于X正轴，上轴向平行于Y正轴；
//NDC空间为标准立方体空间，xyz范围均为-1到1；
//世界坐标系下某点经过MVP变换后的坐标为：
Vector3D MVP_Transform(Vector3D p, Camera c)
{
	Vector3D v = p;

	v = World_Transform_scale(v, scale);

	v = View_Transform(v, c);
	//	return Project_Transform(v, 2.0, 4.0, 1.0, -1.0, 1.0, -1.0);
	//	cout << "摄像机位置：" << cam_pos_z << endl;
	return Project_Transform(v, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
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
	//设置显示窗口的背景颜色为白色
	glClearColor(1.0 / 255.0, 1.0 / 255.0, 127.0 / 255.0, 1.0f);

}

//-------------------------------------定义Gouraud光照模式下的光照算法----------------------------------------
//假设直射光源为白光

const int n = 10;

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
const int Width = 800;
const int Height = 800;
float zbuffer[Width][Height];
inline void Z_buffer_init()
{
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			zbuffer[i][j] = 10000.0;
		}
	}
}


float depth(Point2D p, Point2D p1, Point2D p2, Point2D p3, float ZA, float ZB, float ZC)
{
	float x = p.x;
	float i = p.y;
	/** 计算重心系数 */
	float alph = (-(x - p2.x)*(p3.y - p2.y) + (i - p2.y)*(p3.x - p2.x)) / (-(p1.x - p2.x)*(p3.y - p2.y) + (p1.y - p2.y)*(p3.x - p2.x));
	float beta = (-(x - p3.x)*(p1.y - p3.y) + (i - p3.y)*(p1.x - p3.x)) / (-(p2.x - p3.x)*(p1.y - p3.y) + (p2.y - p3.y)*(p1.x - p3.x));

	//  做了透射矫正差值
	float res = 1 / ((alph / ZA) + (beta / ZB) + ((1 - alph - beta) / ZC));

	//  没有做透射矫正差值
	//	float res = ZA * alph + ZB * beta + ZC * (1 - alph - beta);

	return res;
}





Vector3D pos0(0.0f, 0.0f, -5.0f);
Vector3D look0(0.0f, 0.0f, 1.0f);
Vector3D rig0(1.0f, 0.0f, 0.0f);
Vector3D up0(0.0f, 1.0f, 0.0f);
Camera Cam0 = { pos0, look0, rig0, up0 };
Camera camera_rotate(float xRot_c, float yRot_c, Camera cam, float cam_z)
{
	Vector3D  pos = { 0.0f, 0.0f, cam_z };
	cam.pos = Rotation_xy(pos, xRot_c, yRot_c);
	cam.look = { -cam.pos.x, -cam.pos.y, -cam.pos.z };
	Vector3D r_point0 = { 1.0, 0.0, cam_z };
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
	return cam;
}



//--------------------------------------------------绘制三角形（通过叉积判断点是否在三角形内）-----------------------------------------------------------
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



//--------------------------------------------------------------定义三角形着色算法（重心插值算法）---------------------------------------------------------
Point2D GetInterpolation(Point2D p, Point2D p1, const Point2D p2, Point2D p3, float ZA, float ZB, float ZC, Point2D a_uv, Point2D b_uv, Point2D c_uv)
{
	float x = p.x;
	float i = p.y;

	float alph = (-(x - p2.x)*(p3.y - p2.y) + (i - p2.y)*(p3.x - p2.x)) / (-(p1.x - p2.x)*(p3.y - p2.y) + (p1.y - p2.y)*(p3.x - p2.x));
	float beta = (-(x - p3.x)*(p1.y - p3.y) + (i - p3.y)*(p1.x - p3.x)) / (-(p2.x - p3.x)*(p1.y - p3.y) + (p2.y - p3.y)*(p1.x - p3.x));

	//  进行透视矫正
	float  Zt = 1 / ((alph / ZA) + (beta / ZB) + ((1 - alph - beta) / ZC));
	float  u = ((a_uv.x * alph / ZA) + (b_uv.x * beta / ZB) + (c_uv.x * (1 - alph - beta) / ZC)) * Zt;
	float  v = ((a_uv.y * alph / ZA) + (b_uv.y * beta / ZB) + (c_uv.y * (1 - alph - beta) / ZC)) * Zt;

	//  不进行透视矫正
	//	float u = a_uv.x * alph + b_uv.x * beta + c_uv.x * (1 - alph - beta);
	//	float v = a_uv.y * alph + b_uv.y * beta + c_uv.y * (1 - alph - beta);

	int pixel_x = u;
	pixel_x = min(dpi - 1, pixel_x);
	pixel_x = max(0, pixel_x);
	int pixel_y = v;
	pixel_y = min(dpi - 1, pixel_y);
	pixel_y = max(0, pixel_y);
	Point2D res{ pixel_x, pixel_y };
	return res;
}



//三角形重心插值（纹理坐标插值）
color_RGB GetInterpolationColor(Point2D p, Point2D p1, Point2D p2, Point2D p3, float ZA, float ZB, float ZC, const color_RGB c1, const color_RGB c2, const color_RGB c3)
{
	float x = p.x;
	float i = p.y;
	float alph = (-(x - p2.x)*(p3.y - p2.y) + (i - p2.y)*(p3.x - p2.x)) / (-(p1.x - p2.x)*(p3.y - p2.y) + (p1.y - p2.y)*(p3.x - p2.x));
	float beta = (-(x - p3.x)*(p1.y - p3.y) + (i - p3.y)*(p1.x - p3.x)) / (-(p2.x - p3.x)*(p1.y - p3.y) + (p2.y - p3.y)*(p1.x - p3.x));

	float  Zt = 1 / ((alph / ZA) + (beta / ZB) + ((1 - alph - beta) / ZC));
	float  pixel_r = ((c1.red * alph / ZA) + (c2.red * beta / ZB) + (c3.red * (1 - alph - beta) / ZC)) * Zt;
	float  pixel_g = ((c1.green * alph / ZA) + (c2.green * beta / ZB) + (c3.green * (1 - alph - beta) / ZC)) * Zt;
	float  pixel_b = ((c1.blue * alph / ZA) + (c2.blue * beta / ZB) + (c3.blue * (1 - alph - beta) / ZC)) * Zt;
	color_RGB res{ pixel_r, pixel_g, pixel_b };
	return res;
}


//背面剔除（剔除一个模型中背对摄像机的面，减小一半的计算量）
bool Backculling(Vector3D a, Vector3D b, Vector3D c, Camera cam)
{
	Vector3D l1 = b - a;
	Vector3D l2 = c - a;
	Vector3D normal = l1.Crossproduct(l2);
	Vector3D view = cam.pos - a;
	if (normal.DotProduct(view) > 0)
		return true;
	else
		return false;

}



//-----------------------------------------------------光照和纹理叠加渲染(Scanline扫描算法)-------------------------------------------------------------
//立方体渲染
void Triangle_display(Triangle t, float xrot, float yrot, float xrot_c, float yrot_c, float cam_z, Vector3D light)
{
	Vector3D a = Rotation_xy(t.a, xrot, yrot);
	Vector3D b = Rotation_xy(t.b, xrot, yrot);
	Vector3D c = Rotation_xy(t.c, xrot, yrot);

	Camera cam = camera_rotate(xrot_c, yrot_c, Cam0, cam_z);
	//Camera cam0 = camera_rotate(xrot_c, yrot_c, Cam0, cam_z);
	//	cout << "摄像机位置" <<cam.pos.x << "  " << cam.pos.y << "  " << cam.pos.z << endl;

	Vector3D a1 = World_Transform_scale(a, scale);
	Vector3D b1 = World_Transform_scale(b, scale);
	Vector3D c1 = World_Transform_scale(c, scale);

	Vector3D a2 = View_Transform(a1, cam);
	Vector3D b2 = View_Transform(b1, cam);
	Vector3D c2 = View_Transform(c1, cam);


	Vector3D a_t = Project_Transform(a, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D b_t = Project_Transform(b, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D c_t = Project_Transform(c, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);

	Vector3D a_s = Project_Transform(a2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D b_s = Project_Transform(b2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D c_s = Project_Transform(c2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);

	//	cout << Backculling(a_s, b_s, c_s, Cam0) << endl;
	if (Backculling(a_s, b_s, c_s, Cam0))
	{
		//背面剔除后进行绘制
		Point2D a_s_2D{ (int)round(a_s.x * 400), (int)round(a_s.y * 400) };
		Point2D b_s_2D{ (int)round(b_s.x * 400), (int)round(b_s.y * 400) };
		Point2D c_s_2D{ (int)round(c_s.x * 400), (int)round(c_s.y * 400) };


		float ka = 0.6f;
		float kd_a[3] = { 0.8f, 0.8f, 0.8f };
		float kd_b[3] = { 0.8f, 0.8f, 0.8f };
		float kd_c[3] = { 0.8f, 0.8f, 0.8f };
		float ks = 0.6f;

		Vector3D L_a = light;
		Vector3D L_b = light;
		Vector3D L_c = light;

		Vector3D N_a = Rotation_xy(t.normal_a, xrot, yrot);
		Vector3D N_b = Rotation_xy(t.normal_b, xrot, yrot);
		Vector3D N_c = Rotation_xy(t.normal_c, xrot, yrot);

		Vector3D V_a(cam.pos.x - a.x, cam.pos.y - a.y, cam.pos.z - a.z);
		Vector3D V_b(cam.pos.x - b.x, cam.pos.y - b.y, cam.pos.z - b.z);
		Vector3D V_c(cam.pos.x - c.x, cam.pos.y - c.y, cam.pos.z - c.z);

		color_RGB ca = shador(ka, kd_a, ks, L_a, N_a, V_a);
		color_RGB cb = shador(ka, kd_b, ks, L_b, N_b, V_b);
		color_RGB cc = shador(ka, kd_c, ks, L_c, N_c, V_c);

		color_RGB color[3] = { ca, cb, cc };
		float ZA = a2.z;
		float ZB = b2.z;
		float ZC = c2.z;

		//----------------------------逐点叉积法绘制三角形---------------------------------
			//确定三角形所在的矩形框
		int xmin = a_s_2D.x;
		int xmax = a_s_2D.x;
		if (b_s_2D.x < xmin) xmin = b_s_2D.x;
		if (c_s_2D.x < xmin) xmin = c_s_2D.x;
		if (b_s_2D.x > xmax) xmax = b_s_2D.x;
		if (c_s_2D.x > xmax) xmax = c_s_2D.x;
		int ymin = a_s_2D.y;
		int ymax = a_s_2D.y;
		if (b_s_2D.y < ymin) ymin = b_s_2D.y;
		if (c_s_2D.y < ymin) ymin = c_s_2D.y;
		if (b_s_2D.y > ymax) ymax = b_s_2D.y;
		if (c_s_2D.y > ymax) ymax = c_s_2D.y;

		//利用向量叉积确定是否在三角形内部
		Point2D tmp(0, 0);
		xmin = max(xmin, -400);
		xmax = min(xmax, 400);
		ymin = max(ymin, -400);
		ymax = min(ymax, 400);
		for (int i = xmin; i <= xmax; i++)
		{
			for (int j = ymin; j <= ymax; j++)
			{
				tmp.x = i;
				tmp.y = j;
				if (inTriangle(tmp, a_s_2D, b_s_2D, c_s_2D) == true)
				{
					if (depth(tmp, a_s_2D, b_s_2D, c_s_2D, ZA, ZB, ZC) < zbuffer[i + 400][j + 400])
					{
						zbuffer[i + 400][j + 400] = depth(tmp, a_s_2D, b_s_2D, c_s_2D, ZA, ZB, ZC);
						Point2D world_uv = GetInterpolation(tmp, a_s_2D, b_s_2D, c_s_2D, ZA, ZB, ZC, t.a_uv, t.b_uv, t.c_uv);
						color_RGB color_texture = texture_uv[world_uv.x][world_uv.y];
						color_RGB color_light = GetInterpolationColor(tmp, a_s_2D, b_s_2D, c_s_2D, ZA, ZB, ZC, ca, cb, cc);
						color_RGB color_res = color_texture.color_add(color_light);
						glColor3f(color_res.red, color_res.green, color_res.blue);
						glVertex3f(i / 400.0, j / 400.0, 0);
					}
				}

			}

		}

	}

	//---------------------------扫描线法绘制三角形-----------------------------------------
	/*
		Point2D p[3] = { a_s_2D, b_s_2D, c_s_2D };
		//将三角形三个点按y值大小排序
		if (p[1].y > p[0].y)
		{
			swap(t.a, t.b);
			swap(t.a_uv, t.b_uv);
			swap(p[0], p[1]);
			swap(color[0], color[1]);
			swap(ZA, ZB);
		}
		if (p[2].y > p[0].y)
		{
			swap(t.a, t.c);
			swap(t.a_uv, t.c_uv);
			swap(p[0], p[2]);
			swap(color[0], color[2]);
			swap(ZA, ZC);
		}
		if (p[1].y < p[2].y)
		{
			swap(t.b, t.c);
			swap(t.b_uv, t.c_uv);
			swap(p[1], p[2]);
			swap(color[1], color[2]);
			swap(ZB, ZC);
		}

		if (p[0].y == p[2].y)
			return;

		int px = (int)round(float(p[1].y - p[0].y) / (p[2].y - p[0].y) * (p[2].x - p[0].x) + p[0].x);
		Point2D mid(px, p[1].y);
		//绘制平底三角形
		if (p[0].y > mid.y)
		{
			int ymax = min(p[0].y, 400);
			int ymin = max(p[1].y, -400);
			for (int y = ymax; y >= ymin; y--)
			{
				int left_x = (int)round(p[0].x - (int)((p[0].y - y)*(p[0].x - p[1].x) / (p[0].y - p[1].y)));
				int righ_x = (int)round(p[0].x - (int)((p[0].y - y)*(p[0].x - mid.x) / (p[0].y - mid.y)));

				if (left_x > righ_x)   swap(left_x, righ_x);
				left_x = max(left_x, -400);
				righ_x = min(righ_x, 400);
				Point2D sreen_coo{ left_x, y };

				for (int i = left_x; i <= righ_x; ++i)
				{
					sreen_coo.x = i;
					if (depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC) < zbuffer[i + 400][y + 400])
					{
						zbuffer[i + 400][y + 400] = depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC);
						Point2D world_uv = GetInterpolation(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC, t.a_uv, t.b_uv, t.c_uv);
						color_RGB color_texture = texture_uv[world_uv.x][world_uv.y];
						color_RGB color_light = GetInterpolationColor(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC, ca, cb, cc);
						color_RGB color_res = color_texture.color_add(color_light);
						glColor3f(color_res.red, color_res.green, color_res.blue);
						glVertex3f(sreen_coo.x / 400.0, y / 400.0, 0);
					}
				}
			}
		}

		//绘制平顶三角形
		if (p[2].y < mid.y)
		{
			int ymax = min(p[1].y, 400);
			int ymin = max(p[2].y, -400);
			for (int y = ymax; y >= ymin; y--)
			{
				int left_x = (int)round(p[2].x - (int)((p[2].y - y)*(p[2].x - p[1].x) / (p[2].y - p[1].y)));
				int righ_x = (int)round(p[2].x - (int)((p[2].y - y)*(p[2].x - mid.x) / (p[2].y - mid.y)));

				if (left_x > righ_x)   swap(left_x, righ_x);
				left_x = max(left_x, -400);
				righ_x = min(righ_x, 400);
				Point2D sreen_coo{ left_x, y };
				for (int i = left_x; i <= righ_x; ++i)
				{
					sreen_coo.x = i;
					if (depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC) < zbuffer[i + 400][y + 400])
					{
						zbuffer[i + 400][y + 400] = depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC);
						Point2D world_uv = GetInterpolation(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC, t.a_uv, t.b_uv, t.c_uv);
						color_RGB color_texture = texture_uv[world_uv.x][world_uv.y];
						color_RGB color_light = GetInterpolationColor(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC, ca, cb, cc);
						color_RGB color_res = color_texture.color_add(color_light);

						glColor3f(color_res.red, color_res.green, color_res.blue);
						glVertex3f(sreen_coo.x / 400.0, y / 400.0, 0);
					}
				}
			}
		}
		*/
}


void Floor_display(Triangle t, float xrot_c, float yrot_c, float cam_z, Vector3D light)
{

	Vector3D a = t.a;
	Vector3D b = t.b;
	Vector3D c = t.c;

	Camera cam = camera_rotate(xrot_c, yrot_c, Cam0, cam_z);
	//	cout << "摄像机位置" <<cam.pos.x << "  " << cam.pos.y << "  " << cam.pos.z << endl;

	Vector3D a2 = View_Transform(a, cam);
	Vector3D b2 = View_Transform(b, cam);
	Vector3D c2 = View_Transform(c, cam);

	Vector3D a_s = Project_Transform(a2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D b_s = Project_Transform(b2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D c_s = Project_Transform(c2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);

	Point2D a_s_2D{ (int)round(a_s.x * 400), (int)round(a_s.y * 400) };
	Point2D b_s_2D{ (int)round(b_s.x * 400), (int)round(b_s.y * 400) };
	Point2D c_s_2D{ (int)round(c_s.x * 400), (int)round(c_s.y * 400) };

	float ZA = a2.z;
	float ZB = b2.z;
	float ZC = c2.z;

	Point2D p[3] = { a_s_2D, b_s_2D, c_s_2D };
	//将三角形三个点按y值大小排序
	if (p[1].y > p[0].y)
	{
		swap(t.a, t.b);
		swap(t.a_uv, t.b_uv);
		swap(p[0], p[1]);
		//		swap(color[0], color[1]);
		swap(ZA, ZB);
	}
	if (p[2].y > p[0].y)
	{
		swap(t.a, t.c);
		swap(t.a_uv, t.c_uv);
		swap(p[0], p[2]);
		//		swap(color[0], color[2]);
		swap(ZA, ZC);
	}
	if (p[1].y < p[2].y)
	{
		swap(t.b, t.c);
		swap(t.b_uv, t.c_uv);
		swap(p[1], p[2]);
		//		swap(color[1], color[2]);
		swap(ZB, ZC);
	}

	if (p[0].y == p[2].y)
		return;

	int px = (int)round(float(p[1].y - p[0].y) / (p[2].y - p[0].y) * (p[2].x - p[0].x) + p[0].x);
	Point2D mid(px, p[1].y);
	//绘制平底三角形
	if (p[0].y > mid.y)
	{
		int ymax = min(p[0].y, 400);
		int ymin = max(p[1].y, -400);
		for (int y = ymax; y >= ymin; y--)
		{
			int left_x = (int)round(p[0].x - (int)((p[0].y - y)*(p[0].x - p[1].x) / (p[0].y - p[1].y)));
			int righ_x = (int)round(p[0].x - (int)((p[0].y - y)*(p[0].x - mid.x) / (p[0].y - mid.y)));

			if (left_x > righ_x)   swap(left_x, righ_x);
			left_x = max(left_x, -400);
			righ_x = min(righ_x, 400);
			Point2D sreen_coo{ left_x, y };

			for (int i = left_x; i <= righ_x + 1; ++i)
			{
				sreen_coo.x = i;
				if (zbuffer[i + 400][y + 400] - depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC) > 0.001)
				{
					zbuffer[i + 400][y + 400] = depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC);

					glColor3f(0.6f, 0.6f, 0.6f);
					glVertex3f(sreen_coo.x / 400.0, y / 400.0, 0);
				}
			}
		}
	}

	//绘制平顶三角形
	if (p[2].y < mid.y)
	{
		int ymax = min(p[1].y, 400);
		int ymin = max(p[2].y, -400);
		for (int y = ymax; y >= ymin; y--)
		{
			int left_x = (int)round(p[2].x - (int)((p[2].y - y)*(p[2].x - p[1].x) / (p[2].y - p[1].y)));
			int righ_x = (int)round(p[2].x - (int)((p[2].y - y)*(p[2].x - mid.x) / (p[2].y - mid.y)));

			if (left_x > righ_x)   swap(left_x, righ_x);
			left_x = max(left_x, -400);
			righ_x = min(righ_x, 400);
			Point2D sreen_coo{ left_x, y };
			for (int i = left_x; i <= righ_x + 1; ++i)
			{
				sreen_coo.x = i;
				if (depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC) < zbuffer[i + 400][y + 400])
				{
					zbuffer[i + 400][y + 400] = depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC);

					glColor3f(0.6f, 0.6f, 0.6f);
					glVertex3f(sreen_coo.x / 400.0, y / 400.0, 0);
				}
			}
		}
	}
}



void Shadow_display(Triangle t, float xrot, float yrot, float xrot_c, float yrot_c, float cam_z, Vector3D light)
{

	Camera cam = camera_rotate(xrot_c, yrot_c, Cam0, cam_z);
	//	cout << "摄像机位置" <<cam.pos.x << "  " << cam.pos.y << "  " << cam.pos.z << endl;

	Vector3D a = Rotation_xy(t.a, xrot, yrot);
	Vector3D b = Rotation_xy(t.b, xrot, yrot);
	Vector3D c = Rotation_xy(t.c, xrot, yrot);

	Vector3D a1 = World_Transform_scale(a, scale);
	Vector3D b1 = World_Transform_scale(b, scale);
	Vector3D c1 = World_Transform_scale(c, scale);

	a1.y = -1.99;
	b1.y = -1.99;
	c1.y = -1.99;

	Vector3D a2 = View_Transform(a1, cam);
	Vector3D b2 = View_Transform(b1, cam);
	Vector3D c2 = View_Transform(c1, cam);

	Vector3D a_s = Project_Transform(a2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D b_s = Project_Transform(b2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);
	Vector3D c_s = Project_Transform(c2, -1.0 - cam_pos_z, 1.0 - cam_pos_z, tan_angle, 1.0);

	Point2D a_s_2D{ (int)round(a_s.x * 400), (int)round(a_s.y * 400) };
	Point2D b_s_2D{ (int)round(b_s.x * 400), (int)round(b_s.y * 400) };
	Point2D c_s_2D{ (int)round(c_s.x * 400), (int)round(c_s.y * 400) };

	float ZA = a2.z;
	float ZB = b2.z;
	float ZC = c2.z;

	Point2D p[3] = { a_s_2D, b_s_2D, c_s_2D };
	//将三角形三个点按y值大小排序
	if (p[1].y > p[0].y)
	{
		swap(t.a, t.b);
		swap(t.a_uv, t.b_uv);
		swap(p[0], p[1]);
		//		swap(color[0], color[1]);
		swap(ZA, ZB);
	}
	if (p[2].y > p[0].y)
	{
		swap(t.a, t.c);
		swap(t.a_uv, t.c_uv);
		swap(p[0], p[2]);
		//		swap(color[0], color[2]);
		swap(ZA, ZC);
	}
	if (p[1].y < p[2].y)
	{
		swap(t.b, t.c);
		swap(t.b_uv, t.c_uv);
		swap(p[1], p[2]);
		//		swap(color[1], color[2]);
		swap(ZB, ZC);
	}

	if (p[0].y == p[2].y)
		return;

	int px = (int)round(float(p[1].y - p[0].y) / (p[2].y - p[0].y) * (p[2].x - p[0].x) + p[0].x);
	Point2D mid(px, p[1].y);
	//绘制平底三角形
	if (p[0].y > mid.y)
	{
		int ymax = min(p[0].y, 400);
		int ymin = max(p[1].y, -400);
		for (int y = ymax; y >= ymin; y--)
		{
			int left_x = (int)round(p[0].x - (int)((p[0].y - y)*(p[0].x - p[1].x) / (p[0].y - p[1].y)));
			int righ_x = (int)round(p[0].x - (int)((p[0].y - y)*(p[0].x - mid.x) / (p[0].y - mid.y)));

			if (left_x > righ_x)   swap(left_x, righ_x);
			left_x = max(left_x, -400);
			righ_x = min(righ_x, 400);
			Point2D sreen_coo{ left_x, y };

			for (int i = left_x; i <= righ_x; ++i)
			{
				sreen_coo.x = i;
				if (depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC) < zbuffer[i + 400][y + 400])
				{
					zbuffer[i + 400][y + 400] = depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC);

					glColor3f(0.1f, 0.1f, 0.1f);
					glVertex3f(sreen_coo.x / 400.0, y / 400.0, 0);
				}
			}
		}
	}

	//绘制平顶三角形
	if (p[2].y < mid.y)
	{
		int ymax = min(p[1].y, 400);
		int ymin = max(p[2].y, -400);
		for (int y = ymax; y >= ymin; y--)
		{
			int left_x = (int)round(p[2].x - (int)((p[2].y - y)*(p[2].x - p[1].x) / (p[2].y - p[1].y)));
			int righ_x = (int)round(p[2].x - (int)((p[2].y - y)*(p[2].x - mid.x) / (p[2].y - mid.y)));

			if (left_x > righ_x)   swap(left_x, righ_x);
			left_x = max(left_x, -400);
			righ_x = min(righ_x, 400);
			Point2D sreen_coo{ left_x, y };
			for (int i = left_x; i <= righ_x; ++i)
			{
				sreen_coo.x = i;
				if (depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC) < zbuffer[i + 400][y + 400])
				{
					zbuffer[i + 400][y + 400] = depth(sreen_coo, p[0], p[1], p[2], ZA, ZB, ZC);

					glColor3f(0.1f, 0.1f, 0.1f);
					glVertex3f(sreen_coo.x / 400.0, y / 400.0, 0);
				}
			}
		}
	}

}

