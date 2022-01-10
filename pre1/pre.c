/* includes */
#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

/*define*/
#define WORLD_CHUNK 64
#define DRAW_DIST 8

/* functions */
// Wavelet
void Wavelet1d(double[256], double);
void Wavelet2d(double[256][256], double);
// PerlinNoize
void PerlinNoize1d(double *, int, int, int, unsigned int);
void PerlinNoize2di(int *, int, int, int, unsigned int, int);
void PerlinNoize2dd(double *, int, int, int, unsigned int, int);
// World Create
void CreateChunk(int, int);
// Save/Load
void SaveChunk(int *, int, int);
void LoadChunk(int *, int, int);
void LoadItem(int *);
void SaveItem(int *);
void SaveStatus(int *);
void LoadStatus(int *);
// DrawItemBoxItems
void DrawTexBlock2d(int, int, double, double);
void DrawTexItem(int, int, double, double, double, double);
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
// create
double *biome;
double *diamond_rate;
double *gold_rate;
double *iron_rate;
double *coal_rate;
double *wood_rate;
char *wood_put;
double *onechunk1d_y;

unsigned int SEED_NUM = 0;
int seed_length=0;
char seed_str[20]={0};

// player position info
double player_x = 136.0;
double player_y = 255.0;
double player_z = 136.0;

double resporn_player_x = 136.0;
double resporn_player_y = 255.0;
double resporn_player_z = 136.0;

int player_block_x = 136;
int player_block_y = 255;
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

int view_block_x, view_block_y, view_block_z;
int view_chunk_x, view_chunk_y, view_chunk_z;
int view_local_x, view_local_y, view_local_z;
int next_view_block_x, next_view_block_y, next_view_block_z;
int next_view_chunk_x, next_view_chunk_y, next_view_chunk_z;
int next_view_local_x, next_view_local_y, next_view_local_z;

double dig_hardness = 100.0;
double block_hardness[16][6] = {
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
    {0.75, 0.40, 0.20, 0.15, 0.10, 0.10},
    {0.75, 0.40, 0.20, 0.15, 0.10, 0.10},
    {15.0, 1.15, 0.60, 0.40, 0.20, 0.30},
    {15.0, 2.25, 1.15, 0.75, 0.40, 0.60},
    {15.0, 7.50, 1.15, 0.75, 1.25, 0.60},
    {15.0, 7.50, 3.75, 0.75, 1.25, 0.60},
    {15.0, 7.50, 3.75, 0.75, 1.25, 0.60},
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
    {0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
    {3.00, 1.50, 0.75, 0.50, 0.25, 0.40},
    {0.30, 0.15, 0.10, 0.10, 0.05, 0.05},
    {3.00, 1.50, 0.75, 0.50, 0.25, 0.40},
    {3.75, 1.90, 0.95, 0.65, 0.35, 0.50},
    {17.5, 2.65, 1.35, 0.90, 0.45, 0.70},
    {0.45, 0.00, 0.00, 0.00, 0.00, 0.00},
    };

int block_breaker[16] = {0, 2, 2, 1, 1, 1, 1, 1, 0, 0, 3, 4, 3, 3, 1, 0};
int canget[16] = {0, 0, 0, 1, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0};

double hungry_time = 1.0;
double hungry = 0.0;

double eating_time = 0.0;
double eat_time = 1.0;

int no_eat = 0;
double no_eating_time = 0.0;
double no_eat_time = 1.0;

int full_eat = 0;
double full_eating_time = 0.0;
double full_eat_time = 1.0;

int dig_tag = 0;
double player_dig_speed = 0.075;

double diamond_create_rate = 0.001;
double gold_create_rate = 0.001;
double iron_create_rate = 0.005;
double coal_create_rate = 0.01;
double wood_create_rate[2] = {0.01, 0.1};
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
int dig_now_x = -1000, dig_now_y = -1000, dig_now_z = -1000;
int dig_flag = 0;

// Player model position info - {x_min, x_max, y_min, y_max, z_min, z_max}
double player_model_pos_info[3][6] = {
    {-0.1125, 0.1125, 0.1125, -0.5625, -0.1125, 0.1125},
    {-0.1125, 0.1125, -0.825, -0.15, -0.225, 0.225},
    {-0.225, 0.225, -0.15, 0.3, -0.225, 0.225}
    };
// Player model mov info - {x, y, z}
double player_model_mov_info[4][3] = {
    {0.0, -0.9375, 0.1125},
    {0.0, -0.9375, -0.1125},
    {0.0, -0.2625, -0.3375},
    {0.0, -0.2625, 0.3375}
    };
// Player model color info - {x, y, z}
double player_model_color_info[6][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 0.5, 0.0}
    };

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
char net_str[30];
char het_str[30];
char infomation[60];
// for debug
int flag = 0;
int debugmode = 0;
// status
int *player_bag;
int *player_status;
double player_hide_hungry = 0;
//scene
int scene = 0;

//for loading
int load_counter = 0;

// pause
int debug = 0;
int pause = 0;
int enventory = 0;
int craft = 0;
int fire = 0;
int menu = 0;

// swap
int dragging_item[4] = {-1, 0, 0, -1};

// craft itembox 4 x {etr-id, Bid, NoB, durability}
int craft_itembox[4][4] = {
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1}
    };
// crafted item 1 x {Bid, NoB, durability}
int crafted_item[3] = {0, 0, -1};

// craft itembox 9 x {etr-id, Bid, NoB, durability}
int bigcraft_itembox[9][4] = {
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1}
    };
// crafted item 1 x {Bid, NoB, durability}
int bigcrafted_item[3] = {0, 0, -1};

// funrnace material {etr-id, Bid, NoB, durability}
int material_item[4] = {-1, 0, 0, -1};
// funrnace fuel {etr-id, Bid, NoB, durability}
int fuel_item[4] = {-1, 0, 0, -1};
// funrnace product {Bid, NoB, durability}
int product_item[3] = {0, 0, -1};

// recipe22            ip ip ip ip   op op  db
int recipe22[7][7] = {
    {10, 0, 0, 0, 12, 4, -1},    // lamber patern1
    {0, 10, 0, 0, 12, 4, -1},    // lamber patern2
    {0, 0, 10, 0, 12, 4, -1},    // lamber patern3
    {0, 0, 0, 10, 12, 4, -1},    // lamber patern4
    {12, 12, 12, 12, 13, 1, -1}, // craft table
    {12, 0, 12, 0, 1025, 4, -1}, // bar patern1
    {0, 12, 0, 12, 1025, 4, -1}  // bar patern2
};                               // bar patern2

// recipe33                ip   ip   ip   ip   ip   ip   ip   ip   ip   op op   db
int recipe33[60][12] = {
    {10, 0, 0, 0, 0, 0, 0, 0, 0, 12, 4, -1},           // lamber patern1
    {0, 10, 0, 0, 0, 0, 0, 0, 0, 12, 4, -1},           // lamber patern2
    {0, 0, 10, 0, 0, 0, 0, 0, 0, 12, 4, -1},           // lamber patern3
    {0, 0, 0, 10, 0, 0, 0, 0, 0, 12, 4, -1},           // lamber patern4
    {0, 0, 0, 0, 10, 0, 0, 0, 0, 12, 4, -1},           // lamber patern5
    {0, 0, 0, 0, 0, 10, 0, 0, 0, 12, 4, -1},           // lamber patern6
    {0, 0, 0, 0, 0, 0, 10, 0, 0, 12, 4, -1},           // lamber patern7
    {0, 0, 0, 0, 0, 0, 0, 10, 0, 12, 4, -1},           // lamber patern8
    {0, 0, 0, 0, 0, 0, 0, 0, 10, 12, 4, -1},           // lamber patern9
    {0, 0, 0, 0, 12, 12, 0, 12, 12, 13, 1, -1},        // craft table patern1
    {0, 0, 0, 12, 12, 0, 12, 12, 0, 13, 1, -1},        // craft table patern2
    {0, 12, 12, 0, 12, 12, 0, 0, 0, 13, 1, -1},        // craft table patern3
    {12, 12, 0, 12, 12, 0, 0, 0, 0, 13, 1, -1},        // craft table patern4
    {3, 3, 3, 3, 0, 3, 3, 3, 3, 14, 1, -1},            // furnace
    {12, 0, 0, 12, 0, 0, 0, 0, 0, 1025, 4, -1},        // bar patern1
    {0, 12, 0, 0, 12, 0, 0, 0, 0, 1025, 4, -1},        // bar patern2
    {0, 0, 12, 0, 0, 12, 0, 0, 0, 1025, 4, -1},        // bar patern3
    {0, 0, 0, 12, 0, 0, 12, 0, 0, 1025, 4, -1},        // bar patern4
    {0, 0, 0, 0, 12, 0, 0, 12, 0, 1025, 4, -1},        // bar patern5
    {0, 0, 0, 0, 0, 12, 0, 0, 12, 1025, 4, -1},        // bar patern6
    {12, 12, 12, 0, 1025, 0, 0, 1025, 0, 1026, 1, 59}, //     ork pickaxe
    {3, 3, 3, 0, 1025, 0, 0, 1025, 0, 1027, 1, 131},   //    rock pickaxe
    {5, 5, 5, 0, 1025, 0, 0, 1025, 0, 1028, 1, 250},   //    iron pickaxe
    {6, 6, 6, 0, 1025, 0, 0, 1025, 0, 1039, 1, 32},    //    gold pickaxe
    {7, 7, 7, 0, 1025, 0, 0, 1025, 0, 1030, 1, 1561},  // diamond pickaxe
    {12, 0, 0, 1025, 0, 0, 1025, 0, 0, 1031, 1, 59},   //     ork shovel pattern 1
    {0, 12, 0, 0, 1025, 0, 0, 1025, 0, 1031, 1, 59},   //     ork shovel pattern 2
    {0, 0, 12, 0, 0, 1025, 0, 0, 1025, 1031, 1, 59},   //     ork shovel pattern 3
    {3, 0, 0, 1025, 0, 0, 1025, 0, 0, 1032, 1, 131},   //    rock shovel pattern 1
    {0, 3, 0, 0, 1025, 0, 0, 1025, 0, 1032, 1, 131},   //    rock shovel pattern 2
    {0, 0, 3, 0, 0, 1025, 0, 0, 1025, 1032, 1, 131},   //    rock shovel pattern 3
    {5, 0, 0, 1025, 0, 0, 1025, 0, 0, 1033, 1, 250},   //    iron shovel pattern 1
    {0, 5, 0, 0, 1025, 0, 0, 1025, 0, 1033, 1, 250},   //    iron shovel pattern 2
    {0, 0, 5, 0, 0, 1025, 0, 0, 1025, 1033, 1, 250},   //    iron shovel pattern 3
    {6, 0, 0, 1025, 0, 0, 1025, 0, 0, 1034, 1, 32},    //    gold shovel pattern 1
    {0, 6, 0, 0, 1025, 0, 0, 1025, 0, 1034, 1, 32},    //    gold shovel pattern 2
    {0, 0, 6, 0, 0, 1025, 0, 0, 1025, 1034, 1, 32},    //    gold shovel pattern 3
    {7, 0, 0, 1025, 0, 0, 1025, 0, 0, 1035, 1, 1561},  // diamond shovel pattern 1
    {0, 7, 0, 0, 1025, 0, 0, 1025, 0, 1035, 1, 1561},  // diamond shovel pattern 2
    {0, 0, 7, 0, 0, 1025, 0, 0, 1025, 1035, 1, 1561},  // diamond shovel pattern 3
    {12, 12, 0, 1025, 12, 0, 1025, 0, 0, 1036, 1, 59}, //     ork axe pattern 1
    {12, 12, 0, 12, 1025, 0, 0, 1025, 0, 1036, 1, 59}, //     ork axe pattern 2
    {0, 12, 12, 0, 1025, 12, 0, 1025, 0, 1036, 1, 59}, //     ork axe pattern 3
    {0, 12, 12, 0, 12, 1025, 0, 0, 1025, 1036, 1, 59}, //     ork axe pattern 4
    {3, 3, 0, 1025, 3, 0, 1025, 0, 0, 1037, 1, 131},   //    rock axe pattern 1
    {3, 3, 0, 3, 1025, 0, 0, 1025, 0, 1037, 1, 131},   //    rock axe pattern 2
    {0, 3, 3, 0, 1025, 3, 0, 1025, 0, 1037, 1, 131},   //    rock axe pattern 3
    {0, 3, 3, 0, 3, 1025, 0, 0, 1025, 1037, 1, 131},   //    rock axe pattern 4
    {5, 5, 0, 1025, 5, 0, 1025, 0, 0, 1038, 1, 250},   //    iron axe pattern 1
    {5, 5, 0, 5, 1025, 0, 0, 1025, 0, 1038, 1, 250},   //    iron axe pattern 2
    {0, 5, 5, 0, 1025, 5, 0, 1025, 0, 1038, 1, 250},   //    iron axe pattern 3
    {0, 5, 5, 0, 5, 1025, 0, 0, 1025, 1038, 1, 250},   //    iron axe pattern 4
    {6, 6, 0, 1025, 6, 0, 1025, 0, 0, 1039, 1, 32},    //    gold axe pattern 1
    {6, 6, 0, 6, 1025, 0, 0, 1025, 0, 1039, 1, 32},    //    gold axe pattern 2
    {0, 6, 6, 0, 1025, 6, 0, 1025, 0, 1039, 1, 32},    //    gold axe pattern 3
    {0, 6, 6, 0, 6, 1025, 0, 0, 1025, 1039, 1, 32},    //    gold axe pattern 4
    {7, 7, 0, 1025, 7, 0, 1025, 0, 0, 1040, 1, 1561},  // diamond axe pattern 1
    {7, 7, 0, 7, 1025, 0, 0, 1025, 0, 1040, 1, 1561},  // diamond axe pattern 2
    {0, 7, 7, 0, 1025, 7, 0, 1025, 0, 1040, 1, 1561},  // diamond axe pattern 3
    {0, 7, 7, 0, 7, 1025, 0, 0, 1025, 1040, 1, 1561},  // diamond axe pattern 4
    };

// furnace
int furnace_ope = 0;
double furnace_opetime = 0;
double furnace_createtime = 10.0;
double furnace_fueltime = 0.0;
double furnace_fuelnow = 100.0;
double furnace_fueltime_block[17] = {0.0, 0.0, 0.0, 0.0, 80.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 0.0, 15.0, 15.0, 0.0, 0.0, 0.0};
double furnace_fueltime_items[17] = {5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0};
int furnace_creating = -1;
// recipe (furnace) type: {material, product}
double furnace_recipe[1][2] = {{2, 15}};

// durability
int durability_list[15] = {59, 131, 250, 32, 1561, 59, 131, 250, 32, 1561, 59, 131, 250, 32, 1561};
int durability_empty_list[15] = {3, 6, 10, 2, 61, 3, 6, 10, 2, 61, 3, 6, 10, 2, 61};

// texture
GLuint img;
pngInfo info;
GLuint img2;
pngInfo info2;

// pickle code
int pickle_ci[5] = {12, 3, 5, 6, 7};

int main(int argc, char **argv)
{
    // require 272.25MB
    chunk = (int *)malloc(sizeof(int) * (2 * DRAW_DIST + 1) * (2 * DRAW_DIST + 1) * 16 * 256 * 16);
    nextchunk = (int *)malloc(sizeof(int) * (2 * DRAW_DIST + 1) * (2 * DRAW_DIST + 1) * 16 * 256 * 16);
    map2d = (int *)malloc(sizeof(int) * (2 * DRAW_DIST + 1) * (2 * DRAW_DIST + 1) * 16 * 16);
    chunkdata_x = (double *)malloc(sizeof(double) * (2 * DRAW_DIST + 1) * 16);
    chunkdata_y = (double *)malloc(sizeof(double) * (2 * DRAW_DIST + 1) * 16);
    chunkdata_z = (double *)malloc(sizeof(double) * (2 * DRAW_DIST + 1) * 16);
    player_bag = (int *)malloc(sizeof(int) * 108);
    player_status = (int *)malloc(sizeof(int) * 2);
    // Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(640, 480);

    // Create Window
    glutEnterGameMode();

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

    // load image
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    img = pngBind("img/texture.png", PNG_NOMIPMAP, PNG_ALPHA, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    img2 = pngBind("img/logo.png", PNG_NOMIPMAP, PNG_ALPHA, &info2, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    glutMainLoop();
    return (0);
}

void render(void)
{
    if (scene==0)
    {
        // window size
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        int i=0, j=0;
        char title_btn1[10];
        char title_btn2[10];
        char title_btn3[10];
        sprintf(title_btn1, "COUNTINUE");
        sprintf(title_btn2, "NEWGAME");
        sprintf(title_btn3, "QUIT");
        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Camera setting
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, w, 0.0, h);
        glScaled(1, -1, 1);
        glTranslated(0, -h, 0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
        glBindTexture(GL_TEXTURE_2D, img);
        glBegin(GL_QUADS);
        for(; i<w; i+=64)
        {
            for(j=0; j<192; j+=64)
            {
                glTexCoord2f(0.125, 0.0625);
                glVertex2i(i, j);
                glTexCoord2f(0.25, 0.0625);
                glVertex2i(i+64, j);
                glTexCoord2f(0.25, 0.125);
                glVertex2i(i+64, j+64);
                glTexCoord2f(0.125, 0.125);
                glVertex2i(i, j+64);
            }
            for(j=192; j<h; j+=64)
            {
                glTexCoord2f(0.125, 0.1875);
                glVertex2i(i, j);
                glTexCoord2f(0.25, 0.1875);
                glVertex2i(i+64, j);
                glTexCoord2f(0.25, 0.25);
                glVertex2i(i+64, j+64);
                glTexCoord2f(0.125, 0.25);
                glVertex2i(i, j+64);
            }
            j-=64;
            glTexCoord2f(0.125, 0.5);
            glVertex2i(i, j);
            glTexCoord2f(0.25, 0.5);
            glVertex2i(i+64, j);
            glTexCoord2f(0.25, 0.5625);
            glVertex2i(i+64, j+64);
            glTexCoord2f(0.125, 0.5625);
            glVertex2i(i, j+64);
        }
        glEnd();
        glBindTexture(GL_TEXTURE_2D, img2);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2i(w/2-256, h/2-160);
            glTexCoord2f(1, 0);
            glVertex2i(w/2+256, h/2-160);
            glTexCoord2f(1, 1);
            glVertex2i(w/2+256, h/2-32);
            glTexCoord2f(0, 1);
            glVertex2i(w/2-256, h/2-32);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        // Btn
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_QUADS);
            glVertex2i(w/2-128, h/2+96);
            glVertex2i(w/2+128, h/2+96);
            glVertex2i(w/2+128, h/2+32);
            glVertex2i(w/2-128, h/2+32);

            glVertex2i(w/2-128, h/2+192);
            glVertex2i(w/2+128, h/2+192);
            glVertex2i(w/2+128, h/2+128);
            glVertex2i(w/2-128, h/2+128);

            glVertex2i(w/2-128, h/2+288);
            glVertex2i(w/2+128, h/2+288);
            glVertex2i(w/2+128, h/2+224);
            glVertex2i(w/2-128, h/2+224);
        glEnd();
        glColor3f(1, 1, 1);
        glRasterPos2i(w/2-54, h/2+64+12);
        for (j = 0; j < 9; j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title_btn1[j]);
        glRasterPos2i(w/2-42, h/2+160+12);
        for (j = 0; j < 7; j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title_btn2[j]);
        glRasterPos2i(w/2-24, h/2+256+12);
        for (j = 0; j < 4; j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title_btn3[j]);
        glPopMatrix();
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
    }
    if (scene == 1)
    {
        // window size
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        int i=0, j=0;
        char inputseed[25];
        char back_str[5];
        char create_str[15];
        sprintf(inputseed, "INPUT SEED NUMBER (MAX:25)");
        sprintf(back_str, "Back");
        sprintf(create_str, "Create World");
        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Camera setting
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, w, 0.0, h);
        glScaled(1, -1, 1);
        glTranslated(0, -h, 0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
        glBindTexture(GL_TEXTURE_2D, img);
        glBegin(GL_QUADS);
        for(; i<w; i+=64)
        {
            for(j=0; j<192; j+=64)
            {
                glTexCoord2f(0.125, 0.0625);
                glVertex2i(i, j);
                glTexCoord2f(0.25, 0.0625);
                glVertex2i(i+64, j);
                glTexCoord2f(0.25, 0.125);
                glVertex2i(i+64, j+64);
                glTexCoord2f(0.125, 0.125);
                glVertex2i(i, j+64);
            }
            for(j=192; j<h; j+=64)
            {
                glTexCoord2f(0.125, 0.1875);
                glVertex2i(i, j);
                glTexCoord2f(0.25, 0.1875);
                glVertex2i(i+64, j);
                glTexCoord2f(0.25, 0.25);
                glVertex2i(i+64, j+64);
                glTexCoord2f(0.125, 0.25);
                glVertex2i(i, j+64);
            }
            j-=64;
            glTexCoord2f(0.125, 0.5);
            glVertex2i(i, j);
            glTexCoord2f(0.25, 0.5);
            glVertex2i(i+64, j);
            glTexCoord2f(0.25, 0.5625);
            glVertex2i(i+64, j+64);
            glTexCoord2f(0.125, 0.5625);
            glVertex2i(i, j+64);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        // gage
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_QUADS);
            glVertex2i(w/4, h/2+32);
            glVertex2i(3*w/4, h/2+32);
            glVertex2i(3*w/4, h/2-32);
            glVertex2i(w/4, h/2-32);

            glVertex2i(w/4, h/2+128);
            glVertex2i(3*w/4, h/2+128);
            glVertex2i(3*w/4, h/2+64);
            glVertex2i(w/4, h/2+64);
            if (seed_length > 0)
            {
                glVertex2i(w/4, h/2+224);
                glVertex2i(3*w/4, h/2+224);
                glVertex2i(3*w/4, h/2+160);
                glVertex2i(w/4, h/2+160);
            }
        glEnd();
        glColor3f(1, 1, 1);
        glRasterPos2i(w/2-156, h/2+12);
        if (seed_length == 0)
        {
            for (j = 0; j < 26; j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, inputseed[j]);
        }
        else
        {
            glRasterPos2i(w/2-6*seed_length, h/2+12);
            for (j = 0; j < seed_length; j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, seed_str[j]);
        }
        glRasterPos2i(w/2-12, h/2+108);
        for (j = 0; j < 4; j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, back_str[j]);
        if (seed_length > 0)
        {
            glRasterPos2i(w/2-72, h/2+204);
            for (j = 0; j <12 ; j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, create_str[j]);
        }
        glPopMatrix();
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
    }
    else if (scene == 2)
    {
        // window size
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        int i=0, j=0;
        char sentence[18];
        double percentage = (double)(load_counter) / (double)(WORLD_CHUNK*WORLD_CHUNK);
        int per_length = percentage * (double)(w/2);

        if (load_counter < WORLD_CHUNK*WORLD_CHUNK)
            sprintf(sentence, "Creating World ...");
        else
            sprintf(sentence, " Loading World ...");
        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Camera setting
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, w, 0.0, h);
        glScaled(1, -1, 1);
        glTranslated(0, -h, 0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
        glBindTexture(GL_TEXTURE_2D, img);
        glBegin(GL_QUADS);
        for(; i<w; i+=64)
        {
            for(j=0; j<192; j+=64)
            {
                glTexCoord2f(0.125, 0.0625);
                glVertex2i(i, j);
                glTexCoord2f(0.25, 0.0625);
                glVertex2i(i+64, j);
                glTexCoord2f(0.25, 0.125);
                glVertex2i(i+64, j+64);
                glTexCoord2f(0.125, 0.125);
                glVertex2i(i, j+64);
            }
            for(j=192; j<h; j+=64)
            {
                glTexCoord2f(0.125, 0.1875);
                glVertex2i(i, j);
                glTexCoord2f(0.25, 0.1875);
                glVertex2i(i+64, j);
                glTexCoord2f(0.25, 0.25);
                glVertex2i(i+64, j+64);
                glTexCoord2f(0.125, 0.25);
                glVertex2i(i, j+64);
            }
            j-=64;
            glTexCoord2f(0.125, 0.5);
            glVertex2i(i, j);
            glTexCoord2f(0.25, 0.5);
            glVertex2i(i+64, j);
            glTexCoord2f(0.25, 0.5625);
            glVertex2i(i+64, j+64);
            glTexCoord2f(0.125, 0.5625);
            glVertex2i(i, j+64);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        // gage
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_QUADS);
            glVertex2i(w/4, h/2+96);
            glVertex2i(3*w/4, h/2+96);
            glVertex2i(3*w/4, h/2+32);
            glVertex2i(w/4, h/2+32);
        glEnd();
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
            glVertex2i(w/4, h/2+96);
            glVertex2i(w/4+per_length, h/2+96);
            glVertex2i(w/4+per_length, h/2+32);
            glVertex2i(w/4, h/2+32);
        glEnd();
        glColor3f(1, 1, 1);
        glRasterPos2i(w/2-108, h/2+12);
        for (j = 0; j < 18; j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, sentence[j]);
        glPopMatrix();
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
        if (load_counter <= WORLD_CHUNK*WORLD_CHUNK-1)
        {
            CreateChunk(load_counter / WORLD_CHUNK, load_counter % WORLD_CHUNK);
        }
        if (load_counter == WORLD_CHUNK*WORLD_CHUNK)
        {
            free(diamond_rate);
            free(gold_rate);
            free(iron_rate);
            free(wood_rate);
            free(wood_put);
            free(coal_rate);
            free(onechunk1d_y);
            for (int i = 0; i < 108; i++)
                *(player_bag + i) = 0;
            *(player_status + 0) = 20;
            *(player_status + 1) = 18;
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    LoadChunk(chunk + i * 1114112 + j * 65536, i, j);
            printf("successA");
            fflush(stdout);
            while(*(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + player_local_z) == 0)
            {
                player_y -= 1.0;
                player_block_y --;
            }
            printf("successB");
            fflush(stdout);
            resporn_player_y = player_y;
            scene = 3;
        }
        load_counter ++;
    }
    else if (scene == 3)
    {
        double speed = 0.21;
        double sneek_speed = 0.304;
        double dash_speed = 1.3;
        double next_player_x = player_x, next_player_z = player_z;
        int next_player_block_x, next_player_block_z;
        int next_player_chunk_x, next_player_chunk_z;
        int next_player_local_x, next_player_local_z;
        int pickle_type = 0;
        int shovel_type = 0;
        int axe_type = 0;

        if (key_state[4] == 1)
            speed *= sneek_speed;
        else if (key_state[5] == 1 && *(player_status + 1) > 6)
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
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    SaveChunk(chunk + i * 1114112 + j * 65536, i + player_pre_loadchunk_x, j + player_pre_loadchunk_z);
            player_pre_loadchunk_x = 0;
            player_pre_loadchunk_z = 0;
            next_player_x = resporn_player_x;
            next_player_x = resporn_player_x;
            player_y = resporn_player_y;
            next_player_z = resporn_player_z;
            *(player_status) = 20;
            *(player_status + 1) = 18;
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    LoadChunk(chunk + i * 1114112 + j * 65536, i, j);
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
                    if (player_jump_tick == -1)
                        hungry += 0.02;
                    else
                        hungry += 0.16;
                else
                    hungry += 0.002;
            }
        }

        next_player_block_x = (int)next_player_x;
        next_player_chunk_x = (int)next_player_block_x / 16;
        next_player_local_x = (int)next_player_block_x % 16;
        if (*(chunk + (next_player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (next_player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + player_local_z) == 0 && *(chunk + (next_player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (next_player_local_x)*4096 + ((int)(player_y - 0.5)) * 16 + player_local_z) == 0 && next_player_x > 16.0 && next_player_x < 16.0 * (WORLD_CHUNK - 1))
        {
            player_x = next_player_x;
            player_block_x = next_player_block_x;
            player_chunk_x = next_player_chunk_x;
            player_local_x = next_player_local_x;
        }

        next_player_block_z = (int)next_player_z;
        next_player_chunk_z = (int)next_player_block_z / 16;
        next_player_local_z = (int)next_player_block_z % 16;
        if (*(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + next_player_local_z) == 0 && *(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 0.5)) * 16 + next_player_local_z) == 0 && next_player_z > 16.0 && next_player_z < 16.0 * (WORLD_CHUNK - 1))
        {
            player_z = next_player_z;
            player_block_z = next_player_block_z;
            player_chunk_z = next_player_chunk_z;
            player_local_z = next_player_local_z;
        }

        // reload chunk
        if (player_chunk_x - player_pre_chunk_x >= 4 || player_chunk_x - player_pre_chunk_x <= -4 || player_chunk_z - player_pre_chunk_z >= 4 || player_chunk_z - player_pre_chunk_z <= -4)
        {
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    SaveChunk(chunk + i * 1114112 + j * 65536, i + player_pre_loadchunk_x, j + player_pre_loadchunk_z);
            player_pre_chunk_x = player_chunk_x;
            player_pre_loadchunk_x = player_pre_chunk_x - 8;
            if (player_pre_loadchunk_x < 0)
                player_pre_loadchunk_x = 0;
            if (player_pre_loadchunk_x > WORLD_CHUNK - (2 * DRAW_DIST + 1))
                player_pre_loadchunk_x = WORLD_CHUNK - (2 * DRAW_DIST + 1);
            player_pre_chunk_z = player_chunk_z;
            player_pre_loadchunk_z = player_pre_chunk_z - 8;
            if (player_pre_loadchunk_z < 0)
                player_pre_loadchunk_z = 0;
            if (player_pre_loadchunk_z > WORLD_CHUNK - (2 * DRAW_DIST + 1))
                player_pre_loadchunk_z = WORLD_CHUNK - (2 * DRAW_DIST + 1);
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    LoadChunk(chunk + i * 1114112 + j * 65536, i + player_pre_loadchunk_x, j + player_pre_loadchunk_z);
        }
        while (1)
        {
            if (player_hide_hungry > 0)
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
                if (*(player_status + 1) > 0)
                {
                    *(player_status + 1) -= 1;
                }
                hungry -= hungry_time;
            }
        }

        if (*(player_status + 1) > 0)
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
        else
            no_eating_time = 0;

        if (*(player_status + 1) < 17)
        {
            full_eat = 0;
        }
        else
        {
            full_eat = 1;
        }
        if (full_eat == 1 && *(player_status) < 20)
        {
            full_eating_time += 0.05;
            if (full_eating_time > full_eat_time)
            {
                *(player_status) += 1;
                full_eating_time -= full_eat_time;
            }
        }
        else
            full_eating_time = 0;

        if (fps_count % 100 == 0)
            gettimeofday(&start, NULL);
        // window size
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        // pickle type
        if (*(player_bag + 3 * debugmode) >= 1026 && *(player_bag + 3 * debugmode) < 1031)
            pickle_type = *(player_bag + 3 * debugmode) - 1025;
        else
            pickle_type = 0;
        // shovel type
        if (*(player_bag + 3 * debugmode) >= 1031 && *(player_bag + 3 * debugmode) < 1036)
            shovel_type = *(player_bag + 3 * debugmode) - 1030;
        else
            shovel_type = 0;
        // axe type
        if (*(player_bag + 3 * debugmode) >= 1036 && *(player_bag + 3 * debugmode) < 1041)
            axe_type = *(player_bag + 3 * debugmode) - 1035;
        else
            axe_type = 0;

        // mouse position
        if (pause == 0)
        {
            mouse_x = w / 2;
            mouse_y = h / 2;
        }

        // y position process
        if (player_fall_tick == -1 && player_jump_tick == -1 && *(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + player_local_z) == 0)
        {
            player_fall_tick = 0;
            player_pre_block_y = player_block_y;
        }

        if (player_fall_tick >= 0)
        {
            player_fall_velocity = JumpVelocity(0.0, player_gravity_acceleration, player_air_resistance, player_fall_tick);
            player_y += player_fall_velocity;
            player_block_y = (int)(player_y - 1.5);
            player_fall_tick++;
            if (*(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + player_local_z) != 0)
            {
                player_y = (double)player_block_y + 2.5;
                player_block_y = (int)(player_y - 1.5);
                player_chunk_y = player_block_y / 16;
                player_local_y = player_block_y % 16;
                player_fall_tick = -1;
                if (player_pre_block_y > player_block_y + 3)
                    *(player_status) -= player_pre_block_y - (player_block_y + 3);
            }
        }

        if (player_jump_tick >= 0)
        {
            player_jump_velocity = JumpVelocity(player_jump_start_velocity, player_gravity_acceleration, player_air_resistance, player_jump_tick);
            player_y += player_jump_velocity;
            player_block_y = (int)(player_y - 1.5);
            player_jump_tick++;
            if (*(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y + 0.2)) * 16 + player_local_z) != 0)
            {
                player_y = (double)player_block_y + 1.8;
                player_block_y = (int)(player_y - 1.5);
                player_chunk_y = player_block_y / 16;
                player_local_y = player_block_y % 16;
                player_jump_tick = -1;
                player_fall_tick = 0;
            }
            else if (*(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + player_local_z) != 0)
            {
                player_y = (double)player_block_y + 2.5;
                player_block_y = (int)(player_y - 1.5);
                player_chunk_y = player_block_y / 16;
                player_local_y = player_block_y % 16;
                player_jump_tick = -1;
                if (player_pre_block_y + 1 > player_block_y + 3)
                    *(player_status) -= (player_pre_block_y + 1) - (player_block_y + 3);
            }
        }

        // EAT
        if (mouse_status[0] == 1)
        {
            if (*(player_bag + 3 * debugmode) == 1041 && *(player_bag + 3 * debugmode + 1) > 0)
            {
                eating_time += 0.05;
                if (eating_time > eat_time)
                {
                    *(player_bag + 3 * debugmode + 1) -= 1;
                    eating_time = 0;
                    *(player_status + 1) += apple_hungry;
                    if (*(player_status + 1) > 18)
                        *(player_status + 1) = 18;
                    player_hide_hungry += apple_hide_hungry;
                    if (player_hide_hungry > *(player_status + 1))
                        player_hide_hungry = (double)(*(player_status + 1));

                    if (*(player_bag + 3 * debugmode + 1) == 0)
                    {
                        *(player_bag + 3 * debugmode) = 0;
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
            for (double a = 0; a < 5.0; a += 0.05)
            {
                dig_search_after_x = player_x + a * cos(xz_rad) * cos(y_rad);
                dig_search_after_block_x = (int)dig_search_after_x;
                dig_search_after_chunk_x = dig_search_after_block_x / 16;
                dig_search_after_local_x = dig_search_after_block_x % 16;
                dig_search_after_y = player_y - a * sin(y_rad);
                dig_search_after_z = player_z + a * sin(xz_rad) * cos(y_rad);
                dig_search_after_block_z = (int)dig_search_after_z;
                dig_search_after_chunk_z = dig_search_after_block_z / 16;
                dig_search_after_local_z = dig_search_after_block_z % 16;
                int tmp = *(chunk + (dig_search_after_chunk_x - player_pre_loadchunk_x) * 1114112 + (dig_search_after_chunk_z - player_pre_loadchunk_z) * 65536 + (dig_search_after_local_x)*4096 + (int)(dig_search_after_y)*16 + dig_search_after_local_z);
                if (tmp > 0 && tmp < 16 && tmp != 8 && tmp != 9)
                {
                    if (dig_now_x == (int)dig_search_after_x && dig_now_y == (int)dig_search_after_y && dig_now_z == (int)dig_search_after_z)
                        dig_hardness -= player_dig_speed;
                    else
                    {
                        dig_now_x = (int)dig_search_after_x;
                        dig_now_y = (int)dig_search_after_y;
                        dig_now_z = (int)dig_search_after_z;
                        if (block_breaker[tmp] == 1)
                            dig_hardness = block_hardness[tmp][pickle_type];
                        else if (block_breaker[tmp] == 2)
                            dig_hardness = block_hardness[tmp][shovel_type];
                        else if (block_breaker[tmp] == 3)
                            dig_hardness = block_hardness[tmp][axe_type];
                        else
                            dig_hardness = block_hardness[tmp][0];
                        dig_tag = 1;
                    }
                    if (dig_hardness <= 0.0)
                    {
                        if (tmp == 11 && (double)rand() / (double)RAND_MAX <= apple_drop_rate)
                        {
                            for (int i = 0; i < 36; i++)
                            {
                                if (*(player_bag + 3 * i) == 0 || (*(player_bag + 3 * i) == 1041 && *(player_bag + 3 * i + 1) < 64))
                                {
                                    *(player_bag + 3 * i) = 1041;
                                    *(player_bag + 3 * i + 1) += 1;
                                    break;
                                }
                            }
                        }
                        else if ((block_breaker[tmp] != 1 && tmp != 11) || (block_breaker[tmp] == 1 && canget[tmp] <= pickle_type))
                        {
                            for (int i = 0; i < 36; i++)
                            {
                                if (*(player_bag + 3 * i) == 0 || (*(player_bag + 3 * i) == tmp && *(player_bag + 3 * i + 1) < 64))
                                {
                                    *(player_bag + 3 * i) = tmp;
                                    *(player_bag + 3 * i + 1) += 1;
                                    break;
                                }
                            }
                        }
                        *(chunk + (dig_search_after_chunk_x - player_pre_loadchunk_x) * 1114112 + (dig_search_after_chunk_z - player_pre_loadchunk_z) * 65536 + (dig_search_after_local_x)*4096 + (int)(dig_search_after_y)*16 + dig_search_after_local_z) = 0;
                        hungry += 0.025;
                        if ((block_breaker[tmp] == 1 && canget[tmp] <= pickle_type) || (block_breaker[tmp] == 2 && shovel_type != 0) || (block_breaker[tmp] == 3 && axe_type != 0))
                            if (*(player_bag + 3 * debugmode + 2) != -1)
                            {
                                *(player_bag + 3 * debugmode + 2) -= 1;
                                if (*(player_bag + 3 * debugmode + 2) == 0)
                                {
                                    *(player_bag + 3 * debugmode) = 0;
                                    *(player_bag + 3 * debugmode + 1) = 0;
                                    *(player_bag + 3 * debugmode + 2) = -1;
                                }
                            }
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
            for (double a = 0; a < 5.0; a += 0.05)
            {
                dig_search_after_x = player_x + a * cos(xz_rad) * cos(y_rad);
                dig_search_after_block_x = (int)dig_search_after_x;
                dig_search_after_chunk_x = dig_search_after_block_x / 16;
                dig_search_after_local_x = dig_search_after_block_x % 16;
                dig_search_after_y = player_y - a * sin(y_rad);
                dig_search_after_z = player_z + a * sin(xz_rad) * cos(y_rad);
                dig_search_after_block_z = (int)dig_search_after_z;
                dig_search_after_chunk_z = dig_search_after_block_z / 16;
                dig_search_after_local_z = dig_search_after_block_z % 16;
                int tmp = *(chunk + (dig_search_after_chunk_x - player_pre_loadchunk_x) * 1114112 + (dig_search_after_chunk_z - player_pre_loadchunk_z) * 65536 + (dig_search_after_local_x)*4096 + (int)(dig_search_after_y)*16 + dig_search_after_local_z);
                if (tmp > 0 && tmp < 8)
                {
                    dig_tag = tmp;
                    break;
                }
                if (tmp >= 10 && tmp < 16)
                {
                    dig_tag = tmp;
                    break;
                }
            }
            int tmp = *(chunk + (dig_search_after_chunk_x - player_pre_loadchunk_x) * 1114112 + (dig_search_after_chunk_z - player_pre_loadchunk_z) * 65536 + (dig_search_after_local_x)*4096 + (int)(dig_search_after_y)*16 + dig_search_after_local_z);
            if (block_breaker[tmp] == 1)
                dig_hardness = block_hardness[tmp][pickle_type];
            else if (block_breaker[tmp] == 2)
                dig_hardness = block_hardness[tmp][shovel_type];
            else if (block_breaker[tmp] == 3)
                dig_hardness = block_hardness[tmp][axe_type];
            else
                dig_hardness = block_hardness[tmp][0];
        }

        // furnace tick update
        furnace_ope = 0;
        for (int i = 0; i < 1; i++)
        {
            if (furnace_recipe[i][0] == material_item[1])
            {
                if ((furnace_recipe[i][1] == product_item[0] || product_item[0] == 0) && product_item[1] != 64)
                {
                    if (fuel_item[1] < 17 && (furnace_fueltime_block[fuel_item[1]] != 0.0 || furnace_fueltime > 0))
                    {
                        furnace_ope = 1;
                        furnace_creating = furnace_recipe[i][1];
                        break;
                    }
                    else if (fuel_item[1] > 1024 && (furnace_fueltime_items[fuel_item[1] - 1025] != 0.0 || furnace_fueltime > 0))
                    {
                        furnace_ope = 1;
                        furnace_creating = furnace_recipe[i][1];
                        break;
                    }
                }
            }
        }
        if (furnace_fueltime > 0)
            furnace_fueltime -= 0.05;
        if (furnace_ope)
        {
            if (furnace_fueltime <= 0)
            {
                if (fuel_item[1] < 17 && (furnace_fueltime_block[fuel_item[1]] != 0.0))
                {
                    furnace_fuelnow = furnace_fueltime_block[fuel_item[1]];
                    furnace_fueltime += furnace_fueltime_block[fuel_item[1]];
                    fuel_item[2]--;
                    if (fuel_item[2] == 0)
                    {
                        fuel_item[0] = -1;
                        fuel_item[1] = 0;
                        fuel_item[3] = -1;
                    }
                }
                else if (fuel_item[1] > 1024 && (furnace_fueltime_items[fuel_item[1] - 1025] != 0.0))
                {
                    furnace_fueltime += furnace_fueltime_items[fuel_item[1] - 1025];
                    fuel_item[2]--;
                    if (fuel_item[2] == 0)
                    {
                        fuel_item[0] = -1;
                        fuel_item[1] = 0;
                        fuel_item[3] = -1;
                    }
                }
            }
            if (furnace_opetime >= furnace_createtime)
            {
                furnace_opetime -= furnace_createtime;
                material_item[2]--;
                if (material_item[2] == 0)
                {
                    material_item[0] = -1;
                    material_item[1] = 0;
                    material_item[3] = -1;
                }
                if (product_item[0] != 0)
                {
                    product_item[1]++;
                }
                else
                {
                    product_item[0] = furnace_creating;
                    product_item[1] = 1;
                    product_item[2] = -1;
                }
            }
            furnace_opetime += 0.05;
        }
        else
            furnace_opetime = 0.0;

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawBuffer(GL_FRONT);

        // Camera setting
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (camera_perspective_mode == 0)
            gluLookAt(player_x, player_y, player_z, player_x + 5.0 * cos(xz_rad) * cos(y_rad), player_y - 5.0 * sin(y_rad), player_z + 5.0 * sin(xz_rad) * cos(y_rad), 0.0, 1.0, 0.0);
        else if (camera_perspective_mode == 1)
            gluLookAt(player_x + 5.0 * cos(xz_rad) * cos(y_rad), player_y - 5.0 * sin(y_rad), player_z + 5.0 * sin(xz_rad) * cos(y_rad), player_x, player_y, player_z, 0.0, 1.0, 0.0);
        else if (camera_perspective_mode == 2)
            gluLookAt(player_x - 5.0 * cos(xz_rad) * cos(y_rad), player_y - 5.0 * sin(y_rad), player_z - 5.0 * sin(xz_rad) * cos(y_rad), player_x, player_y, player_z, 0.0, 1.0, 0.0);

        // Draw
        int dx, dy, dz;
        int draw_xmin, draw_zmin;
        int draw_xmax, draw_zmax;
        int tmp;
        double tex_pd, tex_pu;
        if (player_chunk_x - player_pre_loadchunk_x >= 3)
            draw_xmin = player_chunk_x - player_pre_loadchunk_x - 3;
        else
            draw_xmin = 0;
        if (player_chunk_z - player_pre_loadchunk_z >= 3)
            draw_zmin = player_chunk_z - player_pre_loadchunk_z - 3;
        else
            draw_zmin = 0;
        if (player_chunk_x - player_pre_loadchunk_x < 14)
            draw_xmax = player_chunk_x - player_pre_loadchunk_x + 3;
        else
            draw_xmax = 17;
        if (player_chunk_z - player_pre_loadchunk_z < 14)
            draw_zmax = player_chunk_z - player_pre_loadchunk_z + 3;
        else
            draw_zmax = 17;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
        DrawPlayer();
        glEnd();
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
        glBindTexture(GL_TEXTURE_2D, img);
        glBegin(GL_QUADS);
        for (int i = draw_xmin; i < draw_xmax; i++)
        {
            for (int j = draw_zmin; j < draw_zmax; j++)
            {
                for (int x = 0; x < 16; x++)
                {
                    dx = x + (player_pre_loadchunk_x + i) * 16;
                    for (int z = 0; z < 16; z++)
                    {
                        dz = z + (player_pre_loadchunk_z + j) * 16;
                        for (int y = 0; y < 256; y++)
                        {
                            tmp = *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z);
                            tex_pd = (double)(tmp + 1) / 16.0;
                            tex_pu = (double)(tmp) / 16.0;
                            if (tmp != 0 && tmp != 11 && tmp != 15)
                            {
                                dy = y;
                                // x-Higher
                                if ((i < draw_xmax - 1 && x == 15 && (*(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 0 || *(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 9 || *(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 11 || *(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 15)) || (x < 15 && (*(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 15)))
                                {
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx + 1, dy, dz);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx + 1, dy, dz + 1);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz);
                                }
                                // y-Higher
                                if (y == 255 || (y < 255 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 15)))
                                {
                                    glTexCoord2f(0, tex_pd);
                                    glVertex3i(dx, dy + 1, dz);
                                    glTexCoord2f(0, tex_pu);
                                    glVertex3i(dx, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx + 1, dy + 1, dz);
                                }
                                // z-Higher
                                if ((j < draw_zmax - 1 && z == 15 && (*(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 0 || *(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 9 || *(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 11 || *(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 15)) || (z < 15 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 15)))
                                {
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx, dy, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx, dy + 1, dz + 1);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz + 1);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx + 1, dy, dz + 1);
                                }

                                // x-Lower
                                if ((i > 0 && x == 0 && (*(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 0 || *(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 9 || *(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 11 || *(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 15)) || (x > 0 && (*(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 15)))
                                {
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx, dy, dz);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx, dy, dz + 1);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx, dy + 1, dz);
                                }

                                // y-Lower
                                if (y == 0 || (y > 0 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 15)))
                                {
                                    glNormal3f(0, -1, 0);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx, dy, dz);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx, dy, dz + 1);
                                    glTexCoord2f(0.375, tex_pu);
                                    glVertex3i(dx + 1, dy, dz + 1);
                                    glTexCoord2f(0.375, tex_pd);
                                    glVertex3i(dx + 1, dy, dz);
                                }

                                // z-Lower
                                if ((j > 0 && z == 0 && (*(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 0 || *(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 9 || *(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 11 || *(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 15)) || (z > 0 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 15)))
                                {
                                    glNormal3f(0, 0, -1);
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx, dy, dz);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx, dy + 1, dz);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx + 1, dy, dz);
                                }
                            }
                        }
                    }
                }
            }
        }
        for (int i = draw_xmin; i < draw_xmax; i++)
        {
            for (int j = draw_zmin; j < draw_zmax; j++)
            {
                for (int x = 0; x < 16; x++)
                {
                    dx = x + (player_pre_loadchunk_x + i) * 16;
                    for (int z = 0; z < 16; z++)
                    {
                        dz = z + (player_pre_loadchunk_z + j) * 16;
                        for (int y = 0; y < 256; y++)
                        {
                            tmp = *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z);
                            tex_pd = (double)(tmp + 1) / 16.0;
                            tex_pu = (double)(tmp) / 16.0;
                            if (tmp == 11 || tmp == 15)
                            {
                                dy = y;
                                // x-Higher
                                if ((i < draw_xmax - 1 && x == 15 && (*(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 0 || *(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 9 || *(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 11 || *(chunk + (i + 1) * 1114112 + j * 65536 + y * 16 + z) == 15)) || (x < 15 && (*(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + (x + 1) * 4096 + y * 16 + z) == 15)))
                                {
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx + 1, dy, dz);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx + 1, dy, dz + 1);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz);
                                }
                                // y-Higher
                                if (y == 255 || (y < 255 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y + 1) * 16 + z) == 15)))
                                {
                                    glTexCoord2f(0, tex_pd);
                                    glVertex3i(dx, dy + 1, dz);
                                    glTexCoord2f(0, tex_pu);
                                    glVertex3i(dx, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx + 1, dy + 1, dz);
                                }
                                // z-Higher
                                if ((j < draw_zmax - 1 && z == 15 && (*(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 0 || *(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 9 || *(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 11 || *(chunk + i * 1114112 + (j + 1) * 65536 + x * 4096 + y * 16) == 15)) || (z < 15 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z + 1) == 15)))
                                {
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx, dy, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx, dy + 1, dz + 1);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz + 1);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx + 1, dy, dz + 1);
                                }

                                // x-Lower
                                if ((i > 0 && x == 0 && (*(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 0 || *(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 9 || *(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 11 || *(chunk + (i - 1) * 1114112 + j * 65536 + 15 * 4096 + y * 16 + z) == 15)) || (x > 0 && (*(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + (x - 1) * 4096 + y * 16 + z) == 15)))
                                {
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx, dy, dz);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx, dy, dz + 1);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx, dy + 1, dz + 1);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx, dy + 1, dz);
                                }

                                // y-Lower
                                if (y == 0 || (y > 0 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + (y - 1) * 16 + z) == 15)))
                                {
                                    glNormal3f(0, -1, 0);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx, dy, dz);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx, dy, dz + 1);
                                    glTexCoord2f(0.375, tex_pu);
                                    glVertex3i(dx + 1, dy, dz + 1);
                                    glTexCoord2f(0.375, tex_pd);
                                    glVertex3i(dx + 1, dy, dz);
                                }

                                // z-Lower
                                if ((j > 0 && z == 0 && (*(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 0 || *(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 9 || *(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 11 || *(chunk + i * 1114112 + (j - 1) * 65536 + x * 4096 + y * 16 + 15) == 15)) || (z > 0 && (*(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 0 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 9 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 11 || *(chunk + i * 1114112 + j * 65536 + x * 4096 + y * 16 + z - 1) == 15)))
                                {
                                    glNormal3f(0, 0, -1);
                                    glTexCoord2f(0.125, tex_pd);
                                    glVertex3i(dx, dy, dz);
                                    glTexCoord2f(0.125, tex_pu);
                                    glVertex3i(dx, dy + 1, dz);
                                    glTexCoord2f(0.25, tex_pu);
                                    glVertex3i(dx + 1, dy + 1, dz);
                                    glTexCoord2f(0.25, tex_pd);
                                    glVertex3i(dx + 1, dy, dz);
                                }
                            }
                        }
                    }
                }
            }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        if (dig_flag == 1 && pause == 0)
        {
            if (block_breaker[tmp] == 1)
                bar_length = (dig_hardness / block_hardness[tmp][pickle_type]) * 0.8 - 0.4;
            else
                bar_length = (dig_hardness / block_hardness[tmp][0]) * 0.8 - 0.4;
            glColor3f(0.0, 0.0, 1.0);
            glBegin(GL_QUADS);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad) * 0.4, (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 - cos(xz_rad) * 0.4);
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad) * bar_length, (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 + cos(xz_rad) * bar_length);
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad) * bar_length, (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 + cos(xz_rad) * bar_length);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad) * 0.4, (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 - cos(xz_rad) * 0.4);
            glEnd();
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_QUADS);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad) * (-bar_length), (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 - cos(xz_rad) * (-bar_length));
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad) * 0.4, (double)dig_now_y + 1.55, (double)dig_now_z + 0.5 + cos(xz_rad) * 0.4);
            glVertex3f((double)dig_now_x + 0.5 - sin(xz_rad) * 0.4, (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 + cos(xz_rad) * 0.4);
            glVertex3f((double)dig_now_x + 0.5 + sin(xz_rad) * (-bar_length), (double)dig_now_y + 1.45, (double)dig_now_z + 0.5 - cos(xz_rad) * (-bar_length));
            glEnd();
        }

        double after_x, after_y, after_z;
        // calc view
        for (double a = 0; a < 5.0; a += 0.05)
        {
            after_x = player_x + a * cos(xz_rad) * cos(y_rad);
            next_view_block_x = (int)after_x;
            next_view_chunk_x = next_view_block_x / 16;
            next_view_local_x = next_view_block_x % 16;
            after_y = player_y - a * sin(y_rad);
            next_view_block_y = (int)after_y;
            next_view_chunk_y = next_view_block_y / 16;
            next_view_local_y = next_view_block_y % 16;
            after_z = player_z + a * sin(xz_rad) * cos(y_rad);
            next_view_block_z = (int)after_z;
            next_view_chunk_z = next_view_block_z / 16;
            next_view_local_z = next_view_block_z % 16;
            if (*(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(after_y)*16 + next_view_local_z) != 0)
                break;
            else
                next_view_block_y = -1;
        }

        glLineWidth(3.0);
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);
        glVertex3f(next_view_block_x + 0, next_view_block_y + 0, next_view_block_z + 0);
        glVertex3f(next_view_block_x + 1, next_view_block_y + 0, next_view_block_z + 0);

        glVertex3f(next_view_block_x + 1, next_view_block_y + 0, next_view_block_z + 0);
        glVertex3f(next_view_block_x + 1, next_view_block_y + 0, next_view_block_z + 1);

        glVertex3f(next_view_block_x + 1, next_view_block_y + 0, next_view_block_z + 1);
        glVertex3f(next_view_block_x + 0, next_view_block_y + 0, next_view_block_z + 1);

        glVertex3f(next_view_block_x + 0, next_view_block_y + 0, next_view_block_z + 1);
        glVertex3f(next_view_block_x + 0, next_view_block_y + 0, next_view_block_z + 0);

        glVertex3f(next_view_block_x + 0, next_view_block_y + 0, next_view_block_z + 0);
        glVertex3f(next_view_block_x + 0, next_view_block_y + 1, next_view_block_z + 0);

        glVertex3f(next_view_block_x + 0, next_view_block_y + 0, next_view_block_z + 1);
        glVertex3f(next_view_block_x + 0, next_view_block_y + 1, next_view_block_z + 1);

        glVertex3f(next_view_block_x + 1, next_view_block_y + 0, next_view_block_z + 1);
        glVertex3f(next_view_block_x + 1, next_view_block_y + 1, next_view_block_z + 1);

        glVertex3f(next_view_block_x + 1, next_view_block_y + 0, next_view_block_z + 0);
        glVertex3f(next_view_block_x + 1, next_view_block_y + 1, next_view_block_z + 0);

        glVertex3f(next_view_block_x + 0, next_view_block_y + 1, next_view_block_z + 0);
        glVertex3f(next_view_block_x + 1, next_view_block_y + 1, next_view_block_z + 0);

        glVertex3f(next_view_block_x + 1, next_view_block_y + 1, next_view_block_z + 0);
        glVertex3f(next_view_block_x + 1, next_view_block_y + 1, next_view_block_z + 1);

        glVertex3f(next_view_block_x + 1, next_view_block_y + 1, next_view_block_z + 1);
        glVertex3f(next_view_block_x + 0, next_view_block_y + 1, next_view_block_z + 1);

        glVertex3f(next_view_block_x + 0, next_view_block_y + 1, next_view_block_z + 1);
        glVertex3f(next_view_block_x + 0, next_view_block_y + 1, next_view_block_z + 0);
        glEnd();
        glDisable(GL_DEPTH_TEST);
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
            for (int i = 0; i < 9; i++)
            {
                glVertex2i(itembox_x + 64 * i, itembox_y);
                glVertex2i(itembox_x + 64 * (i + 1), itembox_y);
                glVertex2i(itembox_x + 64 * (i + 1), itembox_y);
                glVertex2i(itembox_x + 64 * (i + 1), itembox_y + 64);
                glVertex2i(itembox_x + 64 * (i + 1), itembox_y + 64);
                glVertex2i(itembox_x + 64 * i, itembox_y + 64);
                glVertex2i(itembox_x + 64 * i, itembox_y + 64);
                glVertex2i(itembox_x + 64 * i, itembox_y);
            }
            glEnd();

            glLineWidth(5.0);
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex2i(itembox_x + 64 * debugmode, itembox_y);
            glVertex2i(itembox_x + 64 * (debugmode + 1), itembox_y);
            glVertex2i(itembox_x + 64 * (debugmode + 1), itembox_y);
            glVertex2i(itembox_x + 64 * (debugmode + 1), itembox_y + 64);
            glVertex2i(itembox_x + 64 * (debugmode + 1), itembox_y + 64);
            glVertex2i(itembox_x + 64 * debugmode, itembox_y + 64);
            glVertex2i(itembox_x + 64 * debugmode, itembox_y + 64);
            glVertex2i(itembox_x + 64 * debugmode, itembox_y);
            glEnd();

            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glColor4ub(255, 255, 255, 255);
            glBindTexture(GL_TEXTURE_2D, img);
            glBegin(GL_QUADS);
            for (int i = 0; i < 9; i++)
            {
                if (*(player_bag + 3 * i) > 0 && *(player_bag + 3 * i) < 16 && *(player_bag + 3 * i) != 9)
                {
                    tex_pd = (double)(*(player_bag + 3 * i) + 1) / 16.0;
                    tex_pu = (double)(*(player_bag + 3 * i)) / 16.0;
                    DrawTexBlock2d(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd);
                }
                else if (*(player_bag + 3 * i) > 1024 && *(player_bag + 3 * i) < 1041)
                {
                    tex_pd = (double)(*(player_bag + 3 * i) - 1024) / 16.0;
                    tex_pu = (double)(*(player_bag + 3 * i) - 1025) / 16.0;
                    DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (*(player_bag + 3 * i) == 1041)
                {
                    tex_pd = (double)(*(player_bag + 3 * i) - 1040) / 16.0;
                    tex_pu = (double)(*(player_bag + 3 * i) - 1041) / 16.0;
                    DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.5, 0.625);
                }
            }
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
            double durability;
            double red;
            double green;
            glBegin(GL_QUADS);
            for (int i = 0; i < 9; i++)
            {
                if (*(player_bag + 3 * i) > 1025 && *(player_bag + 3 * i) < 1041 && *(player_bag + 3 * i + 2) < durability_list[*(player_bag + 3 * i) - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * i + 8, itembox_y + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * i + 56, itembox_y + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * i + 56, itembox_y + 48);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);

                    if (*(player_bag + 3 * i + 2) >= durability_empty_list[*(player_bag + 3 * i) - 1026])
                    {
                        durability = (double)(*(player_bag + 3 * i + 2) - durability_empty_list[*(player_bag + 3 * i) - 1026]) / (double)(durability_list[*(player_bag + 3 * i) - 1026] - durability_empty_list[*(player_bag + 3 * i) - 1026]);
                        if (durability > 0.5)
                        {
                            red = (1.0 - durability) * 2.0;
                            green = 1.0;
                        }
                        if (durability <= 0.5)
                        {
                            red = 1.0;
                            green = durability * 2.0;
                        }
                        glColor3f(red, green, 0.0);
                        glVertex2f(itembox_x + 64 * i + 8, itembox_y + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 48);
                        glColor3f(red, green, 0.0);
                        glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);
                    }
                }
            }
            glEnd();

            for (int i = 0; i < 9; i++)
            {
                glColor3f(1, 1, 1);
                sprintf(items_str, "%2d", *(player_bag + 3 * i + 1));
                glRasterPos2i(itembox_x + 64 * (i + 1) - 23, itembox_y + 59);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
            }

            for (int i = 0; i < 10; i++)
            {
                if (2 * i < *(player_status))
                {
                    glBegin(GL_QUADS);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 18);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 6, itembox_y - 5 - 24);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5);
                    glEnd();
                }
                else
                {
                    glBegin(GL_QUADS);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 18);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 6, itembox_y - 5 - 24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5);
                    glEnd();
                }
                if (2 * i + 1 < *(player_status))
                {
                    glBegin(GL_QUADS);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 18, itembox_y - 5 - 24);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 12);
                    glColor3f(1.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5);
                    glEnd();
                }
                else
                {
                    glBegin(GL_QUADS);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 18);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 18, itembox_y - 5 - 24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 24, itembox_y - 5 - 12);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 25 * i + 12, itembox_y - 5);
                    glEnd();
                }
            }

            for (int i = 0; i < 9; i++)
            {
                if (2 * i < *(player_status + 1))
                {
                    glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 0);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 24);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i, itembox_y - 5 - 12);
                    glEnd();
                }
                else
                {
                    glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 0);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i, itembox_y - 5 - 12);
                    glEnd();
                }
                if (2 * i + 1 < *(player_status + 1))
                {
                    glBegin(GL_TRIANGLES);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 0);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 24);
                    glColor3f(1.0, 1.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 24, itembox_y - 5 - 12);
                    glEnd();
                }
                else
                {
                    glBegin(GL_TRIANGLES);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 0);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 12, itembox_y - 5 - 24);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(itembox_x + 64 * 9 - 25 * 9 + 25 * i + 24, itembox_y - 5 - 12);
                    glEnd();
                }
            }
            glColor3f(1, 1, 1);
            glLineWidth(2.0);
            glBegin(GL_LINES);
            glVertex2i(w / 2, h / 2 + 10);
            glVertex2i(w / 2, h / 2 - 10);
            glVertex2i(w / 2 - 10, h / 2);
            glVertex2i(w / 2 + 10, h / 2);
            glEnd();
            if (debug == 1)
            {
                glColor3f(0, 0, 0);
                glRasterPos2i(0, 16);
                for (int j = 0; j < 60; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, infomation[j]);
                glRasterPos2i(0, 16 * 2);
                for (int j = 0; j < 20; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fps_str[j]);
                glRasterPos2i(0, 16 * 9);
                for (int j = 0; j < 40; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, pos_str[j]);
                glRasterPos2i(0, 16 * 10);
                for (int j = 0; j < 40; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, block_str[j]);
                glRasterPos2i(0, 16 * 11);
                for (int j = 0; j < 40; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, chunk_str[j]);
                glRasterPos2i(0, 16 * 13);
                for (int j = 0; j < 60; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, keystate_str[j]);
                glRasterPos2i(0, 16 * 14);
                for (int j = 0; j < 60; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, loadpos_str[j]);
                glRasterPos2i(0, 16 * 15);
                for (int j = 0; j < 60; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, loadposc_str[j]);
                glRasterPos2i(0, 16 * 16);
                for (int j = 0; j < 30; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, dig_str[j]);
                glRasterPos2i(0, 16 * 18);
                for (int j = 0; j < 30; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ht_str[j]);
                glRasterPos2i(0, 16 * 19);
                for (int j = 0; j < 30; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, hht_str[j]);
                glRasterPos2i(0, 16 * 20);
                for (int j = 0; j < 30; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, net_str[j]);
                glRasterPos2i(0, 16 * 21);
                for (int j = 0; j < 30; j++)
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, het_str[j]);
            }
        }
        else if (menu == 1)
        {
            // window size
            int w = glutGet(GLUT_WINDOW_WIDTH);
            int h = glutGet(GLUT_WINDOW_HEIGHT);
            int i=0, j=0;
            char continue_str[20];
            char save_str[20];
            char title_str[20];
            sprintf(continue_str, "Continue the Game");
            sprintf(save_str, "Save the Game");
            sprintf(title_str, "Return to Title");
            glColor4f(0, 0, 0, 0.5);
            glBegin(GL_QUADS);
                glVertex2i(0, 0);
                glVertex2i(w, 0);
                glVertex2i(w, h);
                glVertex2i(0, h);
            glEnd();
            glColor3f(0.5, 0.5, 0.5);
            glBegin(GL_QUADS);
                glVertex2i(w/4, h/2+64);
                glVertex2i(3*w/4, h/2+64);
                glVertex2i(3*w/4, h/2+128);
                glVertex2i(w/4, h/2+128);

                glVertex2i(w/4, h/2-32);
                glVertex2i(3*w/4, h/2-32);
                glVertex2i(3*w/4, h/2+32);
                glVertex2i(w/4, h/2+32);

                glVertex2i(w/4, h/2-128);
                glVertex2i(3*w/4, h/2-128);
                glVertex2i(3*w/4, h/2-64);
                glVertex2i(w/4, h/2-64);
            glEnd();
            glColor3f(1, 1, 1);
            glRasterPos2i(w/2-108, h/2-84);
            for (j = 0; j < 18; j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, continue_str[j]);
            glRasterPos2i(w/2-78, h/2+12);
            for (j = 0; j < 13; j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, save_str[j]);
            glRasterPos2i(w/2-90, h/2+108);
            for (j = 0; j < 15; j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title_str[j]);
        }
        else
        {
            // variables
            int enventry_x;
            int enventry_y;
            int craftbox_x;
            int craftbox_y;
            int materialbox_x;
            int materialbox_y;
            int fuelbox_x;
            int fuelbox_y;
            int productbox_x;
            int productbox_y;
            int find;
            int find_idx;
            // enventry pos
            enventry_x = (w - 640) / 2;
            enventry_y = (h - 640) / 2;
            // item pos
            itembox_x = enventry_x + 32;
            itembox_y = enventry_y + 32 * 17;

            // inventory frame
            glColor3f(0.8, 0.8, 0.8);
            glBegin(GL_QUADS);
            glVertex2i(enventry_x, enventry_y);
            glVertex2i(enventry_x + 640, enventry_y);
            glVertex2i(enventry_x + 640, enventry_y + 640);
            glVertex2i(enventry_x, enventry_y + 640);
            glEnd();

            // inventory
            glColor3f(0.5, 0.5, 0.5);
            glBegin(GL_QUADS);
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    glVertex2i(itembox_x + 64 * i + 2, enventry_y + 32 * (10 + 2 * j) + 2);
                    glVertex2i(itembox_x + 64 * i + 62, enventry_y + 32 * (10 + 2 * j) + 2);
                    glVertex2i(itembox_x + 64 * i + 62, enventry_y + 32 * (10 + 2 * j) + 62);
                    glVertex2i(itembox_x + 64 * i + 2, enventry_y + 32 * (10 + 2 * j) + 62);
                }
                glVertex2i(itembox_x + 64 * i + 2, itembox_y + 2);
                glVertex2i(itembox_x + 64 * i + 62, itembox_y + 2);
                glVertex2i(itembox_x + 64 * i + 62, itembox_y + 62);
                glVertex2i(itembox_x + 64 * i + 2, itembox_y + 62);
            }
            glEnd();

            // num of items (inventry)
            for (int i = 0; i < 9; i++)
            {
                if (*(player_bag + 3 * i + 1) != 0)
                {
                    glColor3f(0, 0, 0);
                    sprintf(items_str, "%2d", *(player_bag + 3 * i + 1));
                    glRasterPos2i(itembox_x + 64 * (i + 1) - 23, itembox_y + 59);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                }
                for (int j = 0; j < 3; j++)
                {
                    if (*(player_bag + 3 * (9 * j + i + 9) + 1) != 0)
                    {
                        glColor3f(0, 0, 0);
                        sprintf(items_str, "%2d", *(player_bag + 3 * (9 * j + i + 9) + 1));
                        glRasterPos2i(itembox_x + 64 * (i + 1) - 23, itembox_y - 32 - 64 * (3 - j) + 59);
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                    }
                }
            }
            // num of items (dragginh)
            if (dragging_item[1] != 0)
            {
                glColor3f(0, 0, 0);
                sprintf(items_str, "%2d", dragging_item[2]);
                glRasterPos2i(mouse_x + 9, mouse_y + 27);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
            }
            if (enventory == 1)
            {
                // craft box pos
                craftbox_x = itembox_x + 64 * 5;
                craftbox_y = enventry_y + 64;
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_QUADS);
                // armer field
                for (int j = 0; j < 4; j++)
                {
                    glVertex2i(itembox_x + 2, enventry_y + 32 * (1 + 2 * j) + 2);
                    glVertex2i(itembox_x + 62, enventry_y + 32 * (1 + 2 * j) + 2);
                    glVertex2i(itembox_x + 62, enventry_y + 32 * (1 + 2 * j) + 62);
                    glVertex2i(itembox_x + 2, enventry_y + 32 * (1 + 2 * j) + 62);
                }

                // free hand field
                glVertex2i(itembox_x + 64 * 4 + 2, enventry_y + 32 * 7 + 2);
                glVertex2i(itembox_x + 64 * 4 + 62, enventry_y + 32 * 7 + 2);
                glVertex2i(itembox_x + 64 * 4 + 62, enventry_y + 32 * 7 + 62);
                glVertex2i(itembox_x + 64 * 4 + 2, enventry_y + 32 * 7 + 62);
                // pet toy field
                glVertex2i(itembox_x + 64 * 5 + 2, enventry_y + 32 * 7 + 2);
                glVertex2i(itembox_x + 64 * 5 + 62, enventry_y + 32 * 7 + 2);
                glVertex2i(itembox_x + 64 * 5 + 62, enventry_y + 32 * 7 + 62);
                glVertex2i(itembox_x + 64 * 5 + 2, enventry_y + 32 * 7 + 62);

                // craft field - before
                for (int i = 5; i < 7; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        glVertex2i(itembox_x + 64 * i + 2, enventry_y + 32 * (2 + 2 * j) + 2);
                        glVertex2i(itembox_x + 64 * i + 62, enventry_y + 32 * (2 + 2 * j) + 2);
                        glVertex2i(itembox_x + 64 * i + 62, enventry_y + 32 * (2 + 2 * j) + 62);
                        glVertex2i(itembox_x + 64 * i + 2, enventry_y + 32 * (2 + 2 * j) + 62);
                    }
                }

                // craft field - after
                glVertex2i(itembox_x + 64 * 8 + 2, enventry_y + 32 * 3 + 2);
                glVertex2i(itembox_x + 64 * 8 + 62, enventry_y + 32 * 3 + 2);
                glVertex2i(itembox_x + 64 * 8 + 62, enventry_y + 32 * 3 + 62);
                glVertex2i(itembox_x + 64 * 8 + 2, enventry_y + 32 * 3 + 62);
                glEnd();

                // num of items (craft field)
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (craft_itembox[2 * j + i][2] != 0)
                        {
                            glColor3f(0, 0, 0);
                            sprintf(items_str, "%2d", craft_itembox[2 * j + i][2]);
                            glRasterPos2i(craftbox_x + 64 * (i + 1) - 23, craftbox_y + 64 * j + 59);
                            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                        }
                    }
                }

                // search item
                find_idx = -1;
                for (int i = 0; i < 7; i++)
                {
                    find = 1;
                    for (int j = 0; j < 4; j++)
                        if (craft_itembox[j][1] != recipe22[i][j])
                            find = 0;
                    if (find == 1)
                    {
                        crafted_item[0] = recipe22[i][4];
                        crafted_item[1] = recipe22[i][5];
                        crafted_item[2] = recipe22[i][6];
                        find_idx = i;
                        break;
                    }
                }
                if (find_idx == -1)
                {
                    crafted_item[0] = 0;
                    crafted_item[1] = 0;
                    crafted_item[2] = 0;
                }

                // draw items
                glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glColor4ub(255, 255, 255, 255);
                glBindTexture(GL_TEXTURE_2D, img);
                glBegin(GL_QUADS);
                for (int i = 0; i < 9; i++)
                {
                    if (*(player_bag + 3 * i) > 0 && *(player_bag + 3 * i) < 16 && *(player_bag + 3 * i) != 9)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) + 1) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i)) / 16.0;
                        DrawTexBlock2d(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd);
                    }
                    else if (*(player_bag + 3 * i) > 1024 && *(player_bag + 3 * i) < 1041)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) - 1024) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i) - 1025) / 16.0;
                        DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.375, 0.5);
                    }
                    else if (*(player_bag + 3 * i) == 1041)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) - 1040) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i) - 1041) / 16.0;
                        DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.5, 0.625);
                    }
                }
                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (*(player_bag + 3 * (9 * j + i + 9)) > 0 && *(player_bag + 3 * (9 * j + i + 9)) < 16 && *(player_bag + 3 * (9 * j + i + 9)) != 9)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) + 1) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9))) / 16.0;
                            DrawTexBlock2d(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd);
                        }
                        else if (*(player_bag + 3 * (9 * j + i + 9)) > 1024 && *(player_bag + 3 * (9 * j + i + 9)) < 1041)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1024) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1025) / 16.0;
                            DrawTexItem(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd, 0.375, 0.5);
                        }
                        else if (*(player_bag + 3 * (9 * j + i + 9)) == 1041)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1040) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1041) / 16.0;
                            DrawTexItem(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd, 0.5, 0.625);
                        }
                    }
                }
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (craft_itembox[2 * j + i][1] > 0 && craft_itembox[2 * j + i][1] < 16 && craft_itembox[2 * j + i][1] != 9)
                        {
                            tex_pd = (double)(craft_itembox[2 * j + i][1] + 1) / 16.0;
                            tex_pu = (double)(craft_itembox[2 * j + i][1]) / 16.0;
                            DrawTexBlock2d(craftbox_x + 64 * i, craftbox_y + 64 * j, tex_pu, tex_pd);
                        }
                        else if (craft_itembox[2 * j + i][1] > 1024 && craft_itembox[2 * j + i][1] < 1041)
                        {
                            tex_pd = (double)(craft_itembox[2 * j + i][1] - 1024) / 16.0;
                            tex_pu = (double)(craft_itembox[2 * j + i][1] - 1025) / 16.0;
                            DrawTexItem(craftbox_x + 64 * i, craftbox_y + 64 * j, tex_pu, tex_pd, 0.375, 0.5);
                        }
                        else if (craft_itembox[2 * j + i][1] == 1041)
                        {
                            tex_pd = (double)(craft_itembox[2 * j + i][1] - 1040) / 16.0;
                            tex_pu = (double)(craft_itembox[2 * j + i][1] - 1041) / 16.0;
                            DrawTexItem(craftbox_x + 64 * i, craftbox_y + 64 * j, tex_pu, tex_pd, 0.5, 0.625);
                        }
                    }
                }
                if (crafted_item[0] > 0 && crafted_item[0] < 16 && crafted_item[0] != 9)
                {
                    tex_pd = (double)(crafted_item[0] + 1) / 16.0;
                    tex_pu = (double)(crafted_item[0]) / 16.0;
                    DrawTexBlock2d(craftbox_x + 64 * 3, craftbox_y + 32, tex_pu, tex_pd);
                }
                else if (crafted_item[0] > 1024 && crafted_item[0] < 1041)
                {
                    tex_pd = (double)(crafted_item[0] - 1024) / 16.0;
                    tex_pu = (double)(crafted_item[0] - 1025) / 16.0;
                    DrawTexItem(craftbox_x + 64 * 3, craftbox_y + 32, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (crafted_item[0] == 1041)
                {
                    tex_pd = (double)(crafted_item[0] - 1040) / 16.0;
                    tex_pu = (double)(crafted_item[0] - 1041) / 16.0;
                    DrawTexItem(craftbox_x + 64 * 3, craftbox_y + 32, tex_pu, tex_pd, 0.5, 0.625);
                }
                if (dragging_item[1] > 0 && dragging_item[1] < 16 && dragging_item[1] != 9)
                {
                    tex_pd = (double)(dragging_item[1] + 1) / 16.0;
                    tex_pu = (double)(dragging_item[1]) / 16.0;
                    DrawTexBlock2d(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd);
                }
                else if (dragging_item[1] > 1024 && dragging_item[1] < 1041)
                {
                    tex_pd = (double)(dragging_item[1] - 1024) / 16.0;
                    tex_pu = (double)(dragging_item[1] - 1025) / 16.0;
                    DrawTexItem(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (dragging_item[1] == 1041)
                {
                    tex_pd = (double)(dragging_item[1] - 1040) / 16.0;
                    tex_pu = (double)(dragging_item[1] - 1041) / 16.0;
                    DrawTexItem(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd, 0.5, 0.625);
                }
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();
                double durability;
                double red;
                double green;
                glBegin(GL_QUADS);
                for (int i = 0; i < 9; i++)
                {
                    if (*(player_bag + 3 * i) > 1025 && *(player_bag + 3 * i) < 1041 && *(player_bag + 3 * i + 2) < durability_list[*(player_bag + 3 * i) - 1026])
                    {
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 8, itembox_y + 44);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 56, itembox_y + 44);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 56, itembox_y + 48);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);

                        if (*(player_bag + 3 * i + 2) >= durability_empty_list[*(player_bag + 3 * i) - 1026])
                        {
                            durability = (double)(*(player_bag + 3 * i + 2) - durability_empty_list[*(player_bag + 3 * i) - 1026]) / (double)(durability_list[*(player_bag + 3 * i) - 1026] - durability_empty_list[*(player_bag + 3 * i) - 1026]);
                            if (durability > 0.5)
                            {
                                red = (1.0 - durability) * 2.0;
                                green = 1.0;
                            }
                            if (durability <= 0.5)
                            {
                                red = 1.0;
                                green = durability * 2.0;
                            }
                            glColor3f(red, green, 0.0);
                            glVertex2f(itembox_x + 64 * i + 8, itembox_y + 44);
                            glColor3f(red, green, 0.0);
                            glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 44);
                            glColor3f(red, green, 0.0);
                            glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 48);
                            glColor3f(red, green, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);
                        }
                    }
                }

                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (*(player_bag + 3 * (9 * j + i + 9)) > 1025 && *(player_bag + 3 * (9 * j + i + 9)) < 1041 && *(player_bag + 3 * (9 * j + i + 9) + 2) < durability_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026])
                        {
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 56, itembox_y - 64 * (3 - j) - 32 + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 56, itembox_y - 64 * (3 - j) - 32 + 48);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 48);

                            if (*(player_bag + 3 * (9 * j + i + 9) + 2) >= durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026])
                            {
                                durability = (double)(*(player_bag + 3 * (9 * j + i + 9) + 2) - durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026]) / (double)(durability_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026] - durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026]);
                                if (durability > 0.5)
                                {
                                    red = (1.0 - durability) * 2.0;
                                    green = 1.0;
                                }
                                if (durability <= 0.5)
                                {
                                    red = 1.0;
                                    green = durability * 2.0;
                                }
                                glColor3f(red, green, 0.0);
                                glVertex2f(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y - 64 * (3 - j) - 32 + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y - 64 * (3 - j) - 32 + 48);
                                glColor3f(red, green, 0.0);
                                glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 48);
                            }
                        }
                    }
                }
                if (dragging_item[1] > 1025 && dragging_item[1] < 1041 && dragging_item[3] < durability_list[dragging_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 56, mouse_y - 32 + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 56, mouse_y - 32 + 48);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 48);

                    if (dragging_item[3] >= durability_empty_list[dragging_item[1] - 1026])
                    {
                        durability = (double)(dragging_item[3] - durability_empty_list[dragging_item[1] - 1026]) / (double)(durability_list[dragging_item[1] - 1026] - durability_empty_list[dragging_item[1] - 1026]);
                        if (durability > 0.5)
                        {
                            red = (1.0 - durability) * 2.0;
                            green = 1.0;
                        }
                        if (durability <= 0.5)
                        {
                            red = 1.0;
                            green = durability * 2.0;
                        }
                        glColor3f(red, green, 0.0);
                        glVertex2f(mouse_x - 32 + 8, mouse_y - 32 + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(mouse_x - 32 + 8) + 48.0 * durability, mouse_y - 32 + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(mouse_x - 32 + 8) + 48.0 * durability, mouse_y - 32 + 48);
                        glColor3f(red, green, 0.0);
                        glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 48);
                    }
                }
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (craft_itembox[2 * j + i][1] > 1025 && craft_itembox[2 * j + i][1] < 1041 && craft_itembox[2 * j + i][3] < durability_list[craft_itembox[2 * j + i][1] - 1026])
                        {
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 56, craftbox_y + 64 * j + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 56, craftbox_y + 64 * j + 48);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 48);
                            if (craft_itembox[2 * j + i][3] >= durability_empty_list[craft_itembox[2 * j + i][1] - 1026])
                            {
                                durability = (double)(craft_itembox[2 * j + i][3] - durability_empty_list[craft_itembox[2 * j + i][1] - 1026]) / (double)(durability_list[craft_itembox[2 * j + i][1] - 1026] - durability_empty_list[craft_itembox[2 * j + i][1] - 1026]);
                                if (durability > 0.5)
                                {
                                    red = (1.0 - durability) * 2.0;
                                    green = 1.0;
                                }
                                if (durability <= 0.5)
                                {
                                    red = 1.0;
                                    green = durability * 2.0;
                                }
                                glColor3f(red, green, 0.0);
                                glVertex2f(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(craftbox_x + 64 * i + 8) + 48.0 * durability, craftbox_y + 64 * j + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(craftbox_x + 64 * i + 8) + 48.0 * durability, craftbox_y + 64 * j + 48);
                                glColor3f(red, green, 0.0);
                                glVertex2i(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 48);
                            }
                        }
                    }
                }
                glEnd();
            }
            else if (craft == 1)
            {
                // craft box pos
                craftbox_x = itembox_x + 64 * 2;
                craftbox_y = enventry_y + 64;
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_QUADS);
                // craft field - before
                for (int i = 2; i < 5; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        glVertex2i(itembox_x + 64 * i + 2, enventry_y + 32 * (2 + 2 * j) + 2);
                        glVertex2i(itembox_x + 64 * i + 62, enventry_y + 32 * (2 + 2 * j) + 2);
                        glVertex2i(itembox_x + 64 * i + 62, enventry_y + 32 * (2 + 2 * j) + 62);
                        glVertex2i(itembox_x + 64 * i + 2, enventry_y + 32 * (2 + 2 * j) + 62);
                    }
                }

                // craft field - after
                glVertex2i(itembox_x + 64 * 7 + 2, enventry_y + 32 * 4 + 2);
                glVertex2i(itembox_x + 64 * 7 + 62, enventry_y + 32 * 4 + 2);
                glVertex2i(itembox_x + 64 * 7 + 62, enventry_y + 32 * 4 + 62);
                glVertex2i(itembox_x + 64 * 7 + 2, enventry_y + 32 * 4 + 62);
                glEnd();

                // num of items (craft field)
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (bigcraft_itembox[3 * j + i][2] != 0)
                        {
                            glColor3f(0, 0, 0);
                            sprintf(items_str, "%2d", bigcraft_itembox[3 * j + i][2]);
                            glRasterPos2i(craftbox_x + 64 * (i + 1) - 23, craftbox_y + 64 * j + 59);
                            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                        }
                    }
                }

                // search item
                find_idx = -1;
                // search item
                for (int i = 0; i < 60; i++)
                {
                    find = 1;
                    for (int j = 0; j < 9; j++)
                        if (bigcraft_itembox[j][1] != recipe33[i][j])
                            find = 0;
                    if (find == 1)
                    {
                        bigcrafted_item[0] = recipe33[i][9];
                        bigcrafted_item[1] = recipe33[i][10];
                        bigcrafted_item[2] = recipe33[i][11];
                        find_idx = i;
                        break;
                    }
                }
                if (find_idx == -1)
                {
                    bigcrafted_item[0] = 0;
                    bigcrafted_item[1] = 0;
                    bigcrafted_item[2] = -1;
                }

                // draw items
                glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glColor4ub(255, 255, 255, 255);
                glBindTexture(GL_TEXTURE_2D, img);
                glBegin(GL_QUADS);
                for (int i = 0; i < 9; i++)
                {
                    if (*(player_bag + 3 * i) > 0 && *(player_bag + 3 * i) < 16 && *(player_bag + 3 * i) != 9)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) + 1) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i)) / 16.0;
                        DrawTexBlock2d(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd);
                    }
                    else if (*(player_bag + 3 * i) > 1024 && *(player_bag + 3 * i) < 1041)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) - 1024) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i) - 1025) / 16.0;
                        DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.375, 0.5);
                    }
                    else if (*(player_bag + 3 * i) == 1041)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) - 1040) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i) - 1041) / 16.0;
                        DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.5, 0.625);
                    }
                }
                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (*(player_bag + 3 * (9 * j + i + 9)) > 0 && *(player_bag + 3 * (9 * j + i + 9)) < 16 && *(player_bag + 3 * (9 * j + i + 9)) != 9)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) + 1) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9))) / 16.0;
                            DrawTexBlock2d(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd);
                        }
                        else if (*(player_bag + 3 * (9 * j + i + 9)) > 1024 && *(player_bag + 3 * (9 * j + i + 9)) < 1041)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1024) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1025) / 16.0;
                            DrawTexItem(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd, 0.375, 0.5);
                        }
                        else if (*(player_bag + 3 * (9 * j + i + 9)) == 1041)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1040) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1041) / 16.0;
                            DrawTexItem(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd, 0.5, 0.625);
                        }
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (bigcraft_itembox[3 * j + i][1] > 0 && bigcraft_itembox[3 * j + i][1] < 16 && bigcraft_itembox[3 * j + i][1] != 9)
                        {
                            tex_pd = (double)(bigcraft_itembox[3 * j + i][1] + 1) / 16.0;
                            tex_pu = (double)(bigcraft_itembox[3 * j + i][1]) / 16.0;
                            DrawTexBlock2d(craftbox_x + 64 * i, craftbox_y + 64 * j, tex_pu, tex_pd);
                        }
                        else if (bigcraft_itembox[3 * j + i][1] > 1024 && bigcraft_itembox[3 * j + i][1] < 1041)
                        {
                            tex_pd = (double)(bigcraft_itembox[3 * j + i][1] - 1024) / 16.0;
                            tex_pu = (double)(bigcraft_itembox[3 * j + i][1] - 1025) / 16.0;
                            DrawTexItem(craftbox_x + 64 * i, craftbox_y + 64 * j, tex_pu, tex_pd, 0.375, 0.5);
                        }
                        else if (bigcraft_itembox[3 * j + i][1] == 1041)
                        {
                            tex_pd = (double)(bigcraft_itembox[3 * j + i][1] - 1040) / 16.0;
                            tex_pu = (double)(bigcraft_itembox[3 * j + i][1] - 1041) / 16.0;
                            DrawTexItem(craftbox_x + 64 * i, craftbox_y + 64 * j, tex_pu, tex_pd, 0.375, 0.5);
                        }
                    }
                }
                if (bigcrafted_item[0] > 0 && bigcrafted_item[0] < 16 && bigcrafted_item[0] != 9)
                {
                    tex_pd = (double)(bigcrafted_item[0] + 1) / 16.0;
                    tex_pu = (double)(bigcrafted_item[0]) / 16.0;
                    DrawTexBlock2d(craftbox_x + 64 * 5, craftbox_y + 64, tex_pu, tex_pd);
                }
                else if (bigcrafted_item[0] > 1024 && bigcrafted_item[0] < 1041)
                {
                    tex_pd = (double)(bigcrafted_item[0] - 1024) / 16.0;
                    tex_pu = (double)(bigcrafted_item[0] - 1025) / 16.0;
                    DrawTexItem(craftbox_x + 64 * 5, craftbox_y + 64, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (bigcrafted_item[0] == 1041)
                {
                    tex_pd = (double)(bigcrafted_item[0] - 1040) / 16.0;
                    tex_pu = (double)(bigcrafted_item[0] - 1041) / 16.0;
                    DrawTexItem(craftbox_x + 64 * 5, craftbox_y + 64, tex_pu, tex_pd, 0.5, 0.625);
                }
                if (dragging_item[1] > 0 && dragging_item[1] < 16 && dragging_item[1] != 9)
                {
                    tex_pd = (double)(dragging_item[1] + 1) / 16.0;
                    tex_pu = (double)(dragging_item[1]) / 16.0;
                    DrawTexBlock2d(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd);
                }
                else if (dragging_item[1] > 1024 && dragging_item[1] < 1041)
                {
                    tex_pd = (double)(dragging_item[1] - 1024) / 16.0;
                    tex_pu = (double)(dragging_item[1] - 1025) / 16.0;
                    DrawTexItem(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (dragging_item[1] == 1041)
                {
                    tex_pd = (double)(dragging_item[1] - 1040) / 16.0;
                    tex_pu = (double)(dragging_item[1] - 1041) / 16.0;
                    DrawTexItem(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd, 0.5, 0.625);
                }
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();
                double durability;
                double red;
                double green;
                glBegin(GL_QUADS);
                for (int i = 0; i < 9; i++)
                {
                    if (*(player_bag + 3 * i) > 1025 && *(player_bag + 3 * i) < 1041 && *(player_bag + 3 * i + 2) < durability_list[*(player_bag + 3 * i) - 1026])
                    {
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 8, itembox_y + 44);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 56, itembox_y + 44);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 56, itembox_y + 48);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);

                        if (*(player_bag + 3 * i + 2) >= durability_empty_list[*(player_bag + 3 * i) - 1026])
                        {
                            durability = (double)(*(player_bag + 3 * i + 2) - durability_empty_list[*(player_bag + 3 * i) - 1026]) / (double)(durability_list[*(player_bag + 3 * i) - 1026] - durability_empty_list[*(player_bag + 3 * i) - 1026]);
                            if (durability > 0.5)
                            {
                                red = (1.0 - durability) * 2.0;
                                green = 1.0;
                            }
                            if (durability <= 0.5)
                            {
                                red = 1.0;
                                green = durability * 2.0;
                            }
                            glColor3f(red, green, 0.0);
                            glVertex2f(itembox_x + 64 * i + 8, itembox_y + 44);
                            glColor3f(red, green, 0.0);
                            glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 44);
                            glColor3f(red, green, 0.0);
                            glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 48);
                            glColor3f(red, green, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);
                        }
                    }
                }
                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (*(player_bag + 3 * (9 * j + i + 9)) > 1025 && *(player_bag + 3 * (9 * j + i + 9)) < 1041 && *(player_bag + 3 * (9 * j + i + 9) + 2) < durability_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026])
                        {
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 56, itembox_y - 64 * (3 - j) - 32 + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 56, itembox_y - 64 * (3 - j) - 32 + 48);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 48);

                            if (*(player_bag + 3 * (9 * j + i + 9) + 2) >= durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026])
                            {
                                durability = (double)(*(player_bag + 3 * (9 * j + i + 9) + 2) - durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026]) / (double)(durability_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026] - durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026]);
                                if (durability > 0.5)
                                {
                                    red = (1.0 - durability) * 2.0;
                                    green = 1.0;
                                }
                                if (durability <= 0.5)
                                {
                                    red = 1.0;
                                    green = durability * 2.0;
                                }
                                glColor3f(red, green, 0.0);
                                glVertex2f(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y - 64 * (3 - j) - 32 + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y - 64 * (3 - j) - 32 + 48);
                                glColor3f(red, green, 0.0);
                                glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 48);
                            }
                        }
                    }
                }
                if (dragging_item[1] > 1025 && dragging_item[1] < 1041 && dragging_item[3] < durability_list[dragging_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 56, mouse_y - 32 + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 56, mouse_y - 32 + 48);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 48);

                    if (dragging_item[3] >= durability_empty_list[dragging_item[1] - 1026])
                    {
                        durability = (double)(dragging_item[3] - durability_empty_list[dragging_item[1] - 1026]) / (double)(durability_list[dragging_item[1] - 1026] - durability_empty_list[dragging_item[1] - 1026]);
                        if (durability > 0.5)
                        {
                            red = (1.0 - durability) * 2.0;
                            green = 1.0;
                        }
                        if (durability <= 0.5)
                        {
                            red = 1.0;
                            green = durability * 2.0;
                        }
                        glColor3f(red, green, 0.0);
                        glVertex2f(mouse_x - 32 + 8, mouse_y - 32 + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(mouse_x - 32 + 8) + 48.0 * durability, mouse_y - 32 + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(mouse_x - 32 + 8) + 48.0 * durability, mouse_y - 32 + 48);
                        glColor3f(red, green, 0.0);
                        glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 48);
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (bigcraft_itembox[3 * j + i][1] > 1025 && bigcraft_itembox[3 * j + i][1] < 1041 && bigcraft_itembox[3 * j + i][3] < durability_list[bigcraft_itembox[3 * j + i][1] - 1026])
                        {
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 56, craftbox_y + 64 * j + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 56, craftbox_y + 64 * j + 48);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 48);
                            if (bigcraft_itembox[3 * j + i][3] >= durability_empty_list[bigcraft_itembox[3 * j + i][1] - 1026])
                            {
                                durability = (double)(bigcraft_itembox[3 * j + i][3] - durability_empty_list[bigcraft_itembox[3 * j + i][1] - 1026]) / (double)(durability_list[bigcraft_itembox[3 * j + i][1] - 1026] - durability_empty_list[bigcraft_itembox[3 * j + i][1] - 1026]);
                                if (durability > 0.5)
                                {
                                    red = (1.0 - durability) * 2.0;
                                    green = 1.0;
                                }
                                if (durability <= 0.5)
                                {
                                    red = 1.0;
                                    green = durability * 2.0;
                                }
                                glColor3f(red, green, 0.0);
                                glVertex2f(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(craftbox_x + 64 * i + 8) + 48.0 * durability, craftbox_y + 64 * j + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(craftbox_x + 64 * i + 8) + 48.0 * durability, craftbox_y + 64 * j + 48);
                                glColor3f(red, green, 0.0);
                                glVertex2i(craftbox_x + 64 * i + 8, craftbox_y + 64 * j + 48);
                            }
                        }
                    }
                }
                glEnd();
            }
            else if (fire == 1)
            {
                materialbox_x = itembox_x + 64 * 3;
                materialbox_y = enventry_y + 32 * 2;

                fuelbox_x = itembox_x + 64 * 3;
                fuelbox_y = enventry_y + 32 * 6;
                productbox_x = itembox_x + 64 * 6;
                productbox_y = enventry_y + 32 * 4;
                double fuel_percentage = furnace_fueltime / furnace_fuelnow;
                double material_percentage = furnace_opetime / furnace_createtime;
                int fuelbar = 60 - (int)(fuel_percentage * 60.0);
                int material_bar = 60 - (int)(material_percentage * 60.0);
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_QUADS);
                // material field
                glVertex2i(materialbox_x + 2, materialbox_y + 2);
                glVertex2i(materialbox_x + 62, materialbox_y + 2);
                glVertex2i(materialbox_x + 62, materialbox_y + 62);
                glVertex2i(materialbox_x + 2, materialbox_y + 62);

                // fuel field
                glVertex2i(fuelbox_x + 2, fuelbox_y + 2);
                glVertex2i(fuelbox_x + 62, fuelbox_y + 2);
                glVertex2i(fuelbox_x + 62, fuelbox_y + 62);
                glVertex2i(fuelbox_x + 2, fuelbox_y + 62);

                //  product field
                glVertex2i(productbox_x + 2, productbox_y + 2);
                glVertex2i(productbox_x + 62, productbox_y + 2);
                glVertex2i(productbox_x + 62, productbox_y + 62);
                glVertex2i(productbox_x + 2, productbox_y + 62);
                glEnd();
                glColor3f(1, 1, 1);
                glBegin(GL_QUADS);
                // material field
                glVertex2i(materialbox_x + 2, materialbox_y + material_bar);
                glVertex2i(materialbox_x + 62, materialbox_y + material_bar);
                glVertex2i(materialbox_x + 62, materialbox_y + 62);
                glVertex2i(materialbox_x + 2, materialbox_y + 62);

                // fuel field
                glVertex2i(fuelbox_x + 2, fuelbox_y + fuelbar);
                glVertex2i(fuelbox_x + 62, fuelbox_y + fuelbar);
                glVertex2i(fuelbox_x + 62, fuelbox_y + 62);
                glVertex2i(fuelbox_x + 2, fuelbox_y + 62);
                glEnd();
                // num of items (craft field)
                if (material_item[2] != 0)
                {
                    glColor3f(0, 0, 0);
                    sprintf(items_str, "%2d", material_item[2]);
                    glRasterPos2i(materialbox_x + 41, materialbox_y + 59);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                }
                // num of items (craft field)
                if (fuel_item[2] != 0)
                {
                    glColor3f(0, 0, 0);
                    sprintf(items_str, "%2d", fuel_item[2]);
                    glRasterPos2i(fuelbox_x + 41, fuelbox_y + 59);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                }
                // num of items (craft field)
                if (product_item[1] != 0)
                {
                    glColor3f(0, 0, 0);
                    sprintf(items_str, "%2d", product_item[1]);
                    glRasterPos2i(productbox_x + 41, productbox_y + 59);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                }

                // draw items
                glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glColor4ub(255, 255, 255, 255);
                glBindTexture(GL_TEXTURE_2D, img);
                glBegin(GL_QUADS);
                for (int i = 0; i < 9; i++)
                {
                    if (*(player_bag + 3 * i) > 0 && *(player_bag + 3 * i) < 16 && *(player_bag + 3 * i) != 9)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) + 1) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i)) / 16.0;
                        DrawTexBlock2d(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd);
                    }
                    else if (*(player_bag + 3 * i) > 1024 && *(player_bag + 3 * i) < 1041)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) - 1024) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i) - 1025) / 16.0;
                        DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.375, 0.5);
                    }
                    else if (*(player_bag + 3 * i) == 1041)
                    {
                        tex_pd = (double)(*(player_bag + 3 * i) - 1040) / 16.0;
                        tex_pu = (double)(*(player_bag + 3 * i) - 1041) / 16.0;
                        DrawTexItem(itembox_x + 64 * i, itembox_y, tex_pu, tex_pd, 0.5, 0.625);
                    }
                }
                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (*(player_bag + 3 * (9 * j + i + 9)) > 0 && *(player_bag + 3 * (9 * j + i + 9)) < 16 && *(player_bag + 3 * (9 * j + i + 9)) != 9)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) + 1) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9))) / 16.0;
                            DrawTexBlock2d(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd);
                        }
                        else if (*(player_bag + 3 * (9 * j + i + 9)) > 1024 && *(player_bag + 3 * (9 * j + i + 9)) < 1041)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1024) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1025) / 16.0;
                            DrawTexItem(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd, 0.375, 0.5);
                        }
                        else if (*(player_bag + 3 * (9 * j + i + 9)) == 1041)
                        {
                            tex_pd = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1040) / 16.0;
                            tex_pu = (double)(*(player_bag + 3 * (9 * j + i + 9)) - 1041) / 16.0;
                            DrawTexItem(itembox_x + 64 * i, itembox_y - 64 * (3 - j) - 32, tex_pu, tex_pd, 0.5, 0.625);
                        }
                    }
                }
                if (material_item[1] > 0 && material_item[1] < 16 && material_item[1] != 9)
                {
                    tex_pd = (double)(material_item[1] + 1) / 16.0;
                    tex_pu = (double)(material_item[1]) / 16.0;
                    DrawTexBlock2d(materialbox_x, materialbox_y, tex_pu, tex_pd);
                }
                else if (material_item[1] > 1024 && material_item[1] < 1041)
                {
                    tex_pd = (double)(material_item[1] - 1024) / 16.0;
                    tex_pu = (double)(material_item[1] - 1025) / 16.0;
                    DrawTexItem(materialbox_x, materialbox_y, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (material_item[1] == 1041)
                {
                    tex_pd = (double)(material_item[1] - 1040) / 16.0;
                    tex_pu = (double)(material_item[1] - 1041) / 16.0;
                    DrawTexItem(materialbox_x, materialbox_y, tex_pu, tex_pd, 0.5, 0.625);
                }

                if (fuel_item[1] > 0 && fuel_item[1] < 16 && fuel_item[1] != 9)
                {
                    tex_pd = (double)(fuel_item[1] + 1) / 16.0;
                    tex_pu = (double)(fuel_item[1]) / 16.0;
                    DrawTexBlock2d(fuelbox_x, fuelbox_y, tex_pu, tex_pd);
                }
                else if (fuel_item[1] > 1024 && fuel_item[1] < 1041)
                {
                    tex_pd = (double)(fuel_item[1] - 1024) / 16.0;
                    tex_pu = (double)(fuel_item[1] - 1025) / 16.0;
                    DrawTexItem(fuelbox_x, fuelbox_y, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (fuel_item[1] == 1041)
                {
                    tex_pd = (double)(fuel_item[1] - 1040) / 16.0;
                    tex_pu = (double)(fuel_item[1] - 1041) / 16.0;
                    DrawTexItem(fuelbox_x, fuelbox_y, tex_pu, tex_pd, 0.5, 0.625);
                }

                if (product_item[0] > 0 && product_item[0] < 16 && product_item[0] != 9)
                {
                    tex_pd = (double)(product_item[0] + 1) / 16.0;
                    tex_pu = (double)(product_item[0]) / 16.0;
                    DrawTexBlock2d(productbox_x, productbox_y, tex_pu, tex_pd);
                }
                else if (product_item[0] > 1024 && product_item[0] < 1041)
                {
                    tex_pd = (double)(product_item[0] - 1024) / 16.0;
                    tex_pu = (double)(product_item[0] - 1025) / 16.0;
                    DrawTexItem(productbox_x, productbox_y, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (product_item[0] == 1041)
                {
                    tex_pd = (double)(product_item[0] - 1040) / 16.0;
                    tex_pu = (double)(product_item[0] - 1041) / 16.0;
                    DrawTexItem(productbox_x, productbox_y, tex_pu, tex_pd, 0.5, 0.625);
                }

                if (dragging_item[1] > 0 && dragging_item[1] < 16 && dragging_item[1] != 9)
                {
                    tex_pd = (double)(dragging_item[1] + 1) / 16.0;
                    tex_pu = (double)(dragging_item[1]) / 16.0;
                    DrawTexBlock2d(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd);
                }
                else if (dragging_item[1] > 1024 && dragging_item[1] < 1041)
                {
                    tex_pd = (double)(dragging_item[1] - 1024) / 16.0;
                    tex_pu = (double)(dragging_item[1] - 1025) / 16.0;
                    DrawTexItem(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd, 0.375, 0.5);
                }
                else if (dragging_item[1] == 1041)
                {
                    tex_pd = (double)(dragging_item[1] - 1040) / 16.0;
                    tex_pu = (double)(dragging_item[1] - 1041) / 16.0;
                    DrawTexItem(mouse_x - 32, mouse_y - 32, tex_pu, tex_pd, 0.5, 0.625);
                }
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();
                double durability;
                double red;
                double green;
                glBegin(GL_QUADS);
                for (int i = 0; i < 9; i++)
                {
                    if (*(player_bag + 3 * i) > 1025 && *(player_bag + 3 * i) < 1041 && *(player_bag + 3 * i + 2) < durability_list[*(player_bag + 3 * i) - 1026])
                    {
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 8, itembox_y + 44);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 56, itembox_y + 44);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 56, itembox_y + 48);
                        glColor3f(0.0, 0.0, 0.0);
                        glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);

                        if (*(player_bag + 3 * i + 2) >= durability_empty_list[*(player_bag + 3 * i) - 1026])
                        {
                            durability = (double)(*(player_bag + 3 * i + 2) - durability_empty_list[*(player_bag + 3 * i) - 1026]) / (double)(durability_list[*(player_bag + 3 * i) - 1026] - durability_empty_list[*(player_bag + 3 * i) - 1026]);
                            if (durability > 0.5)
                            {
                                red = (1.0 - durability) * 2.0;
                                green = 1.0;
                            }
                            if (durability <= 0.5)
                            {
                                red = 1.0;
                                green = durability * 2.0;
                            }
                            glColor3f(red, green, 0.0);
                            glVertex2f(itembox_x + 64 * i + 8, itembox_y + 44);
                            glColor3f(red, green, 0.0);
                            glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 44);
                            glColor3f(red, green, 0.0);
                            glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y + 48);
                            glColor3f(red, green, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y + 48);
                        }
                    }
                }
                for (int i = 0; i < 9; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (*(player_bag + 3 * (9 * j + i + 9)) > 1025 && *(player_bag + 3 * (9 * j + i + 9)) < 1041 && *(player_bag + 3 * (9 * j + i + 9) + 2) < durability_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026])
                        {
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 56, itembox_y - 64 * (3 - j) - 32 + 44);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 56, itembox_y - 64 * (3 - j) - 32 + 48);
                            glColor3f(0.0, 0.0, 0.0);
                            glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 48);

                            if (*(player_bag + 3 * (9 * j + i + 9) + 2) >= durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026])
                            {
                                durability = (double)(*(player_bag + 3 * (9 * j + i + 9) + 2) - durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026]) / (double)(durability_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026] - durability_empty_list[*(player_bag + 3 * (9 * j + i + 9)) - 1026]);
                                if (durability > 0.5)
                                {
                                    red = (1.0 - durability) * 2.0;
                                    green = 1.0;
                                }
                                if (durability <= 0.5)
                                {
                                    red = 1.0;
                                    green = durability * 2.0;
                                }
                                glColor3f(red, green, 0.0);
                                glVertex2f(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y - 64 * (3 - j) - 32 + 44);
                                glColor3f(red, green, 0.0);
                                glVertex2f((double)(itembox_x + 64 * i + 8) + 48.0 * durability, itembox_y - 64 * (3 - j) - 32 + 48);
                                glColor3f(red, green, 0.0);
                                glVertex2i(itembox_x + 64 * i + 8, itembox_y - 64 * (3 - j) - 32 + 48);
                            }
                        }
                    }
                }
                if (dragging_item[1] > 1025 && dragging_item[1] < 1041 && dragging_item[3] < durability_list[dragging_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 56, mouse_y - 32 + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 56, mouse_y - 32 + 48);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 48);

                    if (dragging_item[3] >= durability_empty_list[dragging_item[1] - 1026])
                    {
                        durability = (double)(dragging_item[3] - durability_empty_list[dragging_item[1] - 1026]) / (double)(durability_list[dragging_item[1] - 1026] - durability_empty_list[dragging_item[1] - 1026]);
                        if (durability > 0.5)
                        {
                            red = (1.0 - durability) * 2.0;
                            green = 1.0;
                        }
                        if (durability <= 0.5)
                        {
                            red = 1.0;
                            green = durability * 2.0;
                        }
                        glColor3f(red, green, 0.0);
                        glVertex2f(mouse_x - 32 + 8, mouse_y - 32 + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(mouse_x - 32 + 8) + 48.0 * durability, mouse_y - 32 + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(mouse_x - 32 + 8) + 48.0 * durability, mouse_y - 32 + 48);
                        glColor3f(red, green, 0.0);
                        glVertex2i(mouse_x - 32 + 8, mouse_y - 32 + 48);
                    }
                }
                if (material_item[1] > 1025 && material_item[1] < 1041 && material_item[3] < durability_list[material_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(materialbox_x + 8, materialbox_y + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(materialbox_x + 56, materialbox_y + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(materialbox_x + 56, materialbox_y + 48);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(materialbox_x + 8, materialbox_y + 48);

                    if (material_item[3] >= durability_empty_list[material_item[1] - 1026])
                    {
                        durability = (double)(material_item[3] - durability_empty_list[material_item[1] - 1026]) / (double)(durability_list[material_item[1] - 1026] - durability_empty_list[material_item[1] - 1026]);
                        if (durability > 0.5)
                        {
                            red = (1.0 - durability) * 2.0;
                            green = 1.0;
                        }
                        if (durability <= 0.5)
                        {
                            red = 1.0;
                            green = durability * 2.0;
                        }
                        glColor3f(red, green, 0.0);
                        glVertex2f(materialbox_x + 8, materialbox_y + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(materialbox_x + 8) + 48.0 * durability, materialbox_y + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(materialbox_x + 8) + 48.0 * durability, materialbox_y + 48);
                        glColor3f(red, green, 0.0);
                        glVertex2i(materialbox_x + 8, materialbox_y + 48);
                    }
                }
                if (fuel_item[1] > 1025 && fuel_item[1] < 1041 && fuel_item[3] < durability_list[fuel_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(fuelbox_x + 8, fuelbox_y + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(fuelbox_x + 56, fuelbox_y + 44);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(fuelbox_x + 56, fuelbox_y + 48);
                    glColor3f(0.0, 0.0, 0.0);
                    glVertex2i(fuelbox_x + 8, fuelbox_y + 48);

                    if (fuel_item[3] >= durability_empty_list[fuel_item[1] - 1026])
                    {
                        durability = (double)(fuel_item[3] - durability_empty_list[fuel_item[1] - 1026]) / (double)(durability_list[fuel_item[1] - 1026] - durability_empty_list[fuel_item[1] - 1026]);
                        if (durability > 0.5)
                        {
                            red = (1.0 - durability) * 2.0;
                            green = 1.0;
                        }
                        if (durability <= 0.5)
                        {
                            red = 1.0;
                            green = durability * 2.0;
                        }
                        glColor3f(red, green, 0.0);
                        glVertex2f(fuelbox_x + 8, fuelbox_y + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(fuelbox_x + 8) + 48.0 * durability, fuelbox_y + 44);
                        glColor3f(red, green, 0.0);
                        glVertex2f((double)(fuelbox_x + 8) + 48.0 * durability, fuelbox_y + 48);
                        glColor3f(red, green, 0.0);
                        glVertex2i(fuelbox_x + 8, fuelbox_y + 48);
                    }
                }
                glEnd();
            }
        }
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopMatrix();
        glFlush();

        // Update
        glutSwapBuffers();
        if (fps_count % 100 == 99)
        {
            gettimeofday(&end, NULL);
            fps[(fps_count / 100) % 10] = 100.0 / ((end.tv_sec + end.tv_sec * 1e-6) - (start.tv_sec + start.tv_sec * 1e-6));
            avg_fps = 0.0;
            for (int i = 0; i < 10; i++)
            {
                avg_fps += fps[i];
            }
            avg_fps /= 10.0;
            if (fps_count / 1000 > 0)
                sprintf(fps_str, "fps: %.2lf         ", avg_fps);
        }
        fps_count++;
        sprintf(infomation, "C-Craft pre1 (dev-ver)");
        sprintf(pos_str, "XYZ: %.2lf %.2lf %.2lf", player_x, player_y, player_z);
        sprintf(block_str, "BLOCK: %d %d %d", player_block_x, player_block_y, player_block_z);
        sprintf(chunk_str, "CHUNK: %d %d %d in %d %d %d", player_local_x, player_local_y, player_local_z, player_chunk_x, player_chunk_y, player_chunk_z);
        sprintf(keystate_str, "F:%d B:%d L:%d R:%d SLOW:%d FAST:%d JUMP:%d", key_state[0], key_state[1], key_state[2], key_state[3], key_state[4], key_state[5], key_state[6]);
        sprintf(loadposc_str, "LOAD CHUNK(PLAYER): %d %d", player_pre_chunk_x, player_pre_chunk_z);
        sprintf(loadpos_str, "LOAD CHUNK(START): %d %d", player_pre_loadchunk_x, player_pre_loadchunk_z);
        sprintf(dig_str, "BLOCK HARD(DIGING): %.2lf", dig_hardness);
        sprintf(ht_str, "HUNGRY POINT: %.2lf", hungry);
        sprintf(hht_str, "HIDE HUNGRY POINT: %.1lf", player_hide_hungry);
        sprintf(net_str, "  NO EATING TIME: %.2lf", no_eating_time);
        sprintf(het_str, "FULL EATING TIME: %.2lf", full_eating_time);
    }
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
    if (scene == 0)
    {

    }
    else if (scene == 2)
    {
    }
    else if (scene == 3)
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
}

void keyboardup(unsigned char c, int x, int y)
{
    if (scene == 0)
    {

    }
    else if (scene == 1)
    {
        if (c >= 32 && c < 127)
        {
            if (seed_length < 25)
            {
                seed_str[seed_length] = c;
                seed_length ++;
            }
        }
        else if (c == 8 || c == 127)
        {
            if (seed_length > 0)
            {
                seed_length --;
            }
        }
    }
    else if (scene == 2)
    {

    }
    else if (scene == 3)
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
        if (c == 27 && menu != 1)
        {
            pause = 1;
            menu = 1;
            craft = 0;
            fire = 0;
            enventory = 0;
        }
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
        else if (c >= '1' && c <= '9')
            debugmode = c - '1';
        else if (c == 'e' && menu != 1)
        {
            if (craft == 1)
                craft = 1 - craft;
            else if (fire == 1)
                fire = 1 - fire;
            else
                enventory = 1 - enventory;
            pause = 1 - pause;
            if (enventory == 0 && dragging_item[0] != -1)
            {
                *(player_bag + 3 * dragging_item[0] + 0) = dragging_item[1];
                *(player_bag + 3 * dragging_item[0] + 1) = dragging_item[2];
                dragging_item[0] = -1;
                dragging_item[1] = 0;
                dragging_item[2] = 0;
            }
            for (int i = 0; i < 4; i++)
            {
                if (enventory == 0 && craft_itembox[i][0] != -1)
                {
                    *(player_bag + 3 * dragging_item[0] + 0) = craft_itembox[i][1];
                    *(player_bag + 3 * dragging_item[0] + 1) = craft_itembox[i][2];
                    craft_itembox[i][0] = -1;
                    craft_itembox[i][1] = 0;
                    craft_itembox[i][2] = 0;
                }
            }
        }
    }
}

void special(int c, int x, int y)
{
}
void specialup(int c, int x, int y)
{
    if (scene == 0)
    {

    }
    else if (scene == 2)
    {

    }
    else if (scene == 3)
    {
        switch (c)
        {
        case GLUT_KEY_F5:
            camera_perspective_mode++;
            camera_perspective_mode %= 3;
            break;
        case GLUT_KEY_F3:
            debug = 1 - debug;
            break;
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    if (scene == 0 && state== GLUT_UP)
    {
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        if (w/2-128 <= x && x < w/2+128 * 9 && h/2+32 <= y && y < h/2+96)
        {
            LoadItem(player_bag);
            LoadStatus(player_status);
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    LoadChunk(chunk + i * 1114112 + j * 65536, i, j);
            while(*(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + player_local_z) == 0)
            {
                player_y -= 1.0;
                player_block_y --;
            }
            resporn_player_y = player_y;
            scene = 3;
        }
        if (w/2-128 <= x && x < w/2+128 * 9 && h/2+128 <= y && y < h/2+192)
        {
            scene = 1;
            seed_length = 0;
        }
        if (w/2-128 <= x && x < w/2+128 * 9 && h/2+224 <= y && y < h/2+288)
        {
            exit(1);
        }
    }
    else if (scene == 1 && state== GLUT_UP)
    {
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);

        if (w/2-128 <= x && x < w/2+128 * 9 && h/2+64 <= y && y < h/2+128)
        {
            scene = 0;
        }
        if (w/2-128 <= x && x < w/2+128 * 9 && h/2+160 <= y && y < h/2+224 && seed_length > 0)
        {
            SEED_NUM = 0;
            for(int i=0; i<seed_length; i++)
            {
                SEED_NUM += seed_str[i];
                SEED_NUM <<= 2;
            }
            scene = 2;
            onechunk1d_y = (double *)malloc(sizeof(double) * 16 * 16);
            diamond_rate = (double *)malloc(sizeof(double) * WORLD_CHUNK * WORLD_CHUNK * 4 * 16 * 4);
            gold_rate = (double *)malloc(sizeof(double) * WORLD_CHUNK * WORLD_CHUNK * 4 * 32 * 4);
            iron_rate = (double *)malloc(sizeof(double) * WORLD_CHUNK * WORLD_CHUNK * 4 * 64 * 4);
            coal_rate = (double *)malloc(sizeof(double) * WORLD_CHUNK * WORLD_CHUNK * 4 * 128 * 4);
            wood_rate = (double *)malloc(sizeof(double) * WORLD_CHUNK * WORLD_CHUNK * 16 * 16);
            wood_put = (char *)malloc(sizeof(char) * WORLD_CHUNK * WORLD_CHUNK * 16 * 16);

            srand(SEED_NUM);
            for (int i = 0; i < WORLD_CHUNK / 4; i++)
                for (int j = 0; j < WORLD_CHUNK / 4; j++)
                {
                    for (int x = 0; x < 16; x++)
                    {
                        for (int z = 0; z < 16; z++)
                        {
                            for (int y = 0; y < 16; y++)
                                *(diamond_rate + 65536 * i + 4096 * j + 256 * x + 16 * y + z) = (double)rand() / (double)RAND_MAX;
                            for (int y = 0; y < 32; y++)
                                *(gold_rate + 131072 * i + 8192 * j + 512 * x + 16 * y + z) = (double)rand() / (double)RAND_MAX;
                            for (int y = 0; y < 64; y++)
                                *(iron_rate + 262144 * i + 16384 * j + 1024 * x + 16 * y + z) = (double)rand() / (double)RAND_MAX;
                            for (int y = 0; y < 128; y++)
                                *(coal_rate + 524288 * i + 32768 * j + 2048 * x + 16 * y + z) = (double)rand() / (double)RAND_MAX;
                        }
                    }
                }
            for (int i = 0; i < WORLD_CHUNK; i++)
                for (int j = 0; j < WORLD_CHUNK; j++)
                    for (int x = 0; x < 16; x++)
                        for (int y = 0; y < 16; y++)
                        {
                            *(wood_put + 16384 * i + 256 * j + 16 * x + y) = 0;
                            *(wood_rate + 16384 * i + 256 * j + 16 * x + y) = (double)rand() / (double)RAND_MAX;
                        }

            for (int i = 0; i < 16; i++)
                PerlinNoize1d(onechunk1d_y + i * 16, i, 1, 1, SEED_NUM);
            
            load_counter = 0;
        }
    }
    else if (scene == 3)
    {
        double before_x, after_x, before_y, after_y, before_z, after_z;
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
            if (state == GLUT_UP)
                mouse_status[0] = -1;
            else
                mouse_status[0] = 1;
            if (mouse_status[0] == 1 && pause == 0)
            {
                for (double a = 0; a < 5.0; a += 0.05)
                {
                    after_x = player_x + a * cos(xz_rad) * cos(y_rad);
                    next_view_block_x = (int)after_x;
                    next_view_chunk_x = next_view_block_x / 16;
                    next_view_local_x = next_view_block_x % 16;
                    after_y = player_y - a * sin(y_rad);
                    next_view_block_y = (int)after_y;
                    next_view_chunk_y = next_view_block_y / 16;
                    next_view_local_y = next_view_block_y % 16;
                    after_z = player_z + a * sin(xz_rad) * cos(y_rad);
                    next_view_block_z = (int)after_z;
                    next_view_chunk_z = next_view_block_z / 16;
                    next_view_local_z = next_view_block_z % 16;
                    if (*(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(after_y)*16 + next_view_local_z) == 13)
                    {
                        pause = 1;
                        craft = 1;
                        break;
                    }
                    else if (*(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(after_y)*16 + next_view_local_z) == 14)
                    {
                        pause = 1;
                        fire = 1;
                        break;
                    }
                    else if (*(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(after_y)*16 + next_view_local_z) != 0 && *(player_bag + 3 * debugmode + 1) != 0 && *(player_bag + 3 * debugmode) <= 1024)
                    {

                        if ((int)after_x != (int)before_x && ((int)before_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int)after_z != (int)player_z))
                        {
                            *(chunk + (view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (view_local_x)*4096 + (int)(after_y)*16 + next_view_local_z) = *(player_bag + 3 * debugmode);
                            *(player_bag + 3 * debugmode + 1) -= 1;
                            if (*(player_bag + 3 * debugmode + 1) == 0)
                                *(player_bag + 3 * debugmode) = 0;
                        }
                        else if ((int)after_y != (int)before_y && ((int)after_x != (int)player_x || ((int)before_y != (int)(player_y - 1.5) && (int)before_y != (int)(player_y - 0.5)) || (int)after_z != (int)player_z))
                        {
                            *(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(before_y)*16 + next_view_local_z) = *(player_bag + 3 * debugmode);
                            *(player_bag + 3 * debugmode + 1) -= 1;
                            if (*(player_bag + 3 * debugmode + 1) == 0)
                                *(player_bag + 3 * debugmode) = 0;
                        }
                        else if ((int)after_z != (int)before_z && ((int)after_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int)before_z != (int)player_z))
                        {
                            *(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(after_y)*16 + view_local_z) = *(player_bag + 3 * debugmode);
                            *(player_bag + 3 * debugmode + 1) -= 1;
                            if (*(player_bag + 3 * debugmode + 1) == 0)
                                *(player_bag + 3 * debugmode) = 0;
                        }
                        break;
                    }
                    before_x = after_x;
                    view_block_x = (int)before_x;
                    view_chunk_x = view_block_x / 16;
                    view_local_x = view_block_x % 16;
                    before_y = after_y;
                    view_block_y = (int)before_y;
                    view_chunk_y = view_block_y / 16;
                    view_local_y = view_block_y % 16;
                    before_z = after_z;
                    view_block_z = (int)before_z;
                    view_chunk_z = view_block_z / 16;
                    view_local_z = view_block_z % 16;
                }
            }

            if (state == GLUT_UP && enventory == 1)
            {
                int w = glutGet(GLUT_WINDOW_WIDTH);
                int h = glutGet(GLUT_WINDOW_HEIGHT);
                int enventry_x = (w - 640) / 2;
                int enventry_y = (h - 640) / 2;
                int itembox_x = enventry_x + 32;
                int itembox_y = enventry_y + 32 * 10;
                int craftbox_x = itembox_x + 64 * 5;
                int craftbox_y = enventry_y + 64;
                int craftedbox_x = itembox_x + 64 * 8;
                int craftedbox_y = enventry_y + 96;
                int tmp1, tmp2, tmp3, tmp4;

                if (craftedbox_x <= x && x < craftedbox_x + 64 && craftedbox_y <= y && y < craftedbox_y + 64)
                {
                    if (dragging_item[0] == -1 || crafted_item[0] == dragging_item[1])
                    {
                        if (crafted_item[0] != 0)
                        {
                            if (crafted_item[0] > 1025 && crafted_item[0] < 1041)
                            {
                                if (dragging_item[2] == 0)
                                {
                                    for (int i = 0; i < 4; i++)
                                    {
                                        if (craft_itembox[i][1] != 0)
                                        {
                                            craft_itembox[i][2] -= 1;
                                            if (craft_itembox[i][2] == 0)
                                            {
                                                craft_itembox[i][0] = -1;
                                                craft_itembox[i][1] = 0;
                                                craft_itembox[i][3] = -1;
                                            }
                                        }
                                    }
                                    dragging_item[0] = 0;
                                    dragging_item[1] = crafted_item[0];
                                    dragging_item[2] += crafted_item[1];
                                    dragging_item[3] = crafted_item[2];
                                }
                            }
                            else if (dragging_item[2] < 64)
                            {
                                for (int i = 0; i < 4; i++)
                                {
                                    if (craft_itembox[i][1] != 0)
                                    {
                                        craft_itembox[i][2] -= 1;
                                        if (craft_itembox[i][2] == 0)
                                        {
                                            craft_itembox[i][0] = -1;
                                            craft_itembox[i][1] = 0;
                                            craft_itembox[i][3] = -1;
                                        }
                                    }
                                }
                                dragging_item[0] = 0;
                                dragging_item[1] = crafted_item[0];
                                dragging_item[2] += crafted_item[1];
                                dragging_item[3] = crafted_item[2];
                            }
                        }
                    }
                }
                if (craftbox_x <= x && x < craftbox_x + 64 * 2 && craftbox_y <= y && y < craftbox_y + 64 * 2)
                {
                    int click_state = ((x - craftbox_x) / 64) + 2 * ((y - craftbox_y) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (craft_itembox[click_state][1] == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = craft_itembox[click_state][0];
                                dragging_item[1] = craft_itembox[click_state][1];
                                dragging_item[2] = craft_itembox[click_state][2];
                                dragging_item[3] = craft_itembox[click_state][3];

                                craft_itembox[click_state][0] = tmp1;
                                craft_itembox[click_state][1] = tmp2;
                                craft_itembox[click_state][2] = tmp3;
                                craft_itembox[click_state][3] = tmp4;
                            }
                            else
                            {
                                if (craft_itembox[click_state][2] < 64)
                                {
                                    craft_itembox[click_state][2] += 1;
                                    if (dragging_item[2] == 1)
                                    {
                                        dragging_item[0] = -1;
                                        dragging_item[1] = 0;
                                        dragging_item[3] = -1;
                                    }
                                    dragging_item[2] -= 1;
                                }
                            }
                        }
                        else
                        {
                            if (craft_itembox[click_state][1] == 0)
                            {
                                craft_itembox[click_state][0] = dragging_item[0];
                                craft_itembox[click_state][1] = dragging_item[1];
                                craft_itembox[click_state][2] += 1;
                                craft_itembox[click_state][3] = dragging_item[3];
                                if (dragging_item[2] == 1)
                                {
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[3] = -1;
                                }
                                dragging_item[2] -= 1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = craft_itembox[click_state][0];
                                dragging_item[1] = craft_itembox[click_state][1];
                                dragging_item[2] = craft_itembox[click_state][2];
                                dragging_item[3] = craft_itembox[click_state][3];

                                craft_itembox[click_state][0] = tmp1;
                                craft_itembox[click_state][1] = tmp2;
                                craft_itembox[click_state][2] = tmp3;
                                craft_itembox[click_state][3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = craft_itembox[click_state][0];
                        dragging_item[1] = craft_itembox[click_state][1];
                        if (craft_itembox[click_state][2] % 2 == 1)
                            dragging_item[2] = craft_itembox[click_state][2] / 2 + 1;
                        else
                            dragging_item[2] = craft_itembox[click_state][2] / 2;
                        dragging_item[3] = craft_itembox[click_state][3];
                        craft_itembox[click_state][2] -= dragging_item[2];
                        if (craft_itembox[click_state][2] == 0)
                        {
                            craft_itembox[click_state][0] = -1;
                            craft_itembox[click_state][1] = 0;
                            craft_itembox[click_state][3] = -1;
                        }
                    }
                }
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64 * 3)
                {
                    int click_state = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) = dragging_item[1];
                                *(player_bag + 3 * click_state + 1) = dragging_item[2];
                                *(player_bag + 3 * click_state + 2) = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) += dragging_item[1];
                                *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                *(player_bag + 3 * click_state + 2) += dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
            }
            else if (state == GLUT_UP && craft == 1)
            {
                int w = glutGet(GLUT_WINDOW_WIDTH);
                int h = glutGet(GLUT_WINDOW_HEIGHT);
                int enventry_x = (w - 640) / 2;
                int enventry_y = (h - 640) / 2;
                int itembox_x = enventry_x + 32;
                int itembox_y = enventry_y + 32 * 10;
                int craftbox_x = itembox_x + 64 * 2;
                int craftbox_y = enventry_y + 64;
                int craftedbox_x = itembox_x + 64 * 7;
                int craftedbox_y = enventry_y + 128;
                int tmp1, tmp2, tmp3, tmp4;

                if (craftedbox_x <= x && x < craftedbox_x + 64 && craftedbox_y <= y && y < craftedbox_y + 64)
                {
                    if (dragging_item[0] == -1 || bigcrafted_item[0] == dragging_item[1])
                    {
                        if (bigcrafted_item[0] != 0)
                        {
                            if (bigcrafted_item[0] > 1025 && bigcrafted_item[0] < 1041)
                            {
                                if (dragging_item[2] == 0)
                                {
                                    for (int i = 0; i < 9; i++)
                                    {
                                        if (bigcraft_itembox[i][1] != 0)
                                        {
                                            bigcraft_itembox[i][2] -= 1;
                                            if (bigcraft_itembox[i][2] == 0)
                                            {
                                                bigcraft_itembox[i][0] = -1;
                                                bigcraft_itembox[i][1] = 0;
                                                bigcraft_itembox[i][3] = -1;
                                            }
                                        }
                                    }
                                    dragging_item[0] = 0;
                                    dragging_item[1] = bigcrafted_item[0];
                                    dragging_item[2] += bigcrafted_item[1];
                                    dragging_item[3] = bigcrafted_item[2];
                                }
                            }
                            else if (dragging_item[2] < 64)
                            {
                                for (int i = 0; i < 9; i++)
                                {
                                    if (bigcraft_itembox[i][1] != 0)
                                    {
                                        bigcraft_itembox[i][2] -= 1;
                                        if (bigcraft_itembox[i][2] == 0)
                                        {
                                            bigcraft_itembox[i][0] = -1;
                                            bigcraft_itembox[i][1] = 0;
                                            bigcraft_itembox[i][3] = -1;
                                        }
                                    }
                                }
                                dragging_item[0] = 0;
                                dragging_item[1] = bigcrafted_item[0];
                                dragging_item[2] += bigcrafted_item[1];
                                dragging_item[3] = bigcrafted_item[2];
                            }
                        }
                    }
                }
                if (craftbox_x <= x && x < craftbox_x + 64 * 3 && craftbox_y <= y && y < craftbox_y + 64 * 3)
                {
                    int click_state = ((x - craftbox_x) / 64) + 3 * ((y - craftbox_y) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (bigcraft_itembox[click_state][1] == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                                ;
                            else
                            {
                                if (bigcraft_itembox[click_state][2] < 64)
                                {
                                    bigcraft_itembox[click_state][2] += 1;
                                    if (dragging_item[2] == 1)
                                    {
                                        dragging_item[0] = -1;
                                        dragging_item[1] = 0;
                                        dragging_item[3] = -1;
                                    }
                                    dragging_item[2] -= 1;
                                }
                            }
                        }
                        else
                        {
                            if (bigcraft_itembox[click_state][1] == 0)
                            {
                                bigcraft_itembox[click_state][0] = dragging_item[0];
                                bigcraft_itembox[click_state][1] = dragging_item[1];
                                bigcraft_itembox[click_state][2] += 1;
                                if (dragging_item[2] == 1)
                                {
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[3] = -1;
                                }
                                dragging_item[2] -= 1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = bigcraft_itembox[click_state][0];
                                dragging_item[1] = bigcraft_itembox[click_state][1];
                                dragging_item[2] = bigcraft_itembox[click_state][2];
                                dragging_item[3] = bigcraft_itembox[click_state][3];

                                bigcraft_itembox[click_state][0] = tmp1;
                                bigcraft_itembox[click_state][1] = tmp2;
                                bigcraft_itembox[click_state][2] = tmp3;
                                bigcraft_itembox[click_state][3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = bigcraft_itembox[click_state][0];
                        dragging_item[1] = bigcraft_itembox[click_state][1];
                        if (bigcraft_itembox[click_state][2] % 2 == 1)
                            dragging_item[2] = bigcraft_itembox[click_state][2] / 2 + 1;
                        else
                            dragging_item[2] = bigcraft_itembox[click_state][2] / 2;
                        dragging_item[3] = bigcraft_itembox[click_state][3];
                        bigcraft_itembox[click_state][2] -= dragging_item[2];
                        if (bigcraft_itembox[click_state][2] == 0)
                        {
                            bigcraft_itembox[click_state][0] = -1;
                            bigcraft_itembox[click_state][1] = 0;
                            bigcraft_itembox[click_state][3] = -1;
                        }
                    }
                }
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64 * 3)
                {
                    int click_state = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) = dragging_item[1];
                                *(player_bag + 3 * click_state + 1) = dragging_item[2];
                                *(player_bag + 3 * click_state + 2) = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) += dragging_item[1];
                                *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                *(player_bag + 3 * click_state + 2) += dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
            }
            else if (state == GLUT_UP && fire == 1)
            {
                int w = glutGet(GLUT_WINDOW_WIDTH);
                int h = glutGet(GLUT_WINDOW_HEIGHT);
                int enventry_x = (w - 640) / 2;
                int enventry_y = (h - 640) / 2;
                int itembox_x = enventry_x + 32;
                int itembox_y = enventry_y + 32 * 10;
                int materialbox_x = itembox_x + 64 * 3;
                int materialbox_y = enventry_y + 32 * 2;
                int fuelbox_x = itembox_x + 64 * 3;
                int fuelbox_y = enventry_y + 32 * 6;
                int productbox_x = itembox_x + 64 * 6;
                int productbox_y = enventry_y + 32 * 4;
                int tmp1, tmp2, tmp3, tmp4;
                if (productbox_x <= x && x < productbox_x + 64 && productbox_y <= y && y < productbox_y + 64)
                {
                    if (dragging_item[0] != -1)
                    {
                        if (product_item[0] == dragging_item[1] && product_item[0] != 0)
                        {
                            dragging_item[2]++;
                            product_item[1]--;
                            if (product_item[1] == 0)
                            {
                                product_item[0] = 0;
                                product_item[1] = 0;
                                product_item[2] = -1;
                            }
                        }
                    }
                    else
                    {
                        if (product_item[0] != 0)
                        {
                            dragging_item[0] = 0;
                            dragging_item[1] = product_item[0];
                            dragging_item[2]++;
                            product_item[1]--;
                            if (product_item[1] == 0)
                            {
                                product_item[0] = 0;
                                product_item[1] = 0;
                                product_item[2] = -1;
                            }
                        }
                    }
                }
                if (materialbox_x <= x && x < materialbox_x + 64 && materialbox_y <= y && y < materialbox_y + 64)
                {
                    if (dragging_item[0] != -1)
                    {
                        if (material_item[1] == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = material_item[0];
                                dragging_item[1] = material_item[1];
                                dragging_item[2] = material_item[2];
                                dragging_item[3] = material_item[3];

                                material_item[0] = tmp1;
                                material_item[1] = tmp2;
                                material_item[2] = tmp3;
                                material_item[3] = tmp4;
                            }
                            else
                            {
                                if (material_item[2] < 64)
                                {
                                    material_item[2] += 1;
                                    if (dragging_item[2] == 1)
                                    {
                                        dragging_item[0] = -1;
                                        dragging_item[1] = 0;
                                        dragging_item[3] = -1;
                                    }
                                    dragging_item[2] -= 1;
                                }
                            }
                        }
                        else
                        {
                            if (material_item[1] == 0)
                            {
                                material_item[0] = dragging_item[0];
                                material_item[1] = dragging_item[1];
                                material_item[2] += 1;
                                material_item[3] += dragging_item[3];
                                if (dragging_item[2] == 1)
                                {
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = -1;
                                }
                                dragging_item[2] -= 1;
                            }
                            else
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = material_item[0];
                                dragging_item[1] = material_item[1];
                                dragging_item[2] = material_item[2];
                                dragging_item[3] = material_item[3];

                                material_item[0] = tmp1;
                                material_item[1] = tmp2;
                                material_item[2] = tmp3;
                                material_item[3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = material_item[0];
                        dragging_item[1] = material_item[1];
                        if (material_item[2] % 2 == 0)
                            dragging_item[2] = material_item[2] / 2;
                        else
                            dragging_item[2] = material_item[2] / 2 + 1;
                        dragging_item[3] = material_item[3];
                        material_item[2] -= dragging_item[2];
                        if (material_item[2] == 0)
                        {
                            material_item[0] = -1;
                            material_item[1] = 0;
                            material_item[3] = -1;
                        }
                    }
                }
                if (fuelbox_x <= x && x < fuelbox_x + 64 && fuelbox_y <= y && y < fuelbox_y + 64)
                {
                    if (dragging_item[0] != -1)
                    {
                        if (fuel_item[1] == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = fuel_item[0];
                                dragging_item[1] = fuel_item[1];
                                dragging_item[2] = fuel_item[2];
                                dragging_item[3] = fuel_item[3];

                                fuel_item[0] = tmp1;
                                fuel_item[1] = tmp2;
                                fuel_item[2] = tmp3;
                                fuel_item[3] = tmp4;
                            }
                            else
                            {
                                if (fuel_item[2] < 64)
                                {
                                    fuel_item[2] += 1;
                                    if (dragging_item[2] == 1)
                                    {
                                        dragging_item[0] = -1;
                                        dragging_item[1] = 0;
                                        dragging_item[3] = -1;
                                    }
                                    dragging_item[2] -= 1;
                                }
                            }
                        }
                        else
                        {
                            if (fuel_item[1] == 0)
                            {
                                fuel_item[0] = dragging_item[0];
                                fuel_item[1] = dragging_item[1];
                                fuel_item[2] += 1;
                                if (dragging_item[2] == 1)
                                {
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[3] = -1;
                                }
                                dragging_item[2] -= 1;
                            }
                            else
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = fuel_item[0];
                                dragging_item[1] = fuel_item[1];
                                dragging_item[2] = fuel_item[2];
                                dragging_item[3] = fuel_item[3];

                                fuel_item[0] = tmp1;
                                fuel_item[1] = tmp2;
                                fuel_item[2] = tmp3;
                                fuel_item[3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = fuel_item[0];
                        dragging_item[1] = fuel_item[1];
                        if (fuel_item[2] % 2 == 1)
                            dragging_item[2] = fuel_item[2] / 2 + 1;
                        else
                            dragging_item[2] = fuel_item[2] / 2;
                        dragging_item[3] = fuel_item[3];
                        fuel_item[2] -= dragging_item[2];
                        if (fuel_item[2] == 0)
                        {
                            fuel_item[0] = -1;
                            fuel_item[1] = 0;
                            fuel_item[3] = -1;
                        }
                    }
                }
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64 * 3)
                {
                    int click_state = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) = dragging_item[1];
                                *(player_bag + 3 * click_state + 1) = dragging_item[2];
                                *(player_bag + 3 * click_state + 2) = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) += dragging_item[1];
                                *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                *(player_bag + 3 * click_state + 2) += dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
            }
        }
        else if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_UP)
                mouse_status[1] = -1;
            else
                mouse_status[1] = 1;
            if (state == GLUT_UP && menu == 1)
            {
                int w = glutGet(GLUT_WINDOW_WIDTH);
                int h = glutGet(GLUT_WINDOW_HEIGHT);
                if (w/2-128 <= x && x < w/2+128 * 9 && h/2-128 <= y && y < h/2-64)
                {
                    pause = 1 - pause;
                    menu = 1 - menu;
                }
                if (w/2-128 <= x && x < w/2+128 * 9 && h/2-32 <= y && y < h/2+32)
                {
                    for (int i=0; i<2*DRAW_DIST+1; i++)
                        for (int j=0; j<2*DRAW_DIST+1; j++)
                            SaveChunk(chunk+i*1114112+j*65536, i+player_pre_loadchunk_x, j+player_pre_loadchunk_z);
                    SaveItem(player_bag);
                    SaveStatus(player_status);

                    pause = 1 - pause;
                    menu = 1 - menu;
                }
                if (w/2-128 <= x && x < w/2+128 * 9 && h/2+64 <= y && y < h/2+128)
                {
                    pause = 1 - pause;
                    menu = 1 - menu;
                    player_x = 136.0;
                    player_y = 255.0;
                    player_z = 136.0;
                    player_block_x = 136;
                    player_block_y = 255;
                    player_block_z = 136;
                    player_pre_loadchunk_x = 0;
                    player_pre_loadchunk_z = 0;
                    player_pre_chunk_x = 8;
                    player_pre_chunk_z = 8;
                    player_pre_block_y;

                    player_chunk_x = 8;
                    player_chunk_y = 5;
                    player_chunk_z = 8;
                    player_local_x = 8;
                    player_local_y = 6;
                    player_local_z = 8;
                    scene = 0;
                }
            }
            if (state == GLUT_UP && enventory == 1)
            {
                int w = glutGet(GLUT_WINDOW_WIDTH);
                int h = glutGet(GLUT_WINDOW_HEIGHT);
                int enventry_x = (w - 640) / 2;
                int enventry_y = (h - 640) / 2;
                int itembox_x = enventry_x + 32;
                int itembox_y = enventry_y + 32 * 10;
                int craftbox_x = itembox_x + 64 * 5;
                int craftbox_y = enventry_y + 64;
                int tmp1, tmp2, tmp3, tmp4;

                if (craftbox_x <= x && x < craftbox_x + 64 * 2 && craftbox_y <= y && y < craftbox_y + 64 * 2)
                {
                    int click_state = ((x - craftbox_x) / 64) + 2 * ((y - craftbox_y) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (craft_itembox[click_state][1] == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = craft_itembox[click_state][0];
                                dragging_item[1] = craft_itembox[click_state][1];
                                dragging_item[2] = craft_itembox[click_state][2];
                                dragging_item[3] = craft_itembox[click_state][3];

                                craft_itembox[click_state][0] = tmp1;
                                craft_itembox[click_state][1] = tmp2;
                                craft_itembox[click_state][2] = tmp3;
                                craft_itembox[click_state][3] = tmp4;
                            }
                            else
                            {
                                if (craft_itembox[click_state][2] + dragging_item[2] <= 64)
                                {
                                    craft_itembox[click_state][2] += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = craft_itembox[click_state][2] + dragging_item[2] - 64;
                                    craft_itembox[click_state][2] = 64;
                                }
                            }
                        }
                        else
                        {
                            if (craft_itembox[click_state][1] == 0)
                            {
                                craft_itembox[click_state][0] = dragging_item[0];
                                craft_itembox[click_state][1] = dragging_item[1];
                                craft_itembox[click_state][2] = dragging_item[2];
                                craft_itembox[click_state][3] = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = craft_itembox[click_state][0];
                                dragging_item[1] = craft_itembox[click_state][1];
                                dragging_item[2] = craft_itembox[click_state][2];
                                dragging_item[3] = craft_itembox[click_state][3];

                                craft_itembox[click_state][0] = tmp1;
                                craft_itembox[click_state][1] = tmp2;
                                craft_itembox[click_state][2] = tmp3;
                                craft_itembox[click_state][3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = craft_itembox[click_state][0];
                        dragging_item[1] = craft_itembox[click_state][1];
                        dragging_item[2] = craft_itembox[click_state][2];
                        dragging_item[3] = craft_itembox[click_state][3];
                        craft_itembox[click_state][0] = -1;
                        craft_itembox[click_state][1] = 0;
                        craft_itembox[click_state][2] = 0;
                        craft_itembox[click_state][3] = -1;
                    }
                }
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64 * 3)
                {
                    int click_state = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[2] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) = dragging_item[1];
                                *(player_bag + 3 * click_state + 1) = dragging_item[2];
                                *(player_bag + 3 * click_state + 2) = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) += dragging_item[1];
                                *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                *(player_bag + 3 * click_state + 2) += dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
            }
            else if (state == GLUT_UP && craft == 1)
            {
                int w = glutGet(GLUT_WINDOW_WIDTH);
                int h = glutGet(GLUT_WINDOW_HEIGHT);
                int enventry_x = (w - 640) / 2;
                int enventry_y = (h - 640) / 2;
                int itembox_x = enventry_x + 32;
                int itembox_y = enventry_y + 32 * 10;
                int craftbox_x = itembox_x + 64 * 2;
                int craftbox_y = enventry_y + 64;
                int tmp1, tmp2, tmp3, tmp4;

                if (craftbox_x <= x && x < craftbox_x + 64 * 3 && craftbox_y <= y && y < craftbox_y + 64 * 3)
                {
                    int click_state = ((x - craftbox_x) / 64) + 3 * ((y - craftbox_y) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (bigcraft_itembox[click_state][1] == dragging_item[1])
                        {
                            if (bigcraft_itembox[click_state][1] > 1025 && bigcraft_itembox[click_state][1] < 1031)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = bigcraft_itembox[click_state][0];
                                dragging_item[1] = bigcraft_itembox[click_state][1];
                                dragging_item[2] = bigcraft_itembox[click_state][2];
                                dragging_item[3] = bigcraft_itembox[click_state][3];

                                bigcraft_itembox[click_state][0] = tmp1;
                                bigcraft_itembox[click_state][1] = tmp2;
                                bigcraft_itembox[click_state][2] = tmp3;
                                bigcraft_itembox[click_state][3] = tmp4;
                            }
                            else
                            {
                                if (bigcraft_itembox[click_state][2] + dragging_item[2] <= 64)
                                {
                                    bigcraft_itembox[click_state][2] += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = bigcraft_itembox[click_state][2] + dragging_item[2] - 64;
                                    bigcraft_itembox[click_state][2] = 64;
                                }
                            }
                        }
                        else
                        {
                            if (bigcraft_itembox[click_state][1] == 0)
                            {
                                bigcraft_itembox[click_state][0] = dragging_item[0];
                                bigcraft_itembox[click_state][1] = dragging_item[1];
                                bigcraft_itembox[click_state][2] = dragging_item[2];
                                bigcraft_itembox[click_state][3] = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = bigcraft_itembox[click_state][0];
                                dragging_item[1] = bigcraft_itembox[click_state][1];
                                dragging_item[2] = bigcraft_itembox[click_state][2];
                                dragging_item[3] = bigcraft_itembox[click_state][3];

                                bigcraft_itembox[click_state][0] = tmp1;
                                bigcraft_itembox[click_state][1] = tmp2;
                                bigcraft_itembox[click_state][2] = tmp3;
                                bigcraft_itembox[click_state][3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = bigcraft_itembox[click_state][0];
                        dragging_item[1] = bigcraft_itembox[click_state][1];
                        dragging_item[2] = bigcraft_itembox[click_state][2];
                        dragging_item[3] = bigcraft_itembox[click_state][3];
                        bigcraft_itembox[click_state][0] = -1;
                        bigcraft_itembox[click_state][1] = 0;
                        bigcraft_itembox[click_state][2] = 0;
                        bigcraft_itembox[click_state][3] = -1;
                    }
                }
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64 * 3)
                {
                    int click_state = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[2] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) = dragging_item[1];
                                *(player_bag + 3 * click_state + 1) = dragging_item[2];
                                *(player_bag + 3 * click_state + 2) = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) += dragging_item[1];
                                *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                *(player_bag + 3 * click_state + 2) += dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
            }
            else if (state == GLUT_UP && fire == 1)
            {
                int w = glutGet(GLUT_WINDOW_WIDTH);
                int h = glutGet(GLUT_WINDOW_HEIGHT);
                int enventry_x = (w - 640) / 2;
                int enventry_y = (h - 640) / 2;
                int itembox_x = enventry_x + 32;
                int itembox_y = enventry_y + 32 * 10;
                int materialbox_x = itembox_x + 64 * 3;
                int materialbox_y = enventry_y + 32 * 2;
                int fuelbox_x = itembox_x + 64 * 3;
                int fuelbox_y = enventry_y + 32 * 6;
                int productbox_x = itembox_x + 64 * 6;
                int productbox_y = enventry_y + 32 * 4;
                int tmp1, tmp2, tmp3, tmp4;

                if (materialbox_x <= x && x < materialbox_x + 64 && materialbox_y <= y && y < materialbox_y + 64)
                {
                    if (dragging_item[0] != -1)
                    {
                        if (material_item[1] == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = material_item[0];
                                dragging_item[1] = material_item[1];
                                dragging_item[2] = material_item[2];
                                dragging_item[3] = material_item[3];

                                material_item[0] = tmp1;
                                material_item[1] = tmp2;
                                material_item[2] = tmp3;
                                material_item[3] = tmp4;
                            }
                            else
                            {
                                if (material_item[2] + dragging_item[2] <= 64)
                                {
                                    material_item[2] += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = material_item[2] + dragging_item[2] - 64;
                                    material_item[2] = 64;
                                }
                            }
                        }
                        else
                        {
                            if (material_item[1] == 0)
                            {
                                material_item[0] = dragging_item[0];
                                material_item[1] = dragging_item[1];
                                material_item[2] = dragging_item[2];
                                material_item[3] = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = material_item[0];
                                dragging_item[1] = material_item[1];
                                dragging_item[2] = material_item[2];
                                dragging_item[3] = material_item[3];

                                material_item[0] = tmp1;
                                material_item[1] = tmp2;
                                material_item[2] = tmp3;
                                material_item[3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = material_item[0];
                        dragging_item[1] = material_item[1];
                        dragging_item[2] = material_item[2];
                        dragging_item[3] = material_item[3];
                        material_item[0] = -1;
                        material_item[1] = 0;
                        material_item[2] = 0;
                        material_item[3] = -1;
                    }
                }

                if (fuelbox_x <= x && x < fuelbox_x + 64 && fuelbox_y <= y && y < fuelbox_y + 64)
                {
                    if (dragging_item[0] != -1)
                    {
                        if (fuel_item[1] == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = fuel_item[0];
                                dragging_item[1] = fuel_item[1];
                                dragging_item[2] = fuel_item[2];
                                dragging_item[3] = fuel_item[3];

                                fuel_item[0] = tmp1;
                                fuel_item[1] = tmp2;
                                fuel_item[2] = tmp3;
                                fuel_item[3] = tmp4;
                            }
                            else
                            {
                                if (fuel_item[2] + dragging_item[2] <= 64)
                                {
                                    fuel_item[2] += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = fuel_item[2] + dragging_item[2] - 64;
                                    fuel_item[2] = 64;
                                }
                            }
                        }
                        else
                        {
                            if (fuel_item[1] == 0)
                            {
                                fuel_item[0] = dragging_item[0];
                                fuel_item[1] = dragging_item[1];
                                fuel_item[2] = dragging_item[2];
                                fuel_item[3] = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = 0;
                                // swap
                                tmp1 = dragging_item[0];
                                tmp2 = dragging_item[1];
                                tmp3 = dragging_item[2];
                                tmp4 = dragging_item[3];

                                dragging_item[0] = fuel_item[0];
                                dragging_item[1] = fuel_item[1];
                                dragging_item[2] = fuel_item[2];
                                dragging_item[3] = fuel_item[3];

                                fuel_item[0] = tmp1;
                                fuel_item[1] = tmp2;
                                fuel_item[2] = tmp3;
                                fuel_item[3] = tmp4;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = fuel_item[0];
                        dragging_item[1] = fuel_item[1];
                        dragging_item[2] = fuel_item[2];
                        dragging_item[3] = fuel_item[3];
                        fuel_item[0] = -1;
                        fuel_item[1] = 0;
                        fuel_item[2] = 0;
                        fuel_item[3] = -1;
                    }
                }

                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64 * 3)
                {
                    int click_state = ((x - itembox_x) / 64) + 9 * ((y - itembox_y) / 64) + 9;
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[2] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) = dragging_item[1];
                                *(player_bag + 3 * click_state + 1) = dragging_item[2];
                                *(player_bag + 3 * click_state + 2) = dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    if (dragging_item[0] != -1)
                    {
                        if (*(player_bag + 3 * click_state) == dragging_item[1])
                        {
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                            else
                            {
                                if (*(player_bag + 3 * click_state + 1) + dragging_item[2] <= 64)
                                {
                                    *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                    dragging_item[0] = -1;
                                    dragging_item[1] = 0;
                                    dragging_item[2] = 0;
                                    dragging_item[3] = -1;
                                }
                                else
                                {
                                    dragging_item[2] = *(player_bag + 3 * click_state + 1) + dragging_item[2] - 64;
                                    *(player_bag + 3 * click_state + 1) = 64;
                                }
                            }
                        }
                        else
                        {
                            if (*(player_bag + 3 * click_state) == 0)
                            {
                                *(player_bag + 3 * click_state + 0) += dragging_item[1];
                                *(player_bag + 3 * click_state + 1) += dragging_item[2];
                                *(player_bag + 3 * click_state + 2) += dragging_item[3];
                                dragging_item[0] = -1;
                                dragging_item[1] = 0;
                                dragging_item[2] = 0;
                                dragging_item[3] = -1;
                            }
                            else
                            {
                                dragging_item[0] = click_state;
                                // swap
                                tmp1 = dragging_item[1];
                                tmp2 = dragging_item[2];
                                tmp3 = dragging_item[3];

                                dragging_item[1] = *(player_bag + 3 * click_state + 0);
                                dragging_item[2] = *(player_bag + 3 * click_state + 1);
                                dragging_item[3] = *(player_bag + 3 * click_state + 2);

                                *(player_bag + 3 * click_state + 0) = tmp1;
                                *(player_bag + 3 * click_state + 1) = tmp2;
                                *(player_bag + 3 * click_state + 2) = tmp3;
                            }
                        }
                    }
                    else
                    {
                        dragging_item[0] = click_state;
                        dragging_item[1] = *(player_bag + 3 * click_state + 0);
                        dragging_item[2] = *(player_bag + 3 * click_state + 1);
                        dragging_item[3] = *(player_bag + 3 * click_state + 2);
                        *(player_bag + 3 * click_state + 0) = 0;
                        *(player_bag + 3 * click_state + 1) = 0;
                        *(player_bag + 3 * click_state + 2) = -1;
                    }
                }
            }
        }
    }
}

void motion(int x, int y)
{
    if (scene == 0)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        mouse_x = x;
        mouse_y = y;
    }
    else if (scene == 2)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        mouse_x = x;
        mouse_y = y;
    }
    else if(scene == 3)
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
            mouse_x = x;
            mouse_y = y;
        }
    }
}

void draging(int x, int y)
{
    if (scene == 0)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
    else if (scene == 2)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
    else if(scene == 3)
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
    for (int x = -32; x < 32; x++)
    {
        dx = (double)x / 32.0;
        if (x > 0)
            x_val = 1.0 - 3.0 * (dx * dx) + 2.0 * (dx * dx * dx);
        else
            x_val = 1.0 - 3.0 * (dx * dx) - 2.0 * (dx * dx * dx);
        data[x + 32] = x_val * amp;
    }
}

void Wavelet2d(double data[256][256], double amp)
{
    double x_val, z_val;
    double dx, dz;
    for (int x = -128; x < 128; x++)
    {
        dx = (double)x / 128.0;
        if (x > 0)
            x_val = 1.0 - 3.0 * (dx * dx) + 2.0 * (dx * dx * dx);
        else
            x_val = 1.0 - 3.0 * (dx * dx) - 2.0 * (dx * dx * dx);
        for (int z = -128; z < 128; z++)
        {
            dz = (double)z / 128.0;
            if (z > 0)
                z_val = 1.0 - 3.0 * (dz * dz) + 2.0 * (dz * dz * dz);
            else
                z_val = 1.0 - 3.0 * (dz * dz) - 2.0 * (dz * dz * dz);
            data[x + 128][z + 128] = x_val * z_val * amp;
        }
    }
}

// PerlinNoize
void PerlinNoize1d(double *data, int chunk_xpos, int direction, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 2;
    int mod_chunk_xpos = chunk_xpos % 2;

    double C[64];    // Wavelet data
    double W[32][2]; // Graph data for marge
    double a[32][3]; // Gradient data

    double dx;

    // Wavelet function
    Wavelet1d(C, 5.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for (int i = 0; i < 32; i++)
    {
        a[i][0] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
        a[i][1] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
        a[i][2] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
    }

    for (int x = 0; x < 32; x++)
    {
        dx = (double)x / 32.0;
        // Calculate Graphs
        W[x][0] = a[noizeport_x][direction] * dx * C[x + 32];
        W[x][1] = a[noizeport_x + 1][direction] * (dx - 1.0) * C[x];
    }

    int xp;
    for (int x = 0; x < 16; x++)
    {
        xp = 16 * mod_chunk_xpos + x;
        dx = (double)(xp) / 32.0;
        *(data + x) = W[xp][0] + dx * (W[xp][1] - W[xp][0]);
    }
}

void PerlinNoize2di(int *data, int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed, int loops)
{
    int noizeport_x = chunk_xpos / 8;
    int noizeport_z = chunk_zpos / 8;
    int mod_chunk_xpos = chunk_xpos % 8;
    int mod_chunk_zpos = chunk_zpos % 8;

    double C[256][256];     // Wavelet data
    double W[128][128][4];  // Graph data for marge
    double Wp[128][128][2]; // Graph data (marge z)
    double a[9][9][2];      // Gradient data

    double dx, dz;

    // Wavelet function
    Wavelet2d(C, 256.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for (int l = 0; l <= loops; l++)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                a[i][j][0] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
                a[i][j][1] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
            }
        }
    }
    for (int x = 0; x < 128; x++)
    {
        dx = (double)x / 128.0;
        for (int z = 0; z < 128; z++)
        {
            dz = (double)z / 128.0;

            // Calculate Graphs
            W[x][z][0] = (a[noizeport_x][noizeport_z][0] * dx + a[noizeport_x][noizeport_z][1] * dz) * C[x + 128][z + 128];
            W[x][z][1] = (a[noizeport_x][noizeport_z + 1][0] * dx + a[noizeport_x][noizeport_z + 1][1] * (dz - 1.0)) * C[x + 128][z];
            W[x][z][2] = (a[noizeport_x + 1][noizeport_z][0] * (dx - 1.0) + a[noizeport_x + 1][noizeport_z][1] * dz) * C[x][z + 128];
            W[x][z][3] = (a[noizeport_x + 1][noizeport_z + 1][0] * (dx - 1.0) + a[noizeport_x + 1][noizeport_z + 1][1] * (dz - 1.0)) * C[x][z];
        }
    }

    for (int x = 0; x < 128; x++)
    {
        for (int z = 0; z < 128; z++)
        {
            dz = (double)z / 128.0;

            // marge (way: z)
            Wp[x][z][0] = W[x][z][0] + dz * (W[x][z][1] - W[x][z][0]);
            Wp[x][z][1] = W[x][z][2] + dz * (W[x][z][3] - W[x][z][2]);
        }
    }
    int xp, zp;
    for (int x = 0; x < 16; x++)
    {
        xp = 16 * mod_chunk_xpos + x;
        dx = (double)(xp) / 128.0;
        for (int z = 0; z < 16; z++)
        {
            zp = 16 * mod_chunk_zpos + z;
            // marge (way: x)
            *(data + 16 * x + z) = 84 + lround(Wp[xp][zp][0] + dx * (Wp[xp][zp][1] - Wp[xp][zp][0]));
        }
    }
}

void PerlinNoize2dd(double *data, int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed, int loops)
{
    int noizeport_x = chunk_xpos / 8;
    int noizeport_z = chunk_zpos / 8;
    int mod_chunk_xpos = chunk_xpos % 8;
    int mod_chunk_zpos = chunk_zpos % 8;

    double C[256][256];     // Wavelet data
    double W[128][128][4];  // Graph data for marge
    double Wp[128][128][2]; // Graph data (marge z)
    double a[9][9][2];      // Gradient data

    double dx, dz;

    // Wavelet function
    Wavelet2d(C, 1.0);

    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));

    // set random gradient
    for (int l = 0; l <= loops; l++)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                a[i][j][0] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
                a[i][j][1] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
            }
        }
    }

    for (int x = 0; x < 128; x++)
    {
        dx = (double)x / 128.0;
        for (int z = 0; z < 128; z++)
        {
            dz = (double)z / 128.0;

            // Calculate Graphs
            W[x][z][0] = (a[noizeport_x][noizeport_z][0] * dx + a[noizeport_x][noizeport_z][1] * dz) * C[x + 128][z + 128];
            W[x][z][1] = (a[noizeport_x][noizeport_z + 1][0] * dx + a[noizeport_x][noizeport_z + 1][1] * (dz - 1.0)) * C[x + 128][z];
            W[x][z][2] = (a[noizeport_x + 1][noizeport_z][0] * (dx - 1.0) + a[noizeport_x + 1][noizeport_z][1] * dz) * C[x][z + 128];
            W[x][z][3] = (a[noizeport_x + 1][noizeport_z + 1][0] * (dx - 1.0) + a[noizeport_x + 1][noizeport_z + 1][1] * (dz - 1.0)) * C[x][z];
        }
    }

    for (int x = 0; x < 128; x++)
    {
        for (int z = 0; z < 128; z++)
        {
            dz = (double)z / 128.0;

            // marge (way: z)
            Wp[x][z][0] = W[x][z][0] + dz * (W[x][z][1] - W[x][z][0]);
            Wp[x][z][1] = W[x][z][2] + dz * (W[x][z][3] - W[x][z][2]);
        }
    }
    int xp, zp;
    for (int x = 0; x < 16; x++)
    {
        xp = 16 * mod_chunk_xpos + x;
        dx = (double)(xp) / 128.0;
        for (int z = 0; z < 16; z++)
        {
            zp = 16 * mod_chunk_zpos + z;
            // marge (way: x)
            *(data + 16 * x + z) = Wp[xp][zp][0] + dx * (Wp[xp][zp][1] - Wp[xp][zp][0]);
        }
    }
}

double JumpVelocity(double start_velocity, double acceleration, double air_resistance, int tick)
{
    double curt_velocity, item1, item2;
    // calculate item 1
    item1 = start_velocity;
    for (int i = 0; i < tick; i++)
        item1 *= (1.0 - air_resistance);

    // calculate item 2
    item2 = 1.0;
    for (int i = 0; i < tick; i++)
        item2 *= (1.0 - air_resistance);
    item2 = acceleration * (1.0 - air_resistance) * (1.0 - item2) / air_resistance;

    // marge items
    curt_velocity = item1 - item2;
    return curt_velocity;
}

void DrawPlayer(void)
{
    if (camera_perspective_mode != 0)
    {
        double px[8], pfx[8], bx[8], bfx[8][2];
        double pz[8], pfz[8], bz[8], bfz[8][2];
        double pfy[8], by[8], bfy[8][2];
        double py1, py2;

        // load rot model
        for (int i = 0; i < 8; i++)
        {
            bx[i] = player_model_pos_info[0][i % 2];
            by[i] = player_model_pos_info[0][2 + (i / 4)];
            bz[i] = player_model_pos_info[0][4 + ((i / 2) % 2)];
        }

        for (int i = 0; i < 8; i++)
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
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (i % 2 == 0)
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
            glVertex3f(player_x + pfx[3], player_y + pfy[3], player_z + pfz[3]);
            glVertex3f(player_x + pfx[2], player_y + pfy[2], player_z + pfz[2]);
            glVertex3f(player_x + pfx[6], player_y + pfy[6], player_z + pfz[6]);
            glVertex3f(player_x + pfx[7], player_y + pfy[7], player_z + pfz[7]);

            glVertex3f(player_x + pfx[5], player_y + pfy[5], player_z + pfz[5]);
            glVertex3f(player_x + pfx[4], player_y + pfy[4], player_z + pfz[4]);
            glVertex3f(player_x + pfx[6], player_y + pfy[6], player_z + pfz[6]);
            glVertex3f(player_x + pfx[7], player_y + pfy[7], player_z + pfz[7]);

            glVertex3f(player_x + pfx[0], player_y + pfy[0], player_z + pfz[0]);
            glVertex3f(player_x + pfx[4], player_y + pfy[4], player_z + pfz[4]);
            glVertex3f(player_x + pfx[6], player_y + pfy[6], player_z + pfz[6]);
            glVertex3f(player_x + pfx[2], player_y + pfy[2], player_z + pfz[2]);

            glVertex3f(player_x + pfx[1], player_y + pfy[1], player_z + pfz[1]);
            glVertex3f(player_x + pfx[0], player_y + pfy[0], player_z + pfz[0]);
            glVertex3f(player_x + pfx[4], player_y + pfy[4], player_z + pfz[4]);
            glVertex3f(player_x + pfx[5], player_y + pfy[5], player_z + pfz[5]);

            glVertex3f(player_x + pfx[1], player_y + pfy[1], player_z + pfz[1]);
            glVertex3f(player_x + pfx[0], player_y + pfy[0], player_z + pfz[0]);
            glVertex3f(player_x + pfx[2], player_y + pfy[2], player_z + pfz[2]);
            glVertex3f(player_x + pfx[3], player_y + pfy[3], player_z + pfz[3]);

            glVertex3f(player_x + pfx[1], player_y + pfy[1], player_z + pfz[1]);
            glVertex3f(player_x + pfx[5], player_y + pfy[5], player_z + pfz[5]);
            glVertex3f(player_x + pfx[7], player_y + pfy[7], player_z + pfz[7]);
            glVertex3f(player_x + pfx[3], player_y + pfy[3], player_z + pfz[3]);
        }

        // Player -non-rot model-
        // body -> head
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                px[j] = player_model_pos_info[i + 1][j % 2];
                pz[j] = player_model_pos_info[i + 1][4 + j / 2];
                pfx[j] = px[j] * cos(xz_rad) - pz[j] * sin(xz_rad);
                pfz[j] = px[j] * sin(xz_rad) + pz[j] * cos(xz_rad);
            }
            py1 = player_model_pos_info[i + 1][2];
            py2 = player_model_pos_info[i + 1][3];

            glColor3f(player_model_color_info[i + 4][0], player_model_color_info[i + 4][1], player_model_color_info[i + 4][2]);
            glVertex3f(player_x + pfx[3], player_y + py1, player_z + pfz[3]);
            glVertex3f(player_x + pfx[2], player_y + py1, player_z + pfz[2]);
            glVertex3f(player_x + pfx[2], player_y + py2, player_z + pfz[2]);
            glVertex3f(player_x + pfx[3], player_y + py2, player_z + pfz[3]);

            glVertex3f(player_x + pfx[1], player_y + py2, player_z + pfz[1]);
            glVertex3f(player_x + pfx[0], player_y + py2, player_z + pfz[0]);
            glVertex3f(player_x + pfx[2], player_y + py2, player_z + pfz[2]);
            glVertex3f(player_x + pfx[3], player_y + py2, player_z + pfz[3]);

            glVertex3f(player_x + pfx[0], player_y + py1, player_z + pfz[0]);
            glVertex3f(player_x + pfx[0], player_y + py2, player_z + pfz[0]);
            glVertex3f(player_x + pfx[2], player_y + py2, player_z + pfz[2]);
            glVertex3f(player_x + pfx[2], player_y + py1, player_z + pfz[2]);

            glVertex3f(player_x + pfx[1], player_y + py1, player_z + pfz[1]);
            glVertex3f(player_x + pfx[0], player_y + py1, player_z + pfz[0]);
            glVertex3f(player_x + pfx[0], player_y + py2, player_z + pfz[0]);
            glVertex3f(player_x + pfx[1], player_y + py2, player_z + pfz[1]);

            glVertex3f(player_x + pfx[1], player_y + py1, player_z + pfz[1]);
            glVertex3f(player_x + pfx[0], player_y + py1, player_z + pfz[0]);
            glVertex3f(player_x + pfx[2], player_y + py1, player_z + pfz[2]);
            glVertex3f(player_x + pfx[3], player_y + py1, player_z + pfz[3]);

            glVertex3f(player_x + pfx[1], player_y + py1, player_z + pfz[1]);
            glVertex3f(player_x + pfx[1], player_y + py2, player_z + pfz[1]);
            glVertex3f(player_x + pfx[3], player_y + py2, player_z + pfz[3]);
            glVertex3f(player_x + pfx[3], player_y + py1, player_z + pfz[3]);
        }
    }
}

void CreateChunk(int chunk_x, int chunk_z)
{
    int *onechunk3d;
    int *onechunk2d;
    double *biome;
    double *onechunk1d_x;
    double *onechunk1d_z;
    double rate_rate;
    int wood_judge;
    int tmp;

    onechunk3d = (int *)malloc(sizeof(int) * 16 * 256 * 16);
    onechunk2d = (int *)malloc(sizeof(int) * 16 * 16);
    biome = (double *)malloc(sizeof(double) * 16 * 16);
    onechunk1d_x = (double *)malloc(sizeof(double) * 16);
    onechunk1d_z = (double *)malloc(sizeof(double) * 16);

    srand(SEED_NUM);
    int i = chunk_x;
    int j = chunk_z;
    glutPostRedisplay();
    PerlinNoize2di(onechunk2d, i, j, 1, SEED_NUM, 0);
    PerlinNoize2dd(biome, i, j, 1, SEED_NUM, 10);
    PerlinNoize1d(onechunk1d_x, i, 0, 1, SEED_NUM);
    PerlinNoize1d(onechunk1d_z, j, 2, 1, SEED_NUM);
    for (int x = 0; x < 16; x++)
    {
        for (int z = 0; z < 16; z++)
        {
            for (int y = 0; y < 256; y++)
            {
                if (*(onechunk1d_x + x) + *(onechunk1d_y + y) + *(onechunk1d_z + z) < 0.68 && *(onechunk1d_x + x) + *(onechunk1d_y + y) + *(onechunk1d_z + z) > -0.68)
                {
                    if (y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        *(onechunk3d + x * 4096 + y * 16 + z) = 3;
                    }
                    else if (y < *(onechunk2d + 16 * x + z))
                    {
                        if (0.05 > *(biome + 16 * x + z))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 1;
                        else
                            *(onechunk3d + x * 4096 + y * 16 + z) = 2;
                    }
                    else
                    {
                        *(onechunk3d + x * 4096 + y * 16 + z) = 0;
                    }
                    if (y < 128 && y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        if (y >= 8 || y <= 50)
                            rate_rate = 1.0;
                        else if (y < 8)
                            rate_rate = (1.0 - (double)(y) / 8.0);
                        else if (y > 50)
                            rate_rate = (double)(128 - y) / 78.0;
                        if (*(coal_rate + 524288 * (i % (WORLD_CHUNK / 4)) + 32768 * (j % (WORLD_CHUNK / 4)) + 2048 * x + 16 * y + z) <= (coal_create_rate * rate_rate))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 4;
                    }
                    if (y < 64 && y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        if (y >= 7 || y <= 57)
                            rate_rate = 1.0;
                        else if (y < 7)
                            rate_rate = (1.0 - (double)(y) / 7.0);
                        else if (y > 57)
                            rate_rate = (double)(64 - y) / 7.0;
                        if (*(iron_rate + 262144 * (i % (WORLD_CHUNK / 4)) + 16384 * (j % (WORLD_CHUNK / 4)) + 1024 * x + 16 * y + z) <= (iron_create_rate * rate_rate))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 5;
                    }
                    if (y < 32 && y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        if (y >= 5 || y <= 28)
                            rate_rate = 1.0;
                        else if (y < 5)
                            rate_rate = (1.0 - (double)(y) / 5.0);
                        else if (y > 28)
                            rate_rate = (double)(32 - y) / 4.0;
                        if (*(gold_rate + 131072 * (i % (WORLD_CHUNK / 4)) + 8192 * (j % (WORLD_CHUNK / 4)) + 512 * x + 16 * y + z) <= (gold_create_rate * rate_rate))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 6;
                    }
                    if (y < 16 && y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        if (y >= 5 || y <= 12)
                            rate_rate = 1.0;
                        else if (y < 5)
                            rate_rate = (1.0 - (double)(y) / 5.0);
                        else if (y > 12)
                            rate_rate = (double)(15 - y) / 3.0;
                        if (*(diamond_rate + 65536 * (i % (WORLD_CHUNK / 4)) + 4096 * (j % (WORLD_CHUNK / 4)) + 256 * x + 16 * y + z) <= (diamond_create_rate * rate_rate))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 7;
                    }
                }
                else
                    *(onechunk3d + x * 4096 + y * 16 + z) = 0;
                if (y == 0)
                {
                    *(onechunk3d + x * 4096 + y * 16 + z) = 8;
                }
            }
        }
    }
    for (int x = 0; x < 16; x++)
    {
        for (int z = 0; z < 16; z++)
        {
            tmp = 0;
            for (int y = 255; y > 64; y--)
            {
                if (*(onechunk3d + x * 4096 + y * 16 + z) != 0)
                {
                    tmp = 1;
                    break;
                }
            }
            if (tmp == 0)
            {
                for (int y = 64; y > 0; y--)
                {
                    if (*(onechunk3d + x * 4096 + y * 16 + z) != 0)
                        break;
                    *(onechunk3d + x * 4096 + y * 16 + z) = 9;
                }
            }
            if (0.05 > *(biome + 16 * x + z) && *(onechunk2d + 16 * x + z) > 64)
            {
                for (int y = *(onechunk2d + 16 * x + z); y >= *(onechunk2d + 16 * x + z) - 3; y--)
                {
                    if (*(onechunk3d + x * 4096 + y * 16 + z) == 1)
                    {
                        if ((*(wood_rate + 163844 * i + 256 * j + 16 * x + y) <= wood_create_rate[0] && 0 > *(biome + 16 * x + z)) || (*(wood_rate + 163844 * i + 256 * j + 16 * x + y) <= wood_create_rate[1] && -0.05 > *(biome + 16 * x + z)))
                        {
                            wood_judge = 0;
                            for (int a = -2; a <= 2; a++)
                            {
                                for (int b = -2; b <= 2; b++)
                                {
                                    // #1
                                    if (x + a < 0 && z + b < 0)
                                    {
                                        if (i < 1 || j < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i - 1) + 256 * (j - 1) + 16 * (16 + x + a) + (16 + z + b));
                                    }
                                    // #2
                                    else if (x + a < 0 && z + b >= 0 && z + b < 16)
                                    {
                                        if (i < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i - 1) + 256 * j + 16 * (16 + x + a) + z + b);
                                    }
                                    // #3
                                    else if (x + a < 0 && z + b <= 16)
                                    {
                                        if (i < 1 || j >= WORLD_CHUNK - 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i - 1) + 256 * (j + 1) + 16 * (16 + x + a) + (-16 + z + b));
                                    }
                                    // #4
                                    else if (x + a >= 0 && x + a < 16 && z + b < 0)
                                    {
                                        if (j < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * i + 256 * (j - 1) + 16 * (x + a) + (16 + z + b));
                                    }
                                    // #5
                                    else if (x + a >= 0 && x + a < 16 && z + b >= 0 && z + b < 16)
                                        tmp = *(wood_put + 16384 * i + 256 * j + 16 * (x + a) + (z + b));
                                    // #6
                                    else if (x + a >= 0 && x + a < 16 && z + b >= 16)
                                    {
                                        if (j >= WORLD_CHUNK - 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * i + 256 * (j + 1) + 16 * (x + a) + (-16 + z + b));
                                    }
                                    // #7
                                    else if (x + a >= 16 && z + b < 0)
                                    {
                                        if (i >= WORLD_CHUNK - 1 && j < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i + 1) + 256 * (j - 1) + 16 * (-16 + x + a) + (16 + z + b));
                                    }
                                    // #8
                                    else if (x + a >= 16 && z + b >= 0 && z + b < 16)
                                    {
                                        if (i >= WORLD_CHUNK - 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i + 1) + 256 * j + 16 * (-16 + x + a) + (z + b));
                                    }
                                    // #9
                                    else if (x + a >= 16 && z + b >= 16)
                                    {
                                        if (i >= WORLD_CHUNK - 1 && j >= WORLD_CHUNK - 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i + 1) + 256 * (j + 1) + 16 * (-16 + x + a) + (-16 + z + b));
                                    }
                                    else
                                        tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);

                                    if (tmp == 1)
                                        wood_judge = 1;
                                }
                            }
                            if (wood_judge == 0)
                            {
                                for (int a = 1; a <= 5; a++)
                                {
                                    *(onechunk3d + x * 4096 + (y + a) * 16 + z) = 10;
                                    *(wood_put + 16384 * i + 256 * j + 16 * x + z) = 1;
                                }
                                for (int c = 3; c < 6; c++)
                                {
                                    for (int a = 3 - c; a <= (c - 3); a++)
                                    {
                                        for (int b = 3 - c; b <= (c - 3); b++)
                                        {
                                            if (x + a >= 0 && x + a < 16 && z + b >= 0 && z + b < 16)
                                            {
                                                if (c == 3)
                                                {
                                                    *(onechunk3d + (x + a) * 4096 + (y + 9 - c) * 16 + (z + b)) = 11;
                                                }
                                                else if (a == 0 && b == 0)
                                                    ;
                                                else
                                                {
                                                    *(onechunk3d + (x + a) * 4096 + (y + 9 - c) * 16 + (z + b)) = 11;
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
    printf("\rcreated chunk: %d, %d (%5.2lf%%)", i, j, (double)(64 * i + j + 1) / 4096.0 * 100.0);
    fflush(stdout);
    free(onechunk3d);
    free(onechunk2d);
    free(onechunk1d_x);
    free(onechunk1d_z);
    free(biome);
}

void SaveChunk(int *chunk, int savex, int savey)
{
    char filename[30];
    sprintf(filename, "data/%d_%d.dat", savex, savey);
    FILE *f = fopen(filename, "wb");
    fwrite(chunk, sizeof(int), 16 * 16 * 256, f);
    fclose(f);
}

void LoadChunk(int *chunk, int loadx, int loady)
{
    char filename[30];
    sprintf(filename, "data/%d_%d.dat", loadx, loady);
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("[C-CRAFT ERROR]: Can't Load Chunk @ (%d, %d) (not found.)", loadx, loady);
        exit(-1);
    }
    else
        fread(chunk, sizeof(int), 16 * 16 * 256, f);
    fclose(f);
}

void SaveItem(int *bag)
{
    FILE *f = fopen("player_bag", "wb");
    fwrite(bag, sizeof(int), 108, f);
    fclose(f);
}

void LoadItem(int *bag)
{
    FILE *f = fopen("player_bag", "rb");
    if (f == NULL)
        for (int i = 0; i < 36; i++)
        {
            *(bag + 3 * i) = 0;
            *(bag + 3 * i + 1) = 0;
            *(bag + 3 * i + 2) = -1;
        }
    else
        fread(bag, sizeof(int), 108, f);
    fclose(f);
}

void SaveStatus(int *status)
{
    FILE *f = fopen("player_status", "wb");
    fwrite(status, sizeof(int), 2, f);
    fclose(f);
}

void LoadStatus(int *status)
{
    FILE *f = fopen("player_status", "rb");
    if (f == NULL)
    {
        *(status + 0) = 20;
        *(status + 1) = 18;
    }
    else
        fread(status, sizeof(int), 2, f);
    fclose(f);
}

void DrawTexBlock2d(int x, int y, double tex_pu, double tex_pd)
{
    glTexCoord2f(0, tex_pu);
    glVertex2i(x + 32, y + 8);
    glTexCoord2f(0.125, tex_pu);
    glVertex2i(x + 56, y + 16);
    glTexCoord2f(0.125, tex_pd);
    glVertex2i(x + 32, y + 32);
    glTexCoord2f(0, tex_pd);
    glVertex2i(x + 8, y + 16);

    glTexCoord2f(0.125, tex_pu);
    glVertex2i(x + 8, y + 16);
    glTexCoord2f(0.25, tex_pu);
    glVertex2i(x + 32, y + 32);
    glTexCoord2f(0.25, tex_pd);
    glVertex2i(x + 32, y + 56);
    glTexCoord2f(0.125, tex_pd);
    glVertex2i(x + 8, y + 40);

    glTexCoord2f(0.125, tex_pu);
    glVertex2i(x + 32, y + 32);
    glTexCoord2f(0.25, tex_pu);
    glVertex2i(x + 56, y + 16);
    glTexCoord2f(0.25, tex_pd);
    glVertex2i(x + 56, y + 40);
    glTexCoord2f(0.125, tex_pd);
    glVertex2i(x + 32, y + 56);
}

void DrawTexItem(int x, int y, double tex_pu, double tex_pd, double tex_pr, double tex_pl)
{
    glTexCoord2f(tex_pr, tex_pu);
    glVertex2i(x + 8, y + 8);
    glTexCoord2f(tex_pl, tex_pu);
    glVertex2i(x + 56, y + 8);
    glTexCoord2f(tex_pl, tex_pd);
    glVertex2i(x + 56, y + 56);
    glTexCoord2f(tex_pr, tex_pd);
    glVertex2i(x + 8, y + 56);
}