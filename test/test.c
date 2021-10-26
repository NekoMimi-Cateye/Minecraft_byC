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
void PerlinNoize3d(int[16][256][16], int, int, int, unsigned int);

// function of Jump velocity
double JumpVelocity(double, double, double, int);

// for GLUT
void render(void);
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void keyboardup(unsigned char, int, int);
void mouse(int, int, int, int);
void motion(int, int);
void special(int, int, int);

// player info
double player_x = 136.0;
double player_y = 86.0;
double player_z = 136.0;
int player_block_x = 136;
int player_block_y = 86;
int player_block_z = 136;

int player_pre_loadchunk_x = 0;
int player_pre_loadchunk_z = 0;
int player_pre_chunk_x = 8;
int player_pre_chunk_z = 8;
int player_chunk_x = 8;
int player_chunk_y = 5;
int player_chunk_z = 8;

int player_local_x = 8;
int player_local_y = 6;
int player_local_z = 8;

double xz_rad = 0.0;
double y_rad = 0.0;

int mouse_x = 0;
int mouse_y = 0;

int key_state[7] = {0};

int player_jump_tick = -1;
double player_jump_start_velocity = 0.42;
double player_gravity_acceleration = 0.08;
double player_air_resistance = 0.02;
double player_jump_velocity = 0.0;

int player_fall_tick = -1;
double player_fall_velocity = 0.0;

// 1 chunk data list
int chunk[33][33][16][256][16];
int map2d[33][33][16][16];

// for draw fps
struct timeval start, end;
double fps[10] = {0};
double avg_fps;
char fps_str[20] = "fps: calcutating...";
int fps_count = 0;

// for draw info
char pos_str[40];
char block_str[40];
char chunk_str[40];
char keystate_str[60];
char loadpos_str[60];
char loadposc_str[60];

// for debug
int flag = 0;

int main(int argc, char **argv)
{
    for (int i=0; i<33; i++)
    {
        for (int j=0; j<33; j++)
        {
            PerlinNoize2d(map2d[i][j], i, j, 1, 0);
        }
    }
    player_y = (double)map2d[player_chunk_x][player_chunk_z][player_local_x][player_local_z] + 1.5;
    player_block_y = (int)(player_y - 1.5);
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
    glutKeyboardUpFunc(keyboardup);
    //glutIgnoreKeyRepeat(GL_TRUE);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
    return(0);
}

void render(void)
{
    double speed = 0.21;
    double sneek_speed = 0.304;
    double dash_speed = 2.0;
    double next_player_x=player_x, next_player_z=player_z;
    int next_player_block_x, next_player_block_z;
    int next_player_chunk_x, next_player_chunk_z;
    int next_player_local_x, next_player_local_z;

    if (key_state[4] == 1)
        speed *= sneek_speed;
    else if (key_state[5] == 1)
        speed *= dash_speed;
    if (key_state[0] == 1)
    {
        next_player_x += speed * cos(xz_rad);
        next_player_z += speed * sin(xz_rad);
    }
    if (key_state[1] == 1)
    {
        next_player_x -= speed * cos(xz_rad);
        next_player_z -= speed * sin(xz_rad);
    }
    if (key_state[2] == 1)
    {
        next_player_x += speed * sin(xz_rad);
        next_player_z -= speed * cos(xz_rad);
    }
    if (key_state[3] == 1)
    {
        next_player_x -= speed * sin(xz_rad);
        next_player_z += speed * cos(xz_rad);
    }
    if (key_state[6] == 1 && player_jump_tick == -1 && player_fall_tick == -1)
    {
        player_jump_tick = 0;
    }

    next_player_block_x = (int)next_player_x;
    next_player_chunk_x = (int)next_player_block_x / 16;
    next_player_local_x = (int)next_player_block_x % 16;
    if(player_y >= (double)map2d[next_player_chunk_x-player_pre_loadchunk_x][player_chunk_z-player_pre_loadchunk_z][next_player_local_x][player_local_z] + 1.5 && next_player_x > 16.0 && next_player_x < 880.0)
    {
        player_x = next_player_x;
        player_block_x = next_player_block_x;
        player_chunk_x = next_player_chunk_x;
        player_local_x = next_player_local_x;
    }

    next_player_block_z = (int)next_player_z;
    next_player_chunk_z = (int)next_player_block_z / 16;
    next_player_local_z = (int)next_player_block_z % 16;
    if(player_y >= (double)map2d[player_chunk_x-player_pre_loadchunk_x][next_player_chunk_z-player_pre_loadchunk_z][player_local_x][next_player_local_z] + 1.5 && next_player_z > 16.0 && next_player_z < 880.0)
    {
        player_z = next_player_z;
        player_block_z = next_player_block_z;
        player_chunk_z = next_player_chunk_z;
        player_local_z = next_player_local_z;
    }
    if (player_chunk_x - player_pre_chunk_x > 8 || player_chunk_x - player_pre_chunk_x < -8 || player_chunk_z - player_pre_chunk_z > 8 || player_chunk_z - player_pre_chunk_z < -8)
    {
        player_pre_chunk_x = player_chunk_x;
        player_pre_loadchunk_x = player_pre_chunk_x - 16;
        if (player_pre_loadchunk_x < 0)
            player_pre_loadchunk_x = 0;
        if (player_pre_loadchunk_x > 23)
            player_pre_loadchunk_x = 23;
        player_pre_chunk_z = player_chunk_z;
        player_pre_loadchunk_z = player_pre_chunk_z - 16;
        if (player_pre_loadchunk_z < 0)
            player_pre_loadchunk_z = 0;
        if (player_pre_loadchunk_z > 23)
            player_pre_loadchunk_z = 23;
        for (int i=0; i<33; i++)
        {
            for (int j=0; j<33; j++)
            {
                PerlinNoize2d(map2d[i][j], player_pre_loadchunk_x+i, player_pre_loadchunk_z+j, 1, 0);
            }
        }
    }

    if (fps_count % 100 == 0)
        gettimeofday(&start, NULL);
    // window size
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    // mouse position
    mouse_x = w / 2;
    mouse_y = h / 2;

    // y position process
    if (player_fall_tick == -1 && player_jump_tick == -1 && player_y > (double)map2d[player_chunk_x-player_pre_loadchunk_x][player_chunk_z-player_pre_loadchunk_z][player_local_x][player_local_z] + 1.5)
        player_fall_tick = 0;

    if (player_fall_tick >= 0 && player_fall_tick % 3 == 0)
    {
        player_fall_velocity = JumpVelocity(0.0, player_gravity_acceleration, player_air_resistance, player_fall_tick / 3);
        player_y += player_fall_velocity;
        player_block_y = (int)(player_y - 1.5);
        if (player_y < (double)map2d[player_chunk_x-player_pre_loadchunk_x][player_chunk_z-player_pre_loadchunk_z][player_local_x][player_local_z] + 1.5)
        {
            player_y = (double)map2d[player_chunk_x-player_pre_loadchunk_x][player_chunk_z-player_pre_loadchunk_z][player_local_x][player_local_z] + 1.5;
            player_block_y = (int)(player_y - 1.5);
            player_chunk_y = player_block_y / 16;
            player_local_y = player_block_y % 16;
            player_fall_tick = -1;
        }
    }
    if (player_fall_tick >= 0)
        player_fall_tick ++;


    if (player_jump_tick >= 0 && player_jump_tick % 3 == 0)
    {
        player_jump_velocity = JumpVelocity(player_jump_start_velocity, player_gravity_acceleration, player_air_resistance, player_jump_tick / 3);
        player_y += player_jump_velocity;
        player_block_y = (int)(player_y - 1.5);
        if (player_y < (double)map2d[player_chunk_x-player_pre_loadchunk_x][player_chunk_z-player_pre_loadchunk_z][player_local_x][player_local_z] + 1.5)
        {
            player_y = (double)map2d[player_chunk_x-player_pre_loadchunk_x][player_chunk_z-player_pre_loadchunk_z][player_local_x][player_local_z] + 1.5;
            player_block_y = (int)(player_y - 1.5);
            player_chunk_y = player_block_y / 16;
            player_local_y = player_block_y % 16;
            player_jump_tick = -1;
        }
    }
    if (player_jump_tick >= 0)
        player_jump_tick ++;

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_FRONT);

    // Camera setting
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(player_x, player_y, player_z, player_x+10.0*cos(xz_rad)*cos(y_rad), player_y-10.0*sin(y_rad), player_z+10.0*sin(xz_rad)*cos(y_rad), 0.0, 1.0, 0.0);

    // Draw
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
        double dx, dy, dz;
        for(int i=0; i<33; i++)
        {
            for(int j=0; j<33; j++)
            {
                for (int x = 0; x < 16; x++)
                {
                    dx = (double)(x+(player_pre_loadchunk_x+i)*16);
                    for (int z = 0; z < 16; z++)
                    {
                        dz = (double)(z+(player_pre_loadchunk_z+j)*16);
                        /*for (double y = 0; y < map2d[i][j][x][z]; y ++)
                        {*/
                            dy = (double)(map2d[i][j][x][z] - 1);
                            // x-Higher
                            if (x == 15 || (x < 15 && map2d[i][j][x][z] != map2d[i][j][x+1][z]))
                            {
                                glColor3f(0.66, 0.43, 0.18);
                                glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                                glColor3f(0.59, 0.39, 0.16);
                                glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                                glColor3f(0.53, 0.34, 0.14);
                                glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                glColor3f(0.59, 0.39, 0.16);
                                glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                            }
                            // y-Higher
                            if (player_pre_loadchunk_x+i > 0 && player_pre_loadchunk_z+j > 0 && player_pre_loadchunk_x+i < 55 && player_pre_loadchunk_z+j && 55)
                            {
                                glColor3f(0, 1, 0);
                                glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                                glColor3f(0, 0.9, 0);
                                glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                glColor3f(0, 0.8, 0);
                                glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                glColor3f(0, 0.9, 0);
                                glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                            }
                            else
                            {
                                glColor3f(0, 1, 0.5);
                                glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                                glColor3f(0, 0.9, 0.45);
                                glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                glColor3f(0, 0.8, 0.4);
                                glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                glColor3f(0, 0.9, 0.45);
                                glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                            }
                            // z-Higher
                            if (z == 15 || (z < 15 && map2d[i][j][x][z] != map2d[i][j][x][z+1]))
                            {
                                glColor3f(0.48, 0.34, 0.14);
                                glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                glColor3f(0.43, 0.31, 0.13);
                                glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                glColor3f(0.38, 0.27, 0.11);
                                glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                glColor3f(0.43, 0.31, 0.13);
                                glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                            }

                            // x-Lower
                            if (x == 0 || (x > 0 && map2d[i][j][x][z] != map2d[i][j][x-1][z]))
                            {
                                glColor3f(0.34, 0.22, 0.09);
                                glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                glColor3f(0.31, 0.2, 0.08);
                                glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                glColor3f(0.27, 0.18, 0.07);
                                glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                glColor3f(0.31, 0.2, 0.08);
                                glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                            }

                            // y-Lower
                            glColor3f(0.64, 0.41, 0.25);
                            glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                            glColor3f(0.58, 0.37, 0.22);
                            glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                            glColor3f(0.51, 0.33, 0.2);
                            glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                            glColor3f(0.58, 0.37, 0.22);
                            glVertex3f(dx+1.0, dy+0.0, dz+0.0);

                            // z-Lower
                            if (z == 0 || (z > 0 && map2d[i][j][x][z] != map2d[i][j][x][z-1]))
                            {
                                glColor3f(0.42, 0.27, 0.11);
                                glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                glColor3f(0.34, 0.22, 0.09);
                                glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                                glColor3f(0.38, 0.24, 0.1);
                                glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                                glColor3f(0.34, 0.22, 0.09);
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
    glLineWidth(3.0);
    int itembox_x =w / 2 - 136;
    int itembox_y = h / 2 + 136;
    glBegin(GL_LINES);
        for (int i=0; i<9; i++)
        {
            glVertex2i(itembox_x +  32*i    , itembox_y);
            glVertex2i(itembox_x +  32*(i+1), itembox_y);
            glVertex2i(itembox_x +  32*(i+1), itembox_y);
            glVertex2i(itembox_x +  32*(i+1), itembox_y + 32);
            glVertex2i(itembox_x +  32*(i+1), itembox_y + 32);
            glVertex2i(itembox_x +  32*i    , itembox_y + 32);
            glVertex2i(itembox_x +  32*i    , itembox_y + 32);
            glVertex2i(itembox_x +  32*i    , itembox_y);
        }
    glEnd();
    glColor3f(0, 0, 0);
    glRasterPos2i(0, 16);
    for(int j=0; j<20; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fps_str[j]);
    glRasterPos2i(0, 16*9);
    for(int j=0; j<40; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, pos_str[j]);
    glRasterPos2i(0, 16*10);
    for(int j=0; j<40; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, block_str[j]);
    glRasterPos2i(0, 16*11);
    for(int j=0; j<40; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, chunk_str[j]);
    glRasterPos2i(0, 16*13);
    for(int j=0; j<60; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, keystate_str[j]);
    glRasterPos2i(0, 16*14);
    for(int j=0; j<60; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, loadpos_str[j]);
    glRasterPos2i(0, 16*15);
    for(int j=0; j<60; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, loadposc_str[j]);
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
    sprintf(pos_str,   "XYZ: %.2lf %.2lf %.2lf", player_x, player_y, player_z);
    sprintf(block_str, "BLOCK: %d %d %d", player_block_x, player_block_y, player_block_z);
    sprintf(chunk_str, "CHUNK: %d %d %d in %d %d %d", player_local_x, player_local_y, player_local_z, player_chunk_x, player_chunk_y, player_chunk_z);
    sprintf(keystate_str, "F:%d B:%d L:%d R:%d SLOW:%d FAST:%d JUMP:%d", key_state[0], key_state[1], key_state[2], key_state[3], key_state[4], key_state[5], key_state[6]);
    sprintf(loadposc_str, "LOAD CHUNK(PLAYER): %d %d", player_pre_chunk_x, player_pre_chunk_z);
    sprintf(loadpos_str, "LOAD CHUNK(START): %d %d", player_pre_loadchunk_x, player_pre_loadchunk_z);
}

void reshape(int width, int height)
{
    // View port
    glViewport(0, 0, width, height);

    // Set field of view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.01, 100000.0);
}

void keyboard(unsigned char c, int x, int y)
{
    int a = glutGetModifiers();
    if ((a & GLUT_ACTIVE_SHIFT) != 0) 
        key_state[4] = 1;
    else
        key_state[4] = 0;
    if ((a & GLUT_ACTIVE_CTRL) != 0)
        key_state[5] = 1;
    else
        key_state[5] = 0;
    if (c == 'q' || c == 27)
        exit(1);
    else if (c == 'w' || c == 'W' || c == 23)
        key_state[0] = 1;
    else if (c == 's' || c == 'S' || c == 1)
        key_state[1] = 1;
    else if (c == 'a' || c == 'A' || c == 19)
        key_state[2] = 1;
    else if (c == 'd' || c == 'D' || c == 4)
        key_state[4] = 1;
    else if (c == ' ')
        key_state[6] = 1;
}

void keyboardup(unsigned char c, int x, int y)
{
    int a = glutGetModifiers();
    if ((a & GLUT_ACTIVE_SHIFT) != 0) 
        key_state[4] = 1;
    else
        key_state[4] = 0;
    if ((a & GLUT_ACTIVE_CTRL) != 0)
        key_state[5] = 1;
    else
        key_state[5] = 0;
    if (c == 'q' || c == 27)
        exit(1);
    else if (c == 'w' || c == 'W' || c == 23)
        key_state[0] = 0;
    else if (c == 's' || c == 'S' || c == 1)
        key_state[1] = 0;
    else if (c == 'a' || c == 'A' || c == 19)
        key_state[2] = 0;
    else if (c == 'd' || c == 'D' || c == 4)
        key_state[4] = 0;
    else if (c == ' ')
        key_state[6] = 0;
}

void special(int c, int x, int y)
{
}


void mouse(int button, int state, int x, int y)
{
}

void motion(int x, int y)
{
    glutWarpPointer(mouse_x, mouse_y);
    glutSetCursor(GLUT_CURSOR_NONE);
    if (mouse_x - x != 0)
        xz_rad -= (double)(mouse_x - x) * M_PI / 180.0 / 8.0;
    if (mouse_y - y != 0 && y_rad > -M_PI / 2.0 && y_rad < M_PI / 2.0)
        y_rad -= (double)(mouse_y - y) * M_PI / 180.0 / 8.0;
    if (y_rad > M_PI / 2.0)
        y_rad = M_PI / 2.0 - 0.01;
    if (y_rad < -M_PI / 2.0)
        y_rad = -M_PI / 2.0 + 0.01;
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1, timer, 0);
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
    double x_val, y_val, z_val;
    double dx, dy, dz;
    for(int x=-128; x<128; x++)
    {
        dx = (double)x / 128.0;
        if (x > 0)
            x_val = 1.0 - 3.0 * (dx * dx) + 2.0 * (dx * dx * dx);
        else
            x_val = 1.0 - 3.0 * (dx * dx) - 2.0 * (dx * dx * dx);
        for(int y=-128; y<128; y++)
        {
            dy = (double)y / 128.0;
            if (y > 0)
                y_val = 1.0 - 3.0 * (dy * dy) + 2.0 * (dy * dy * dy);
            else
                y_val = 1.0 - 3.0 * (dy * dy) - 2.0 * (dy * dy * dy);
            for(int z=-128; z<128; z++)
            {
                dz = (double)z / 128.0;
                if (z > 0)
                    z_val = 1.0 - 3.0 * (dz * dz) + 2.0 * (dz * dz * dz);
                else
                    x_val = 1.0 - 3.0 * (dz * dz) - 2.0 * (dz * dz * dz);
                data[x+128][y+128][z+128] = x_val * y_val * z_val * amp;
            }
        }
    }
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
    double a[8][8][2];                      // Gradient data

    double dx, dz;

    // Wavelet function
    Wavelet2d(C, 100.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
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

void PerlinNoize3d(int data[16][256][16], int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 8;
    int noizeport_z = chunk_zpos / 8;
    int mod_chunk_xpos = chunk_xpos % 8;
    int mod_chunk_zpos = chunk_zpos % 8;

    double C[256][256][256];            // Wavelet data
    double W[128][128][128][8];         // Graph data for marge
    double Wp0[128][128][128][4];       // Graph data (marge z)
    double Wp1[128][128][128][2];       // Graph data (marge z)
    double Wm[128][128][128];           // Graph data (marge z)
    double a[8][2][8][3];               // Gradient data

    double dx, dy, dz;

    // Wavelet function
    Wavelet3d(C, 1.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<2; j++)
        {
            for(int k=0; k<8; k++)
            {
                a[i][j][k][0] = (double)rand() / RAND_MAX;
                a[i][j][k][1] = (double)rand() / RAND_MAX;
            }
        }
    }
    for (int noizeport_y = 0; noizeport_y<2; noizeport_y++)
    {
        for(int x=0; x<128; x++)
        {
            dx = (double)x / 128.0;
            for(int y=0; y<128; y++)
            {
                dy = (double)y / 128.0;
                for(int z=0; z<128; z++)
                {
                    dz = (double)z / 128.0;
                    // Calculate Graphs
                    W[x][y][z][0] = a[noizeport_x+1][noizeport_y+1][noizeport_z+1][0] * dx + a[noizeport_x+1][noizeport_y+1][noizeport_z+1][2] * dy + a[noizeport_x+1][noizeport_y]  [noizeport_z]  [2] * dz * C[x+128][y+128][z+128];
                    W[x][y][z][1] = a[noizeport_x+1][noizeport_y+1][noizeport_z]  [0] * dx + a[noizeport_x+1][noizeport_y+1][noizeport_z]  [2] * dy + a[noizeport_x+1][noizeport_y]  [noizeport_z+1][2] * dz * C[x+128][y+128][z+128];
                    W[x][y][z][2] = a[noizeport_x+1][noizeport_y]  [noizeport_z+1][0] * dx + a[noizeport_x+1][noizeport_y]  [noizeport_z+1][2] * dy + a[noizeport_x+1][noizeport_y+1][noizeport_z]  [2] * dz * C[x+128][y+128][z+128];
                    W[x][y][z][3] = a[noizeport_x+1][noizeport_y]  [noizeport_z]  [0] * dx + a[noizeport_x+1][noizeport_y]  [noizeport_z]  [2] * dy + a[noizeport_x+1][noizeport_y+1][noizeport_z+1][2] * dz * C[x+128][y+128][z+128];
                    W[x][y][z][4] = a[noizeport_x]  [noizeport_y+1][noizeport_z+1][0] * dx + a[noizeport_x]  [noizeport_y+1][noizeport_z+1][2] * dy + a[noizeport_x]  [noizeport_y]  [noizeport_z]  [2] * dz * C[x+128][y+128][z+128];
                    W[x][y][z][5] = a[noizeport_x]  [noizeport_y+1][noizeport_z]  [0] * dx + a[noizeport_x]  [noizeport_y+1][noizeport_z]  [2] * dy + a[noizeport_x]  [noizeport_y]  [noizeport_z+1][2] * dz * C[x+128][y+128][z+128];
                    W[x][y][z][6] = a[noizeport_x]  [noizeport_y]  [noizeport_z+1][0] * dx + a[noizeport_x]  [noizeport_y]  [noizeport_z+1][2] * dy + a[noizeport_x]  [noizeport_y+1][noizeport_z]  [2] * dz * C[x+128][y+128][z+128];
                    W[x][y][z][7] = a[noizeport_x]  [noizeport_y]  [noizeport_z]  [0] * dx + a[noizeport_x]  [noizeport_y]  [noizeport_z]  [2] * dy + a[noizeport_x]  [noizeport_y+1][noizeport_z+1][2] * dz * C[x+128][y+128][z+128];
                }
            }
        }
        for(int x=0; x<128; x++)
        {
            for(int y=0; y<128; y++)
            {
                for(int z=0; z<128; z++)
                {
                    dz = (double)z / 128.0;
                    //marge (way: z)
                    Wp0[x][y][z][0] = W[x][y][z][0] + dz * (W[x][y][z][1] - W[x][y][z][0]);
                    Wp0[x][y][z][1] = W[x][y][z][2] + dz * (W[x][y][z][3] - W[x][y][z][2]);
                    Wp0[x][y][z][2] = W[x][y][z][4] + dz * (W[x][y][z][5] - W[x][y][z][4]);
                    Wp0[x][y][z][3] = W[x][y][z][6] + dz * (W[x][y][z][7] - W[x][y][z][6]);
                }
            }
        }
        for(int x=0; x<128; x++)
        {
            for(int y=0; y<128; y++)
            {
                dy = (double)y / 128.0;
                for(int z=0; z<128; z++)
                {
                    //marge (way: y)
                    Wp1[x][y][z][0] = Wp0[x][y][z][0] + dy * (Wp0[x][y][z][1] - Wp0[x][y][z][0]);
                    Wp1[x][y][z][1] = Wp0[x][y][z][2] + dy * (Wp0[x][y][z][3] - Wp0[x][y][z][2]);
                }
            }
        }

        int xp, yp, zp;
        for(int x=0; x<16; x++)
        {
            xp = 16 * mod_chunk_xpos + x;
            dx = (double)(xp) / 128.0;
            for(int y=0; y<128; y++)
            {
                yp = 128 * noizeport_y + y;
                for(int z=0; z<16; z++)
                {
                    zp = 16 * mod_chunk_zpos + z;
                    //marge (way: x)
                    Wm[x][y][z] = Wp1[xp][y][zp][0] + dx * (Wp1[xp][y][zp][1] - Wp1[xp][y][zp][0]);
                    if (Wm[x][y][z] > 0)
                        data[x][yp][z] = 1;
                    else
                        data[x][yp][z] = 0;
                }
            }
        }
    }
}

double JumpVelocity(double start_velocity, double acceleration, double air_resistance, int tick)
{
    double curt_velocity, item1, item2;
    // calculate item 1
    item1 = start_velocity;
    for (int i=0; i<tick; i++)
        item1 *= (1.0 - air_resistance);

    // calculate item 2
    item2 = 1.0;
    for (int i=0; i<tick; i++)
        item2 *= (1.0 - air_resistance);
    item2 = acceleration * (1.0 - air_resistance) * (1.0 - item2) / air_resistance;

    // marge items
    curt_velocity = item1 - item2;
    return curt_velocity;
}