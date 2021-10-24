#include<GL/glut.h>
#include<GL/glpng.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<math.h>

// Wavelet
void Wavelet2d(double[256][256], double);
void Wavelet3d(double[256][256][256], double);

// PerlinNoize
void PerlinNoize2d(int[16][16], int, int, int, unsigned int);
void PerlinNoize3d(int[16][256][16], int, int, int, int, unsigned int);

// for GLUT
void render(void);
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);

// for develop: rotate camera
double rad = 0.0;

// 1 chunk data list
int chunk[33][33][16][256][16];
int map2d[33][33][16][16];

// for draw fps
struct timeval start, end;
double fps[10] = {0};
double avg_fps;
char fps_str[20] = "fps: calcutating...";
int fps_count = 0;

// for debug
int flag = 0;

int main(int argc, char **argv)
{
    for (int i=0; i<33; i++)
        for (int j=0; j<33; j++)
            PerlinNoize2d(map2d[i][j], i, j, 1, 0);
    // Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(640, 480);

    // Create Window
    glutCreateWindow("Test program");

    // ClearColor
    glClearColor(0.0, 1.0, 1.0, 1.0);

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
    if (fps_count % 100 == 0)
        gettimeofday(&start, NULL);
    // window size
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_FRONT);

    // Camera setting
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    double pos_x = 136.0;
    double pos_y = 86.0;
    double pos_z = 136.0;
    rad += M_PI / 360.0;

	gluLookAt(pos_x, pos_y, pos_z, 136.0+10.0*cos(rad), 86.0, 136.0+10.0*sin(rad), 0.0, 1.0, 0.0);

    // Draw
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
        double dx, dy, dz;
        for(int i=1; i<33; i++)
        {
            for(int j=1; j<33; j++)
            {
                for (int x = 0; x < 16; x++)
                {
                    dx = (double)(x+i*16);
                    for (int z = 0; z < 16; z++)
                    {
                        dz = (double)(z+j*16);
                        /*for (double y = 0; y < map2d[i][j][x][z]; y ++)
                        {*/
                            dy = (double)(map2d[i][j][x][z] - 1);
                            // x-Higher
                            if (x == 15 || (x < 15 && map2d[i][j][x][z] != map2d[i][j][x+1][z]))
                            {
                                glColor3f(0.66, 0.43, 0.18);
                                glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                                glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                                glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                            }
                            // y-Higher
                            glColor3f(0, 1, 0);
                            glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                            glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                            glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                            glVertex3f(dx+1.0, dy+1.0, dz+0.0);

                            // z-Higher
                            if (z == 15 || (z < 15 && map2d[i][j][x][z] != map2d[i][j][x][z+1]))
                            {
                                glColor3f(0.48, 0.34, 0.14);
                                glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                            }

                            // x-Lower
                            if (x == 0 || (x > 0 && map2d[i][j][x][z] != map2d[i][j][x-1][z]))
                            {
                                glColor3f(0.34, 0.22, 0.09);
                                glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                            }

                            // y-Lower
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                            glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                            glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                            glVertex3f(dx+1.0, dy+0.0, dz+0.0);

                            // z-Lower
                            if (z == 0 || (z > 0 && map2d[i][j][x][z] != map2d[i][j][x][z-1]))
                            {
                                glColor3f(0.42, 0.27, 0.11);
                                glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                                glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                                glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                            }
                        /*}*/
                    }
                }
            }
        }
    glEnd();

    // write strings
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0, 0, 0);
    glRasterPos2i(32, 32);
    for(int j=0; j<20; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fps_str[j]);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glFlush();

    // Update
    glutSwapBuffers();
    if (fps_count % 100 == 99)
    {
        gettimeofday(&end, NULL);
        fps[(fps_count / 100) % 10] = 100.0 / ((end.tv_sec + end.tv_sec * 1e-6) - (start.tv_sec + start.tv_sec * 1e-6));
        avg_fps = 0.0;
        for(int i=0; i<10; i++)
        {
            avg_fps += fps[i];
        }
        avg_fps /= 10.0;
        if (fps_count / 1000 > 0)
            sprintf(fps_str, "fps: %.2lf         ", avg_fps);
    }
    fps_count ++;
}

void reshape(int width, int height)
{
    // View port
    glViewport(0, 0, width, height);

    // Set field of view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 1.0, 100000.0);
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


// Wavelet
void Wavelet2d(double data[256][256], double amp)
{
    double x_val, z_val;
    double dx, dz;
    for(int x=-128; x<128; x++)
    {
        dx = (double)x / 128.0;
        if (x > 0)
            x_val = 1.0 - 3.0 * (dx * dx) + 2.0 * (dx * dx * dx);
        else
            x_val = 1.0 - 3.0 * (dx * dx) - 2.0 * (dx * dx * dx);
        for(int z=-128; z<128; z++)
        {
            dz = (double)z / 128.0;
            if (z > 0)
                z_val = 1.0 - 3.0 * (dz * dz) + 2.0 * (dz * dz * dz);
            else
                x_val = 1.0 - 3.0 * (dz * dz) - 2.0 * (dz * dz * dz);
            data[x+128][z+128] = x_val * z_val * amp;
        }
    }
}

void Wavelet3d(double data[256][256][256], double amp)
{

}

// PerlinNoize
void PerlinNoize2d(int data[16][16], int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 8;
    int noizeport_z = chunk_zpos / 8;
    int mod_chunk_xpos = chunk_xpos % 8;
    int mod_chunk_zpos = chunk_zpos % 8;

    double C[256][256];                     // Wavelet data
    double W[128][128][4];                  // Graph data for marge
    double Wp[128][128][2];                 // Graph data (marge z)
    double a[7][7][2];                      // Gradient data

    double dx, dz;

    // Wavelet function
    Wavelet2d(C, 48.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for(int i=0; i<7; i++)
    {
        for(int j=0; j<7; j++)
        {
            a[i][j][0] = (double)rand() / RAND_MAX;
            a[i][j][1] = (double)rand() / RAND_MAX;
        }
    }

    for(int x=0; x<128; x++)
    {
        dx = (double)x / 128.0;
        for(int z=0; z<128; z++)
        {
            dz = (double)z / 128.0;

            // Calculate Graphs
            W[x][z][0] = a[noizeport_x][noizeport_z][0] * dx + a[noizeport_x][noizeport_z][1] * dz * C[x+128][z+128];
            W[x][z][1] = a[noizeport_x][noizeport_z+1][0] * dx + a[noizeport_x][noizeport_z+1][1] * (dz-1.0) * C[x+128][z];
            W[x][z][2] = a[noizeport_x+1][noizeport_z][0] * (dx-1.0) + a[noizeport_x+1][noizeport_z][1] * dz * C[x][z+128];
            W[x][z][3] = a[noizeport_x+1][noizeport_z+1][0] * (dx-1.0) + a[noizeport_x+1][noizeport_z+1][1] * (dz-1.0) * C[x][z];
        }
    }

    for(int x=0; x<128; x++)
    {
        for(int z=0; z<128; z++)
        {
            dz = (double)z / 128.0;

            //marge (way: z)
            Wp[x][z][0] = W[x][z][0] + dz * (W[x][z][1] - W[x][z][0]);
            Wp[x][z][1] = W[x][z][2] + dz * (W[x][z][3] - W[x][z][2]);
        }
    }
    int xp, zp;
    for(int x=0; x<16; x++)
    {
        xp = 16 * mod_chunk_xpos + x;
        dx = (double)(xp) / 128.0;
        for(int z=0; z<16; z++)
        {
            zp = 16 * mod_chunk_zpos + z;
            //marge (way: x)
            data[x][z] = 80 + lround(Wp[xp][zp][0] + dx * (Wp[xp][zp][1] - Wp[xp][zp][0]));
        }
    }
}

void PerlinNoize3d(int data[16][256][16], int x_length, int y_length, int z_length, int use_seed, unsigned int seed)
{

}