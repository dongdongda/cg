// Texture_get.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>



#include<gl/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
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
BYTE *Buffer = NULL;
long LineByteWidth;

BYTE r, g, b;

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


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	glPointSize(1.0f);
	glBegin(GL_POINTS);

	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			if (GetDIBColor(i, j, &r, &g, &b) == 1)
			{
				float red = r / 255.0;
				float green = g / 255.0;
				float blue = b / 255.0;
				glColor3f(red, green, blue);
				glVertex3f((i -256) / 512.0, (j - 256) / 512.0, 0);

			}
			
		}
	}


	glEnd();

	glPopMatrix();


	// 刷新绘图命令
	glutSwapBuffers();
	glFinish();
}


int main(int argc, char *argv[])
{
	int x, y;
	int n;
	char szfilename[255] = "d:\\stones.bmp";
	if (ReadBmp(szfilename) == 0)
	{
		printf("failure to read file %s", szfilename);
		return 1;
	}
	printf("Width: %ld\n", bih.biWidth);
	printf("Height: %ld\n", bih.biHeight);
	printf("BitCount: %d\n\n", (int)bih.biBitCount);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(560, 140);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("OpenGL test");
	init();
	glutDisplayFunc(display);
	glutMainLoop();

/*
	while (1)
	{
		printf("input the X:");
		scanf("%d", &x);
		if (x < 0)
			break;
		printf("input the Y:");
		scanf("%d", &y);
		if (GetDIBColor(x, y, &r, &g, &b) == 1)
			printf("(%d, %d): r:%d, g:%d, b:%d\n", x, y, (int)r, (int)g, (int)b);
		else
			printf("input error.\n");
	}

*/
	free(Buffer);
	return 0;
}


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
int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b)
{
	int dpixeladd;
	BYTE *ptr;
	if (X < 0 || X >= bih.biWidth || Y < 0 || Y >= bih.biHeight)
	{
		return 0;
	}
	dpixeladd = bih.biBitCount / 8;
	ptr = Buffer + X * dpixeladd + Y * LineByteWidth;
	*b = *ptr;
	*g = *(ptr + 1);
	*r = *(ptr + 2);
	return 1;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
