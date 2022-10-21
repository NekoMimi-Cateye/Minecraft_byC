#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "jfont.h"

#define WORLD_CHUNK 64
#define DRAW_DIST 8
void Wavelet1d(double[256], double);
void Wavelet2d(double[256][256], double);
void PerlinNoize1d(double *, int, int, int, unsigned int);
void PerlinNoize2di(int *, int, int, int, unsigned int, int);
void PerlinNoize2dd(double *, int, int, int, unsigned int, int);
void CreateChunk(int, int);
void SaveChunk(int *, int, int);
void LoadChunk(int *, int, int);
void LoadItem(int *);
void SaveItem(int *);
void SaveStatus(int *);
void LoadStatus(int *);
void LoadRecipe22(int *);
void LoadRecipe33(int *);
void DrawTexBlock2d(int, int, double, double);
void DrawTexItem(int, int, double, double, double, double);
void DrawPlayer(void);
double JumpVelocity(double, double, double, int);
void render(void);
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void keyboardup(unsigned char, int, int);
void mouse(int, int, int, int);
void motion(int, int);
void draging(int, int);
void specialup(int, int, int);
void DrawRect(int, int, int, int);
void ClickItemBag(int);
void SwapCursorToItemBag(int);
void DrawTexRect(int, int, int, int, double, double, double, double);

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
char seed_str[32]={0};
double player_x = 136.0;
double player_y = 255.0;
double player_z = 136.0;
int player_block_x = 136;
int player_block_y = 255;
int player_block_z = 136;
int player_chunk_x = 8;
int player_chunk_y = 5;
int player_chunk_z = 8;
int player_local_x = 8;
int player_local_y = 6;
int player_local_z = 8;
int player_pre_loadchunk_x = 0;
int player_pre_loadchunk_z = 0;
int player_pre_chunk_x = 8;
int player_pre_chunk_z = 8;
int player_pre_block_y;
double resporn_player_x = 136.0;
double resporn_player_y = 255.0;
double resporn_player_z = 136.0;
int view_block_x;
int view_block_y;
int view_block_z;
int view_chunk_x;
int view_chunk_y;
int view_chunk_z;
int view_local_x;
int view_local_y;
int view_local_z;
int next_view_block_x;
int next_view_block_y;
int next_view_block_z;
int next_view_chunk_x;
int next_view_chunk_y;
int next_view_chunk_z;
int next_view_local_x;
int next_view_local_y;
int next_view_local_z;
int dig_tag = 0;
double player_dig_speed = 0.05;
double dig_search_before_x;
double dig_search_before_y;
double dig_search_before_z;
double dig_search_after_x;
double dig_search_after_y;
double dig_search_after_z;
int dig_search_before_block_x;
int dig_search_before_block_z;
int dig_search_before_chunk_x;
int dig_search_before_chunk_z;
int dig_search_before_local_x;
int dig_search_before_local_z;
int dig_search_after_block_x;
int dig_search_after_block_z;
int dig_search_after_chunk_x;
int dig_search_after_chunk_z;
int dig_search_after_local_x;
int dig_search_after_local_z;
int dig_now_x;
int dig_now_y;
int dig_now_z;
int dig_flag = 0;
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
int canget[16] = {0, 0, 0, 1, 1, 2, 3, 3, 0, 0, 0, 0, 0, 0, 1, 4};
double diamond_create_rate = 0.001;
double gold_create_rate = 0.001;
double iron_create_rate = 0.005;
double coal_create_rate = 0.01;
double wood_create_rate[2] = {0.01, 0.1};
double apple_drop_rate = 0.1;
double hungry_time = 4.0;
double hungry = 0.0;
double eating_time = 0.0;
double eat_time = 1.0;
int no_eat = 0;
double no_eating_time = 0.0;
double no_eat_time = 1.0;
int full_eat = 0;
double full_eating_time = 0.0;
double full_eat_time = 1.0;
int apple_hungry = 4;
double apple_hide_hungry = 2.4;
double bar_length;
double arm_rad = 0.0;
int arm_d = 1;
double xz_rad = 0.0;
double y_rad = 0.0;
int mouse_x = 0;
int mouse_y = 0;
int key_state[7] = {0};
double mouse_status[2] = {-1, -1};
double player_jump_start_velocity = 0.42;
double player_gravity_acceleration = 0.08;
double player_air_resistance = 0.02;
int player_jump_tick = -1;
double player_jump_velocity = 0.0;
int player_fall_tick = -1;
double player_fall_velocity = 0.0;
int *chunk;
int *player_bag;
int *player_status;
double player_hide_hungry = 0;
struct timeval start, end;
double fps[10] = {0};
double avg_fps;
int fps_count = 0;
char debug_str[21][60] = {0};
int camera_perspective_mode = 0;
int choise = 0;
int scene = 0;
int load_counter = 0;
int pause = 0;
int debug = 0;
int enventry = 0;
int craft = 0;
int fire = 0;
int menu = 0;
int dragging_item[4] = {-1, 0, 0, -1};
int craft_itembox[4][4] = {
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1},
    {-1, 0, 0, -1}
    };
int crafted_item[3] = {0, 0, -1};
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
int bigcrafted_item[3] = {0, 0, -1};
int material_item[4] = {-1, 0, 0, -1};
int fuel_item[4] = {-1, 0, 0, -1};
int product_item[3] = {0, 0, -1};
int recipe22_num = 7;
int* recipe22;
int recipe33_num = 60;
int* recipe33;
int furnace_ope = 0;
int world_tick = 9000;
double furnace_opetime = 0;
double furnace_createtime = 10.0;
double furnace_fueltime = 0.0;
double furnace_fuelnow = 100.0;
double furnace_fueltime_block[17] = 
    {0.0, 0.0, 0.0, 0.0, 80.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 0.0, 15.0, 15.0, 0.0, 0.0, 0.0};
double furnace_fueltime_items[17] = 
    {5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0};
int furnace_creating = -1;
int furnace_recipe_num = 1;
double furnace_recipe[1][2] = {{2, 15}};
int durability_list[15] = 
    {59, 131, 250, 32, 1561, 59, 131, 250, 32, 1561, 59, 131, 250, 32, 1561};
int durability_empty_list[15] =
    {3, 6, 10, 2, 61, 3, 6, 10, 2, 61, 3, 6, 10, 2, 61};
char items_str[3];
int raining = 0;
int rain_x[128];
int rain_y[128];

int see_item;
GLuint img;
pngInfo info;
GLuint img2;
pngInfo info2;
GLuint img3;
pngInfo info3;
GLuint img4;
pngInfo info4;

int main(int argc, char **argv)
{
    chunk = (int *)malloc(sizeof(int) * (2 * DRAW_DIST + 1) * (2 * DRAW_DIST + 1) * 16 * 256 * 16);
    player_bag = (int *)malloc(sizeof(int) * 108);
    player_status = (int *)malloc(sizeof(int) * 2);
    recipe22 = (int *)malloc(sizeof(int) * recipe22_num*7);
    recipe33 = (int *)malloc(sizeof(int) * recipe33_num*12);
    LoadRecipe22(recipe22);
    LoadRecipe33(recipe33);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(640, 480);
    glutEnterGameMode();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    glutSpecialUpFunc(specialup);
    glutMouseFunc(mouse);
    glutMotionFunc(draging);
    glutPassiveMotionFunc(motion);
    glutTimerFunc(100, timer, 0);
    img = pngBind("img/texture.png", PNG_NOMIPMAP, PNG_ALPHA, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    img2 = pngBind("img/logo.png", PNG_NOMIPMAP, PNG_ALPHA, &info2, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    img3 = pngBind("img/status.png", PNG_NOMIPMAP, PNG_ALPHA, &info3, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    img4 = pngBind("img/player_skinA.png", PNG_NOMIPMAP, PNG_ALPHA, &info4, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    glutMainLoop();
    return (0);
}

void render(void)
{
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    if (scene==0)
    {
        int i=0, j=0;
        char title_btn1[10];
        char title_btn2[10];
        char title_btn3[10];
        sprintf(title_btn1, "COUNTINUE");
        sprintf(title_btn2, "NEWGAME");
        sprintf(title_btn3, "QUIT");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_QUADS);
            DrawRect(w/4, 3*w/4, h/2+32, h/2+96);
            DrawRect(w/4, 3*w/4, h/2+128, h/2+192);
            DrawRect(w/4, 3*w/4, h/2+224, h/2+288);
        glEnd();
        glColor3f(1, 1, 1);
        render_jstring(w/2-56, h/2+64+12, "続きからはじめる");
        render_jstring(w/2-56, h/2+160+12, "ワールド新規作成");
        render_jstring(w/2-56, h/2+256+12, "ゲームを終了する");
        glPopMatrix();
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
    }
    if (scene == 1)
    {
        int i=0, j=0;
        char seed_info[100];
        if (seed_length == 0)
            sprintf(seed_info, "シード文字列: 文字列を入力してください", seed_length);
        else if (seed_length < 32)
            sprintf(seed_info, "シード文字列: %2d/32文字を入力", seed_length);
        else
            sprintf(seed_info, "シード文字列: これ以上入力できません", seed_length);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
            DrawRect(w/4, 3*w/4, h/2-32, h/2+32);
        glEnd();

        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_QUADS);
            DrawRect(w/4, 3*w/4, h/2+64, h/2+128);
            if (seed_length > 0)
                DrawRect(w/4, 3*w/4, h/2+160, h/2+224);
        glEnd();
        if (seed_length == 0)
            glColor3f(0, 0, 0);
        else if (seed_length < 32)
            glColor3f(0, 0.25, 0);
        else
            glColor3f(0.25, 0, 0);
        render_jstring(w/4+12, h/2-48, seed_info);
        glColor3f(1, 1, 1);
        if (seed_length != 0)
        {
            glRasterPos2i(w/4+12, h/2+12);
            for (j = 0; j < seed_length; j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, seed_str[j]);
            if (seed_length < 32)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '_');
        }
        render_jstring(w/2-56, h/2+108, "タイトルに戻る");
        if (seed_length > 0)
            render_jstring(w/2-48, h/2+204, "ワールドを作成");
        glPopMatrix();
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
    }
    else if (scene == 2)
    {
        int i=0, j=0;
        char creating_info[100];
        double percentage = (double)(load_counter) / (double)(WORLD_CHUNK*WORLD_CHUNK);
        int per_length = percentage * (double)(w/2);

        if (load_counter < WORLD_CHUNK*WORLD_CHUNK)
        {
            if ((load_counter / 64) % 4 == 0)
                sprintf(creating_info, "ワールドを生成しています - : %5.2lf%%", 100.0*percentage);
            else if ((load_counter / 64) % 4 == 1)
                sprintf(creating_info, "ワールドを生成しています \\ : %5.2lf%%", 100.0*percentage);
            else if ((load_counter / 64) % 4 == 2)
                sprintf(creating_info, "ワールドを生成しています | : %5.2lf%%", 100.0*percentage);
            else
                sprintf(creating_info, "ワールドを生成しています / : %5.2lf%%", 100.0*percentage);
        }
        else
        {
            sprintf(creating_info, "ワールドを読み込んでいます");
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_QUADS);
            DrawRect(w/4, 3*w/4, h/2+32, h/2+96);
        glEnd();
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
            DrawRect(w/4, w/4+per_length, h/2+32, h/2+96);
        glEnd();
        glColor3f(0, 0, 0);
        render_jstring(w*3/8, h/2+72, creating_info);

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
            SaveItem(player_bag);
            SaveStatus(player_status);
            while(*(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 2.5)) * 16 + player_local_z) == 0)
            {
                player_y -= 1.0;
                player_block_y --;
            }
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
        double dash_jump_speed = 1.5;
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
        {
            if (player_jump_tick == -1)
                speed *= dash_speed;
            else
                speed *= dash_jump_speed;
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
            if (key_state[0] == 1 || key_state[1] == 1 || key_state[2] == 1 || key_state[3] == 1)
            {
                if (key_state[5] == 1 && *(player_status + 1) > 6)
                    hungry += 0.01;
                else
                    hungry += 0.001;
            }
        }
        next_player_block_x = (int)next_player_x;
        next_player_chunk_x = (int)next_player_block_x / 16;
        next_player_local_x = (int)next_player_block_x % 16;
        next_player_block_z = (int)next_player_z;
        next_player_chunk_z = (int)next_player_block_z / 16;
        next_player_local_z = (int)next_player_block_z % 16;
        if (*(chunk + (next_player_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_player_chunk_z - player_pre_loadchunk_z) * 65536 + (next_player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + next_player_local_z) == 0 && *(chunk + (next_player_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_player_chunk_z - player_pre_loadchunk_z) * 65536 + (next_player_local_x)*4096 + ((int)(player_y - 0.5)) * 16 + next_player_local_z) == 0 && next_player_x > 16.0 && next_player_x < 16.0 * (WORLD_CHUNK - 1) && next_player_z > 16.0 && next_player_z < 16.0 * (WORLD_CHUNK - 1))
        {
            player_x = next_player_x;
            player_block_x = next_player_block_x;
            player_chunk_x = next_player_chunk_x;
            player_local_x = next_player_local_x;
            player_z = next_player_z;
            player_block_z = next_player_block_z;
            player_chunk_z = next_player_chunk_z;
            player_local_z = next_player_local_z;
        }
        if (player_chunk_x - player_pre_chunk_x >= 4 || player_chunk_x - player_pre_chunk_x <= -4 || player_chunk_z - player_pre_chunk_z >= 4 || player_chunk_z - player_pre_chunk_z <= -4)
        {
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    SaveChunk(chunk + i * 1114112 + j * 65536, i + player_pre_loadchunk_x, j + player_pre_loadchunk_z);
            SaveItem(player_bag);
            SaveStatus(player_status);
            player_pre_chunk_x = player_chunk_x;
            player_pre_loadchunk_x = player_pre_chunk_x - DRAW_DIST;
            player_pre_chunk_z = player_chunk_z;
            player_pre_loadchunk_z = player_pre_chunk_z - DRAW_DIST;
            if (player_pre_loadchunk_x < 0)
                player_pre_loadchunk_x = 0;
            if (player_pre_loadchunk_x > WORLD_CHUNK - (2 * DRAW_DIST + 1))
                player_pre_loadchunk_x = WORLD_CHUNK - (2 * DRAW_DIST + 1);
            if (player_pre_loadchunk_z < 0)
                player_pre_loadchunk_z = 0;
            if (player_pre_loadchunk_z > WORLD_CHUNK - (2 * DRAW_DIST + 1))
                player_pre_loadchunk_z = WORLD_CHUNK - (2 * DRAW_DIST + 1);
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    LoadChunk(chunk + i * 1114112 + j * 65536, i + player_pre_loadchunk_x, j + player_pre_loadchunk_z);
        }
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
        if (key_state[6] == 1 && player_jump_tick == -1 && player_fall_tick == -1 && pause == 0)
        {
            player_jump_tick = 0;
            if (key_state[0] == 1 || key_state[1] == 1 || key_state[2] == 1 || key_state[3] == 1)
            {
                if (key_state[5] == 1 && *(player_status + 1) > 6)
                    hungry += 0.08;
                else
                    hungry += 0.02;
            }
            else
                hungry += 0.02;
            player_pre_block_y = player_block_y;
        }
        if (player_fall_tick == -1 && player_jump_tick == -1 && *(chunk + (player_chunk_x - player_pre_loadchunk_x) * 1114112 + (player_chunk_z - player_pre_loadchunk_z) * 65536 + (player_local_x)*4096 + ((int)(player_y - 1.5)) * 16 + player_local_z) == 0)
        {
            player_fall_tick = 0;
            player_pre_block_y = player_block_y;
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
        if (player_y < -2.0 || *(player_status) <= 0)
        {
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    SaveChunk(chunk + i * 1114112 + j * 65536, i + player_pre_loadchunk_x, j + player_pre_loadchunk_z);
            for (int i = 0; i < 36; i++)
            {
                *(player_bag + 3 * i) = 0;
                *(player_bag + 3 * i + 1) = 0;
                *(player_bag + 3 * i + 2) = -1;
            }
            *(player_status) = 20;
            *(player_status + 1) = 18;
            SaveItem(player_bag);
            SaveStatus(player_status);
            player_pre_loadchunk_x = 0;
            player_pre_loadchunk_z = 0;
            player_x = resporn_player_x;
            next_player_x = resporn_player_x;
            player_y = resporn_player_y;
            player_z = resporn_player_z;
            next_player_z = resporn_player_z;
            for (int i = 0; i < 2 * DRAW_DIST + 1; i++)
                for (int j = 0; j < 2 * DRAW_DIST + 1; j++)
                    LoadChunk(chunk + i * 1114112 + j * 65536, i, j);
        }
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
        if (hungry >= hungry_time)
        {
            if (*(player_status + 1) > 0)
                *(player_status + 1) -= 1;
            hungry -= hungry_time;
        }
        if (*(player_status + 1) > 0)
            no_eat = 0;
        else
            no_eat = 1;
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
            full_eat = 0;
        else
            full_eat = 1;
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
        if (mouse_status[0] == 1 && *(player_bag + 3 * choise) == 1041 && *(player_bag + 3 * choise + 1) > 0)
        {
            eating_time += 0.05;
            if (eating_time > eat_time)
            {
                eating_time = 0;
                *(player_bag + 3 * choise + 1) -= 1;
                if (*(player_bag + 3 * choise + 1) == 0)
                    *(player_bag + 3 * choise) = 0;
                *(player_status + 1) += apple_hungry;
                if (*(player_status + 1) > 18)
                    *(player_status + 1) = 18;
                player_hide_hungry += apple_hide_hungry;
                if (player_hide_hungry > *(player_status + 1))
                    player_hide_hungry = (double)(*(player_status + 1));
            }
        }
        else
            eating_time = 0;

        if (*(player_bag + 3 * choise) >= 1026 && *(player_bag + 3 * choise) < 1031)
            pickle_type = *(player_bag + 3 * choise) - 1025;
        else
            pickle_type = 0;
        if (*(player_bag + 3 * choise) >= 1031 && *(player_bag + 3 * choise) < 1036)
            shovel_type = *(player_bag + 3 * choise) - 1030;
        else
            shovel_type = 0;
        if (*(player_bag + 3 * choise) >= 1036 && *(player_bag + 3 * choise) < 1041)
            axe_type = *(player_bag + 3 * choise) - 1035;
        else
            axe_type = 0;
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
                        else if ((block_breaker[tmp] != 1 && tmp != 11) || (block_breaker[tmp] == 1 && canget[tmp] <= pickle_type && pickle_type != 4) || (block_breaker[tmp] == 1 && canget[tmp] <= 1 && pickle_type == 4))
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
                        if ((block_breaker[tmp] == 1 && canget[tmp] <= pickle_type && pickle_type != 4) || (block_breaker[tmp] == 1 && canget[tmp] <= 1 && pickle_type == 4) || (block_breaker[tmp] == 2 && shovel_type != 0) || (block_breaker[tmp] == 3 && axe_type != 0))
                            if (*(player_bag + 3 * choise + 2) != -1)
                            {
                                *(player_bag + 3 * choise + 2) -= 1;
                                if (*(player_bag + 3 * choise + 2) == 0)
                                {
                                    *(player_bag + 3 * choise) = 0;
                                    *(player_bag + 3 * choise + 1) = 0;
                                    *(player_bag + 3 * choise + 2) = -1;
                                }
                            }
                    }
                    dig_flag = 1;
                    break;
                }
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
        furnace_ope = 0;
        for (int i = 0; i < furnace_recipe_num; i++)
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
                    furnace_fuelnow = furnace_fueltime_block[fuel_item[1]-1025];
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
                    product_item[1]++;
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
        if (pause == 0)
        {
            mouse_x = w / 2;
            mouse_y = h / 2;
        }
        if (fps_count % 100 == 0)
            gettimeofday(&start, NULL);

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
                sprintf(debug_str[1], "fps: %.2lf         ", avg_fps);
            else
                sprintf(debug_str[1], "fps: calculating...");
        }
        sprintf(debug_str[3], "WORLD TICK: %5d", world_tick);
        if (raining == 0)
            sprintf(debug_str[4], "WEATHER: SUNNY");
        else if (raining == 1)
            sprintf(debug_str[4], "WEATHER: RAINY");
        fps_count++;
        world_tick = (world_tick+1) % 18000;
        if (world_tick % 750 == 0)
        {
            if (raining == 1)
            {
                if ((double)rand() / RAND_MAX > 0.5)
                    raining = 0;
            }
            else
            {
                if ((double)rand() / RAND_MAX > 0.05)
                    raining = 1;
                for (int i=0; i<64; i++)
                {
                    rain_x[2*i] = (double)rand() * (double) w / RAND_MAX;
                    rain_x[2*i + 1] = rain_x[2*i] + 10.0 + (double)rand() * 10.0 / RAND_MAX;
                    rain_y[2*i] = (double)rand() * (double) h / RAND_MAX;
                    rain_y[2*i + 1] = rain_y[2*i] + (double)(rain_x[2*i + 1] - rain_x[2*i]) * 4.0;
                }
            }
        }
        if (raining == 0)
        {
            if (world_tick < 3000)
                glClearColor(0.0, 0.0, 0.0, 1.0);
            else if (world_tick < 4500)
                glClearColor(0.0, 0.0, (double)(world_tick-3000) / 3000.0, 1.0);
            else if (world_tick < 6000)
                glClearColor((double)(world_tick-4500) / 3000.0, (double)(world_tick-4500) / 1500.0, (double)(world_tick-3000) / 3000.0, 1.0);
            else if (world_tick < 12000)
                glClearColor(0.5, 1.0, 1.0, 1.0);
            else if (world_tick < 13500)
                glClearColor((world_tick-10500) / 3000.0, (15000-world_tick) / 3000.0, (13500-world_tick) / 1500.0, 1.0);
            else if (world_tick < 15000)
                glClearColor((15000-world_tick) / 1500.0, (15000-world_tick) / 3000.0, 0.0, 1.0);
            else
                glClearColor(0.0, 0.0, 0.0, 1.0);
        }
        else
        {
            if (world_tick < 3000)
                glClearColor(0.0, 0.0, 0.0, 1.0);
            else if (world_tick < 4500)
                glClearColor(0.0, 0.0, (double)(world_tick-3000) * 0.5 / 3000.0, 1.0);
            else if (world_tick < 6000)
                glClearColor((double)(world_tick-4500) * 0.5 / 3000.0, (double)(world_tick-4500) * 0.5 / 1500.0, (double)(world_tick-3000) * 0.5 / 3000.0, 1.0);
            else if (world_tick < 12000)
                glClearColor(0.25, 0.5, 0.5, 1.0);
            else if (world_tick < 13500)
                glClearColor((world_tick-10500) * 0.5 / 3000.0, (15000-world_tick) * 0.5 / 3000.0, (13500-world_tick) * 0.5 / 1500.0, 1.0);
            else if (world_tick < 15000)
                glClearColor((15000-world_tick) * 0.5 / 1500.0, (15000-world_tick) * 0.5 / 3000.0, 0.0, 1.0);
            else
                glClearColor(0.0, 0.0, 0.0, 1.0);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawBuffer(GL_FRONT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (camera_perspective_mode == 0)
            gluLookAt(player_x, player_y, player_z, player_x + 5.0 * cos(xz_rad) * cos(y_rad), player_y - 5.0 * sin(y_rad), player_z + 5.0 * sin(xz_rad) * cos(y_rad), 0.0, 1.0, 0.0);
        else if (camera_perspective_mode == 1)
            gluLookAt(player_x + 5.0 * cos(xz_rad) * cos(y_rad), player_y - 5.0 * sin(y_rad), player_z + 5.0 * sin(xz_rad) * cos(y_rad), player_x, player_y, player_z, 0.0, 1.0, 0.0);
        else if (camera_perspective_mode == 2)
            gluLookAt(player_x - 5.0 * cos(xz_rad) * cos(y_rad), player_y - 5.0 * sin(y_rad), player_z - 5.0 * sin(xz_rad) * cos(y_rad), player_x, player_y, player_z, 0.0, 1.0, 0.0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255, 255, 255, 255);
        glBindTexture(GL_TEXTURE_2D, img4);
        glBegin(GL_QUADS);
        DrawPlayer();
        glEnd();
        glBindTexture(GL_TEXTURE_2D, img);
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
        double after_x, after_y, after_z;
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
        for (int i=0; i<64; i++)
        {
            int tmp;
            tmp = rain_x[2*i + 1] + (double) (rain_x[2*i + 1] - rain_x[2*i]) * 0.75;
            rain_x[2*i] = rain_x[2*i] + (double) (rain_x[2*i + 1] - rain_x[2*i]) * 0.75;
            rain_x[2*i + 1] = tmp;
            tmp = rain_y[2*i + 1] + (double) (rain_y[2*i + 1] - rain_y[2*i]) * 0.75;
            rain_y[2*i] = rain_y[2*i] + (double) (rain_y[2*i + 1] - rain_y[2*i]) * 0.75;
            rain_y[2*i + 1] = tmp;
            if (rain_x[2*i] > w || rain_y[2*i] > h)
            {
                rain_x[2*i] = (double)rand() * (double) w / RAND_MAX;
                rain_x[2*i + 1] = rain_x[2*i] + 10.0 + (double)rand() * 10.0 / RAND_MAX;
                rain_y[2*i] = (double)rand() * (double) h / RAND_MAX;
                rain_y[2*i + 1] = rain_y[2*i] + (double)(rain_x[2*i + 1] - rain_x[2*i]) * 4.0;
            }
        }
        if (raining == 1)
        {
            glColor3f(0.5, 0.5, 1.0);
            glBegin(GL_LINES);
            for (int i = 0; i < 64; i++)
                glVertex2i(rain_x[i], rain_y[i]);
            glEnd();
        }
        int itembox_x = w / 2 - 288;
        int itembox_y = h - 128;
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
                glVertex2i(itembox_x + 64 * choise, itembox_y);
                glVertex2i(itembox_x + 64 * (choise + 1), itembox_y);
                glVertex2i(itembox_x + 64 * (choise + 1), itembox_y);
                glVertex2i(itembox_x + 64 * (choise + 1), itembox_y + 64);
                glVertex2i(itembox_x + 64 * (choise + 1), itembox_y + 64);
                glVertex2i(itembox_x + 64 * choise, itembox_y + 64);
                glVertex2i(itembox_x + 64 * choise, itembox_y + 64);
                glVertex2i(itembox_x + 64 * choise, itembox_y);
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
                    DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i + 56, itembox_y + 44, itembox_y + 48);
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
                        DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i + 8 + (int)(48.0 * durability), itembox_y + 44, itembox_y + 48);
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

            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glColor4ub(255, 255, 255, 255);
            glBindTexture(GL_TEXTURE_2D, img3);
            glBegin(GL_QUADS);
            for (int i = 0; i < 10; i++)
            {
                if (2 * i + 1 < *(player_status))
                {
                    tex_pu = 0;
                    tex_pd = 0.25;
                    DrawTexRect(itembox_x + 25 * i, itembox_x + 25 * i + 24, itembox_y - 5 - 29, itembox_y - 5, tex_pu, tex_pd, 0, 0.25);
                }
                else if (2 * i < *(player_status))
                {
                    tex_pu = 0.25;
                    tex_pd = 0.5;
                    DrawTexRect(itembox_x + 25 * i, itembox_x + 25 * i + 24, itembox_y - 5 - 29, itembox_y - 5, tex_pu, tex_pd, 0, 0.25);
                }
                else
                {
                    tex_pu = 0.5;
                    tex_pd = 0.75;
                    DrawTexRect(itembox_x + 25 * i, itembox_x + 25 * i + 24, itembox_y - 5 - 29, itembox_y - 5, tex_pu, tex_pd, 0, 0.25);
                }
            }
            for (int i = 0; i < 9; i++)
            {
                if (2 * i + 1 < *(player_status+1))
                {
                    tex_pu = 0;
                    tex_pd = 0.25;
                    DrawTexRect(itembox_x + 64 * 9 - 25 * 9 + 25 * i, itembox_x + 64 * 9 - 25 * 9 + 25 * i + 24, itembox_y - 5 - 29, itembox_y - 5, tex_pu, tex_pd, 0.25, 0.5);
                }
                else if (2 * i < *(player_status+1))
                {
                    tex_pu = 0.25;
                    tex_pd = 0.5;
                    DrawTexRect(itembox_x + 64 * 9 - 25 * 9 + 25 * i, itembox_x + 64 * 9 - 25 * 9 + 25 * i + 24, itembox_y - 5 - 29, itembox_y - 5, tex_pu, tex_pd, 0.25, 0.5);
                }
                else
                {
                    tex_pu = 0.5;
                    tex_pd = 0.75;
                    DrawTexRect(itembox_x + 64 * 9 - 25 * 9 + 25 * i, itembox_x + 64 * 9 - 25 * 9 + 25 * i + 24, itembox_y - 5 - 29, itembox_y - 5, tex_pu, tex_pd, 0.25, 0.5);
                }
            }
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
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
                sprintf(debug_str[0], "C-Craft v1.0");
                sprintf(debug_str[8], "XYZ: %.2lf %.2lf %.2lf", player_x, player_y, player_z);
                sprintf(debug_str[9], "BLOCK: %d %d %d", player_block_x, player_block_y, player_block_z);
                sprintf(debug_str[10], "CHUNK: %d %d %d in %d %d %d", player_local_x, player_local_y, player_local_z, player_chunk_x, player_chunk_y, player_chunk_z);
                sprintf(debug_str[12], "F:%d B:%d L:%d R:%d SLOW:%d FAST:%d JUMP:%d", key_state[0], key_state[1], key_state[2], key_state[3], key_state[4], key_state[5], key_state[6]);
                sprintf(debug_str[13], "LOAD CHUNK(PLAYER): %d %d", player_pre_chunk_x, player_pre_chunk_z);
                sprintf(debug_str[14], "LOAD CHUNK(START): %d %d", player_pre_loadchunk_x, player_pre_loadchunk_z);
                sprintf(debug_str[15], "BLOCK HARD(DIGING): %.2lf", dig_hardness);
                sprintf(debug_str[17], "HUNGRY POINT: %.2lf", hungry);
                sprintf(debug_str[18], "HIDE HUNGRY POINT: %.1lf", player_hide_hungry);
                sprintf(debug_str[19], "  NO EATING TIME: %.2lf", no_eating_time);
                sprintf(debug_str[20], "FULL EATING TIME: %.2lf", full_eating_time);

                glColor4f(0.5, 0.5, 0.5, 0.5);
                glBegin(GL_QUADS);
                    DrawRect(0, 200, 0+4, 16+4);
                    DrawRect(0, 175, 16+4, 32+4);
                    DrawRect(0, 175, 48+4, 64+4);
                    DrawRect(0, 150, 64+4, 80+4);

                    DrawRect(0, 225, 128+4, 144+4);
                    DrawRect(0, 175, 144+4, 160+4);
                    DrawRect(0, 250, 160+4, 176+4);

                    DrawRect(0, 325, 192+4, 208+4);
                    DrawRect(0, 250, 208+4, 224+4);
                    DrawRect(0, 250, 224+4, 240+4);
                    DrawRect(0, 225, 240+4, 256+4);

                    DrawRect(0, 162, 272+4, 288+4);
                    DrawRect(0, 200, 288+4, 304+4);
                    DrawRect(0, 200, 304+4, 320+4);
                    DrawRect(0, 200, 320+4, 336+4);
                glEnd();
                glColor3f(1.0, 1.0, 1.0);                
                for(int i = 0; i <21; i++)
                {
                    glRasterPos2i(0, 16*i+16);
                    for (int j = 0; j < 60; j++)
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, debug_str[i][j]);
                }
            }
        }
        else if (menu == 1)
        {
            int w = glutGet(GLUT_WINDOW_WIDTH);
            int h = glutGet(GLUT_WINDOW_HEIGHT);
            int i=0, j=0;
            glColor4f(0, 0, 0, 0.5);
            glBegin(GL_QUADS);
                glVertex2i(0, 0);
                glVertex2i(w, 0);
                glVertex2i(w, h);
                glVertex2i(0, h);
            glEnd();
            glColor3f(0.5, 0.5, 0.5);
            glBegin(GL_QUADS);
                DrawRect(w/4, 3*w/4, h/2-128, h/2-64);
                DrawRect(w/4, 3*w/4, h/2-32, h/2+32);
                DrawRect(w/4, 3*w/4, h/2+64, h/2+128);
            glEnd();
            glColor3f(1, 1, 1);
            render_jstring(w/2-40, h/2-84, "ゲームに戻る");
            render_jstring(w/2-16, h/2+12, "セーブ");
            render_jstring(w/2-48, h/2+108, "タイトルに戻る");
        }
        else
        {
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
            enventry_x = (w - 640) / 2;
            enventry_y = (h - 640) / 2;
            itembox_x = enventry_x + 32;
            itembox_y = enventry_y + 32 * 17;
            glColor3f(0.8, 0.8, 0.8);
            glBegin(GL_QUADS);
                DrawRect(enventry_x, enventry_x+640, enventry_y, enventry_y+640);
            glEnd();
            glColor3f(0.5, 0.5, 0.5);
            glBegin(GL_QUADS);
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 3; j++)
                    DrawRect(itembox_x + 64 * i + 2, itembox_x + 64 * i + 62, enventry_y + 320 + 64*j + 2, enventry_y + 320 + 64*j + 62);
                DrawRect(itembox_x + 64 * i + 2, itembox_x + 64 * i + 62, itembox_y + 2, itembox_y + 62);
            }
            glEnd();
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
            if (dragging_item[1] != 0)
            {
                glColor3f(0, 0, 0);
                sprintf(items_str, "%2d", dragging_item[2]);
                glRasterPos2i(mouse_x + 9, mouse_y + 27);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
            }
            if (enventry == 1)
            {
                craftbox_x = itembox_x + 64 * 5;
                craftbox_y = enventry_y + 64;
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_QUADS);
                for (int j = 0; j < 4; j++)
                    DrawRect(itembox_x + 2, itembox_x + 62, enventry_y + 32 + 64*j + 2, enventry_y + 32 + 64*j + 62);
                DrawRect(itembox_x + 256 + 2, itembox_x + 256 + 62, enventry_y + 224 + 2, enventry_y + 224 + 62);
                DrawRect(itembox_x + 320 + 2, itembox_x + 320 + 62, enventry_y + 224 + 2, enventry_y + 224 + 62);

                for (int i = 5; i < 7; i++)
                    for (int j = 0; j < 2; j++)
                        DrawRect(itembox_x + 64 * i + 2, itembox_x + 64 * i + 62, enventry_y + 64 + 64*j + 2, enventry_y + 64 + 64*j + 62);
                DrawRect(itembox_x + 512 + 2, itembox_x + 512 + 62, enventry_y + 96 + 2, enventry_y + 96 + 62);
                glEnd();
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
                find_idx = -1;
                for (int i = 0; i < recipe22_num; i++)
                {
                    find = 1;
                    for (int j = 0; j < 4; j++)
                        if (craft_itembox[j][1] != *(recipe22+i*7+j))
                            find = 0;
                    if (find == 1)
                    {
                        crafted_item[0] = *(recipe22+i*7+4);
                        crafted_item[1] = *(recipe22+i*7+5);
                        crafted_item[2] = *(recipe22+i*7+6);
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
                        DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i + 56, itembox_y + 44, itembox_y + 48);
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
                            DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i + 8 + (int)(48.0 * durability), itembox_y + 44, itembox_y + 48);
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
                            DrawRect(itembox_x + 64*i + 8, itembox_x + 64*i + 56, itembox_y - 224 + 64*j + 44, itembox_y - 224 + 64*j + 48);
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
                                DrawRect(itembox_x + 64*i + 8, itembox_x + 64*i + 8 + (int)(48.0 * durability), itembox_y - 224 + 64*j + 44, itembox_y - 224 + 64*j + 48);
                            }
                        }
                    }
                }
                if (dragging_item[1] > 1025 && dragging_item[1] < 1041 && dragging_item[3] < durability_list[dragging_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    DrawRect(mouse_x - 24, mouse_x + 24, mouse_y + 12, mouse_y + 16);
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
                        DrawRect(mouse_x - 24, mouse_x - 24 + (int)(48.0 * durability), mouse_y +12, mouse_y + 16);
                    }
                }
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (craft_itembox[2 * j + i][1] > 1025 && craft_itembox[2 * j + i][1] < 1041 && craft_itembox[2 * j + i][3] < durability_list[craft_itembox[2 * j + i][1] - 1026])
                        {
                            glColor3f(0.0, 0.0, 0.0);
                            DrawRect(craftbox_x + 64*i + 8, craftbox_x + 64*i + 56, craftbox_y + 64 * j + 44, craftbox_y + 64*j + 48);
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
                                DrawRect(craftbox_x + 64*i + 8, craftbox_x + 64*i + 8 + (int)(48.0 * durability), craftbox_y + 64 * j + 44, craftbox_y + 64*j + 48);
                            }
                        }
                    }
                }
                glEnd();
            }
            else if (craft == 1)
            {
                craftbox_x = itembox_x + 64 * 2;
                craftbox_y = enventry_y + 64;
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_QUADS);
                for (int i = 2; i < 5; i++)
                    for (int j = 0; j < 3; j++)
                        DrawRect(itembox_x + 64 * i + 2, itembox_x + 64 * i + 62, enventry_y + 64 + 64*j + 2, enventry_y + 64 + 64*j + 62);
                DrawRect(itembox_x + 448 + 2, itembox_x + 448 + 62, enventry_y + 128 + 2, enventry_y + 128 + 62);
                glEnd();
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
                find_idx = -1;
                for (int i = 0; i < recipe33_num; i++)
                {
                    find = 1;
                    for (int j = 0; j < 9; j++)
                        if (bigcraft_itembox[j][1] != *(recipe33+12*i+j))
                            find = 0;
                    if (find == 1)
                    {
                        bigcrafted_item[0] = *(recipe33+12*i+9);
                        bigcrafted_item[1] = *(recipe33+12*i+10);
                        bigcrafted_item[2] = *(recipe33+12*i+11);
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
                        DrawRect(itembox_x + 64*i + 8, itembox_x + 64 * i + 56, itembox_y + 44, itembox_y + 48);
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
                            DrawRect(itembox_x + 64*i + 8, itembox_x + 64 * i + 8 + (int)(48.0 * durability), itembox_y + 44, itembox_y + 48);
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
                            DrawRect(itembox_x+ 64*i + 8, itembox_x+ 64*i + 56, itembox_y - 224 + 64*j + 44, itembox_y - 224 + 64*j + 48);
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
                            DrawRect(itembox_x+ 64*i + 8, itembox_x+ 64*i + 8 + (int)(48.0 * durability), itembox_y - 224 + 64*j + 44, itembox_y - 224 + 64*j + 48);
                            }
                        }
                    }
                }
                if (dragging_item[1] > 1025 && dragging_item[1] < 1041 && dragging_item[3] < durability_list[dragging_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    DrawRect(mouse_x-24, mouse_x+24, mouse_y+12, mouse_y+16);
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
                        DrawRect(mouse_x-24, mouse_x-24+(int)(48.0 * durability), mouse_y+12, mouse_y+16);
                    }
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (bigcraft_itembox[3 * j + i][1] > 1025 && bigcraft_itembox[3 * j + i][1] < 1041 && bigcraft_itembox[3 * j + i][3] < durability_list[bigcraft_itembox[3 * j + i][1] - 1026])
                        {
                            glColor3f(0.0, 0.0, 0.0);
                            DrawRect(craftbox_x + 64*i + 8, craftbox_x + 64*i + 56, craftbox_y + 64*j + 44, craftbox_y+ 64*j + 48);
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
                                DrawRect(craftbox_x + 64*i + 8, craftbox_x + 64*i + 8 + (int)(48.0 * durability), craftbox_y + 64*j + 44, craftbox_y+ 64*j + 48);
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
                    DrawRect(materialbox_x+2, materialbox_x+62, materialbox_y+2, materialbox_y+62);
                    DrawRect(fuelbox_x+2, fuelbox_x+62, fuelbox_y+2, fuelbox_y+62);
                    DrawRect(productbox_x+2, productbox_x+62, productbox_y+2, productbox_y+62);
                glEnd();
                glColor3f(1, 1, 1);
                glBegin(GL_QUADS);
                    DrawRect(materialbox_x + 2, materialbox_x + 62, materialbox_y + material_bar, materialbox_y + 62);
                    DrawRect(fuelbox_x + 2, fuelbox_x + 62, fuelbox_y + fuelbar, fuelbox_y + 62);
                glEnd();
                if (material_item[2] != 0)
                {
                    glColor3f(0, 0, 0);
                    sprintf(items_str, "%2d", material_item[2]);
                    glRasterPos2i(materialbox_x + 41, materialbox_y + 59);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                }
                if (fuel_item[2] != 0)
                {
                    glColor3f(0, 0, 0);
                    sprintf(items_str, "%2d", fuel_item[2]);
                    glRasterPos2i(fuelbox_x + 41, fuelbox_y + 59);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                }
                if (product_item[1] != 0)
                {
                    glColor3f(0, 0, 0);
                    sprintf(items_str, "%2d", product_item[1]);
                    glRasterPos2i(productbox_x + 41, productbox_y + 59);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[0]);
                    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, items_str[1]);
                }
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
                        DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i +56, itembox_y + 44, itembox_y + 48);
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
                            DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i + 8 + (int)(48.0 * durability), itembox_y + 44, itembox_y + 48);
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
                            DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i + 56, itembox_y - 224 + 64*j + 44, itembox_y - 224 + 64*j + 48);
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
                                DrawRect(itembox_x + 64 * i + 8, itembox_x + 64 * i + 8 + (int)(48.0*durability), itembox_y - 224 + 64*j + 44, itembox_y - 224 + 64*j + 48);
                            }
                        }
                    }
                }
                if (dragging_item[1] > 1025 && dragging_item[1] < 1041 && dragging_item[3] < durability_list[dragging_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    DrawRect(mouse_x - 24, mouse_x+24, mouse_y - 12, mouse_y+16);
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
                        DrawRect(mouse_x - 24, mouse_x-24+(int)(48.0 * durability), mouse_y - 12, mouse_y+16);
                    }
                }
                if (material_item[1] > 1025 && material_item[1] < 1041 && material_item[3] < durability_list[material_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    DrawRect(materialbox_x+8, materialbox_x + 56, materialbox_y+44, materialbox_y+48);
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
                        DrawRect(materialbox_x+8, materialbox_x + 8 + (int)(48.0 * durability), materialbox_y+44, materialbox_y+48);
                    }
                }
                if (fuel_item[1] > 1025 && fuel_item[1] < 1041 && fuel_item[3] < durability_list[fuel_item[1] - 1026])
                {
                    glColor3f(0.0, 0.0, 0.0);
                    DrawRect(fuelbox_x+8, fuelbox_x + 56, fuelbox_y+44, fuelbox_y+48);
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
                        DrawRect(fuelbox_x+8, fuelbox_x + 8 + (int)(48.0 * durability), fuelbox_y+44, fuelbox_y+48);
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
        glutSwapBuffers();
    }
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.01, 100000.0);
}

void keyboard(unsigned char c, int x, int y)
{
    if (scene == 3)
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
        else if (c == 's' || c == 'S' || c == 19)
            key_state[1] = 1;
        else if (c == 'a' || c == 'A' || c == 1)
            key_state[2] = 1;
        else if (c == 'd' || c == 'D' || c == 4)
            key_state[3] = 1;
        else if (c == ' ')
            key_state[6] = 1;
    }
}

void keyboardup(unsigned char c, int x, int y)
{
    if (scene == 1)
    {
        if (c >= 32 && c < 127)
        {
            if (seed_length < 32)
            {
                seed_str[seed_length] = c;
                seed_length ++;
            }
        }
        else if (c == 8 || c == 127)
            if (seed_length > 0)
                seed_length --;
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
            enventry = 0;
            dragging_item[0] = -1;
            dragging_item[1] = 0;
            dragging_item[2] = 0;
            for (int i = 0; i < 4; i++)
            {
                craft_itembox[i][0] = -1;
                craft_itembox[i][1] = 0;
                craft_itembox[i][2] = 0;
            }
            for (int i = 0; i < 9; i++)
            {
                bigcraft_itembox[i][0] = -1;
                bigcraft_itembox[i][1] = 0;
                bigcraft_itembox[i][2] = 0;
            }
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
            choise = c - '1';
        else if (c == 'e' && menu != 1)
        {
            if (craft == 1)
                craft = 1 - craft;
            else if (fire == 1)
                fire = 1 - fire;
            else
                enventry = 1 - enventry;
            pause = 1 - pause;
            dragging_item[0] = -1;
            dragging_item[1] = 0;
            dragging_item[2] = 0;
            for (int i = 0; i < 4; i++)
            {
                craft_itembox[i][0] = -1;
                craft_itembox[i][1] = 0;
                craft_itembox[i][2] = 0;
            }
            for (int i = 0; i < 9; i++)
            {
                bigcraft_itembox[i][0] = -1;
                bigcraft_itembox[i][1] = 0;
                bigcraft_itembox[i][2] = 0;
            }
        }
    }
}

void specialup(int c, int x, int y)
{
    if (scene == 3)
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
        if (w/4 <= x && x < w*3/4 && h/2+32 <= y && y < h/2+96)
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
        if (w/4 <= x && x < 3*w/4 && h/2+128 <= y && y < h/2+192)
        {
            scene = 1;
            seed_length = 0;
        }
        if (w/4 <= x && x < 3*w/4 && h/2+224 <= y && y < h/2+288)
        {
            exit(1);
        }
    }
    else if (scene == 1 && state== GLUT_UP)
    {
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        if (w/4 <= x && x < 3*w/4 && h/2+64 <= y && y < h/2+128)
            scene = 0;
        if (w/4 <= x && x < 3*w/4 && h/2+160 <= y && y < h/2+224 && seed_length > 0)
        {
            SEED_NUM = 0;
            for(int i=0; i<seed_length; i++)
            {
                SEED_NUM += seed_str[i];
                SEED_NUM ^=  (SEED_NUM << 2);
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
        if (button == GLUT_RIGHT_BUTTON)
        {
            if (state == GLUT_UP)
                mouse_status[0] = -1;
            else
                mouse_status[0] = 1;
            if (mouse_status[0] == 1 && pause == 0)
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
                    else if (*(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(after_y)*16 + next_view_local_z) != 0 && *(player_bag + 3 * choise + 1) != 0 && *(player_bag + 3 * choise) <= 1024)
                    {

                        if ((int)after_x != (int)before_x && ((int)before_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int)after_z != (int)player_z))
                        {
                            *(chunk + (view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (view_local_x)*4096 + (int)(after_y)*16 + next_view_local_z) = *(player_bag + 3 * choise);
                            *(player_bag + 3 * choise + 1) -= 1;
                            if (*(player_bag + 3 * choise + 1) == 0)
                                *(player_bag + 3 * choise) = 0;
                        }
                        else if ((int)after_y != (int)before_y && ((int)after_x != (int)player_x || ((int)before_y != (int)(player_y - 1.5) && (int)before_y != (int)(player_y - 0.5)) || (int)after_z != (int)player_z))
                        {
                            *(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (next_view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(before_y)*16 + next_view_local_z) = *(player_bag + 3 * choise);
                            *(player_bag + 3 * choise + 1) -= 1;
                            if (*(player_bag + 3 * choise + 1) == 0)
                                *(player_bag + 3 * choise) = 0;
                        }
                        else if ((int)after_z != (int)before_z && ((int)after_x != (int)player_x || ((int)after_y != (int)(player_y - 1.5) && (int)after_y != (int)(player_y - 0.5)) || (int)before_z != (int)player_z))
                        {
                            *(chunk + (next_view_chunk_x - player_pre_loadchunk_x) * 1114112 + (view_chunk_z - player_pre_loadchunk_z) * 65536 + (next_view_local_x)*4096 + (int)(after_y)*16 + view_local_z) = *(player_bag + 3 * choise);
                            *(player_bag + 3 * choise + 1) -= 1;
                            if (*(player_bag + 3 * choise + 1) == 0)
                                *(player_bag + 3 * choise) = 0;
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
            if (state == GLUT_UP && enventry == 1)
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
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
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
                    ClickItemBag(click_state);
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    ClickItemBag(click_state);
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
                            if (dragging_item[1] > 1025 && dragging_item[1] < 1041);
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
                            if (craft_itembox[click_state][1] == 0)
                            {
                                bigcraft_itembox[click_state][0] = dragging_item[0];
                                bigcraft_itembox[click_state][1] = dragging_item[1];
                                bigcraft_itembox[click_state][2] += 1;
                                bigcraft_itembox[click_state][3] = dragging_item[3];
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
                    ClickItemBag(click_state);
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    ClickItemBag(click_state);
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
                                material_item[3] = dragging_item[3];
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
                                fuel_item[3] = dragging_item[3];
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
                    ClickItemBag(click_state);                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    ClickItemBag(click_state);
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
                    player_chunk_x = 8;
                    player_chunk_y = 5;
                    player_chunk_z = 8;
                    player_local_x = 8;
                    player_local_y = 6;
                    player_local_z = 8;
                    scene = 0;
                }
            }
            if (state == GLUT_UP && enventry == 1)
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
                    ClickItemBag(click_state);
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    ClickItemBag(click_state);
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
                    ClickItemBag(click_state);
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    ClickItemBag(click_state);
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
                    ClickItemBag(click_state);
                }
                itembox_y = enventry_y + 32 * 17;
                if (itembox_x <= x && x < itembox_x + 64 * 9 && itembox_y <= y && y < itembox_y + 64)
                {
                    int click_state = ((x - itembox_x) / 64);
                    ClickItemBag(click_state);
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
    else if (scene == 1)
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
        mouse_x = x;
        mouse_y = y;
    }
    else if (scene == 1)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        mouse_x = x;
        mouse_y = y;
    }
    else if(scene == 3)
    {
        if (pause == 0)
        {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(mouse_x, mouse_y);
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

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1, timer, 0);
}

void Wavelet1d(double data[64], double amp)
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

void PerlinNoize1d(double *data, int chunk_xpos, int direction, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 2;
    int mod_chunk_xpos = chunk_xpos % 2;
    double C[64];
    double W[32][2];
    double a[32][3];
    double dx;
    Wavelet1d(C, 5.0);
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));
    for (int i = 0; i < 32; i++)
    {
        a[i][0] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
        a[i][1] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
        a[i][2] = (double)(rand() - RAND_MAX / 2) / (double)RAND_MAX;
    }
    for (int x = 0; x < 32; x++)
    {
        dx = (double)x / 32.0;
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
    double C[256][256];
    double W[128][128][4];
    double Wp[128][128][2];
    double a[9][9][2];
    double dx, dz;
    Wavelet2d(C, 256.0);
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));
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
    double C[256][256];
    double W[128][128][4];
    double Wp[128][128][2];
    double a[9][9][2];
    double dx, dz;
    Wavelet2d(C, 1.0);
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));
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
            *(data + 16 * x + z) = Wp[xp][zp][0] + dx * (Wp[xp][zp][1] - Wp[xp][zp][0]);
        }
    }
}

double JumpVelocity(double start_velocity, double acceleration, double air_resistance, int tick)
{
    double curt_velocity, item1, item2;
    item1 = start_velocity;
    for (int i = 0; i < tick; i++)
        item1 *= (1.0 - air_resistance);
    item2 = 1.0;
    for (int i = 0; i < tick; i++)
        item2 *= (1.0 - air_resistance);
    item2 = acceleration * (1.0 - air_resistance) * (1.0 - item2) / air_resistance;
    curt_velocity = item1 - item2;
    return curt_velocity;
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
                        if (y >= 8 && y <= 50)
                            rate_rate = 1.0;
                        else if (y < 8)
                            rate_rate = ((double)(y) / 8.0);
                        else if (y > 50)
                            rate_rate = (double)(128 - y) / 78.0;
                        if (*(coal_rate + 524288 * (i % (WORLD_CHUNK / 4)) + 32768 * (j % (WORLD_CHUNK / 4)) + 2048 * x + 16 * y + z) <= (coal_create_rate * rate_rate))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 4;
                    }
                    if (y < 64 && y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        if (y >= 7 && y <= 57)
                            rate_rate = 1.0;
                        else if (y < 7)
                            rate_rate = ((double)(y) / 7.0);
                        else if (y > 57)
                            rate_rate = (double)(64 - y) / 7.0;
                        if (*(iron_rate + 262144 * (i % (WORLD_CHUNK / 4)) + 16384 * (j % (WORLD_CHUNK / 4)) + 1024 * x + 16 * y + z) <= (iron_create_rate * rate_rate))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 5;
                    }
                    if (y < 32 && y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        if (y >= 5 && y <= 28)
                            rate_rate = 1.0;
                        else if (y < 5)
                            rate_rate = ((double)(y) / 5.0);
                        else if (y > 28)
                            rate_rate = (double)(32 - y) / 4.0;
                        if (*(gold_rate + 131072 * (i % (WORLD_CHUNK / 4)) + 8192 * (j % (WORLD_CHUNK / 4)) + 512 * x + 16 * y + z) <= (gold_create_rate * rate_rate))
                            *(onechunk3d + x * 4096 + y * 16 + z) = 6;
                    }
                    if (y < 16 && y < *(onechunk2d + 16 * x + z) - 3)
                    {
                        if (y >= 5 && y <= 12)
                            rate_rate = 1.0;
                        else if (y < 5)
                            rate_rate = ((double)(y) / 5.0);
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
                                    if (x + a < 0 && z + b < 0)
                                    {
                                        if (i < 1 || j < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i - 1) + 256 * (j - 1) + 16 * (16 + x + a) + (16 + z + b));
                                    }
                                    else if (x + a < 0 && z + b >= 0 && z + b < 16)
                                    {
                                        if (i < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i - 1) + 256 * j + 16 * (16 + x + a) + z + b);
                                    }
                                    else if (x + a < 0 && z + b <= 16)
                                    {
                                        if (i < 1 || j >= WORLD_CHUNK - 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i - 1) + 256 * (j + 1) + 16 * (16 + x + a) + (-16 + z + b));
                                    }
                                    else if (x + a >= 0 && x + a < 16 && z + b < 0)
                                    {
                                        if (j < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * i + 256 * (j - 1) + 16 * (x + a) + (16 + z + b));
                                    }
                                    else if (x + a >= 0 && x + a < 16 && z + b >= 0 && z + b < 16)
                                        tmp = *(wood_put + 16384 * i + 256 * j + 16 * (x + a) + (z + b));
                                    else if (x + a >= 0 && x + a < 16 && z + b >= 16)
                                    {
                                        if (j >= WORLD_CHUNK - 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * i + 256 * (j + 1) + 16 * (x + a) + (-16 + z + b));
                                    }
                                    else if (x + a >= 16 && z + b < 0)
                                    {
                                        if (i >= WORLD_CHUNK - 1 && j < 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i + 1) + 256 * (j - 1) + 16 * (-16 + x + a) + (16 + z + b));
                                    }
                                    else if (x + a >= 16 && z + b >= 0 && z + b < 16)
                                    {
                                        if (i >= WORLD_CHUNK - 1)
                                            tmp = *(wood_put + 16384 * i + 256 * j + 16 * x + z);
                                        else
                                            tmp = *(wood_put + 16384 * (i + 1) + 256 * j + 16 * (-16 + x + a) + (z + b));
                                    }
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
    free(onechunk3d);
    free(onechunk2d);
    free(onechunk1d_x);
    free(onechunk1d_z);
    free(biome);
}

void DrawPlayer(void)
{
    double player_model_pos_info[3][6] = {
        {-0.1125, 0.1125, 0.1125, -0.5625, -0.1125, 0.1125},
        {-0.1125, 0.1125, -0.825, -0.15, -0.225, 0.225},
        {-0.225, 0.225, -0.15, 0.3, -0.225, 0.225}
        };
    double player_model_mov_info[4][3] = {
        {0.0, -0.9375, 0.1125},
        {0.0, -0.9375, -0.1125},
        {0.0, -0.2625, -0.3375},
        {0.0, -0.2625, 0.3375}
        };
    double player_model_tex_info[6][24] = {
        //reg
        {0.75, 0.0, 1.0, 0.75, 0.75, 0.5, 1.0, 0.75, 0.75, 0.0, 1.0, 0.75, 0.5, 0.0, 0.75, 0.75, 0.25, 0.25, 0.5, 0.5, 0.5, 0, 0.75, 0.75},
        {0.75, 0.0, 1.0, 0.75, 0.75, 0.5, 1.0, 0.75, 0.5, 0.0, 0.75, 0.75, 0.75, 0.0, 1.0, 0.75, 0.25, 0.25, 0.5, 0.5, 0.5, 0, 0.75, 0.75},
        // arm
        {0.25, 0.0, 0.5, 0.75, 0.25, 0.5, 0.5, 0.75, 0.25, 0.0, 0.5, 0.75, 0.25, 0.0, 0.5, 0.75, 0.25, 0.0, 0.5, 0.25, 0.25, 0.0, 0.5, 0.75},
        {0.25, 0.0, 0.5, 0.75, 0.25, 0.5, 0.5, 0.75, 0.25, 0.0, 0.5, 0.75, 0.25, 0.0, 0.5, 0.75, 0.25, 0.0, 0.5, 0.25, 0.25, 0.0, 0.5, 0.75},
        //body
        {0.25, 0.25, 0.5, 0.5, 0.25, 0.25, 0.5, 0.5, 0.25, 0.75, 0.5, 1.0, 0.25, 0.25, 0.5, 0.5, 0.25, 0.25, 0.5, 0.5, 0.25, 0.75, 0.5, 1.0},
        //head
        {0, 0.25, 0.25, 0.5, 0.0, 0.0, 0.25, 0.25, 0.0, 0.0, 0.25, 0.25, 0.0, 0.25, 0.25, 0.5, 0, 0.75, 0, 1.0, 0.0, 0.5, 0.25, 0.75},
        };
    if (camera_perspective_mode != 0)
    {
        double px[8], pfx[8], bx[8], bfx[8][2];
        double pz[8], pfz[8], bz[8], bfz[8][2];
        double pfy[8], by[8], bfy[8][2];
        double py1, py2;
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
                if (i/2 == 0)
                {
                    pfx[j] = px[j] * cos(xz_rad-arm_rad*0.1) - pz[j] * sin(xz_rad-arm_rad*0.1);
                    pfz[j] = px[j] * sin(xz_rad-arm_rad*0.1) + pz[j] * cos(xz_rad-arm_rad*0.1);
                }
                else
                {
                    pfx[j] = px[j] * cos(xz_rad+arm_rad*0.1) - pz[j] * sin(xz_rad+arm_rad*0.1);
                    pfz[j] = px[j] * sin(xz_rad+arm_rad*0.1) + pz[j] * cos(xz_rad+arm_rad*0.1);
                }
            }
            glTexCoord2f(player_model_tex_info[i][2], player_model_tex_info[i][1]);
            glVertex3f(player_x + pfx[3], player_y + pfy[3], player_z + pfz[3]);
            glTexCoord2f(player_model_tex_info[i][0], player_model_tex_info[i][1]);
            glVertex3f(player_x + pfx[2], player_y + pfy[2], player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i][0], player_model_tex_info[i][3]);
            glVertex3f(player_x + pfx[6], player_y + pfy[6], player_z + pfz[6]);
            glTexCoord2f(player_model_tex_info[i][2], player_model_tex_info[i][3]);
            glVertex3f(player_x + pfx[7], player_y + pfy[7], player_z + pfz[7]);
            glTexCoord2f(player_model_tex_info[i][4], player_model_tex_info[i][5]);
            glVertex3f(player_x + pfx[5], player_y + pfy[5], player_z + pfz[5]);
            glTexCoord2f(player_model_tex_info[i][4], player_model_tex_info[i][7]);
            glVertex3f(player_x + pfx[4], player_y + pfy[4], player_z + pfz[4]);
            glTexCoord2f(player_model_tex_info[i][6], player_model_tex_info[i][7]);
            glVertex3f(player_x + pfx[6], player_y + pfy[6], player_z + pfz[6]);
            glTexCoord2f(player_model_tex_info[i][6], player_model_tex_info[i][5]);
            glVertex3f(player_x + pfx[7], player_y + pfy[7], player_z + pfz[7]);
            glTexCoord2f(player_model_tex_info[i][8], player_model_tex_info[i][9]);
            glVertex3f(player_x + pfx[0], player_y + pfy[0], player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i][8], player_model_tex_info[i][11]);
            glVertex3f(player_x + pfx[4], player_y + pfy[4], player_z + pfz[4]);
            glTexCoord2f(player_model_tex_info[i][10], player_model_tex_info[i][11]);
            glVertex3f(player_x + pfx[6], player_y + pfy[6], player_z + pfz[6]);
            glTexCoord2f(player_model_tex_info[i][10], player_model_tex_info[i][9]);
            glVertex3f(player_x + pfx[2], player_y + pfy[2], player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i][12], player_model_tex_info[i][13]);
            glVertex3f(player_x + pfx[1], player_y + pfy[1], player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i][14], player_model_tex_info[i][13]);
            glVertex3f(player_x + pfx[0], player_y + pfy[0], player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i][14], player_model_tex_info[i][15]);
            glVertex3f(player_x + pfx[4], player_y + pfy[4], player_z + pfz[4]);
            glTexCoord2f(player_model_tex_info[i][12], player_model_tex_info[i][15]);
            glVertex3f(player_x + pfx[5], player_y + pfy[5], player_z + pfz[5]);
            glTexCoord2f(player_model_tex_info[i][16], player_model_tex_info[i][17]);
            glVertex3f(player_x + pfx[1], player_y + pfy[1], player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i][16], player_model_tex_info[i][19]);
            glVertex3f(player_x + pfx[0], player_y + pfy[0], player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i][18], player_model_tex_info[i][19]);
            glVertex3f(player_x + pfx[2], player_y + pfy[2], player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i][18], player_model_tex_info[i][17]);
            glVertex3f(player_x + pfx[3], player_y + pfy[3], player_z + pfz[3]);
            glTexCoord2f(player_model_tex_info[i][20], player_model_tex_info[i][21]);
            glVertex3f(player_x + pfx[1], player_y + pfy[1], player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i][20], player_model_tex_info[i][23]);
            glVertex3f(player_x + pfx[5], player_y + pfy[5], player_z + pfz[5]);
            glTexCoord2f(player_model_tex_info[i][22], player_model_tex_info[i][23]);
            glVertex3f(player_x + pfx[7], player_y + pfy[7], player_z + pfz[7]);
            glTexCoord2f(player_model_tex_info[i][22], player_model_tex_info[i][21]);
            glVertex3f(player_x + pfx[3], player_y + pfy[3], player_z + pfz[3]);
        }
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                px[j] = player_model_pos_info[i + 1][j % 2];
                pz[j] = player_model_pos_info[i + 1][4 + j / 2];
                pfx[j] = px[j] * cos(xz_rad+arm_rad*0.1) - pz[j] * sin(xz_rad+arm_rad*0.1);
                pfz[j] = px[j] * sin(xz_rad+arm_rad*0.1) + pz[j] * cos(xz_rad+arm_rad*0.1);
            }
            py1 = player_model_pos_info[i + 1][2];
            py2 = player_model_pos_info[i + 1][3];
            glTexCoord2f(player_model_tex_info[i+4][0], player_model_tex_info[i+4][3]);
            glVertex3f(player_x + pfx[3], player_y + py1, player_z + pfz[3]);
            glTexCoord2f(player_model_tex_info[i+4][2], player_model_tex_info[i+4][3]);
            glVertex3f(player_x + pfx[2], player_y + py1, player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i+4][2], player_model_tex_info[i+4][1]);
            glVertex3f(player_x + pfx[2], player_y + py2, player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i+4][0], player_model_tex_info[i+4][1]);
            glVertex3f(player_x + pfx[3], player_y + py2, player_z + pfz[3]);
            glTexCoord2f(player_model_tex_info[i+4][4], player_model_tex_info[i+4][5]);
            glVertex3f(player_x + pfx[1], player_y + py2, player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i+4][4], player_model_tex_info[i+4][7]);
            glVertex3f(player_x + pfx[0], player_y + py2, player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i+4][6], player_model_tex_info[i+4][7]);
            glVertex3f(player_x + pfx[2], player_y + py2, player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i+4][6], player_model_tex_info[i+4][5]);
            glVertex3f(player_x + pfx[3], player_y + py2, player_z + pfz[3]);
            glTexCoord2f(player_model_tex_info[i+4][8], player_model_tex_info[i+4][11]);
            glVertex3f(player_x + pfx[0], player_y + py1, player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i+4][8], player_model_tex_info[i+4][9]);
            glVertex3f(player_x + pfx[0], player_y + py2, player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i+4][10], player_model_tex_info[i+4][9]);
            glVertex3f(player_x + pfx[2], player_y + py2, player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i+4][10], player_model_tex_info[i+4][11]);
            glVertex3f(player_x + pfx[2], player_y + py1, player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i+4][12], player_model_tex_info[i+4][15]);
            glVertex3f(player_x + pfx[1], player_y + py1, player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i+4][14], player_model_tex_info[i+4][15]);
            glVertex3f(player_x + pfx[0], player_y + py1, player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i+4][14], player_model_tex_info[i+4][13]);
            glVertex3f(player_x + pfx[0], player_y + py2, player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i+4][12], player_model_tex_info[i+4][13]);
            glVertex3f(player_x + pfx[1], player_y + py2, player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i+4][18], player_model_tex_info[i+4][19]);
            glVertex3f(player_x + pfx[1], player_y + py1, player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i+4][18], player_model_tex_info[i+4][17]);
            glVertex3f(player_x + pfx[0], player_y + py1, player_z + pfz[0]);
            glTexCoord2f(player_model_tex_info[i+4][16], player_model_tex_info[i+4][17]);
            glVertex3f(player_x + pfx[2], player_y + py1, player_z + pfz[2]);
            glTexCoord2f(player_model_tex_info[i+4][16], player_model_tex_info[i+4][19]);
            glVertex3f(player_x + pfx[3], player_y + py1, player_z + pfz[3]);
            glTexCoord2f(player_model_tex_info[i+4][20], player_model_tex_info[i+4][23]);
            glVertex3f(player_x + pfx[1], player_y + py1, player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i+4][20], player_model_tex_info[i+4][21]);
            glVertex3f(player_x + pfx[1], player_y + py2, player_z + pfz[1]);
            glTexCoord2f(player_model_tex_info[i+4][22], player_model_tex_info[i+4][21]);
            glVertex3f(player_x + pfx[3], player_y + py2, player_z + pfz[3]);
            glTexCoord2f(player_model_tex_info[i+4][22], player_model_tex_info[i+4][23]);
            glVertex3f(player_x + pfx[3], player_y + py1, player_z + pfz[3]);
        }
    }
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
    FILE *f = fopen("data/player_bag.dat", "wb");
    fwrite(bag, sizeof(int), 108, f);
    fclose(f);
}

void LoadItem(int *bag)
{
    FILE *f = fopen("data/player_bag.dat", "rb");
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

void LoadRecipe22(int *bag)
{
    FILE *f = fopen("data/recipe22.dat", "rb");
    if (f == NULL)
        for (int i = 0; i < recipe22_num*7; i++)
            *(recipe22 + i) = -1;
    else
        fread(recipe22, sizeof(int), recipe22_num*7, f);
    fclose(f);
}

void LoadRecipe33(int *bag)
{
    FILE *f = fopen("data/recipe33.dat", "rb");
    if (f == NULL)
        for (int i = 0; i < recipe33_num*12; i++)
            *(recipe33 + i) = -1;
    else
        fread(recipe33, sizeof(int), recipe33_num*12, f);
    fclose(f);
}

void SaveStatus(int *status)
{
    FILE *f = fopen("data/player_status.dat", "wb");
    fwrite(status, sizeof(int), 2, f);
    fclose(f);
}

void LoadStatus(int *status)
{
    FILE *f = fopen("data/player_status.dat", "rb");
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

void DrawRect(int xs, int xe, int ys, int ye)
{
    glVertex2i(xs, ys);
    glVertex2i(xe, ys);
    glVertex2i(xe, ye);
    glVertex2i(xs, ye);
}

void DrawTexRect(int xs, int xe, int ys, int ye, double tex_pu, double tex_pd, double tex_pr, double tex_pl)
{
    glTexCoord2f(tex_pr, tex_pu);
    glVertex2i(xs, ys);
    glTexCoord2f(tex_pl, tex_pu);
    glVertex2i(xe, ys);
    glTexCoord2f(tex_pl, tex_pd);
    glVertex2i(xe, ye);
    glTexCoord2f(tex_pr, tex_pd);
    glVertex2i(xs, ye);
}

void ClickItemBag(int click_state)
{
    if (dragging_item[0] != -1)
    {
        if (*(player_bag + 3 * click_state) == dragging_item[1])
        {
            if (dragging_item[1] > 1025 && dragging_item[1] < 1041)
                SwapCursorToItemBag(click_state);
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
                SwapCursorToItemBag(click_state);
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

void SwapCursorToItemBag(int click_state)
{
    int tmp1;
    int tmp2;
    int tmp3;
    dragging_item[0] = click_state;
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