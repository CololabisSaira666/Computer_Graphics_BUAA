#include<GL/glut.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>

#define STEP 0.5f

int windowWidth = 1000;
int windowHeight = 500;

GLfloat angle = 0.0f;
GLfloat xyz[3] = { 0.0f, 2.0f, 0.0f };

GLuint texGround;
GLuint texWall;


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
	
	// 背景颜色
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

static const GLfloat vertex_list[][3] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
};

static const GLint index_list[][4] = {
    0, 2, 3, 1,
    0, 4, 6, 2,
    0, 1, 5, 4,
    4, 5, 7, 6,
    1, 3, 7, 5,
    2, 6, 7, 3,
};

static const GLfloat coord_list[][2] = {
    0,0,
    0,1,
    1,1,
    1,0,
};

int power_of_two(int n)
{
    if (n <= 0)
        return 0;
    return (n & (n - 1)) == 0;
}

GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID = 0, texture_ID = 0;

    FILE* pFile;
    int err = fopen_s(&pFile, file_name, "rb");
    if (err != 0) { return 0; }

    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, 54, SEEK_SET);

    {
        GLint line_bytes = width * 3;
        while (line_bytes % 4 != 0)
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    pixels = (GLubyte*)malloc(total_bytes);
    if (pixels == 0)
    {
        fclose(pFile);
        return 0;
    }

    if (fread(pixels, total_bytes, 1, pFile) <= 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if (!power_of_two(width)
            || !power_of_two(height)
            || width > max
            || height > max)
        {
            const GLint new_width = 256;
            const GLint new_height = 256; 
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;

            new_line_bytes = new_width * 3;
            while (new_line_bytes % 4 != 0)
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;

            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if (new_pixels == 0)
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }

            gluScaleImage(GL_RGB,
                width, height, GL_UNSIGNED_BYTE, pixels,
                new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }

    glGenTextures(1, &texture_ID);
    if (texture_ID == 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    GLint lastTextureID = last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
    free(pixels);
    return texture_ID;
}


void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 加载单位阵
	glLoadIdentity();
	// 设置相机的位置和视角
    gluLookAt(-1.5, 1.5, 1.5, 0, 0, 0, 0, 0, 1);
    glRotatef(angle, xyz[0], xyz[1], xyz[2]);
    glBindTexture(GL_TEXTURE_2D, texWall);
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            glTexCoord2f(coord_list[j][0], coord_list[j][1]);
            glVertex3fv(vertex_list[index_list[i][j]]);
        }
    }
    glEnd();
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
    glEnable(GL_TEXTURE_2D);    // 启用纹理
    texWall = load_texture("1.bmp");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutTimerFunc(500, timer_function, 0);
	// 进入主循环
	glutMainLoop();
	return 0;
}