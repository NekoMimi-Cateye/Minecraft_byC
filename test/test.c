#include<GL/glut.h>
#include<GL/glpng.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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

double rad = 0.0;

// 1 chunk data list
int chunk[16][256][16];
int map2d[16][16];

// for debug
int flag = 0;

int main(int argc, char **argv)
{
    // load map

    // Initialization
    glutInit(&argc, argv);
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
    // window size
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_FRONT);

    // View port
    glViewport(0, 0, w, h);

    // Set field of view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / h, 1.0, 10000000.0);

    // Camera setting
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    double pos_x = 96.0 * cos(rad) + 192.0;
    double pos_y = 100.0;
    double pos_z = 96.0 * sin(rad) + 192.0;
    rad += M_PI / 360.0;

	gluLookAt(pos_x, pos_y, pos_z, 192.0, 80.0, 192.0, 0.0, 1.0, 0.0);

    // Draw
    double dx, dy, dz;
    for(int i=8; i<16; i++)
    {
        for(int j=8; j<16; j++)
        {
            PerlinNoize2d(map2d, i, j, 1, 0);
            for (int x = 0; x < 16; x++)
            {
                dx = (double)(x+i*16);
                for (int z = 0; z < 16; z++)
                {
                    dz = (double)(z+j*16);
                    /*for (double y = 0; y < map2d[x][z]; y ++)
                    {*/
                        dy = (double)(map2d[x][z] - 1);
                        glEnable(GL_DEPTH_TEST);
                        glBegin(GL_QUADS);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+1.0, dz+0.0);

                            glColor3f(0, 1, 0);
                            glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                            glColor3f(0, 1, 0);
                            glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                            glColor3f(0, 1, 0);
                            glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                            glColor3f(0, 1, 0);
                            glVertex3f(dx+1.0, dy+1.0, dz+0.0);

                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+0.0, dz+1.0);

                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+1.0, dz+0.0);

                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+0.0, dz+0.0);

                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                        glEnd();
                    /*}*/
                }
            }
        }
    }

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
    glColor3f(1, 1, 1);
    glRasterPos2i(32, 32);
    char str[15] = "Hello, World!";
    for(int j=0; j<15; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[j]);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glFlush();

    // Update
    glutSwapBuffers();
}

void reshape(int width, int height)
{

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
            if(flag < 2)
                printf("%lf  ", C[x][z]);
        }
        if(flag < 2)
            printf("\n");
    }
    if(flag < 2)
        printf("------------------------------");
    fflush(stdout);

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
            if(flag < 2)
                printf("%d  ", data[x][z]);
        }
        if(flag < 2)
            printf("\n");
    }
    if(flag < 2)
        printf("------------------------------");
    fflush(stdout);
    flag ++;
}

void PerlinNoize3d(int data[16][256][16], int x_length, int y_length, int z_length, int use_seed, unsigned int seed)
{

}