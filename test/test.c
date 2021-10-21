#include<GL/glut.h>
#include<GL/glpng.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

void render(void);
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);

double rad = 0.0;

int main(int argc, char **argv)
{
    // Initialization
    glutInit(&argc, argv);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(640, 480);

    // Create Window
    glutCreateWindow("Test program");

    // Functions
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(100, timer, 0);
    glutMainLoop();

    return(0);
}

void render(void)
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw
    glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex3f(1.0, 0.0, 0.0);
        glColor3f(1, 1, 0);
        glVertex3f(1.0, 0.0, 1.0);
        glColor3f(0, 1, 0);
        glVertex3f(1.0, 1.0, 1.0);
        glColor3f(0, 1, 1);
        glVertex3f(1.0, 1.0, 0.0);

        glColor3f(0, 1, 1);
        glVertex3f(0.0, 1.0, 0.0);
        glColor3f(0, 1, 0);
        glVertex3f(0.0, 1.0, 1.0);
        glColor3f(1, 1, 0);
        glVertex3f(1.0, 1.0, 1.0);
        glColor3f(1, 0, 0);
        glVertex3f(1.0, 1.0, 0.0);

        glColor3f(0, 1, 1);
        glVertex3f(0.0, 0.0, 1.0);
        glColor3f(0, 1, 0);
        glVertex3f(0.0, 1.0, 1.0);
        glColor3f(1, 1, 0);
        glVertex3f(1.0, 1.0, 1.0);
        glColor3f(1, 0, 0);
        glVertex3f(1.0, 0.0, 1.0);

        glColor3f(1, 0, 0);
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(1, 1, 0);
        glVertex3f(0.0, 0.0, 1.0);
        glColor3f(0, 1, 0);
        glVertex3f(0.0, 1.0, 1.0);
        glColor3f(0, 1, 1);
        glVertex3f(0.0, 1.0, 0.0);

        glColor3f(0, 1, 1);
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(0, 1, 0);
        glVertex3f(0.0, 0.0, 1.0);
        glColor3f(1, 1, 0);
        glVertex3f(1.0, 0.0, 1.0);
        glColor3f(1, 0, 0);
        glVertex3f(1.0, 0.0, 0.0);

        glColor3f(0, 1, 1);
        glVertex3f(0.0, 0.0, 0.0);
        glColor3f(0, 1, 0);
        glVertex3f(0.0, 1.0, 0.0);
        glColor3f(1, 1, 0);
        glVertex3f(1.0, 1.0, 0.0);
        glColor3f(1, 0, 0);
        glVertex3f(1.0, 0.0, 0.0);

    glEnd();

    // Look at
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    double pos_x = 3.0 * cos(rad) + 0.5;
    double pos_y = 3.5;
    double pos_z = 3.0 * sin(rad) + 0.5;
    rad += M_PI / 180.0;

	gluLookAt(pos_x, pos_y, pos_z, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0);

    // Update
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / height, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    double pos_x = 3.0 * cos(rad);
    double pos_y = 3.0;
    double pos_z = 3.0 * sin(rad);
    rad += M_PI / 180.0;

	gluLookAt(pos_x, pos_y, pos_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char c, int x, int y)
{
    if (c == 'q' || c == 27)
        exit(1);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
        exit(1);
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}