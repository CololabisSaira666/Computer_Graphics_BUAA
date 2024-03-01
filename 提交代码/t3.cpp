#include<GL/glut.h>
#include<math.h>
#define STEP 0.5f

GLfloat v[][3] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.942809f, -0.333333f,
	-0.816497f, -0.471405f, -0.333333f,
	0.816497f, -0.471405f, -0.333333f
};

int windowWidth = 1000;
int windowHeight = 500;

GLfloat angle = 0.0f;
GLfloat xyz[3] = { 0.0f, 0.0f, 1.0f };

void init() {
	//开启深度
	glEnable(GL_DEPTH_TEST);
	// 确定正面
	glFrontFace(GL_CCW);
	// 不绘制背面
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	// 启用抗锯齿
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 设置材质
	GLfloat mat_ambient[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat mat_diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat mat_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[4] = { 100.0f };

	// 设置颜色材料，使光照模式下仍然可以显示原本的颜色
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	// 启用平滑着色功能
	glShadeModel(GL_SMOOTH);
	// 启用光照功能
	glEnable(GL_LIGHTING);
	// 启用0，1号光源
	glEnable(GL_LIGHT0);

	// 背景颜色
	glClearColor(0.7, 0.7, 0.7, 1.0);
}

void triangle(GLfloat* a, GLfloat* b, GLfloat* c) // 绘制三角形
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(c);
	glEnd();
}

void normalize(GLfloat* p) {
	double d = 0.0;
	int i;
	for (i = 0; i < 3; i++) d += p[i] * p[i];
	d = sqrt(d);
	if (d > 0.0) for (i = 0; i < 3; i++) p[i] /= d;
}

void divide_triangle(GLfloat* a, GLfloat* b, GLfloat* c, int n)
{
	GLfloat v1[3], v2[3], v3[3];
	int j;
	if (n > 0)
	{
		for (j = 0; j < 3; j++) v1[j] = a[j] + b[j];
		normalize(v1);
		for (j = 0; j < 3; j++) v2[j] = a[j] + c[j];
		normalize(v2);
		for (j = 0; j < 3; j++) v3[j] = c[j] + b[j];
		normalize(v3);
		divide_triangle(a, v2, v1, n - 1);
		divide_triangle(c, v3, v2, n - 1);
		divide_triangle(b, v1, v3, n - 1);
		divide_triangle(v1, v2, v3, n - 1);
	}
	else
		triangle(a, b, c);
}

void tetrahedton(int n) // 绘制四面体
{
	divide_triangle(v[0], v[1], v[2], n);
	divide_triangle(v[3], v[2], v[1], n);
	divide_triangle(v[0], v[3], v[1], n);
	divide_triangle(v[0], v[2], v[3], n);
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
		3, 2, 1, 0,
		4, 5, 6, 7,
		0, 4, 7, 3,
		6, 5, 1, 2,
		0, 1, 5, 4,
		7, 6, 2, 3
	};

	// 绘制立方体的六个面
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < 6; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			glColor3fv(colors[index[i][j]]);
			glVertex3fv(vertex[index[i][j]]);
		}
	}
	glEnd();
}

void shadow(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat m[16] = { 0.0f };
	m[0] = m[5] = m[10] = 1.0f;
	m[11] = -1.0f / z;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);
	glMultMatrixf(m);
	glTranslatef(-x, -y, -z);
	tetrahedton(4);
	glPopMatrix();
}

void frame()
{
	// 绘制平面
	glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(8.0f, 8.0f, 0.0f);
	glVertex3f(-8.0f, 8.0f, 0.0f);
	glVertex3f(-8.0f, -8.0f, 0.0f);
	glVertex3f(8.0f, -8.0f, 0.0f);
	glEnd();
}

void display()
{
	GLfloat light_position[4] = { 0.0f, 5.0f, 3.0f, 1.0f };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 加载单位阵
	glLoadIdentity();
	// 设置相机的位置和视角
	gluLookAt(5, 0, 3, 0.0, 0.0, 0.0, 0, 0, 1);
	// 绘制坐标平面
	glDisable(GL_LIGHT0);
	frame();
	glEnable(GL_LIGHT0);
	// 设置光源
	glPushMatrix();
	glRotatef(angle, xyz[0], xyz[1], xyz[2]);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();

	// 绘制
	// 球体
	glTranslatef(0.0f, 2.0f, 1.0f);
	tetrahedton(4);
	glTranslatef(0.0f, 0.0f, -1.0f);
	shadow(5+5 * cos(angle * 3.1415 / 180), 5+5*sin(angle*3.1415/180), 10.0f);
	glTranslatef(0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -2.0f, -1.0f);
	// 立方体
	glTranslatef(0.0f, -2.0f, 1.0f);
	color_cube();
	glTranslatef(0.0f, 2.0f, -1.0f);

	glutSwapBuffers();
}

void timer_function(GLint value)
{
	angle += STEP;
	if (angle > 360.0) angle -= 360.0;
	glutPostRedisplay();
	glutTimerFunc(15, timer_function, 0);
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
	//gluLookAt(4, 2, 2, 0.0, 0.0, 0.0, 0, 0, 1);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	centerWindow();
	glutCreateWindow("sphere");
	// 绘制
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(500, timer_function, 0);
	// 进入主循环
	glutMainLoop();
	return 0;
}