/* includes */
#include<GL/glut.h>
#include<GL/glpng.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<math.h>

/*define*/
#define SEED_NUM 1234567

/* functions */
// Wavelet
void Wavelet1d(double[256], double);
void Wavelet2d(double[256][256], double);
void Wavelet3d(double*, double);
// PerlinNoize
void PerlinNoize1d(double*, int, int, int, unsigned int);
void PerlinNoize2d(int[16][16], int, int, int, unsigned int);
void PerlinNoize3d(int*, int, int, int, unsigned int);
// DrawPlayer
void DrawPlayer(void);
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
void draging(int, int);
void special(int, int, int);
void specialup(int, int, int);

/* valiables */
// player position info
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

double dig_hardness = 9.0;
double block_hardness = 0.75;
double player_dig_speed = 0.05;

double dig_search_before_x, dig_search_after_x, dig_search_before_y, dig_search_after_y, dig_search_before_z, dig_search_after_z;
int dig_search_before_block_x, dig_search_before_block_z;
int dig_search_before_chunk_x, dig_search_before_chunk_z;
int dig_search_before_local_x, dig_search_before_local_z;
int dig_search_after_block_x, dig_search_after_block_z;
int dig_search_after_chunk_x, dig_search_after_chunk_z;
int dig_search_after_local_x, dig_search_after_local_z;
int dig_now_x =-1000, dig_now_y=-1000, dig_now_z=-1000;
int dig_flag = 0;

// Player model position info - {x_min, x_max, y_min, y_max, z_min, z_max}
double player_model_pos_info[3][6] = {{-0.1125, 0.1125, 0.1125, -0.5625, -0.1125, 0.1125},
                                      {-0.1125, 0.1125, -0.825, -0.15, -0.225, 0.225},
                                      {-0.225, 0.225, -0.15, 0.3, -0.225, 0.225}};
// Player model mov info - {x, y, z}
double player_model_mov_info[4][3] = {{0.0, -0.9375, 0.1125},
                                      {0.0, -0.9375, -0.1125},
                                      {0.0, -0.2625, -0.3375},
                                      {0.0, -0.2625, 0.3375}};
// Player model color info - {x, y, z}
double player_model_color_info[6][3] = {{0.0, 0.0, 0.0},
                                        {0.0, 0.0, 0.0},
                                        {0.0, 1.0, 0.0},
                                        {0.0, 1.0, 0.0},
                                        {0.0, 1.0, 1.0},
                                        {1.0, 0.5, 0.0}};
// player arm(+buttom) rotation
double arm_rad = 0.0;
int arm_d = 1;
// camera rotation
double xz_rad = 0.0;
double y_rad = 0.0;
// mouse positioning
int mouse_x = 0;
int mouse_y = 0;
// key status
int key_state[7] = {0};
// player constants param
double player_jump_start_velocity = 0.42;
double player_gravity_acceleration = 0.08;
double player_air_resistance = 0.02;
// player jumping status
int player_jump_tick = -1;
double player_jump_velocity = 0.0;
// player falling status
int player_fall_tick = -1;
double player_fall_velocity = 0.0;
// map data
double *chunkdata_x;
double *chunkdata_y;
double *chunkdata_z;
int *chunk;
int map2d[33][33][16][16];
// mouse status
double mouse_status[3] = {-1, -1, -1};
// for draw fps
struct timeval start, end;
double fps[10] = {0};
double avg_fps;
char fps_str[20] = "fps: calcutating...";
int fps_count = 0;
// camera mode
int camera_perspective_mode = 0;
// for draw info
char pos_str[40];
char block_str[40];
char chunk_str[40];
char keystate_str[60];
char loadpos_str[60];
char loadposc_str[60];
char dig_str[30];
// for debug
int flag = 0;

int main(int argc, char **argv)
{
    // require 272.25MB
    chunk = (int*)malloc(sizeof(int)*17*17*16*256*16);
    chunkdata_x = (double*)malloc(sizeof(double)*17*16);
    chunkdata_y = (double*)malloc(sizeof(double)*16*16);
    chunkdata_z = (double*)malloc(sizeof(double)*17*16);
    for (int i=0; i<17; i++)
        for (int j=0; j<17; j++)
            PerlinNoize2d(map2d[i][j], i, j, 1, SEED_NUM);
    
    for (int i=0; i<17; i++)
    {
        PerlinNoize1d(chunkdata_x+i*16, i, 0, 1, SEED_NUM);
        PerlinNoize1d(chunkdata_z+i*16, i, 2, 1, SEED_NUM);
    }
    for (int i=0; i<16; i++)
    {
        PerlinNoize1d(chunkdata_y+i*16, i, 1, 1, SEED_NUM);
    }
    for (int i=0; i<17; i++)
    {
        for (int j=0; j<17; j++)
        {
            for (int x=0; x<16; x++)
            {
                for (int z=0; z<16; z++)
                {
                    for (int y=0; y<256; y++)
                    {
                        if (*(chunkdata_x+i*16+x)+*(chunkdata_y+y)+*(chunkdata_z+j*16+z) < 0.68 && y < map2d[i][j][x][z])
                            *(chunk+i*1114112+j*65536+x*4096+y*16+z) = 1;
                        else
                            *(chunk+i*1114112+j*65536+x*4096+y*16+z) = 0;
                    }
                }
            }
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
    glutSpecialUpFunc(specialup);
    glutMouseFunc(mouse);
    glutMotionFunc(draging);
    glutPassiveMotionFunc(motion);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
    return(0);
}

void render(void)
{
    double speed = 0.21;
    double sneek_speed = 0.304;
    double dash_speed = 1.3;
    double next_player_x=player_x, next_player_z=player_z;
    int next_player_block_x, next_player_block_z;
    int next_player_chunk_x, next_player_chunk_z;
    int next_player_local_x, next_player_local_z;

    if (key_state[4] == 1)
        speed *= sneek_speed;
    else if (key_state[5] == 1)
        speed *= dash_speed;
    if (key_state[0] == 1 || key_state[1] == 1 || key_state[2] == 1 || key_state[3] == 1)
    {
        arm_rad += (24.0 * (double)arm_d) * M_PI / 360.0;
        if (arm_rad > M_PI / 3.0)
        {
            arm_rad = M_PI / 3.0;
            arm_d *= -1;
        }
        if (arm_rad < -M_PI / 3.0)
        {
            arm_rad = -M_PI / 3.0;
            arm_d *= -1;
        }
    }
    else
    {
        arm_rad = 0.0;
    }

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
    if(*(chunk+(next_player_chunk_x-player_pre_loadchunk_x)*1114112+(player_chunk_z-player_pre_loadchunk_z)*65536+(next_player_local_x)*4096+((int)(player_y - 1.5))*16+player_local_z) == 0 && *(chunk+(next_player_chunk_x-player_pre_loadchunk_x)*1114112+(player_chunk_z-player_pre_loadchunk_z)*65536+(next_player_local_x)*4096+((int)(player_y - 0.5))*16+player_local_z) == 0 && next_player_x > 16.0 && next_player_x < 880.0)
    {
        player_x = next_player_x;
        player_block_x = next_player_block_x;
        player_chunk_x = next_player_chunk_x;
        player_local_x = next_player_local_x;
    }

    next_player_block_z = (int)next_player_z;
    next_player_chunk_z = (int)next_player_block_z / 16;
    next_player_local_z = (int)next_player_block_z % 16;
    if(*(chunk+(player_chunk_x-player_pre_loadchunk_x)*1114112+(next_player_chunk_z-player_pre_loadchunk_z)*65536+(player_local_x)*4096+((int)(player_y - 1.5))*16+next_player_local_z) == 0 && *(chunk+(player_chunk_x-player_pre_loadchunk_x)*1114112+(next_player_chunk_z-player_pre_loadchunk_z)*65536+(player_local_x)*4096+((int)(player_y - 0.5))*16+next_player_local_z) == 0 && next_player_z > 16.0 && next_player_z < 880.0)
    {
        player_z = next_player_z;
        player_block_z = next_player_block_z;
        player_chunk_z = next_player_chunk_z;
        player_local_z = next_player_local_z;
    }
    if (player_chunk_x - player_pre_chunk_x >= 4 || player_chunk_x - player_pre_chunk_x <= -4 || player_chunk_z - player_pre_chunk_z >= 4 || player_chunk_z - player_pre_chunk_z <= -4)
    {
        player_pre_chunk_x = player_chunk_x;
        player_pre_loadchunk_x = player_pre_chunk_x - 8;
        if (player_pre_loadchunk_x < 0)
            player_pre_loadchunk_x = 0;
        if (player_pre_loadchunk_x > 38)
            player_pre_loadchunk_x = 38;
        player_pre_chunk_z = player_chunk_z;
        player_pre_loadchunk_z = player_pre_chunk_z - 8;
        if (player_pre_loadchunk_z < 0)
            player_pre_loadchunk_z = 0;
        if (player_pre_loadchunk_z > 38)
            player_pre_loadchunk_z = 38;
        for (int i=0; i<17; i++)
            for (int j=0; j<17; j++)
                PerlinNoize2d(map2d[i][j], i+player_pre_loadchunk_x, j+player_pre_loadchunk_z, 1, SEED_NUM);
        
        for (int i=0; i<17; i++)
        {
            PerlinNoize1d(chunkdata_x+i*16, i+player_pre_loadchunk_x, 0, 1, SEED_NUM);
            PerlinNoize1d(chunkdata_z+i*16, i+player_pre_loadchunk_z, 2, 1, SEED_NUM);
        }
        for (int i=0; i<17; i++)
        {
            for (int j=0; j<17; j++)
            {
                for (int x=0; x<16; x++)
                {
                    for (int z=0; z<16; z++)
                    {
                        for (int y=0; y<256; y++)
                        {
                            if (*(chunkdata_x+i*16+x)+*(chunkdata_y+y)+*(chunkdata_z+j*16+z) < 0.68 && y < map2d[i][j][x][z])
                                *(chunk+i*1114112+j*65536+x*4096+y*16+z) = 1;
                            else
                                *(chunk+i*1114112+j*65536+x*4096+y*16+z) = 0;
                        }
                    }
                }
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
    if (player_fall_tick == -1 && player_jump_tick == -1 && *(chunk+(player_chunk_x-player_pre_loadchunk_x)*1114112+(player_chunk_z-player_pre_loadchunk_z)*65536+(player_local_x)*4096+((int)(player_y - 1.5))*16+player_local_z) == 0)
        player_fall_tick = 0;

    if (player_fall_tick >= 0)
    {
        player_fall_velocity = JumpVelocity(0.0, player_gravity_acceleration, player_air_resistance, player_fall_tick);
        player_y += player_fall_velocity;
        player_block_y = (int)(player_y - 1.5);
        player_fall_tick ++;
        if (*(chunk+(player_chunk_x-player_pre_loadchunk_x)*1114112+(player_chunk_z-player_pre_loadchunk_z)*65536+(player_local_x)*4096+((int)(player_y - 1.5))*16+player_local_z) == 1)
        {
            player_y = (double)player_block_y + 2.5;
            player_block_y = (int)(player_y - 1.5);
            player_chunk_y = player_block_y / 16;
            player_local_y = player_block_y % 16;
            player_fall_tick = -1;
        }
    }

    if (player_jump_tick >= 0)
    {
        player_jump_velocity = JumpVelocity(player_jump_start_velocity, player_gravity_acceleration, player_air_resistance, player_jump_tick);
        player_y += player_jump_velocity;
        player_block_y = (int)(player_y - 1.5);
        player_jump_tick ++;
        if (*(chunk+(player_chunk_x-player_pre_loadchunk_x)*1114112+(player_chunk_z-player_pre_loadchunk_z)*65536+(player_local_x)*4096+((int)(player_y - 1.5))*16+player_local_z) == 1)
        {
            player_y = (double)player_block_y + 2.5;
            player_block_y = (int)(player_y - 1.5);
            player_chunk_y = player_block_y / 16;
            player_local_y = player_block_y % 16;
            player_jump_tick = -1;
        }
    }

    // DIG
    if (mouse_status[1] == 1)
    {
        dig_flag = 0;
        for(double a=0; a<5.0; a+= 0.05)
        {
            dig_search_after_x = player_x+a*cos(xz_rad)*cos(y_rad);
            dig_search_after_block_x = (int)dig_search_after_x;
            dig_search_after_chunk_x = dig_search_after_block_x / 16;
            dig_search_after_local_x = dig_search_after_block_x % 16;
            dig_search_after_y = player_y-a*sin(y_rad);
            dig_search_after_z = player_z+a*sin(xz_rad)*cos(y_rad);
            dig_search_after_block_z = (int)dig_search_after_z;
            dig_search_after_chunk_z = dig_search_after_block_z / 16;
            dig_search_after_local_z = dig_search_after_block_z % 16;
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 1)
            {
                if (dig_now_x == (int)dig_search_after_x && dig_now_y == (int)dig_search_after_y && dig_now_z == (int)dig_search_after_z)
                    dig_hardness -= player_dig_speed;
                else
                {
                    dig_now_x = (int)dig_search_after_x;
                    dig_now_y = (int)dig_search_after_y;
                    dig_now_z = (int)dig_search_after_z;
                    dig_hardness = block_hardness;
                }
                if (dig_hardness <= 0.0)
                    *(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) = 0;
                dig_flag = 1;
                break;
            }
            dig_search_before_x = dig_search_after_x;
            dig_search_before_block_x = (int)dig_search_before_x;
            dig_search_before_chunk_x = dig_search_before_block_x / 16;
            dig_search_before_local_x = dig_search_before_block_x % 16;
            dig_search_before_y = dig_search_after_y;
            dig_search_before_z = dig_search_after_z;
            dig_search_before_block_z = (int)dig_search_before_z;
            dig_search_before_chunk_z = dig_search_before_block_z / 16;
            dig_search_before_local_z = dig_search_before_block_z % 16;
        }
    }
    else
    {
        dig_hardness = block_hardness;
        dig_flag = 0;
    }

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawBuffer(GL_FRONT);

    // Camera setting
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    if (camera_perspective_mode == 0)
    	gluLookAt(player_x, player_y, player_z, player_x+5.0*cos(xz_rad)*cos(y_rad), player_y-5.0*sin(y_rad), player_z+5.0*sin(xz_rad)*cos(y_rad), 0.0, 1.0, 0.0);
    else if (camera_perspective_mode == 1)
    	gluLookAt(player_x+5.0*cos(xz_rad)*cos(y_rad), player_y-5.0*sin(y_rad), player_z+5.0*sin(xz_rad)*cos(y_rad), player_x, player_y, player_z, 0.0, 1.0, 0.0);
    else if (camera_perspective_mode == 2)
    	gluLookAt(player_x-5.0*cos(xz_rad)*cos(y_rad), player_y-5.0*sin(y_rad), player_z-5.0*sin(xz_rad)*cos(y_rad), player_x, player_y, player_z, 0.0, 1.0, 0.0);

    // Draw
    glEnable(GL_DEPTH_TEST);
    glBegin(GL_QUADS);
        DrawPlayer();
        double dx, dy, dz;
        int draw_xmin, draw_zmin;
        int draw_xmax, draw_zmax;
        if (player_chunk_x - player_pre_loadchunk_x >= 4)
            draw_xmin = player_chunk_x - player_pre_loadchunk_x - 4;
        else
            draw_xmin = 0;
        if (player_chunk_z - player_pre_loadchunk_z >= 4)
            draw_zmin = player_chunk_z - player_pre_loadchunk_z - 4;
        else
            draw_zmin = 0;
        if (player_chunk_x - player_pre_loadchunk_x < 12)
            draw_xmax = player_chunk_x - player_pre_loadchunk_x + 5;
        else
            draw_xmax = 17;
        if (player_chunk_z - player_pre_loadchunk_z < 12)
            draw_zmax = player_chunk_z - player_pre_loadchunk_z + 5;
        else
            draw_zmax = 17;
        for(int i=draw_xmin; i<draw_xmax; i++)
        {
            for(int j=draw_zmin; j<draw_zmax; j++)
            {
                for (int x = 0; x < 16; x++)
                {
                    dx = (double)(x+(player_pre_loadchunk_x+i)*16);
                    for (int z = 0; z < 16; z++)
                    {
                        dz = (double)(z+(player_pre_loadchunk_z+j)*16);
                        for (int y = 0; y < 256; y ++)
                        {
                            if (*(chunk+i*1114112+j*65536+x*4096+y*16+z) == 1)
                            {
                                dy = (double) y;
                                // x-Higher
                                if (x == 15 || (x < 15 && *(chunk+i*1114112+j*65536+(x+1)*4096+y*16+z) == 0))
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
                                if (y == 255 || (y < 255 && *(chunk+i*1114112+j*65536+x*4096+(y+1)*16+z) == 0))
                                {
                                    if (player_pre_loadchunk_x+i > 0 && player_pre_loadchunk_z+j > 0 && player_pre_loadchunk_x+i < 55 && player_pre_loadchunk_z+j < 55)
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
                                }
                                // z-Higher
                                if (z == 15 || (z < 15 && *(chunk+i*1114112+j*65536+x*4096+y*16+z+1) == 0))
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
                                if (x == 0 || (x > 0 && *(chunk+i*1114112+j*65536+(x-1)*4096+y*16+z) == 0))
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
                                if (y == 0 || (y > 0 && *(chunk+i*1114112+j*65536+x*4096+(y-1)*16+z) == 0))
                                {
                                    glColor3f(0.64, 0.41, 0.25);
                                    glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                    glColor3f(0.58, 0.37, 0.22);
                                    glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                    glColor3f(0.51, 0.33, 0.2);
                                    glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                                    glColor3f(0.58, 0.37, 0.22);
                                    glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                                }

                                // z-Lower
                                if (z == 0 || (z > 0 && *(chunk+i*1114112+j*65536+x*4096+y*16+z-1) == 0))
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
                            }
                        }
                    }
                }
            }
        }
    glEnd();
    if (dig_flag == 1)
    {
        double bar_length = (dig_hardness / block_hardness) * 0.8 - 0.4;
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_QUADS);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad)*0.4, (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 - cos(xz_rad)*0.4);
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad)*bar_length, (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 + cos(xz_rad)*bar_length);
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad)*bar_length, (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 + cos(xz_rad)*bar_length);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad)*0.4, (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 - cos(xz_rad)*0.4);
        glEnd();
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad)*(-bar_length), (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 - cos(xz_rad)*(-bar_length));
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad)*0.4, (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 + cos(xz_rad)*0.4);
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad)*0.4, (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 + cos(xz_rad)*0.4);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad)*(-bar_length), (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 - cos(xz_rad)*(-bar_length));
        glEnd();
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
    glLineWidth(3.0);
    int itembox_x = w / 2 - 288;
    int itembox_y = h - 128;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        for (int i=0; i<9; i++)
        {
            glVertex2i(itembox_x +  64*i    , itembox_y);
            glVertex2i(itembox_x +  64*(i+1), itembox_y);
            glVertex2i(itembox_x +  64*(i+1), itembox_y);
            glVertex2i(itembox_x +  64*(i+1), itembox_y + 64);
            glVertex2i(itembox_x +  64*(i+1), itembox_y + 64);
            glVertex2i(itembox_x +  64*i    , itembox_y + 64);
            glVertex2i(itembox_x +  64*i    , itembox_y + 64);
            glVertex2i(itembox_x +  64*i    , itembox_y);
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
    glRasterPos2i(0, 16*16);
    for(int j=0; j<30; j++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, dig_str[j]);
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
    sprintf(dig_str, "BLOCK HARD(DIGING): %.2lf", dig_hardness);
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
        key_state[3] = 1;
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
        key_state[3] = 0;
    else if (c == ' ')
        key_state[6] = 0;
}

void special(int c, int x, int y)
{
}
void specialup(int c, int x, int y)
{
    switch (c)
    {
        case GLUT_KEY_F5:
            camera_perspective_mode ++;
            camera_perspective_mode %= 3;
            break;
    }
}

void mouse(int button, int state, int x, int y)
{
    double before_x, after_x, before_y, after_y, before_z, after_z;
    int view_block_x, view_block_z;
    int view_chunk_x, view_chunk_z;
    int view_local_x, view_local_z;
    int next_view_block_x, next_view_block_z;
    int next_view_chunk_x, next_view_chunk_z;
    int next_view_local_x, next_view_local_z;
    before_x = player_x;
    view_block_x = (int)before_x;
    view_chunk_x = view_block_x / 16;
    view_local_x = view_block_x % 16;
    before_y = player_y;
    before_z = player_z;
    view_block_z = (int)before_z;
    view_chunk_z = view_block_z / 16;
    view_local_z = view_block_z % 16;
    if (button == GLUT_RIGHT_BUTTON)
    {
        mouse_status[0] *= -1;
        if (mouse_status[0] == 1)
        {
            for(double a=0; a<5.0; a+= 0.05)
            {
                after_x = player_x+a*cos(xz_rad)*cos(y_rad);
                next_view_block_x = (int)after_x;
                next_view_chunk_x = next_view_block_x / 16;
                next_view_local_x = next_view_block_x % 16;
                after_y = player_y-a*sin(y_rad);
                after_z = player_z+a*sin(xz_rad)*cos(y_rad);
                next_view_block_z = (int)after_z;
                next_view_chunk_z = next_view_block_z / 16;
                next_view_local_z = next_view_block_z % 16;
                if(*(chunk+(next_view_chunk_x-player_pre_loadchunk_x)*1114112+(next_view_chunk_z-player_pre_loadchunk_z)*65536+(next_view_local_x)*4096+(int)(after_y)*16+next_view_local_z) == 1)
                {
                    if((int)after_x != (int)before_x && ((int) before_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int) after_z != (int)player_z))
                        *(chunk+(view_chunk_x-player_pre_loadchunk_x)*1114112+(next_view_chunk_z-player_pre_loadchunk_z)*65536+(view_local_x)*4096+(int)(after_y)*16+next_view_local_z) = 1;
                    else if ((int)after_y != (int)before_y && ((int) after_x != (int)player_x || ((int)before_y != (int)(player_y - 1.5) && (int)before_y != (int)(player_y - 0.5)) || (int) after_z != (int)player_z))
                        *(chunk+(next_view_chunk_x-player_pre_loadchunk_x)*1114112+(next_view_chunk_z-player_pre_loadchunk_z)*65536+(next_view_local_x)*4096+(int)(before_y)*16+next_view_local_z) = 1;
                    else if ((int)after_z != (int)before_z && ((int) after_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int) before_z != (int)player_z))
                        *(chunk+(next_view_chunk_x-player_pre_loadchunk_x)*1114112+(view_chunk_z-player_pre_loadchunk_z)*65536+(next_view_local_x)*4096+(int)(after_y)*16+view_local_z) = 1;
                    break;
                }
                before_x = after_x;
                view_block_x = (int)before_x;
                view_chunk_x = view_block_x / 16;
                view_local_x = view_block_x % 16;
                before_y = after_y;
                before_z = after_z;
                view_block_z = (int)before_z;
                view_chunk_z = view_block_z / 16;
                view_local_z = view_block_z % 16;
            }
        }
    }
    else if (button == GLUT_LEFT_BUTTON)
        mouse_status[1] *= -1;
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

void draging(int x, int y)
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
void Wavelet1d(double data[32], double amp)
{
    double x_val;
    double dx;
    for(int x=-32; x<32; x++)
    {
        dx = (double)x / 32.0;
        if (x > 0)
            x_val = 1.0 - 3.0 * (dx * dx) + 2.0 * (dx * dx * dx);
        else
            x_val = 1.0 - 3.0 * (dx * dx) - 2.0 * (dx * dx * dx);
        data[x+32] = x_val * amp;
    }
}

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
                z_val = 1.0 - 3.0 * (dz * dz) - 2.0 * (dz * dz * dz);
            data[x+128][z+128] = x_val * z_val * amp;
        }
    }
}

void Wavelet3d(double *data, double amp)
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
                *(data+(x+128)*65536+(y+128)*256+z+128) = x_val * y_val * z_val * amp;
            }
        }
    }
}

// PerlinNoize
void PerlinNoize1d(double* data, int chunk_xpos, int direction, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 2;
    int mod_chunk_xpos = chunk_xpos % 2;

    double C[64];                     // Wavelet data
    double W[32][2];                  // Graph data for marge
    double a[29][3];                      // Gradient data

    double dx;

    // Wavelet function
    Wavelet1d(C, 5.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for(int i=0; i<8; i++)
    {
        a[i][0] = (double)rand() / RAND_MAX;
        a[i][1] = (double)rand() / RAND_MAX;
        a[i][2] = (double)rand() / RAND_MAX;
    }

    for(int x=0; x<32; x++)
    {
        dx = (double)x / 32.0;
        // Calculate Graphs
        W[x][0] = a[noizeport_x][direction] * dx * C[x+32];
        W[x][1] = a[noizeport_x+1][direction] * (dx-1.0) * C[x];
    }

    int xp;
    for(int x=0; x<16; x++)
    {
        xp = 16 * mod_chunk_xpos + x;
        dx = (double)(xp) / 32.0;
        *(data+x) = W[xp][0] + dx * (W[xp][1] - W[xp][0]);
    }
}

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
    Wavelet2d(C, 128.0);

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



void PerlinNoize3d(int* data, int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 8;
    int noizeport_z = chunk_zpos / 8;
    int mod_chunk_xpos = chunk_xpos % 8;
    int mod_chunk_zpos = chunk_zpos % 8;

    // require 400MB + a (memory)
    double *C = (double*)malloc(sizeof(double)*256*256*256);        // Wavelet data
    double *W = (double*)malloc(sizeof(double)*128*128*128*8);      // Graph data for marge
    double *Wp0 = (double*)malloc(sizeof(double)*128*128*128*4);    // Graph data (marge z)
    double *Wp1 = (double*)malloc(sizeof(double)*128*128*128*2);    // Graph data (marge z)
    double *Wm = (double*)malloc(sizeof(double)*128*128*128);       // Graph data (marge z)
    double *a = (double*)malloc(sizeof(double)*8*2*8*3);            // Gradient data

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
                *(a+i*48+j*24+k*3+0) = (double)rand() / RAND_MAX;
                *(a+i*48+j*24+k*3+1) = (double)rand() / RAND_MAX;
                *(a+i*48+j*24+k*3+2) = (double)rand() / RAND_MAX;
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
                    *(W+x*131072+y*1024+z*8+0) = *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+1) = *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+2) = *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+3) = *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+4) = *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+5) = *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+6) = *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+7) = *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);

                    //marge (way: z)
                    *(Wp0+x*65536+y*512+z*4+0) = *(W+x*131072+y*1024+z*8+0) + dz * (*(W+x*131072+y*1024+z*8+1) - *(W+x*131072+y*1024+z*8+0));
                    *(Wp0+x*65536+y*512+z*4+1) = *(W+x*131072+y*1024+z*8+2) + dz * (*(W+x*131072+y*1024+z*8+3) - *(W+x*131072+y*1024+z*8+2));
                    *(Wp0+x*65536+y*512+z*4+2) = *(W+x*131072+y*1024+z*8+4) + dz * (*(W+x*131072+y*1024+z*8+5) - *(W+x*131072+y*1024+z*8+4));
                    *(Wp0+x*65536+y*512+z*4+3) = *(W+x*131072+y*1024+z*8+6) + dz * (*(W+x*131072+y*1024+z*8+7) - *(W+x*131072+y*1024+z*8+6));

                    //marge (way: y)
                    *(Wp1+x*32768+y*256+z*2+0) = *(Wp0+x*65536+y*512+z*4+0) + dy * (*(Wp0+x*65536+y*512+z*4+1) - *(Wp0+x*65526+y*512+z*4+0));
                    *(Wp1+x*32768+y*256+z*2+1) = *(Wp0+x*65536+y*512+z*4+2) + dy * (*(Wp0+x*65536+y*512+z*4+3) - *(Wp0+x*65526+y*512+z*4+2));
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
                    *(Wm+x*16384+y*128+z) = *(Wp1+xp*32768+y*256+zp*2+0) + dx * (*(Wp1+xp*32768+y*256+zp*2+1) - *(Wp1+xp*32768+y*256+zp*2+0));
                    if (*(Wm+x*16384+y*128+z) > 0)
                        *(data+x*2048+yp*16+z) = 1;
                    else
                        *(data+chunk_xpos*2162688+chunk_zpos*65536+x*4096+yp*16+z) = 0;
                }
            }
        }
    }
    free(C);
    free(W);
    free(Wp0);
    free(Wp1);
    free(Wm);
    free(a);
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

void DrawPlayer(void)
{
    if(camera_perspective_mode != 0)
    {
        double px[8], pfx[8], bx[8], bfx[8][2];
        double pz[8], pfz[8], bz[8], bfz[8][2];
        double pfy[8], by[8], bfy[8][2];
        double py1, py2;

        // load rot model
        for(int i=0; i<8; i++)
        {
            bx[i] = player_model_pos_info[0][i%2];
            by[i] = player_model_pos_info[0][2+(i/4)];
            bz[i] = player_model_pos_info[0][4+((i/2)%2)];
        }

        for(int i=0; i<8; i++)
        {
            bfx[i][0] = bx[i] * cos(arm_rad) - by[i] * sin(arm_rad);
            bfy[i][0] = bx[i] * sin(arm_rad) + by[i] * cos(arm_rad);
            bfx[i][1] = bx[i] * cos(-arm_rad) - by[i] * sin(-arm_rad);
            bfy[i][1] = bx[i] * sin(-arm_rad) + by[i] * cos(-arm_rad);
            bfz[i][0] = bz[i];
            bfz[i][1] = bz[i];
        }

        // Player -rot model-
        // buttom1 -> buttom2 -> arm2 -> arm1
        for (int i=0; i<4; i++)
        {
            for(int j=0; j<8; j++)
            {
                if (i%2 == 0)
                {
                    px[j] = player_model_mov_info[i][0] + bfx[j][0];
                    pfy[j] = player_model_mov_info[i][1] + bfy[j][0];
                    pz[j] = player_model_mov_info[i][2] + bfz[j][0];
                }
                else
                {
                    px[j] = player_model_mov_info[i][0] + bfx[j][1];
                    pfy[j] = player_model_mov_info[i][1] + bfy[j][1];
                    pz[j] = player_model_mov_info[i][2] + bfz[j][1];                    
                }
                pfx[j] = px[j] * cos(xz_rad) - pz[j] * sin(xz_rad);
                pfz[j] = px[j] * sin(xz_rad) + pz[j] * cos(xz_rad);
            }
            glColor3f(player_model_color_info[i][0], player_model_color_info[i][1], player_model_color_info[i][2]);
            glVertex3f(player_x+pfx[3], player_y+pfy[3], player_z+pfz[3]);
            glVertex3f(player_x+pfx[2], player_y+pfy[2], player_z+pfz[2]);
            glVertex3f(player_x+pfx[6], player_y+pfy[6], player_z+pfz[6]);
            glVertex3f(player_x+pfx[7], player_y+pfy[7], player_z+pfz[7]);

            glVertex3f(player_x+pfx[5], player_y+pfy[5], player_z+pfz[5]);
            glVertex3f(player_x+pfx[4], player_y+pfy[4], player_z+pfz[4]);
            glVertex3f(player_x+pfx[6], player_y+pfy[6], player_z+pfz[6]);
            glVertex3f(player_x+pfx[7], player_y+pfy[7], player_z+pfz[7]);

            glVertex3f(player_x+pfx[0], player_y+pfy[0], player_z+pfz[0]);
            glVertex3f(player_x+pfx[4], player_y+pfy[4], player_z+pfz[4]);
            glVertex3f(player_x+pfx[6], player_y+pfy[6], player_z+pfz[6]);
            glVertex3f(player_x+pfx[2], player_y+pfy[2], player_z+pfz[2]);

            glVertex3f(player_x+pfx[1], player_y+pfy[1], player_z+pfz[1]);
            glVertex3f(player_x+pfx[0], player_y+pfy[0], player_z+pfz[0]);
            glVertex3f(player_x+pfx[4], player_y+pfy[4], player_z+pfz[4]);
            glVertex3f(player_x+pfx[5], player_y+pfy[5], player_z+pfz[5]);

            glVertex3f(player_x+pfx[1], player_y+pfy[1], player_z+pfz[1]);
            glVertex3f(player_x+pfx[0], player_y+pfy[0], player_z+pfz[0]);
            glVertex3f(player_x+pfx[2], player_y+pfy[2], player_z+pfz[2]);
            glVertex3f(player_x+pfx[3], player_y+pfy[3], player_z+pfz[3]);

            glVertex3f(player_x+pfx[1], player_y+pfy[1], player_z+pfz[1]);
            glVertex3f(player_x+pfx[5], player_y+pfy[5], player_z+pfz[5]);
            glVertex3f(player_x+pfx[7], player_y+pfy[7], player_z+pfz[7]);
            glVertex3f(player_x+pfx[3], player_y+pfy[3], player_z+pfz[3]);
        }

        // Player -non-rot model-
        // body -> head
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                px[j] = player_model_pos_info[i+1][j%2];
                pz[j] = player_model_pos_info[i+1][4+j/2];
                pfx[j] = px[j] * cos(xz_rad) - pz[j] * sin(xz_rad);
                pfz[j] = px[j] * sin(xz_rad) + pz[j] * cos(xz_rad);
            }
            py1 = player_model_pos_info[i+1][2];
            py2 = player_model_pos_info[i+1][3];

            glColor3f(player_model_color_info[i+4][0], player_model_color_info[i+4][1], player_model_color_info[i+4][2]);
            glVertex3f(player_x+pfx[3], player_y+py1, player_z+pfz[3]);
            glVertex3f(player_x+pfx[2], player_y+py1, player_z+pfz[2]);
            glVertex3f(player_x+pfx[2], player_y+py2, player_z+pfz[2]);
            glVertex3f(player_x+pfx[3], player_y+py2, player_z+pfz[3]);

            glVertex3f(player_x+pfx[1], player_y+py2, player_z+pfz[1]);
            glVertex3f(player_x+pfx[0], player_y+py2, player_z+pfz[0]);
            glVertex3f(player_x+pfx[2], player_y+py2, player_z+pfz[2]);
            glVertex3f(player_x+pfx[3], player_y+py2, player_z+pfz[3]);

            glVertex3f(player_x+pfx[0], player_y+py1, player_z+pfz[0]);
            glVertex3f(player_x+pfx[0], player_y+py2, player_z+pfz[0]);
            glVertex3f(player_x+pfx[2], player_y+py2, player_z+pfz[2]);
            glVertex3f(player_x+pfx[2], player_y+py1, player_z+pfz[2]);

            glVertex3f(player_x+pfx[1], player_y+py1, player_z+pfz[1]);
            glVertex3f(player_x+pfx[0], player_y+py1, player_z+pfz[0]);
            glVertex3f(player_x+pfx[0], player_y+py2, player_z+pfz[0]);
            glVertex3f(player_x+pfx[1], player_y+py2, player_z+pfz[1]);

            glVertex3f(player_x+pfx[1], player_y+py1, player_z+pfz[1]);
            glVertex3f(player_x+pfx[0], player_y+py1, player_z+pfz[0]);
            glVertex3f(player_x+pfx[2], player_y+py1, player_z+pfz[2]);
            glVertex3f(player_x+pfx[3], player_y+py1, player_z+pfz[3]);

            glVertex3f(player_x+pfx[1], player_y+py1, player_z+pfz[1]);
            glVertex3f(player_x+pfx[1], player_y+py2, player_z+pfz[1]);
            glVertex3f(player_x+pfx[3], player_y+py2, player_z+pfz[3]);
            glVertex3f(player_x+pfx[3], player_y+py1, player_z+pfz[3]);
        }
    }
}