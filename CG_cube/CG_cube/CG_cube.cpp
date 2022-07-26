// CG_cube.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

#include "function.h"
#include<math.h>
#include<gl/glut.h>
#include <GL/freeglut_ext.h>




//鼠标旋转角度设置
bool mouseLeftDown = false;
bool mouseRightDown = false;

float xdiff = 0.0f;
float ydiff = 0.0f;
float xdiff_c = 0.0f;
float ydiff_c = 0.0f;

//当鼠标按键按下时记录鼠标坐标
float oldx;  
float oldy;


//------------------------定义一个立方体-----------------------------
Vector3D a_0(-0.5f, -0.5f, -0.5f);
Vector3D b_0(0.5f, -0.5f, -0.5f);
Vector3D c_0(0.5f, -0.5f, 0.5f);
Vector3D d_0(-0.5f, -0.5f, 0.5f);
Vector3D e_0(-0.5f, 0.5f, -0.5f);
Vector3D f_0(0.5f, 0.5f, -0.5f);
Vector3D g_0(0.5f, 0.5f, 0.5f);
Vector3D h_0(-0.5f, 0.5f, 0.5f);
Vector3D a0(-0.5f, -0.5f, -0.5f);
Vector3D b0(0.5f, -0.5f, -0.5f);
Vector3D c0(0.5f, -0.5f, 0.5f);
Vector3D d0(-0.5f, -0.5f, 0.5f);
Vector3D e0(-0.5f, 0.5f, -0.5f);
Vector3D f0(0.5f, 0.5f, -0.5f);
Vector3D g0(0.5f, 0.5f, 0.5f);
Vector3D h0(-0.5f, 0.5f, 0.5f);


color_RGB ca{ 1.0f, 1.0f, 1.0f };
color_RGB cb{ 0.0f, 1.0f, 0.0f };
color_RGB cc{ 0.0f, 0.0f, 1.0f };
color_RGB cd{ 1.0f, 0.0f, 0.0f };
color_RGB ce{ 0.0f, 1.0f, 1.0f };
color_RGB cf{ 1.0f, 0.0f, 1.0f };
color_RGB cg{ 1.0f, 1.0f, 1.0f };
color_RGB ch{ 1.0f, 1.0f, 0.0f };

Vector3D L(0.0f, 1.0f, 0.0f);
Vector3D N_a(-1.0f, 1.0f, -1.0f);
Vector3D N_b(1.0f, 1.0f, -1.0f);
Vector3D N_c(0.0f, 1.0f, -1.0f);
Vector3D N_d(0.0f, 1.0f, -1.0f);
Vector3D V_a(1.0f, 0.0f, -3.0f);
Vector3D V_b(-1.0f, 0.0f, -3.0f);
Vector3D V_c(0.0f, -1.0f, -3.0f);
Vector3D V_d(0.0f, 0.0f, -2.0f);
float ka = 0.1f;
float kd_a[3] = { 0.2f, 0.2f, 0.2f };
float kd_b[3] = { 0.2f, 0.2f, 0.2f };
float kd_c[3] = { 0.2f, 0.2f, 0.2f };
float kd_d[3] = { 0.2f, 0.2f, 0.2f };
float ks = 0.8f;

/*
color_RGB ca = shador(ka, kd_a, ks, L, N_a, V_a);
color_RGB cb = shador(ka, kd_b, ks, L, N_b, V_b);
color_RGB cc = shador(ka, kd_c, ks, L, N_c, V_c);
color_RGB cd = shador(ka, kd_d, ks, L, N_d, V_d);
*/



void display()
{

	glClear(GL_COLOR_BUFFER_BIT);
//	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();

	cout << xRot << " " << yRot << endl;
	cout << cos(xRot) << " " << sin(xRot) << endl;
	a0 = Rotation_xy(a_0, xRot, yRot);
	b0 = Rotation_xy(b_0, xRot, yRot);
	c0 = Rotation_xy(c_0, xRot, yRot);
	d0 = Rotation_xy(d_0, xRot, yRot);
	e0 = Rotation_xy(e_0, xRot, yRot);
	f0 = Rotation_xy(f_0, xRot, yRot);
	g0 = Rotation_xy(g_0, xRot, yRot);
	h0 = Rotation_xy(h_0, xRot, yRot);

//	cout << a0.x << " " << a0.y << " " << a0.z << endl;
//	cout << b0.x << " " << b0.y << " " << b0.z << endl;
//	cout << c0.x << " " << c0.y << " " << c0.z << endl;
//	cout << d0.x << " " << d0.y << " " << d0.z << endl;
//	glRotatef(yrot, 0.0f, 1.0f, 0.0f);

	Vector3D a_s = MVP_Transform(a0, cam);
	Vector3D b_s = MVP_Transform(b0, cam);
	Vector3D c_s = MVP_Transform(c0, cam);
	Vector3D d_s = MVP_Transform(d0, cam);
	Vector3D e_s = MVP_Transform(e0, cam);
	Vector3D f_s = MVP_Transform(f0, cam);
	Vector3D g_s = MVP_Transform(g0, cam);
	Vector3D h_s = MVP_Transform(h0, cam);

	Point2D a_s_2D{ (int)round(a_s.x * 400), (int)round(a_s.y * 400) };
	Point2D b_s_2D{ (int)round(b_s.x * 400), (int)round(b_s.y * 400) };
	Point2D c_s_2D{ (int)round(c_s.x * 400), (int)round(c_s.y * 400) };
	Point2D d_s_2D{ (int)round(d_s.x * 400), (int)round(d_s.y * 400) };
	Point2D e_s_2D{ (int)round(e_s.x * 400), (int)round(e_s.y * 400) };
	Point2D f_s_2D{ (int)round(f_s.x * 400), (int)round(f_s.y * 400) };
	Point2D g_s_2D{ (int)round(g_s.x * 400), (int)round(g_s.y * 400) };
	Point2D h_s_2D{ (int)round(h_s.x * 400), (int)round(h_s.y * 400) };


	glPointSize(1.0f);
	glBegin(GL_POINTS);
	drawTriangle(a_s_2D, b_s_2D, c_s_2D, ca, cb, cc, a_s.z, b_s.z, c_s.z);
	drawTriangle(a_s_2D, c_s_2D, d_s_2D, ca, cc, cd, a_s.z, c_s.z, d_s.z);
	drawTriangle(a_s_2D, b_s_2D, f_s_2D, ca, cb, cf, a_s.z, b_s.z, f_s.z);
	drawTriangle(a_s_2D, e_s_2D, f_s_2D, ca, ce, cf, a_s.z, e_s.z, f_s.z);
	drawTriangle(g_s_2D, e_s_2D, f_s_2D, cg, ce, cf, g_s.z, e_s.z, f_s.z);
	drawTriangle(g_s_2D, e_s_2D, h_s_2D, cg, ce, ch, g_s.z, e_s.z, h_s.z);
	drawTriangle(g_s_2D, d_s_2D, h_s_2D, cg, cd, ch, g_s.z, d_s.z, h_s.z);
	drawTriangle(g_s_2D, d_s_2D, c_s_2D, cg, cd, cc, g_s.z, d_s.z, c_s.z);
	drawTriangle(b_s_2D, f_s_2D, c_s_2D, cb, cf, cc, b_s.z, f_s.z, c_s.z);
	drawTriangle(g_s_2D, f_s_2D, c_s_2D, cg, cf, cc, g_s.z, f_s.z, c_s.z);
	drawTriangle(a_s_2D, e_s_2D, d_s_2D, ca, ce, cd, a_s.z, e_s.z, d_s.z);
	drawTriangle(h_s_2D, e_s_2D, d_s_2D, ch, ce, cd, h_s.z, e_s.z, d_s.z);

//	DDALine(a_s.x, a_s.y, ca, b_s.x, b_s.y, cb);
//	DDALine(a_s.x, a_s.y, ca, c_s.x, c_s.y, cc);
//	DDALine(b_s.x, b_s.y, cb, c_s.x, c_s.y, cc);
	glEnd();

	glPopMatrix();

	/*
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	Draw_Triangle(a_s.x, a_s.y, b_s.x, b_s.y, c_s.x, c_s.y);
	Draw_Triangle(a_s.x, a_s.y, b_s.x, b_s.y, d_s.x, d_s.y);
	Draw_Triangle(a_s.x, a_s.y, c_s.x, c_s.y, d_s.x, d_s.y);
	Draw_Triangle(b_s.x, b_s.y, c_s.x, c_s.y, d_s.x, d_s.y);
	glEnd();
	*/


// 刷新绘图命令
	glutSwapBuffers();
//	glFinish();

	Z_buffer_init();
}

Triangle top1({ 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, 0.5f });
Triangle top2({ -0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, 0.5f });
Triangle bottom1({ 0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, 0.5f });
Triangle bottom2({ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, 0.5f });
Triangle left1({ -0.5f, 0.5f, 0.5f }, { -0.5f, 0.5f, -0.5f }, { -0.5f, -0.5f, 0.5f });
Triangle left2({ -0.5f, -0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f }, { -0.5f, -0.5f, 0.5f });
Triangle right1({ 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, -0.5f }, { 0.5f, -0.5f, 0.5f });
Triangle right2({ 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, { 0.5f, -0.5f, 0.5f });
Triangle front1({ 0.5f, 0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f });
Triangle front2({ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f });
Triangle back1({ 0.5f, 0.5f, 0.5f }, { 0.5f, -0.5f, 0.5f }, { -0.5f, 0.5f, 0.5f });
Triangle back2({ -0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, 0.5f }, { -0.5f, 0.5f, 0.5f });

Vector3D light = { 0.0f, 0.0f, -1.0f };

void display_new()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();


	glPointSize(1.0f);
	glBegin(GL_POINTS);
	Triangle_display(top1, xRot, yRot, cam, light);
	Triangle_display(top2, xRot, yRot, cam, light);
	Triangle_display(bottom1, xRot, yRot, cam, light);
	Triangle_display(bottom2, xRot, yRot, cam, light);
	Triangle_display(left1, xRot, yRot, cam, light);
	Triangle_display(left2, xRot, yRot, cam, light);
	Triangle_display(right1, xRot, yRot, cam, light);
	Triangle_display(right2, xRot, yRot, cam, light);
	Triangle_display(front1, xRot, yRot, cam, light);
	Triangle_display(front2, xRot, yRot, cam, light);
	Triangle_display(back1, xRot, yRot, cam, light);
	Triangle_display(back2, xRot, yRot, cam, light);
	glEnd();


	glPopMatrix();


	// 刷新绘图命令
	glutSwapBuffers();
	glFinish();

	Z_buffer_init();
}


//直线裁剪
void display_clip()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();

	glBegin(GL_LINES);
//  LiangBarsky(200, 200, -200, -200, -240, 240, 240, -240);
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
	glFinish();
}


/*
//--------------------------------------控制键盘切换视角----------------------------------------------
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP && cam.pos.y <= 0.4f)
		cam.pos.y += 0.1f;

	if (key == GLUT_KEY_DOWN && cam.pos.y >= -0.5f)
		cam.pos.y -= 0.1f;

	if (key == GLUT_KEY_LEFT && cam.pos.x >= -0.4f)
		cam.pos.x -= 0.1f;

	if (key == GLUT_KEY_RIGHT && cam.pos.x <= 0.4f)
		cam.pos.x += 0.1f;

	cout << "坐标" << cam.pos.x << "   " << cam.pos.y << endl;
	/*
		if (xRot > 360.0f)
			xRot -= 360.0f;

		if (xRot < -360.0f)
			xRot += 360.0f;

		if (yRot > 360.0f)
			yRot -= 360.0f;

		if (yRot < -360.0f)
			yRot += 360.0f;
	
	// 刷新窗口
	glutPostRedisplay();
}
*/


//--------------------------------------控制鼠标切换视角----------------------------------------------

// 鼠标事件
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseLeftDown = true;
		oldx = x;
		oldy = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		mouseRightDown = true;
		oldx = x;
		oldy = y;
	}
	else
	{
		mouseLeftDown = false;
		mouseRightDown = false;
	}

}

// 鼠标移动事件
void mouseMotion(int x, int y) {
	if (mouseLeftDown) {
		yRot -= 0.3 * (x - oldx);
		xRot -= 0.3 * (y - oldy);

		if (xRot > 360.0f)
			xRot -= 360.0f;

		if (xRot < -360.0f)
			xRot += 360.0f;

		if (yRot > 360.0f)
			yRot -= 360.0f;

		if (yRot < -360.0f)
			yRot += 360.0f;

		std::cout << "yRot:" << yRot << "\txRot" << xRot << std::endl;
		glutPostRedisplay();
	}

	if (mouseRightDown) {
		yRot_c -= 0.3 * (x - oldx);
		xRot_c -= 0.3 * (y - oldy);

		if (xRot_c > 360.0f)
			xRot_c -= 360.0f;

		if (xRot_c < -360.0f)
			xRot_c += 360.0f;

		if (yRot_c > 360.0f)
			yRot_c -= 360.0f;

		if (yRot_c < -360.0f)
			yRot_c += 360.0f;

		std::cout << "yRot_c:" << yRot << "\txRot_c" << xRot << std::endl;
		glutPostRedisplay();
	}
}

//鼠标滚轮
void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		// 放大
		cam_pos_z += 0.1;
	}
	else
	{
		// 缩小
		cam_pos_z -= 0.1;
	}
	glutPostRedisplay();
}




int main(int argc, char *argv[])
{
	Z_buffer_init();

	char szfilename[255] = "d:\\stones.bmp";
	if (ReadBmp(szfilename) == 0)
	{
		printf("failure to read file %s", szfilename);
	}
	printf("Width: %ld\n", bih.biWidth);
	printf("Height: %ld\n", bih.biHeight);
	printf("BitCount: %d\n\n", (int)bih.biBitCount);

	texture_init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(560, 140);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("OpenGL test");
	init();
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutDisplayFunc(display_new);
	glutMainLoop();

	free(Buffer);
	return 0;


}

