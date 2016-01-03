//
// Created by xuzhi on 16-1-3.
//

#ifndef EAT_SNAKE_MAIN_H
#define EAT_SNAKE_MAIN_H
#include "cJSON.h"


void draw();

void menu();

void dead();

void Play();

void delayed(unsigned ms);

void difficult();

void get_random_position_s(int8_t *x, int8_t *y, int8_t n);

void get_random_position(int8_t *x, int8_t *y);

void Save();

void Pause();

void Load();

void Rank();

void Record();

void change_map(int maps) ;

void next_level();

void pass();

void Key_re();

void Judge();

struct timespec delay;
struct timespec dummy;

typedef struct snake {
    int8_t x;
    int8_t y;
    struct snake *previous;
    struct snake *next;
} snake;

//basic snake pointer var
snake *head, *tail;


//food position
int8_t food_x, food_y;

char map[30][30];
int8_t poison[2][10];

int8_t bomb_x, bomb_y;

int8_t game_on = 1, level = 1, score = 0, timer = 1, dir = 4, food = 0, bomb = 0, gift, node = 1,load=0;

char *diff;
#endif //EAT_SNAKE_MAIN_H
