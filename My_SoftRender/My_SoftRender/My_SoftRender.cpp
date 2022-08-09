// My_SoftRender.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "readObj.h"
#include "softRender.h"

//存放obj文件顶点信息的数组
std::vector<glm::vec3> points;
std::vector<glm::vec2> texcoords;
std::vector<glm::vec3> normals;


//是否已经载入obj及BMP文件信息
bool is_import = false;

//鼠标旋转角度设置
bool mouseLeftDown = false;
bool mouseRightDown = false;


//当鼠标按键按下时记录鼠标坐标
float oldx;
float oldy;

//计算帧数
int frame = 0, time, timebase = 0;

//定义光照方向
Vector3D light = { 0, 1, 0 };
bool shadow_look = false;

//------------------------定义一个地面-----------------------------
Triangle floor1({ 2.0f, -2.0f, 2.0f }, { -2.0f, -2.0f, -2.0f }, { -2.0f, -2.0f, 2.0f }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 1, 1 }, { 0, 0 }, { 1, 0 });
Triangle floor2({ 2.0f, -2.0f, 2.0f }, { -2.0f, -2.0f, -2.0f }, { 2.0f, -2.0f, -2.0f }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 1, 1 }, { 0, 0 }, { 0, 1 });

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	int fps = 0;
	if (time - timebase > 1000) {
		fps = frame * 1000.0 / (time - timebase);
		cout << "帧率：" << fps << endl;
		timebase = time;
		frame = 0;
	}

	glPointSize(1.0f);
	glBegin(GL_POINTS);



	Camera cam = camera_rotate(xRot_c, yRot_c, Cam0, cam_pos_z);
	shadow_look = false;
	//	cout << cam.look.y << endl;
	if (cam.look.y < 0.4)   shadow_look = true;
	for (int i = 0; i < points.size() / 3; i++)
	{
		int index = i * 3;
		Vector3D a = { points[index].x, points[index].y, points[index].z };
		Vector3D b = { points[index + 1].x, points[index + 1].y, points[index + 1].z };
		Vector3D c = { points[index + 2].x, points[index + 2].y, points[index + 2].z };
		Point2D a_uv = { (int)round(texcoords[index].x * dpi), (int)round(texcoords[index].y * dpi) };
		Point2D b_uv = { (int)round(texcoords[index + 1].x * dpi), (int)round(texcoords[index + 1].y * dpi) };
		Point2D c_uv = { (int)round(texcoords[index + 2].x * dpi), (int)round(texcoords[index + 2].y * dpi) };
		Vector3D a_normal = { normals[index].x, normals[index].y, normals[index].z };
		Vector3D b_normal = { normals[index + 1].x, normals[index + 1].y, normals[index + 1].z };
		Vector3D c_normal = { normals[index + 2].x, normals[index + 2].y, normals[index + 2].z };


		Triangle tri(a, b, c, a_normal, b_normal, c_normal, a_uv, b_uv, c_uv);
		Triangle_display(tri, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light);

		if (shadow_look == true)
			Shadow_display(tri, xRot, yRot, xRot_c, yRot_c, cam_pos_z, light);
	}
	Floor_display(floor1, xRot_c, yRot_c, cam_pos_z, light);
	Floor_display(floor2, xRot_c, yRot_c, cam_pos_z, light);


	glEnd();
	glPopMatrix();


	// 刷新绘图命令
	glutSwapBuffers();
	glFinish();

	Z_buffer_init();
	//	zbuffer_light_init();
}

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
		yRot += 0.2 * (x - oldx);
		xRot += 0.2 * (y - oldy);

		if (xRot > 360.0f)
			xRot -= 360.0f;

		if (xRot < -360.0f)
			xRot += 360.0f;

		if (yRot > 360.0f)
			yRot -= 360.0f;

		if (yRot < -360.0f)
			yRot += 360.0f;

		//		std::cout << "yRot:" << yRot << "\txRot" << xRot << std::endl;
		oldx = x;
		oldy = y;
		glutPostRedisplay();
	}

	if (mouseRightDown) {
		yRot_c -= 0.2 * (x - oldx);
		xRot_c -= 0.2 * (y - oldy);

		if (xRot_c > 360.0f)
			xRot_c -= 360.0f;

		if (xRot_c < -360.0f)
			xRot_c += 360.0f;

		if (yRot_c > 360.0f)
			yRot_c -= 360.0f;

		if (yRot_c < -360.0f)
			yRot_c += 360.0f;

		//		std::cout << "yRot_c:" << yRot_c << "\txRot_c" << xRot_c << std::endl;
		oldx = x;
		oldy = y;
		glutPostRedisplay();
	}
}

//鼠标滚轮
void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		// 放大
		if (scale < 4)
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
	if (!is_import)
	{
		readObj("models/res/diablo3_pose.obj", points, texcoords, normals);

		char szfilename[255] = "models/res/diablo3_pose_diffuse.bmp";
		if (ReadBmp(szfilename) == 0)
		{
			printf("failure to read file %s", szfilename);
		}
		printf("Width: %ld\n", bih.biWidth);
		printf("Height: %ld\n", bih.biHeight);
		printf("BitCount: %d\n\n", (int)bih.biBitCount);
		texture_init();

		is_import = true;
	}

	Z_buffer_init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(560, 140);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("SoftRender Opengl");
	init();
	glutKeyboardFunc(SpecialKeys);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutDisplayFunc(display);
	glutMainLoop();

	free(Buffer);
	return 0;
	/*
		std::cout << points.size() << "    " << texcoords.size() << "       " << normals.size() << endl;
		std::cout << points[0].x << "    " << points[0].y << "    " << points[0].z << endl;
		std::cout << points[1].x << "    " << points[1].y << "    " << points[1].z << endl;
		std::cout << points[2].x << "    " << points[2].y << "    " << points[2].z << endl;
		std::cout << texcoords[0].x << "    " << texcoords[0].y << "    " << endl;
		std::cout << texcoords[1].x << "    " << texcoords[1].y << "    " << endl;
		std::cout << texcoords[2].x << "    " << texcoords[2].y << "    " << endl;
		system("pause");
	*/
}