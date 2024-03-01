#include<GL/glut.h>
#include<math.h>

GLfloat v[][3] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 0.942809f, -0.333333f,
	-0.816497f, -0.471405f, -0.333333f,
	0.816497f, -0.471405f, -0.333333f
};

int windowWidth = 500;
int windowHeight = 500;

void init() {
	//glEnable(GL_DEPTH_TEST); //开启深度
	glFrontFace(GL_CCW); // 设置逆时针，正面
	glCullFace(GL_BACK); // 不绘制背面
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 背景颜色
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void triangle(GLfloat* a, GLfloat* b, GLfloat* c) // 绘制三角形
{
	glColor3f(0, 0, 0);
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	// 加载单位阵
	glLoadIdentity();
	// 设置相机的位置和视角
	gluLookAt(2, 2, 2, 0.0, 0.0, 0.0, -1, -1, 1);
	// 绘制
	tetrahedton(4);
	glutSwapBuffers();
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
	gluLookAt(2, 2, 2, 0.0, 0.0, 0.0, -1, -1, 1);
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
	// 进入主循环
	glutMainLoop();
	return 0;
}