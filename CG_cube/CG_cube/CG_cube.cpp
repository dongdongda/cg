// CG_cube.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

#include "function.h"
#include<math.h>
#include<gl/glut.h>
#include <GL/freeglut_ext.h>
#include<GL\freeglut.h>


//贴图分辨率尺寸
int width = 512;
int floor_width = 512;

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

//计算帧数
int frame = 0, time, timebase = 0;



void display_new()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	int fps = 0;
	if (time - timebase > 1000) {
		fps = frame * 1000.0 / (time - timebase);
		cout <<"帧率：" << fps << endl;
		timebase = time;
		frame = 0;
	}



	glPointSize(1.0f);
	glBegin(GL_POINTS);
	Triangle_display(top1, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(top2, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(bottom1, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(bottom2, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(left1, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(left2, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(right1, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(right2, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(front1, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(front2, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(back1, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);
	Triangle_display(back2, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light, width);

	Floor_display(floor1, xRot_c, yRot_c, cam_pos_z, light, floor_width);
	Floor_display(floor2, xRot_c, yRot_c, cam_pos_z, light, floor_width);

	zbuffer_light_update(light, xRot, yRot);
    shadow(xRot_c, yRot_c, cam_pos_z);

	glEnd();
	glPopMatrix();


	// 刷新绘图命令
	glutSwapBuffers();
	glFinish();

	Z_buffer_init();
	zbuffer_light_init();
}



/*
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
*/


//--------------------------------------控制键盘切换视角----------------------------------------------
void SpecialKeys(unsigned char key, int x, int y)
{
//	if (key == GLUT_KEY_UP && pos0.y <= 0.4f)
//		pos0.y += 0.1f;

	switch (key) {
	case GLUT_KEY_F1:
	    break;
	case GLUT_KEY_UP:
	    break;
	case GLUT_KEY_DOWN:
	    break;
	default:
		break;
	}
	glutPostRedisplay();
}
	



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

//		std::cout << "yRot:" << yRot << "\txRot" << xRot << std::endl;
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

//		std::cout << "yRot_c:" << yRot_c << "\txRot_c" << xRot_c << std::endl;
		glutPostRedisplay();
	}
}

//鼠标滚轮
void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		// 放大
		if (scale < 1.0)
			scale *= 1.1;
	}
	else
	{
		// 缩小
		if (scale > 0.1)
			scale /= 1.1;
	}
	//	std::cout << "缩放倍数:" << scale << std::endl;
	glutPostRedisplay();
}




int main(int argc, char *argv[])
{
	Z_buffer_init();
	zbuffer_light_init();

	char szfilename[255] = "d:\\container.bmp";
	if (ReadBmp(szfilename) == 0)
	{
		printf("failure to read file %s", szfilename);
	}
	printf("Width: %ld\n", bih.biWidth);
	printf("Height: %ld\n", bih.biHeight);
	printf("BitCount: %d\n\n", (int)bih.biBitCount);
	texture_init();

	char floor[255] = "d:\\w_floor.bmp";
	if (ReadBmp(floor) == 0)
	{
		printf("failure to read file %s", szfilename);
	}
	printf("Width: %ld\n", bih.biWidth);
	printf("Height: %ld\n", bih.biHeight);
	printf("BitCount: %d\n\n", (int)bih.biBitCount);
	texture_floor_init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(560, 140);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("OpenGL test");
	init();
	glutKeyboardFunc(SpecialKeys);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutDisplayFunc(display_new);
	glutMainLoop();

	free(Buffer);
	return 0;


}

