#include<GL/glut.h>
#include<math.h>
#include<iostream>
#define STEP 0.5f
using namespace std;

// 旋转初始的角度
GLfloat angle = 0.0f;
// 设置旋转轴：两个三维的点确定的旋转轴
GLfloat axis[][3] = {
	0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

int windowWidth = 500;
int windowHeight = 500;

void init() {
	glEnable(GL_DEPTH_TEST); //开启深度
	
	glFrontFace(GL_CCW);
	// 不绘制背面
	glCullFace(GL_BACK);

	glPolygonMode(GL_BACK, GL_LINE); // 设置反面为边缘绘制模式
	//glEnable(GL_CULL_FACE);
	// 背景颜色
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void triangle()
{
	// 设置三角形顶点坐标
	static const GLfloat vertex1[][3] = {
		0.0f, 0.0f, 1.0f,
		-1.0f,  0.0f, 0.0f,
		1.0f,  0.0f, -1.0f
	};
	// 顶点颜色
	static const GLfloat colors1[][3] = {
		0.0, 0.0, 0.0,
		0.0, 1.0, 1.0
	};
	// 绘制面
	static const GLint index1[][3] = {
		2, 1, 0,
		2, 0, 1
	};
	glBegin(GL_TRIANGLES);
		for (unsigned int j = 0; j < 3; j++) {
			glColor3fv(colors1[0]);
			glVertex3fv(vertex1[index1[0][j]]);
		}
	glEnd();
	glBegin(GL_TRIANGLES);
	for (unsigned int j = 0; j < 3; j++) {
		glColor3fv(colors1[1]);
		glVertex3fv(vertex1[index1[1][j]]);
	}
	glEnd();
}

void color_cube()
{
	// 设置八个顶点坐标
	static const GLfloat vertex[][3] = {
		-1.0f, -1.0f, -1.0f,
		1.0f,  -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f,  -1.0f, 1.0f,
		1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f
	};
	// 设置8个顶点的颜色
	static const GLfloat colors[][3] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
	};
	// 绘制六个面时顶点的顺序
	static const GLint index[][4] = {
		//3, 2, 1, 0,
		//4, 5, 6, 7,
		0, 4, 7, 3,
		6, 5, 1, 2,
		0, 1, 5, 4,
		7, 6, 2, 3
	};

	// 绘制立方体的六个面
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			glColor3fv(colors[index[i][j]]);
			glVertex3fv(vertex[index[i][j]]);
		}
	}
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 加载单位阵
	glLoadIdentity();
	// 设置相机的位置和视角
	gluLookAt(3, 3, 3, 0.0, 0.0, 0.0, 0, 0, 1);
	// 旋转
	glTranslatef(axis[0][0], axis[0][1], axis[0][2]);
	glRotatef(angle, axis[1][0] - axis[0][0], axis[1][1] - axis[0][1], axis[1][2] - axis[0][2]);
	color_cube();
	triangle();
	glutSwapBuffers();
}


void timer_function(GLint value)
{
	angle += STEP;
	if (angle > 360.0) angle -= 360.0;
	glutPostRedisplay();
	glutTimerFunc(50, timer_function, 0);
}

void centerWindow()
{
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	int posX = (screenWidth - windowWidth) / 2;
	int posY = (screenHeight - windowHeight) / 2;

	glutInitWindowPosition(posX, posY);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3, 3, 3, 0.0, 0.0, 0.0, 0, 0, 1);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	centerWindow();
	glutCreateWindow("color_cube");

	// 绘制
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(500, timer_function, 0);

	// 进入主循环
	glutMainLoop();
	return 0;
}