/* includes */
#include<GL/glut.h>
#include<GL/glpng.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<math.h>

/*define*/
#define SEED_NUM 20183140
#define WORLD_CHUNK 64
#define DRAW_DIST 8
//#define NEWGAME

/* functions */
// Wavelet
void Wavelet1d(double[256], double);
void Wavelet2d(double[256][256], double);
// PerlinNoize
void PerlinNoize1d(double*, int, int, int, unsigned int);
void PerlinNoize2di(int*, int, int, int, unsigned int, int);
void PerlinNoize2dd(double*, int, int, int, unsigned int, int);
// World Create
void CreateWorld();
// Save/Load
void SaveChunk(int*, int, int);
void LoadChunk(int*, int, int);
void LoadItem(int*);
void SaveItem(int*);
void SaveStatus(int*);
void LoadStatus(int*);
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

double resporn_player_x = 136.0;
double resporn_player_y = 96.0;
double resporn_player_z = 136.0;

int player_block_x = 136;
int player_block_y = 86;
int player_block_z = 136;

int player_pre_loadchunk_x = 0;
int player_pre_loadchunk_z = 0;

int player_pre_chunk_x = 8;
int player_pre_chunk_z = 8;

int player_pre_block_y;

int player_chunk_x = 8;
int player_chunk_y = 5;
int player_chunk_z = 8;

int player_local_x = 8;
int player_local_y = 6;
int player_local_z = 8;

double dig_hardness = 9.0;
double soil_hardness = 0.75;
double sand_hardness = 0.75;
double rock_hardness = 7.5;
double diamond_hardness = 15.0;
double tree_hardness = 3.0;
double leaf_hardness = 0.3;

double hungry_time = 1.0;
double hungry = 0.0;

double eating_time = 0.0;
double eat_time = 1.0;

int no_eat = 1;
double no_eating_time = 0.0;
double no_eat_time = 1.0;

int dig_tag = 0;
double player_dig_speed = 0.075;

double diamond_create_rate = 0.0013;
double wood_create_rate = 0.01;
double apple_drop_rate = 0.05;
int apple_hungry = 4;
double apple_hide_hungry = 2.4;

double bar_length;

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

// block color info - {r, g, b, a}
double bci[12][6][4] = {{{0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}},
                        {{0.66, 0.43, 0.18, 1.00}, {0.00, 1.00, 0.00, 1.00}, {0.48, 0.34, 0.14, 1.00}, {0.34, 0.22, 0.09, 1.00}, {0.64, 0.41, 0.25, 1.00}, {0.42, 0.27, 0.11, 1.00}},
                        {{0.53, 0.53, 0.53, 1.00}, {0.74, 0.74, 0.74, 1.00}, {0.47, 0.47, 0.47, 1.00}, {0.38, 0.38, 0.38, 1.00}, {0.24, 0.24, 0.24, 1.00}, {0.30, 0.30, 0.30, 1.00}},
                        {{0.33, 0.70, 0.72, 1.00}, {0.45, 0.93, 0.96, 1.00}, {0.25, 0.52, 0.54, 1.00}, {0.18, 0.39, 0.41, 1.00}, {0.14, 0.29, 0.30, 1.00}, {0.11, 0.22, 0.23, 1.00}},
                        {{0.09, 0.09, 0.09, 1.00}, {0.09, 0.09, 0.09, 1.00}, {0.09, 0.09, 0.09, 1.00}, {0.09, 0.09, 0.09, 1.00}, {0.09, 0.09, 0.09, 1.00}, {0.09, 0.09, 0.09, 1.00}},
                        {{0.78, 0.73, 0.61, 1.00}, {0.83, 0.78, 0.65, 1.00}, {0.73, 0.68, 0.57, 1.00}, {0.68, 0.64, 0.53, 1.00}, {0.58, 0.54, 0.46, 1.00}, {0.63, 0.59, 0.49, 1.00}},
                        {{0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}},
                        {{0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}},
                        {{0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}},
                        {{0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}, {0.00, 0.00, 0.00, 0.00}},
                        {{0.56, 0.29, 0.22, 1.00}, {0.89, 0.55, 0.39, 1.00}, {0.51, 0.26, 0.20, 1.00}, {0.51, 0.26, 0.20, 1.00}, {0.46, 0.24, 0.18, 1.00}, {0.41, 0.22, 0.16, 1.00}},
                        {{0.13, 0.49, 0.39, 1.00}, {0.15, 0.54, 0.43, 1.00}, {0.12, 0.44, 0.35, 1.00}, {0.11, 0.39, 0.31, 1.00}, {0.09, 0.34, 0.27, 1.00}, {0.08, 0.29, 0.23, 1.00}}};

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
int *nextchunk;
int *map2d;
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
char items_str[3];
char ht_str[30];
char hht_str[30];
char evtr_str[38];
char infomation[60];
// for debug
int flag = 0;
int debugmode = 0;
// status
int* player_bag;
int* player_status;
double player_hide_hungry = 0;

// pause
int pause = 0;
int enventory = 0;

// swap
int dragging_item = 0;

int main(int argc, char **argv)
{
    // require 272.25MB
    chunk = (int*)malloc(sizeof(int)*(2*DRAW_DIST+1)*(2*DRAW_DIST+1)*16*256*16);
    nextchunk = (int*)malloc(sizeof(int)*(2*DRAW_DIST+1)*(2*DRAW_DIST+1)*16*256*16);
    map2d = (int*)malloc(sizeof(int)*(2*DRAW_DIST+1)*(2*DRAW_DIST+1)*16*16);    
    chunkdata_x = (double*)malloc(sizeof(double)*(2*DRAW_DIST+1)*16);
    chunkdata_y = (double*)malloc(sizeof(double)*(2*DRAW_DIST+1)*16);
    chunkdata_z = (double*)malloc(sizeof(double)*(2*DRAW_DIST+1)*16);
    player_bag = (int*)malloc(sizeof(int)*72);
    player_status = (int*)malloc(sizeof(int)*2);
    #ifdef NEWGAME
        CreateWorld();
        for(int i=0; i<72; i++)
            *(player_bag+i) = 0;
        *(player_status+0) = 20;
        *(player_status+1) = 18;
    #else
        LoadItem(player_bag);
        LoadStatus(player_status);
    #endif
    for (int i=0; i<2*DRAW_DIST+1; i++)
        for (int j=0; j<2*DRAW_DIST+1; j++)
            LoadChunk(chunk+i*1114112+j*65536, i, j);
    /* TODO: resporn argorizm
    player_y = (double)*(map2d+4352*player_chunk_x+256*player_chunk_z+16*player_local_x+player_local_z) + 1.5;
    player_block_y = (int)(player_y - 1.5);
    */
    // Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(640, 480);

    // Create Window
    glutEnterGameMode ();
    
    // ClearColor
    glClearColor(0.0, 1.0, 1.0, 1.0);

    // Functions
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
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
    if (pause == 0)
    {
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
    }
    if (player_y < -2.0 || *(player_status) <= 0)
    {
        next_player_x = resporn_player_x;
        player_y = resporn_player_y;
        next_player_z = resporn_player_z;
        *(player_status) = 20;
        *(player_status+1) = 18;
    }
    if (key_state[6] == 1 && player_jump_tick == -1 && player_fall_tick == -1 && pause == 0)
    {
        player_jump_tick = 0;
        hungry += 0.025;
        player_pre_block_y = player_block_y;
    }

    if (pause == 0)
    {
        if (key_state[0] == 1 || key_state[1] == 1 || key_state[2] == 1 || key_state[3] == 1)
        {
            if (key_state[4] == 1)
                if (player_jump_tick != -1)
                    hungry += 0.02;
                else
                    hungry += 0.16;
            else
                hungry += 0.002;
        }
        else
        {
            if (key_state[4] == 1)
                hungry += 0.10;
        }
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

    // reload chunk    
    if (player_chunk_x - player_pre_chunk_x >= 4 || player_chunk_x - player_pre_chunk_x <= -4 || player_chunk_z - player_pre_chunk_z >= 4 || player_chunk_z - player_pre_chunk_z <= -4)
    {
        for (int i=0; i<2*DRAW_DIST+1; i++)
            for (int j=0; j<2*DRAW_DIST+1; j++)
                SaveChunk(chunk+i*1114112+j*65536, i+player_pre_loadchunk_x, j+player_pre_loadchunk_z);
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
        if (player_pre_loadchunk_z > WORLD_CHUNK-(2*DRAW_DIST+1))
            player_pre_loadchunk_z = WORLD_CHUNK-(2*DRAW_DIST+1);
        for (int i=0; i<2*DRAW_DIST+1; i++)
            for (int j=0; j<2*DRAW_DIST+1; j++)
                LoadChunk(chunk+i*1114112+j*65536, i+player_pre_loadchunk_x, j+player_pre_loadchunk_z);
    }
    while(1)
    {
        if(player_hide_hungry > 0)
        {
            if (player_hide_hungry > hungry)
            {
                player_hide_hungry -= hungry;
                hungry = 0.0;
            }
            else
            {
                player_hide_hungry = 0.0;
                hungry -= player_hide_hungry;
            }
        }
        if (hungry < hungry_time)
            break;
        else
        {
            if (*(player_status+1) > 0)
            {
                *(player_status+1) -= 1;
            }
            hungry -= hungry_time;
        }
    }

    if (*(player_status+1) > 0)
    {
        no_eat = 0;
    }
    else
    {
        no_eat = 1;
    }
    if (no_eat == 1)
    {
        no_eating_time += 0.05;
        if (no_eating_time > no_eat_time)
        {
            *(player_status) -= 1;
            no_eating_time -= no_eat_time;
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
    {
        player_fall_tick = 0;
        player_pre_block_y = player_block_y;
    }

    if (player_fall_tick >= 0)
    {
        player_fall_velocity = JumpVelocity(0.0, player_gravity_acceleration, player_air_resistance, player_fall_tick);
        player_y += player_fall_velocity;
        player_block_y = (int)(player_y - 1.5);
        player_fall_tick ++;
        if (*(chunk+(player_chunk_x-player_pre_loadchunk_x)*1114112+(player_chunk_z-player_pre_loadchunk_z)*65536+(player_local_x)*4096+((int)(player_y - 1.5))*16+player_local_z) != 0)
        {
            player_y = (double)player_block_y + 2.5;
            player_block_y = (int)(player_y - 1.5);
            player_chunk_y = player_block_y / 16;
            player_local_y = player_block_y % 16;
            player_fall_tick = -1;
            if (player_pre_block_y > player_block_y+3)
                *(player_status) -= player_pre_block_y - (player_block_y+3);
        }
    }

    if (player_jump_tick >= 0)
    {
        player_jump_velocity = JumpVelocity(player_jump_start_velocity, player_gravity_acceleration, player_air_resistance, player_jump_tick);
        player_y += player_jump_velocity;
        player_block_y = (int)(player_y - 1.5);
        player_jump_tick ++;
        if (*(chunk+(player_chunk_x-player_pre_loadchunk_x)*1114112+(player_chunk_z-player_pre_loadchunk_z)*65536+(player_local_x)*4096+((int)(player_y - 1.5))*16+player_local_z) != 0)
        {
            player_y = (double)player_block_y + 2.5;
            player_block_y = (int)(player_y - 1.5);
            player_chunk_y = player_block_y / 16;
            player_local_y = player_block_y % 16;
            player_jump_tick = -1;
            if (player_pre_block_y+1 > player_block_y+3)
                *(player_status) -= (player_pre_block_y+1) - (player_block_y+3);
        }
    }

    // EAT
    if (mouse_status[0] == 1)
    {
        if (*(player_bag+2*debugmode) == 1025 && *(player_bag+2*debugmode+1) > 0)
        {
            eating_time += 0.05;
            if (eating_time > eat_time)
            {
                *(player_bag+2*debugmode+1) -= 1;
                eating_time = 0;
                *(player_status+1) += apple_hungry;
                if (*(player_status+1) > 18)
                    *(player_status+1) = 18;
                player_hide_hungry += apple_hide_hungry;
                if (player_hide_hungry > *(player_status+1))
                    player_hide_hungry = (double)(*(player_status+1));

                if (*(player_bag+2*debugmode+1) == 0)
                {
                    *(player_bag+2*debugmode) = 0;
                }
            }
        }
        else
        {
            eating_time = 0;            
        }
    }
    else
    {
        eating_time = 0;
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
                    dig_hardness = soil_hardness;
                    dig_tag = 1;
                }
                if (dig_hardness <= 0.0)
                {
                    for(int i=0; i<9; i++)
                    {
                        if(*(player_bag+2*i) == 0 || (*(player_bag+2*i) == 1 && *(player_bag+2*i+1) < 64))
                        {
                            *(player_bag+2*i) = 1;
                            *(player_bag+2*i+1) += 1;
                            break;
                        }
                    }
                    *(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) = 0;
                    hungry += 0.025;
                }
                dig_flag = 1;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 2)
            {
                if (dig_now_x == (int)dig_search_after_x && dig_now_y == (int)dig_search_after_y && dig_now_z == (int)dig_search_after_z)
                    dig_hardness -= player_dig_speed;
                else
                {
                    dig_now_x = (int)dig_search_after_x;
                    dig_now_y = (int)dig_search_after_y;
                    dig_now_z = (int)dig_search_after_z;
                    dig_hardness = rock_hardness;
                    dig_tag = 2;
                }
                if (dig_hardness <= 0.0)
                {
                    for(int i=0; i<9; i++)
                    {
                        if(*(player_bag+2*i) == 0 || (*(player_bag+2*i) == 2 && *(player_bag+2*i+1) < 64))
                        {
                            *(player_bag+2*i) = 2;
                            *(player_bag+2*i+1) += 1;
                            break;
                        }
                    }
                    *(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) = 0;
                    hungry += 0.025;
                }
                dig_flag = 1;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 3)
            {
                if (dig_now_x == (int)dig_search_after_x && dig_now_y == (int)dig_search_after_y && dig_now_z == (int)dig_search_after_z)
                    dig_hardness -= player_dig_speed;
                else
                {
                    dig_now_x = (int)dig_search_after_x;
                    dig_now_y = (int)dig_search_after_y;
                    dig_now_z = (int)dig_search_after_z;
                    dig_hardness = diamond_hardness;
                    dig_tag = 3;
                }
                if (dig_hardness <= 0.0)
                {
                    for(int i=0; i<9; i++)
                    {
                        if(*(player_bag+2*i) == 0 || (*(player_bag+2*i) == 3 && *(player_bag+2*i+1) < 64))
                        {
                            *(player_bag+2*i) = 3;
                            *(player_bag+2*i+1) += 1;
                            break;
                        }
                    }
                    *(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) = 0;
                    hungry += 0.025;
                }
                dig_flag = 1;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 5)
            {
                if (dig_now_x == (int)dig_search_after_x && dig_now_y == (int)dig_search_after_y && dig_now_z == (int)dig_search_after_z)
                    dig_hardness -= player_dig_speed;
                else
                {
                    dig_now_x = (int)dig_search_after_x;
                    dig_now_y = (int)dig_search_after_y;
                    dig_now_z = (int)dig_search_after_z;
                    dig_hardness = sand_hardness;
                    dig_tag = 5;
                }
                if (dig_hardness <= 0.0)
                {
                    for(int i=0; i<9; i++)
                    {
                        if(*(player_bag+2*i) == 0 || (*(player_bag+2*i) == 5 && *(player_bag+2*i+1) < 64))
                        {
                            *(player_bag+2*i) = 5;
                            *(player_bag+2*i+1) += 1;
                            break;
                        }
                    }
                    *(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) = 0;
                    hungry += 0.025;
                }
                dig_flag = 1;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 10)
            {
                if (dig_now_x == (int)dig_search_after_x && dig_now_y == (int)dig_search_after_y && dig_now_z == (int)dig_search_after_z)
                    dig_hardness -= player_dig_speed;
                else
                {
                    dig_now_x = (int)dig_search_after_x;
                    dig_now_y = (int)dig_search_after_y;
                    dig_now_z = (int)dig_search_after_z;
                    dig_hardness = tree_hardness;
                    dig_tag = 10;
                }
                if (dig_hardness <= 0.0)
                {
                    for(int i=0; i<9; i++)
                    {
                        if(*(player_bag+2*i) == 0 || (*(player_bag+2*i) == 10 && *(player_bag+2*i+1) < 64))
                        {
                            *(player_bag+2*i) = 10;
                            *(player_bag+2*i+1) += 1;
                            break;
                        }
                    }
                    *(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) = 0;
                    hungry += 0.025;
                }
                dig_flag = 1;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 11)
            {
                if (dig_now_x == (int)dig_search_after_x && dig_now_y == (int)dig_search_after_y && dig_now_z == (int)dig_search_after_z)
                    dig_hardness -= player_dig_speed;
                else
                {
                    dig_now_x = (int)dig_search_after_x;
                    dig_now_y = (int)dig_search_after_y;
                    dig_now_z = (int)dig_search_after_z;
                    dig_hardness = leaf_hardness;
                    dig_tag = 11;

                }
                if (dig_hardness <= 0.0)
                {
                    if ((double)rand() / (double)RAND_MAX <= apple_drop_rate)
                    {
                        for(int i=0; i<9; i++)
                        {
                            if(*(player_bag+2*i) == 0 || (*(player_bag+2*i) == 1025 && *(player_bag+2*i+1) < 64))
                            {
                                *(player_bag+2*i) = 1025;
                                *(player_bag+2*i+1) += 1;
                                break;
                            }
                        }
                    }
                    *(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) = 0;
                    hungry += 0.025;
                }
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
                dig_tag = 1;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 2)
            {
                dig_tag = 2;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 3)
            {
                dig_tag = 3;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 10)
            {
                dig_tag = 10;
                break;
            }
            if(*(chunk+(dig_search_after_chunk_x-player_pre_loadchunk_x)*1114112+(dig_search_after_chunk_z-player_pre_loadchunk_z)*65536+(dig_search_after_local_x)*4096+(int)(dig_search_after_y)*16+dig_search_after_local_z) == 11)
            {
                dig_tag = 11;
                break;
            }
        }
        if (dig_tag == 1)
            dig_hardness = soil_hardness;
        if (dig_tag == 2)
            dig_hardness = rock_hardness;
        if (dig_tag == 3)
            dig_hardness = diamond_hardness;
        if (dig_tag == 5)
            dig_hardness = sand_hardness;
        if (dig_tag == 10)
            dig_hardness = tree_hardness;
        if (dig_tag == 11)
            dig_hardness = leaf_hardness;
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
        int tmp;
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
                            tmp = *(chunk+i*1114112+j*65536+x*4096+y*16+z);
                            if (tmp != 0)
                            {
                                dy = (double) y;
                                // x-Higher
                                if ((i < draw_xmax-1 && x == 15 && *(chunk+(i+1)*1114112+j*65536+y*16+z) == 0) || (x < 15 && *(chunk+i*1114112+j*65536+(x+1)*4096+y*16+z) == 0))
                                {
                                    glColor4f(bci[tmp][0][0], bci[tmp][0][1], bci[tmp][0][2], bci[tmp][0][3]);
                                    glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                                    glColor4f(0.9*bci[tmp][0][0], 0.9*bci[tmp][0][1], 0.9*bci[tmp][0][2], bci[tmp][0][3]);
                                    glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                                    glColor4f(0.8*bci[tmp][0][0], 0.8*bci[tmp][0][1], 0.8*bci[tmp][0][2], bci[tmp][0][3]);
                                    glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                    glColor4f(0.9*bci[tmp][0][0], 0.9*bci[tmp][0][1], 0.9*bci[tmp][0][2], bci[tmp][0][3]);
                                    glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                                }
                                // y-Higher
                                if (y == 255 || (y < 255 && *(chunk+i*1114112+j*65536+x*4096+(y+1)*16+z) == 0))
                                {
                                    glColor4f(bci[tmp][1][0], bci[tmp][1][1], bci[tmp][1][2], bci[tmp][1][3]);
                                    glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                                    glColor4f(0.9*bci[tmp][1][0], 0.9*bci[tmp][1][1], 0.9*bci[tmp][1][2], bci[tmp][1][3]);
                                    glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                    glColor4f(0.8*bci[tmp][1][0], 0.8*bci[tmp][1][1], 0.8*bci[tmp][1][2], bci[tmp][1][3]);;
                                    glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                    glColor4f(0.9*bci[tmp][1][0], 0.9*bci[tmp][1][1], 0.9*bci[tmp][1][2], bci[tmp][1][3]);
                                    glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                                }
                                // z-Higher
                                if ((j < draw_zmax-1 && z == 15 && *(chunk+i*1114112+(j+1)*65536+x*4096+y*16) == 0) || (z < 15 && *(chunk+i*1114112+j*65536+x*4096+y*16+z+1) == 0))
                                {
                                    glColor4f(bci[tmp][2][0], bci[tmp][2][1], bci[tmp][2][2], bci[tmp][2][3]);
                                    glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                    glColor4f(0.9*bci[tmp][2][0], 0.9*bci[tmp][2][1], 0.9*bci[tmp][2][2], bci[tmp][2][3]);
                                    glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                    glColor4f(0.8*bci[tmp][2][0], 0.8*bci[tmp][2][1], 0.8*bci[tmp][2][2], bci[tmp][2][3]);;
                                    glVertex3f(dx+1.0, dy+1.0, dz+1.0);
                                    glColor4f(0.9*bci[tmp][2][0], 0.9*bci[tmp][2][1], 0.9*bci[tmp][2][2], bci[tmp][2][3]);
                                    glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                                }

                                // x-Lower
                                if ((i > 0 && x == 0 && *(chunk+(i-1)*1114112+j*65536+15*4096+y*16+z) == 0) || (x > 0 && *(chunk+i*1114112+j*65536+(x-1)*4096+y*16+z) == 0))
                                {
                                    glColor4f(bci[tmp][3][0], bci[tmp][3][1], bci[tmp][3][2], bci[tmp][3][3]);
                                    glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                    glColor4f(0.9*bci[tmp][3][0], 0.9*bci[tmp][3][1], 0.9*bci[tmp][3][2], bci[tmp][3][3]);
                                    glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                    glColor4f(0.8*bci[tmp][3][0], 0.8*bci[tmp][3][1], 0.8*bci[tmp][3][2], bci[tmp][3][3]);;
                                    glVertex3f(dx+0.0, dy+1.0, dz+1.0);
                                    glColor4f(0.9*bci[tmp][3][0], 0.9*bci[tmp][3][1], 0.9*bci[tmp][3][2], bci[tmp][3][3]);
                                    glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                                }

                                // y-Lower
                                if (y == 0 || (y > 0 && *(chunk+i*1114112+j*65536+x*4096+(y-1)*16+z) == 0))
                                {
                                    glColor4f(bci[tmp][4][0], bci[tmp][4][1], bci[tmp][4][2], bci[tmp][4][3]);
                                    glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                    glColor4f(0.9*bci[tmp][4][0], 0.9*bci[tmp][4][1], 0.9*bci[tmp][4][2], bci[tmp][4][3]);
                                    glVertex3f(dx+0.0, dy+0.0, dz+1.0);
                                    glColor4f(0.8*bci[tmp][4][0], 0.8*bci[tmp][4][1], 0.8*bci[tmp][4][2], bci[tmp][4][3]);;
                                    glVertex3f(dx+1.0, dy+0.0, dz+1.0);
                                    glColor4f(0.9*bci[tmp][4][0], 0.9*bci[tmp][4][1], 0.9*bci[tmp][4][2], bci[tmp][4][3]);
                                    glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                                }

                                // z-Lower
                                if ((j > 0 && z == 0 && *(chunk+i*1114112+(j-1)*65536+x*4096+y*16+15) == 0) || (z > 0 && *(chunk+i*1114112+j*65536+x*4096+y*16+z-1) == 0))
                                {
                                    glColor4f(bci[tmp][5][0], bci[tmp][5][1], bci[tmp][5][2], bci[tmp][5][3]);
                                    glVertex3f(dx+0.0, dy+0.0, dz+0.0);
                                    glColor4f(0.9*bci[tmp][5][0], 0.9*bci[tmp][5][1], 0.9*bci[tmp][5][2], bci[tmp][5][3]);
                                    glVertex3f(dx+0.0, dy+1.0, dz+0.0);
                                    glColor4f(0.8*bci[tmp][5][0], 0.8*bci[tmp][5][1], 0.8*bci[tmp][5][2], bci[tmp][5][3]);;
                                    glVertex3f(dx+1.0, dy+1.0, dz+0.0);
                                    glColor4f(0.9*bci[tmp][5][0], 0.9*bci[tmp][5][1], 0.9*bci[tmp][5][2], bci[tmp][5][3]);
                                    glVertex3f(dx+1.0, dy+0.0, dz+0.0);
                                }
                            }
                        }
                    }
                }
            }
        }
    glEnd();
    if (dig_flag == 1 && pause == 0)
    {
        if (dig_tag == 1)
            bar_length = (dig_hardness / soil_hardness) * 0.8 - 0.4;
        else if (dig_tag == 2)
            bar_length = (dig_hardness / rock_hardness) * 0.8 - 0.4;
        else if (dig_tag == 3)
            bar_length = (dig_hardness / diamond_hardness) * 0.8 - 0.4;
        else if (dig_tag == 5)
            bar_length = (dig_hardness / sand_hardness) * 0.8 - 0.4;
        else if (dig_tag == 10)
            bar_length = (dig_hardness / tree_hardness) * 0.8 - 0.4;
        else if (dig_tag == 11)
            bar_length = (dig_hardness / leaf_hardness) * 0.8 - 0.4;
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
    glLineWidth(2.0);
    int itembox_x = w / 2 - 288;
    int itembox_y = h - 128;
    // enventory
    if (pause == 0)
    {
        glColor3f(0.9, 0.9, 0.9);
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

        glLineWidth(5.0);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
            glVertex2i(itembox_x +  64*debugmode    , itembox_y);
            glVertex2i(itembox_x +  64*(debugmode+1), itembox_y);
            glVertex2i(itembox_x +  64*(debugmode+1), itembox_y);
            glVertex2i(itembox_x +  64*(debugmode+1), itembox_y + 64);
            glVertex2i(itembox_x +  64*(debugmode+1), itembox_y + 64);
            glVertex2i(itembox_x +  64*debugmode    , itembox_y + 64);
            glVertex2i(itembox_x +  64*debugmode    , itembox_y + 64);
            glVertex2i(itembox_x +  64*debugmode    , itembox_y);
        glEnd();
        glBegin(GL_QUADS);
            for (int i=0; i<9; i++)
            {
                if (*(player_bag+2*i) != 0 && *(player_bag+2*i) <= 1024)
                {
                    glColor3f(bci[*(player_bag+2*i)][0][0], bci[*(player_bag+2*i)][0][1], bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*i+20, itembox_y+20);
                    glColor3f(0.9*bci[*(player_bag+2*i)][0][0], 0.9*bci[*(player_bag+2*i)][0][1], 0.9*bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*(i+1)-20, itembox_y+20);
                    glColor3f(0.8*bci[*(player_bag+2*i)][0][0], 0.8*bci[*(player_bag+2*i)][0][1], 0.8*bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*(i+1)-20, itembox_y + 44);
                    glColor3f(0.9*bci[*(player_bag+2*i)][0][0], 0.9*bci[*(player_bag+2*i)][0][1], 0.9*bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*i+20, itembox_y + 44);
                }
            }
        glEnd();

        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_TRIANGLE_FAN);
            for (int i=0; i<9; i++)
            {
                if (*(player_bag+2*i) == 1025)
                {
                    glVertex2i(itembox_x + 64*i + 32, itembox_y + 32);
                    for(double deg=0.0; deg<=360.0; deg+=3.0)
                    {
                        glVertex2i(itembox_x + 64*i + 32 + 16* cos(M_PI * deg / 180.0) , itembox_y + 32 + 16 * sin(M_PI * deg / 180.0));
                        glVertex2i(itembox_x + 64*i + 32 + 16 * cos(M_PI * (deg+3.0) / 180.0) , itembox_y + 32 + 16 * sin(M_PI * (deg+3.0) / 180.0));
                    }
                }
            }
        glEnd();

        for (int i=0; i<10; i++)
        {
            if (2*i < *(player_status))
            {
                glBegin(GL_QUADS);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i, itembox_y-5-18);
                glEnd();
                glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i, itembox_y-5-18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+6, itembox_y-5-24);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5);
                glEnd();
            }
            else
            {
                glBegin(GL_QUADS);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i, itembox_y-5-18);
                glEnd();
                glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i, itembox_y-5-18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+6, itembox_y-5-24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5);
                glEnd();
            }
            if (2*i+1 < *(player_status))
            {
                glBegin(GL_QUADS);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i+12, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                glEnd();
                glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+18, itembox_y-5-24);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i+12, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5);
                glEnd();
            }
            else
            {
                glBegin(GL_QUADS);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i+12, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                glEnd();
                glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5-18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+18, itembox_y-5-24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25*i+12, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+24, itembox_y-5-12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x +  25*i+12, itembox_y-5);
                glEnd();
            }
        }

        for (int i=0; i<9; i++)
        {
            if (2*i < *(player_status+1))
            {
                glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9 + 25*i+12, itembox_y-5-0);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  + 25*i+12, itembox_y-5-24);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  + 25*i, itembox_y-5-12);
                glEnd();
            }
            else
            {
                glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+12, itembox_y-5-0);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+12, itembox_y-5-24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i, itembox_y-5-12);
                glEnd();
            }
            if (2*i+1 < *(player_status+1))
            {
                glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+12, itembox_y-5-0);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+12, itembox_y-5-24);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+24, itembox_y-5-12);
                glEnd();
            }
            else
            {
                glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+12, itembox_y-5-0);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+12, itembox_y-5-24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64*9 - 25*9  +  25*i+24, itembox_y-5-12);
                glEnd();
            }
        }

        for (int i=0; i<9; i++)
        {
            glColor3f(1, 1, 1);
            sprintf(items_str, "%2d", *(player_bag+2*i+1));
            glRasterPos2i(itembox_x + 64*(i+1)-23, itembox_y + 59);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
        }
        glLineWidth(2.0);
        glBegin(GL_LINES);
            glVertex2i(w/2, h/2+10);
            glVertex2i(w/2, h/2-10);
            glVertex2i(w/2-10, h/2);
            glVertex2i(w/2+10, h/2);
        glEnd();
        glColor3f(0, 0, 0);
        glRasterPos2i(0, 16);
        for(int j=0; j<60; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, infomation[j]);
        glRasterPos2i(0, 16*2);
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
        glRasterPos2i(0, 16*18);
        for(int j=0; j<30; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ht_str[j]);
        glRasterPos2i(0, 16*19);
        for(int j=0; j<30; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, hht_str[j]);

    }
    if (enventory == 1)
    {

        int enventry_x = (w - 640) / 2;
        int enventry_y = (h - 640) / 2;
        itembox_x = enventry_x + 32;
        itembox_y = enventry_y + 32*17;

        // items
        glBegin(GL_QUADS);
            for (int i=0; i<9; i++)
            {
                if (*(player_bag+2*i) != 0 && *(player_bag+2*i) <= 1024)
                {
                    glColor3f(bci[*(player_bag+2*i)][0][0], bci[*(player_bag+2*i)][0][1], bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*i+20, itembox_y+20);
                    glColor3f(0.9*bci[*(player_bag+2*i)][0][0], 0.9*bci[*(player_bag+2*i)][0][1], 0.9*bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*(i+1)-20, itembox_y+20);
                    glColor3f(0.8*bci[*(player_bag+2*i)][0][0], 0.8*bci[*(player_bag+2*i)][0][1], 0.8*bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*(i+1)-20, itembox_y + 44);
                    glColor3f(0.9*bci[*(player_bag+2*i)][0][0], 0.9*bci[*(player_bag+2*i)][0][1], 0.9*bci[*(player_bag+2*i)][0][2]);
                    glVertex2i(itembox_x +  64*i+20, itembox_y + 44);
                }
            }
            for (int i=0; i<9; i++)
            {
                for (int j=0; j<3; j++)
                {
                    if (*(player_bag+2*(9*j+i+9)) != 0 && *(player_bag+2*(9*j+i+9)) <= 1024)
                    {
                        glColor3f(bci[*(player_bag+2*(9*j+i+9))][0][0], bci[*(player_bag+2*(9*j+i+9))][0][1], bci[*(player_bag+2*(9*j+i+9))][0][2]);
                        glVertex2i(itembox_x +  64*i+20, itembox_y-32-64*(3-j)+20);
                        glColor3f(0.9*bci[*(player_bag+2*(9*j+i+9))][0][0], 0.9*bci[*(player_bag+2*(9*j+i+9))][0][1], 0.9*bci[*(player_bag+2*(9*j+i+9))][0][2]);
                        glVertex2i(itembox_x +  64*(i+1)-20, itembox_y-32-64*(3-j)+20);
                        glColor3f(0.8*bci[*(player_bag+2*(9*j+i+9))][0][0], 0.8*bci[*(player_bag+2*(9*j+i+9))][0][1], 0.8*bci[*(player_bag+2*(9*j+i+9))][0][2]);
                        glVertex2i(itembox_x +  64*(i+1)-20, itembox_y-32-64*(3-j) + 44);
                        glColor3f(0.9*bci[*(player_bag+2*(9*j+i+9))][0][0], 0.9*bci[*(player_bag+2*(9*j+i+9))][0][1], 0.9*bci[*(player_bag+2*(9*j+i+9))][0][2]);
                        glVertex2i(itembox_x +  64*i+20, itembox_y-32-64*(3-j) + 44);
                    }
                }
            }
        glEnd();
        for (int i=0; i<9; i++)
        {
            glColor3f(0, 0, 0);
            sprintf(items_str, "%2d", *(player_bag+2*i+1));
            glRasterPos2i(itembox_x + 64*(i+1)-23, itembox_y + 59);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
        }

        for (int i=0; i<9; i++)
        {
            for (int j=0; j<3; j++)
            {
                glColor3f(0, 0, 0);
                sprintf(items_str, "%2d", *(player_bag+2*(9*j+i+9)+1));
                glRasterPos2i(itembox_x + 64*(i+1)-23, itembox_y-32-64*(3-j) + 59);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
            }
        }


        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);
                glVertex2i(itembox_x+64+2, enventry_y+32+2);
                glVertex2i(itembox_x+64*3+62, enventry_y+32+2);
                glVertex2i(itembox_x+64*3+62, enventry_y+32*7+62);
                glVertex2i(itembox_x+64+2, enventry_y+32*7+62);
        glEnd();
        // 4 4 3 4 9 7
        glRasterPos2i(itembox_x+14, enventry_y+32*1+36);
        for(int j=0; j<4; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, evtr_str[j]);
        glRasterPos2i(itembox_x+14, enventry_y+32*3+36);
        for(int j=5; j<9; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, evtr_str[j]);
        glRasterPos2i(itembox_x+18, enventry_y+32*5+36);
        for(int j=10; j<13; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, evtr_str[j]);
        glRasterPos2i(itembox_x+14, enventry_y+32*7+36);
        for(int j=14; j<18; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, evtr_str[j]);
        glRasterPos2i(itembox_x+256+14, enventry_y+32*7+36);
        for(int j=19; j<23; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, evtr_str[j]);
        glRasterPos2i(itembox_x+320+2, enventry_y+32*7+36);
        for(int j=24; j<31; j++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, evtr_str[j]);

        glColor3f(0.5, 0.5, 0.5);
        glLineWidth(2.0);
        glBegin(GL_QUADS);
            // armer field
            for (int j=0; j<4; j++)
            {
                glVertex2i(itembox_x+2, enventry_y+32*(1+2*j)+2);
                glVertex2i(itembox_x+62, enventry_y+32*(1+2*j)+2);
                glVertex2i(itembox_x+62, enventry_y+32*(1+2*j)+62);
                glVertex2i(itembox_x+2, enventry_y+32*(1+2*j)+62);
            }

            // free hand field
            glVertex2i(itembox_x+64*4+2, enventry_y+32*7+2);
            glVertex2i(itembox_x+64*4+62, enventry_y+32*7+2);
            glVertex2i(itembox_x+64*4+62, enventry_y+32*7+62);
            glVertex2i(itembox_x+64*4+2, enventry_y+32*7+62);
            // pet toy field
            glVertex2i(itembox_x+64*5+2, enventry_y+32*7+2);
            glVertex2i(itembox_x+64*5+62, enventry_y+32*7+2);
            glVertex2i(itembox_x+64*5+62, enventry_y+32*7+62);
            glVertex2i(itembox_x+64*5+2, enventry_y+32*7+62);

            //craft field - before
            for (int i=5; i<7; i++)
            {
                for (int j=0; j<2; j++)
                {
                    glVertex2i(itembox_x+64*i+2, enventry_y+32*(2+2*j)+2);
                    glVertex2i(itembox_x+64*i+62, enventry_y+32*(2+2*j)+2);
                    glVertex2i(itembox_x+64*i+62, enventry_y+32*(2+2*j)+62);
                    glVertex2i(itembox_x+64*i+2, enventry_y+32*(2+2*j)+62);
                }
            }

            //craft field - after
                glVertex2i(itembox_x+64*8+2, enventry_y+32*3+2);
                glVertex2i(itembox_x+64*8+62, enventry_y+32*3+2);
                glVertex2i(itembox_x+64*8+62, enventry_y+32*3+62);
                glVertex2i(itembox_x+64*8+2, enventry_y+32*3+62);

            // inventory
            for (int i=0; i<9; i++)
            {
                for (int j=0; j<3; j++)
                {
                    glVertex2i(itembox_x+64*i+2, enventry_y+32*(10+2*j)+2);
                    glVertex2i(itembox_x+64*i+62, enventry_y+32*(10+2*j)+2);
                    glVertex2i(itembox_x+64*i+62, enventry_y+32*(10+2*j)+62);
                    glVertex2i(itembox_x+64*i+2, enventry_y+32*(10+2*j)+62);
                }
                glVertex2i(itembox_x+64*i+2, itembox_y+2);
                glVertex2i(itembox_x+64*i+62, itembox_y+2);
                glVertex2i(itembox_x+64*i+62, itembox_y+62);
                glVertex2i(itembox_x+64*i+2, itembox_y+62);
            }
        glEnd();

        glColor3f(0.8, 0.8, 0.8);
        glBegin(GL_QUADS);
            glVertex2i(enventry_x, enventry_y);
            glVertex2i(enventry_x + 640, enventry_y);
            glVertex2i(enventry_x + 640, enventry_y + 640);
            glVertex2i(enventry_x, enventry_y + 640);
        glEnd();
    }

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
    sprintf(infomation, "AniBon pre1 (dev-ver)");
    sprintf(pos_str,   "XYZ: %.2lf %.2lf %.2lf", player_x, player_y, player_z);
    sprintf(block_str, "BLOCK: %d %d %d", player_block_x, player_block_y, player_block_z);
    sprintf(chunk_str, "CHUNK: %d %d %d in %d %d %d", player_local_x, player_local_y, player_local_z, player_chunk_x, player_chunk_y, player_chunk_z);
    sprintf(keystate_str, "F:%d B:%d L:%d R:%d SLOW:%d FAST:%d JUMP:%d", key_state[0], key_state[1], key_state[2], key_state[3], key_state[4], key_state[5], key_state[6]);
    sprintf(loadposc_str, "LOAD CHUNK(PLAYER): %d %d", player_pre_chunk_x, player_pre_chunk_z);
    sprintf(loadpos_str, "LOAD CHUNK(START): %d %d", player_pre_loadchunk_x, player_pre_loadchunk_z);
    sprintf(dig_str, "BLOCK HARD(DIGING): %.2lf", dig_hardness);
    sprintf(ht_str, "HUNGRY POINT: %.2lf", hungry);
    sprintf(hht_str, "HIDE HUNGRY POINT: %.1lf", player_hide_hungry);
    sprintf(evtr_str, "HEAD BODY LEG FOOT FREE PET TOY");
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
    {
        for (int i=0; i<2*DRAW_DIST+1; i++)
            for (int j=0; j<2*DRAW_DIST+1; j++)
                SaveChunk(chunk+i*1114112+j*65536, i+player_pre_loadchunk_x, j+player_pre_loadchunk_z);
        SaveItem(player_bag);
        SaveStatus(player_status);
        exit(1);
    }
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
    if (c == 'w' || c == 'W' || c == 23)
        key_state[0] = 0;
    else if (c == 's' || c == 'S' || c == 1)
        key_state[1] = 0;
    else if (c == 'a' || c == 'A' || c == 19)
        key_state[2] = 0;
    else if (c == 'd' || c == 'D' || c == 4)
        key_state[3] = 0;
    else if (c == ' ')
        key_state[6] = 0;
    else if (c >= '1' && c <= '9')
        debugmode = c - '1';
    else if (c == 'e')
    {
        enventory = 1 - enventory;
        pause = 1 - pause;
    }
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
                if(*(chunk+(next_view_chunk_x-player_pre_loadchunk_x)*1114112+(next_view_chunk_z-player_pre_loadchunk_z)*65536+(next_view_local_x)*4096+(int)(after_y)*16+next_view_local_z) != 0 && *(player_bag+2*debugmode+1) != 0 && *(player_bag+2*debugmode) <= 1024)
                {
                    if((int)after_x != (int)before_x && ((int) before_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int) after_z != (int)player_z))
                    {
                        *(chunk+(view_chunk_x-player_pre_loadchunk_x)*1114112+(next_view_chunk_z-player_pre_loadchunk_z)*65536+(view_local_x)*4096+(int)(after_y)*16+next_view_local_z) = *(player_bag+2*debugmode);
                        *(player_bag+2*debugmode+1) -= 1;
                        if (*(player_bag+2*debugmode+1) == 0)
                            *(player_bag+2*debugmode) = 0;
                    }
                    else if ((int)after_y != (int)before_y && ((int) after_x != (int)player_x || ((int)before_y != (int)(player_y - 1.5) && (int)before_y != (int)(player_y - 0.5)) || (int) after_z != (int)player_z))
                    {
                        *(chunk+(next_view_chunk_x-player_pre_loadchunk_x)*1114112+(next_view_chunk_z-player_pre_loadchunk_z)*65536+(next_view_local_x)*4096+(int)(before_y)*16+next_view_local_z) = *(player_bag+2*debugmode);
                        *(player_bag+2*debugmode+1) -= 1;
                        if (*(player_bag+2*debugmode+1) == 0)
                            *(player_bag+2*debugmode) = 0;
                    }
                    else if ((int)after_z != (int)before_z && ((int) after_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int) before_z != (int)player_z))
                    {
                        *(chunk+(next_view_chunk_x-player_pre_loadchunk_x)*1114112+(view_chunk_z-player_pre_loadchunk_z)*65536+(next_view_local_x)*4096+(int)(after_y)*16+view_local_z) = *(player_bag+2*debugmode);
                        *(player_bag+2*debugmode+1) -= 1;
                        if (*(player_bag+2*debugmode+1) == 0)
                            *(player_bag+2*debugmode) = 0;
                    }
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
    {
        mouse_status[1] *= -1;
        if (state == GLUT_DOWN && enventory == 1)
        {
            int w = glutGet(GLUT_WINDOW_WIDTH);
            int h = glutGet(GLUT_WINDOW_HEIGHT);
            int enventry_x = (w - 640) / 2;
            int enventry_y = (h - 640) / 2;
            int itembox_x = enventry_x + 32;
            int itembox_y = enventry_y + 32*10;
            if (itembox_x <= x && x < itembox_x+64*9 && itembox_y <= y && y < itembox_y+64*3)
            {
                dragging_item = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                printf("drag_from: %d\n", dragging_item);
            }
            itembox_y = enventry_y + 32*17;
            if (itembox_x <= x && x < itembox_x+64*9 && itembox_y <= y && y < itembox_y+64)
            {
                dragging_item = ((x - itembox_x) / 64);
                printf("drag_from: %d\n", dragging_item);
            }
        }
        else if(state == GLUT_UP && enventory == 1)
        {
            int w = glutGet(GLUT_WINDOW_WIDTH);
            int h = glutGet(GLUT_WINDOW_HEIGHT);
            int enventry_x = (w - 640) / 2;
            int enventry_y = (h - 640) / 2;
            int itembox_x = enventry_x + 32;
            int itembox_y = enventry_y + 32*10;
            int tmp1, tmp2;
            if (itembox_x <= x && x < itembox_x+64*9 && itembox_y <= y && y < itembox_y+64*3)
            {
                int drop_to = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                tmp1 = *(player_bag+2*dragging_item);
                tmp2 = *(player_bag+2*dragging_item+1);
                *(player_bag+2*dragging_item) = *(player_bag+2*drop_to);
                *(player_bag+2*dragging_item+1) = *(player_bag+2*drop_to+1);
                *(player_bag+2*drop_to) = tmp1;
                *(player_bag+2*drop_to+1) = tmp2;
            }
            itembox_y = enventry_y + 32*17;
            if (itembox_x <= x && x < itembox_x+64*9 && itembox_y <= y && y < itembox_y+64)
            {
                int drop_to = ((x - itembox_x) / 64);
                tmp1 = *(player_bag+2*dragging_item);
                tmp2 = *(player_bag+2*dragging_item+1);
                *(player_bag+2*dragging_item) = *(player_bag+2*drop_to);
                *(player_bag+2*dragging_item+1) = *(player_bag+2*drop_to+1);
                *(player_bag+2*drop_to) = tmp1;
                *(player_bag+2*drop_to+1) = tmp2;
            }
        }
    }
}

void motion(int x, int y)
{
    if (pause == 0)
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
    else
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

void draging(int x, int y)
{
    if (pause == 0)
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
    else
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
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

// PerlinNoize
void PerlinNoize1d(double* data, int chunk_xpos, int direction, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 2;
    int mod_chunk_xpos = chunk_xpos % 2;

    double C[64];                     // Wavelet data
    double W[32][2];                  // Graph data for marge
    double a[32][3];                      // Gradient data

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
        a[i][0] = (double)(rand() - RAND_MAX/2) / (double)RAND_MAX;
        a[i][1] = (double)(rand() - RAND_MAX/2) / (double)RAND_MAX;
        a[i][2] = (double)(rand() - RAND_MAX/2) / (double)RAND_MAX;
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

void PerlinNoize2di(int *data, int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed, int loops)
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
    for (int l=0; l<=loops; l++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                a[i][j][0] = (double)rand() / (double)RAND_MAX;
                a[i][j][1] = (double)rand() / (double)RAND_MAX;
            }
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
            *(data+16*x+z) = 80 + lround(Wp[xp][zp][0] + dx * (Wp[xp][zp][1] - Wp[xp][zp][0]));
        }
    }
}

void PerlinNoize2dd(double *data, int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed, int loops)
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
    Wavelet2d(C, 1.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for (int l=0; l<=loops; l++)
    {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                a[i][j][0] = (double)(rand() - RAND_MAX/2) / (double)RAND_MAX;
                a[i][j][1] = (double)(rand() - RAND_MAX/2) / (double)RAND_MAX;
            }
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
            *(data+16*x+z) = Wp[xp][zp][0] + dx * (Wp[xp][zp][1] - Wp[xp][zp][0]);
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

void CreateWorld()
{
    int* onechunk3d;
    int* onechunk2d;
    double* biome;
    double* diamond_rate;
    double* wood_rate;
    char* wood_put;
    double* onechunk1d_x;
    double* onechunk1d_y;
    double* onechunk1d_z;
    double rate_rate;
    int wood_judge;
    int tmp;


    onechunk3d = (int*)malloc(sizeof(int)*16*256*16);
    onechunk2d = (int*)malloc(sizeof(int)*16*16);
    biome = (double*)malloc(sizeof(double)*16*16);
    onechunk1d_x = (double*)malloc(sizeof(double)*16);
    onechunk1d_y = (double*)malloc(sizeof(double)*16*16);
    onechunk1d_z = (double*)malloc(sizeof(double)*16);
    diamond_rate = (double*)malloc(sizeof(double)*64*64*16*16*16);
    wood_rate = (double*)malloc(sizeof(double)*64*64*16*16);
    wood_put = (char*)malloc(sizeof(char)*64*64*16*16);

    srand(SEED_NUM);
    for (int i=0; i<64; i++)
        for (int j=0; j<64; j++)
            for (int x=0; x<16; x++)
                for (int y=0; y<16; y++)
                    for (int z=0; z<16; z++)
                        *(diamond_rate+262144*i+4096*j+256*x+16*y+z) = (double)rand() / (double)RAND_MAX;
    for (int i=0; i<64; i++)
        for (int j=0; j<64; j++)
            for (int x=0; x<16; x++)
                for (int y=0; y<16; y++)
                    {
                        *(wood_put+16384*i+256*j+16*x+y) = 0;
                        *(wood_rate+16384*i+256*j+16*x+y) = (double)rand() / (double)RAND_MAX;
                    }

    for (int i=0; i<16; i++)
        PerlinNoize1d(onechunk1d_y+i*16, i, 1, 1, SEED_NUM);

    for (int i=0; i<WORLD_CHUNK; i++)
    {
        for (int j=0; j<WORLD_CHUNK; j++)
        {
            PerlinNoize2di(onechunk2d, i, j, 1, SEED_NUM, 0);
            PerlinNoize2dd(biome, i, j, 1, SEED_NUM, 10);
            PerlinNoize1d(onechunk1d_x, i, 0, 1, SEED_NUM);
            PerlinNoize1d(onechunk1d_z, j, 2, 1, SEED_NUM);
            for (int x=0; x<16; x++)
            {
                for (int z=0; z<16; z++)
                {
                    for (int y=0; y<256; y++)
                    {
                        if (*(onechunk1d_x+x)+*(onechunk1d_y+y)+*(onechunk1d_z+z) < 0.68 && *(onechunk1d_x+x)+*(onechunk1d_y+y)+*(onechunk1d_z+z) > -0.68)
                        {
                            if (y < *(onechunk2d+16*x+z)-3)
                            {
                                *(onechunk3d+x*4096+y*16+z) = 2;
                            }
                            else if (y < *(onechunk2d+16*x+z))
                            {
                                if (0.01 > *(biome+16*x+z))
                                    *(onechunk3d+x*4096+y*16+z) = 1;
                                else
                                    *(onechunk3d+x*4096+y*16+z) = 5;
                            }
                            else
                            {
                                *(onechunk3d+x*4096+y*16+z) = 0;                                
                            }
                            if (y < 16)
                            {
                                if (y >= 5 || y <= 12)
                                    rate_rate = 1.0;
                                else if (y < 5)
                                    rate_rate = (1.0 - (double)(y) / 5.0);
                                else if (y > 12)
                                    rate_rate = (double)(15 - y) / 3.0;
                                if (*(diamond_rate+262144*i+4096*j+256*x+16*y+z) <= (diamond_create_rate * rate_rate))
                                    *(onechunk3d+x*4096+y*16+z) = 3;
                            }
                        }
                        else
                            *(onechunk3d+x*4096+y*16+z) = 0;
                        if (y == 0)
                        {
                            *(onechunk3d+x*4096+y*16+z) = 4;
                        }
                    }
                }
            }
            for (int x=0; x<16; x++)
            {
                for (int z=0; z<16; z++)
                {
                    if (0.01 > *(biome+16*x+z))
                    {
                        for (int y=*(onechunk2d+16*x+z); y>=*(onechunk2d+16*x+z)-3; y--)
                        {
                            if (*(onechunk3d+x*4096+y*16+z) == 1)
                            {
                                if (*(wood_rate+163844*i+256*j+16*x+y) <= wood_create_rate)
                                {
                                    wood_judge = 0;
                                    for(int a=-2; a<=2; a++)
                                    {
                                        for(int b=-2; b<=2; b++)
                                        {
                                            // #1
                                            if (x+a<0 && z+b<0)
                                            {
                                                if (i<1 || j<1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*(i-1)+256*(j-1)+16*(16+x+a)+(16+z+b));
                                            }
                                            // #2
                                            else if (x+a<0 && z+b>=0 && z+b<16)
                                            {
                                                if (i<1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*(i-1)+256*j+16*(16+x+a)+z+b);
                                            }
                                            // #3
                                            else if (x+a<0 && z+b<=16)
                                            {
                                                if (i<1 || j>=WORLD_CHUNK-1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*(i-1)+256*(j+1)+16*(16+x+a)+(-16+z+b));
                                            }
                                            // #4
                                            else if (x+a>=0 && x+a<16 && z+b<0)
                                            {
                                                if (j<1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*i+256*(j-1)+16*(x+a)+(16+z+b));
                                            }
                                            // #5
                                            else if (x+a>=0 && x+a<16 && z+b>=0 && z+b<16)
                                                tmp = *(wood_put+16384*i+256*j+16*(x+a)+(z+b));
                                            // #6
                                            else if (x+a>=0 && x+a<16 && z+b>=16)
                                            {
                                                if (j>=WORLD_CHUNK-1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*i+256*(j+1)+16*(x+a)+(-16+z+b));
                                            }
                                            // #7
                                            else if (x+a>=16 && z+b<0)
                                            {
                                                if (i>=WORLD_CHUNK-1 && j < 1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*(i+1)+256*(j-1)+16*(-16+x+a)+(16+z+b));
                                            }
                                            // #8
                                            else if (x+a>=16 && z+b>=0 && z+b<16)
                                            {
                                                if (i>=WORLD_CHUNK-1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*(i+1)+256*j+16*(-16+x+a)+(z+b));
                                            }
                                            // #9
                                            else if (x+a>=16 && z+b>=16)
                                            {
                                                if (i>=WORLD_CHUNK-1 && j>=WORLD_CHUNK-1)
                                                    tmp = *(wood_put+16384*i+256*j+16*x+z);
                                                else 
                                                    tmp = *(wood_put+16384*(i+1)+256*(j+1)+16*(-16+x+a)+(-16+z+b));
                                            }
                                            else
                                                tmp = *(wood_put+16384*i+256*j+16*x+z);

                                            if (tmp == 1)
                                                wood_judge = 1;
                                        }
                                    }
                                    if (wood_judge == 0)
                                    {
                                        for(int a=1; a<=5; a++)
                                        {
                                            *(onechunk3d+x*4096+(y+a)*16+z) = 10;
                                            *(wood_put+16384*i+256*j+16*x+z) = 1;
                                        }
                                        for(int c=3; c<6; c++)
                                        {
                                            for(int a=3-c; a<=(c-3); a++)
                                            {
                                                for(int b=3-c; b<=(c-3); b++)
                                                {
                                                    if (x+a>=0 && x+a<16 && z+b>=0 && z+b<16)
                                                    {
                                                        if (c == 3)
                                                        {
                                                            *(onechunk3d+(x+a)*4096+(y+9-c)*16+(z+b)) = 11;
                                                        }
                                                        else if (a == 0 && b == 0);
                                                        else
                                                        {
                                                            *(onechunk3d+(x+a)*4096+(y+9-c)*16+(z+b)) = 11;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
            SaveChunk(onechunk3d, i, j);
            printf("\rcreated chunk: %d, %d (%5.2lf%%)", i, j, (double)(64*i+j+1) / 4096.0 * 100.0);
            fflush(stdout);
        }
    }
    free(onechunk3d);
    free(onechunk2d);
    free(onechunk1d_x);
    free(onechunk1d_y);
    free(onechunk1d_z);
}

void SaveChunk(int* chunk, int savex, int savey)
{
    char filename[30];
    sprintf(filename, "data/%d_%d.dat", savex, savey);
    FILE *f = fopen(filename, "wb");
    fwrite(chunk, sizeof(int), 16*16*256, f);
    fclose(f);
}

void LoadChunk(int* chunk, int loadx, int loady)
{
    char filename[30];
    sprintf(filename, "data/%d_%d.dat", loadx, loady);
    FILE *f = fopen(filename, "rb");
    fread(chunk, sizeof(int), 16*16*256, f);
    fclose(f);
}

void SaveItem(int* bag)
{
    FILE *f = fopen("player_bag", "wb");
        fwrite(bag, sizeof(int), 72, f);
    fclose(f);
}

void LoadItem(int* bag)
{
    FILE *f = fopen("player_bag", "rb");
    if (f == NULL)
        for(int i=0; i<72; i++)
            *(bag+i) = 0;
    else
        fread(bag, sizeof(int), 72, f);
    fclose(f);
}

void SaveStatus(int* status)
{
    FILE *f = fopen("player_status", "wb");
        fwrite(status, sizeof(int), 2, f);
    fclose(f);
}

void LoadStatus(int* status)
{
    FILE *f = fopen("player_status", "rb");
    if (f == NULL)
    {
        *(status+0) = 20;
        *(status+1) = 18;
    }
    else
        fread(status, sizeof(int), 2, f);
    fclose(f);
}