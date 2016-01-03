#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "main.h"

/** snake operation**/
void create_snake(char x, char y) {
    snake *temp;
    temp = (snake *) malloc(sizeof(snake));
    if (!temp) {
        printf("memory error\n");
        return;
    }

    head = temp;
    tail = temp;

    temp->x = x;
    temp->y = y;
}

void move_snake(int mode) {
    snake *temp, *p1;
    int8_t x, y;
    temp = (snake *) malloc(sizeof(snake));
    if (!temp) {
        printf("memory error\n");
        return;
    }

    x = head->x;
    y = head->y;
    switch (mode) {
        case 1:
            temp->x = x;
            temp->y = y - 1;
            break;
        case 2:
            temp->x = head->x;
            temp->y = y + 1;
            break;
        case 3:
            temp->x = x - 1;
            temp->y = y;
            break;
        case 4:
            temp->x = x + 1;
            temp->y = y;
            break;
        default:
            break;
    }

    temp->next = head;
    head->previous = temp;

    p1 = tail->previous;
    head = temp;
    free(tail);
    tail = p1;
}

/** eat operation **/

void eat_food() {
    snake *temp;
    temp = (snake *) malloc(sizeof(snake));
    if (!temp) {
        printf("memory error\n");
        return;
    }
    temp->x = food_x;
    temp->y = food_y;

    head->previous = temp;
    temp->next = head;

    head = temp;
}

void eat_poison() {
    snake *p;
    p = tail;
    tail = tail->previous;
    free(p);
}

void eat_bomb() {
    snake *p1, *p2, *p3;
    p1 = head;
    p2 = tail;
    while (p1 != p2) {
        p1 = p1->next;
        if (p1 == p2)
            break;
        p2 = p2->previous;
    }
    p3 = tail;
    tail = p1->previous;
    while (p1 != p3) {
        p1 = p1->next;
        free(p1->previous);
    }
}


/** random get **/
void get_food(int8_t *x, int8_t *y) {
    srand((unsigned) time(NULL) + 2); /*随机种子*/
    do {
        food_x = rand() % 29;
        food_y = rand() % 29;
    }
    while (map[food_y][food_x] == '#');

    *x = food_x;
    *y = food_y;
}

void get_random_position(int8_t *x, int8_t *y) {
    srand((unsigned) time(NULL)); /*随机种子*/
    do {
        *x = rand() % 29;
        *y = rand() % 29;
    }
    while (map[*y][*x] == '#');
}

void get_random_position_s(int8_t *x, int8_t *y, int8_t n) {
    srand((unsigned) time(NULL) + n * 3); /*随机种子*/
    do {
        *x = rand() % 29;
        *y = rand() % 29;
    }
    while (map[*y][*x] == '#');
}

void init() {
    food=0;
    bomb=0;
    dir=4;
    game_on=1;
    node=1;
    change_map(level);
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);

}

int main(void) {
    int i;
    init();
    menu();
    Play();
    endwin();
}

void draw() {
    int i, j;
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_CYAN);
    attron(COLOR_PAIR(1));
    for (i = 0; i < 30; i++)
        for (j = 0; j < 30; j++)
            if (map[i][j] == '#')
                mvprintw(i, j, "#");
    attroff(COLOR_PAIR(1));
    refresh();
}

void menu() {
    int location = 3;
    int ch2=0, which = 0;
    char str[4][5] = {"Play", "Load", "Rank", "Exit"};
    while (ch2 != '\n') {

        switch (ch2) {
            case (KEY_UP):
                if (location <= 9 && location > 3) {
                    location -= 2;
                    which--;
                }
                break;
            case (KEY_DOWN):
                if (location < 9 && location >= 3) {
                    location += 2;
                    which++;
                }
                break;
            default:
                break;
        }

        mvprintw(1, 1, "Smart Snake Game");
        mvprintw(3, 1, "Play");
        mvprintw(5, 1, "Load");
        mvprintw(7, 1, "Rank");
        mvprintw(9, 1, "Exit");
        attron(A_REVERSE);
        mvprintw(location, 1, "%s", str[which]);
        attroff(A_REVERSE);
        refresh();
        ch2 = getch();
    }
    switch (which) {
        case 0:
            score=0;
            return;
        case 1:
            Load();
            break;
        case 2:
            Rank();
            break;
        case 3:
            endwin();
            exit(1);
        default:
            break;
    }
}

void Rank() {
    int i = 3;
    FILE *fp;
    char *s;
    fp = fopen("rank.snake", "r");
    if (fp == NULL) {
        printf("file error");
        return;
    }
    fseek(fp, 0, SEEK_END);
    unsigned len = ftell(fp);
    s = (char *) malloc(len + 1);
    rewind(fp);
    fread(s, 1, len, fp);
    s[len] = 0;


        clear();
        mvprintw(1, 1, "RANK");
            mvprintw(i, 1, "%s", s);
            attron(A_REVERSE);
    while (getch() != '\n') {
        mvprintw(1, 10, "EXIT");
        attroff(A_REVERSE);
        refresh();
    }
    menu();
}

void Judge(){
    snake *p;
    int i=0,j;
    if (food_x == (head->x) && food_y == (head->y)) {
        food = 0;
        eat_food();
        score += gift;
    }
    if (food == 0) {
        get_food(&food_x, &food_y);
        food = 1;
    }

    if (map[head->y][head->x] == '#') {
        dead();
    }
    if (bomb == 0) {
        get_random_position(&bomb_x, &bomb_y);
        bomb = 1;
    }
    if (bomb_x == (head->x) && bomb_y == (head->y)) {
        bomb = 0;
        eat_bomb();
        score -= 50;
    }
    p = head;
    while (p != NULL) {
        if (p != head && p != head->next && p->x == head->x && p->y == head->y)
            dead();
        if (p == tail)
            break;
        p = p->next;

    }

    j = 0;
    while (j < level * 2) {
        if (head->x == poison[0][j] && head->y == poison[1][j]) {
            eat_poison();
            while (j < level * 3) {
                get_random_position_s(&poison[0][j], &poison[1][j], i + j);
                j++;
                i+=4;
            }
            score -= 20;
            break;
        }
        j++;
    }
}

void Play() {
    int j;
    unsigned i = 9999;
    snake *p1;
    if(load==1)
        load=0;
    else {
        create_snake(5, 5);
    }
    difficult();
    while (game_on) {
        clear();
        draw();
        Judge();
        attron(A_REVERSE);
        init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
        attron(COLOR_PAIR(3));
        mvprintw(food_y, food_x, "^");
        attroff(COLOR_PAIR(3));
        mvprintw(0, 30, "score:");
        init_pair(4, COLOR_RED, COLOR_RED);
        attron(COLOR_PAIR(4));
        mvprintw(bomb_y, bomb_x, "@");
        attroff(COLOR_PAIR(4));
        move(0, 37);
        printw("%d", score);
        mvprintw(1, 30, "DIFFICULT:%s", diff);
        mvprintw(2, 30, "LEVEL:%d", level);
        mvprintw(3, 30, "NODE:%d", node);
        attroff(A_REVERSE);
        i++;
        j = 0;
        init_pair(5, COLOR_BLACK, COLOR_GREEN);

        attron(COLOR_PAIR(5));
        if (i % 10000 != 0) {
            if (i % 3 == 0)
                while (j < level * 3) {
                    mvprintw(poison[1][j], poison[0][j], "W");
                    j++;
                }

        }
        else {
            while (j < level * 3) {
                get_random_position_s(&poison[0][j], &poison[1][j], i + j);
                j++;
            }
            i = 0;
        }
        attroff(COLOR_PAIR(5));

        init_pair(2, COLOR_BLUE, COLOR_BLUE);
        attron(COLOR_PAIR(2));
        node = 0;
        p1=head;
        while (p1 != NULL) {
            mvprintw(p1->y, p1->x, "O");
            node++;
            if (tail == p1)
                break;
            p1 = p1->next;
        }
        attroff(COLOR_PAIR(2));
        refresh();
        Key_re();
        nanosleep(&delay, &dummy);
        if (game_on == 0)
            break;
        if(node==6){
            next_level();
        }
    }
}

void Key_re(){
    int ch;
    ch = getch();
    switch (ch) {
        case KEY_UP:
            if (dir != 2)
                dir = 1;
            beep();
            break;
        case KEY_DOWN:
            if (dir != 1)
                dir = 2;
            break;
        case KEY_LEFT:
            if (dir != 4)
                dir = 3;
            break;
        case KEY_RIGHT:
            if (dir != 3)
                dir = 4;
            break;
        case 27:
            Pause();
        default:
            break;
    }
    move_snake(dir);
};

void next_level(){
    if(level==3){
        pass();
    }else{
        level++;
        change_map(level);
        Play();
    }
}

void pass(){
    int location = 3;
    int ch=0, which = 0;
    Record();
    char str[2][10] = {"EXIT", "Retry"};
    score = 0;
    get_food(&food_x, &food_y);
    while (ch != '\n') {
        switch (ch) {
            case (KEY_UP):
                if (location <= 5 && location > 3) {
                    location -= 2;
                    which--;
                }
                break;
            case (KEY_DOWN):
                if (location < 5 && location >= 3) {
                    location += 2;
                    which++;
                }
                break;
            default:
                break;
        }
        mvprintw(1, 1, "YOU WIN");
        mvprintw(3, 1, "EXIT");
        mvprintw(5, 1, "Retry");
        attron(A_REVERSE);
        mvprintw(location, 1, "%s", str[which]);
        attroff(A_REVERSE);
        refresh();
        ch = getch();
    }
    switch (which) {
        case 0:
            endwin();
            exit(1);
        case 1:
            Play();
            break;
        default:
            break;
    }

}

void dead() {
    int location = 3;
    int ch=0, which = 0;
    Record();
    char str[2][10] = {"EXIT", "Retry"};
    score = 0;
    get_food(&food_x, &food_y);
    while (ch != '\n') {
        switch (ch) {
            case (KEY_UP):
                if (location <= 5 && location > 3) {
                    location -= 2;
                    which--;
                }
                break;
            case (KEY_DOWN):
                if (location < 5 && location >= 3) {
                    location += 2;
                    which++;
                }
                break;
            default:
                break;
        }

        mvprintw(1, 1, "YOU LOSE");
        mvprintw(3, 1, "EXIT");
        mvprintw(5, 1, "Retry");
        attron(A_REVERSE);
        mvprintw(location, 1, "%s", str[which]);
        attroff(A_REVERSE);
        refresh();
        ch = getch();
    }
    switch (which) {
        case 0:
            endwin();
            exit(1);
        case 1:
            Play();
            break;
        default:
            break;
    }

}

void Pause() {
    int location = 3;
    int ch=0, which = 0;
    char str[3][10] = {"BACK", "SAVE", "EXIT"};
    get_food(&food_x, &food_y);
    while (ch != '\n') {
        switch (ch) {
            case (KEY_UP):
                if (location <= 7 && location > 3) {
                    location -= 2;
                    which--;
                }
                break;
            case (KEY_DOWN):
                if (location < 7 && location >= 3) {
                    location += 2;
                    which++;
                }
                break;
            default:
                break;
        }

        mvprintw(1, 1, "What do you want to do?");
        mvprintw(3, 1, "BACK");
        mvprintw(5, 1, "SAVE");
        mvprintw(7, 1, "EXIT");
        attron(A_REVERSE);
        mvprintw(location, 1, "%s", str[which]);
        attroff(A_REVERSE);
        refresh();
        ch = getch();
    }
    switch (which) {
        case 0:
            break;
        case 1:
            Save();
            break;
        case 2:
            menu();
        default:
            break;
    }

}

void Save() {
    FILE *fp;
    char *s;
    cJSON *root, *p1, *p2;
    root = cJSON_CreateObject();
    snake *pa;
    cJSON_AddNumberToObject(root, "score", score);
    cJSON_AddNumberToObject(root, "level", level);
    cJSON_AddNumberToObject(root, "dir", dir);
    p1 = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "position", p1);
    pa = head;
    while (pa != NULL) {
        p2 = cJSON_CreateObject();
        cJSON_AddNumberToObject(p2, "x", pa->x);
        cJSON_AddNumberToObject(p2, "y", pa->y);
        if (pa == tail)
            break;
        pa = pa->next;
        cJSON_AddItemToArray(p1, p2);
    }
    s = cJSON_Print(root);
    cJSON_Delete(root);
    fp = fopen("dave.snake", "w");
    fprintf(fp, "%s", s);
    fclose(fp);
}

void Load() {
    int i, max;
    FILE *fp;
    char *s;
    cJSON *root, *p1, *p2, *p3;
    snake *temp, *pa;
    fp = fopen("dave.snake", "r");
    if (fp == NULL) {
        printf("file error");
        return;
    }
    fseek(fp, 0, SEEK_END);
    unsigned len = ftell(fp);
    s = (char *) malloc(len + 1);
    rewind(fp);
    fread(s, 1, len, fp);
    s[len] = 0;
    root = cJSON_Parse(s);
    p1 = cJSON_GetObjectItem(root, "score");
    score = p1->valueint;
    p1 = cJSON_GetObjectItem(root, "level");
    level = p1->valueint;
    p1 = cJSON_GetObjectItem(root, "dir");
    dir = p1->valueint;
    head = NULL;
    pa = NULL;
    p1 = cJSON_GetObjectItem(root, "position");
    max = cJSON_GetArraySize(p1);
    for (i = 0; i < max; i++) {
        temp = (snake *) malloc(sizeof(snake));
        p2 = cJSON_GetArrayItem(p1, i);
        p3 = cJSON_GetObjectItem(p2, "x");
        temp->x = p3->valueint;
        p3 = cJSON_GetObjectItem(p2, "y");
        temp->y = p3->valueint;
        if (head == NULL)
            head = temp;
        if (pa != NULL) {
            temp->previous = pa;
            pa->next = temp;
        }
        pa = temp;
        tail=temp;
    }
    fclose(fp);
    load=1;
    game_on=1;
}

void change_map(int maps) {
    switch (maps) {
        case 1:
            strcpy(map[0],"##############################");
            strcpy(map[1],"#****************************#");
            strcpy(map[2] , "#****************************#");
            strcpy(map[3] , "#****************************#");
            strcpy(map[4] , "#****************************#");
            strcpy(map[5] , "#****************************#");
            strcpy(map[6] , "#****************************#");
            strcpy(map[7] , "#****************************#");
            strcpy(map[8] , "#****************************#");
            strcpy(map[9] , "#****************************#");
            strcpy(map[10] , "#****************************#");
            strcpy(map[11] , "#****************************#");
            strcpy(map[12] , "#****************************#");
            strcpy(map[13] , "#****************************#");
            strcpy(map[14] , "#****************************#");
            strcpy(map[15] , "#****************************#");
            strcpy(map[16] , "#****************************#");
            strcpy(map[17] , "#****************************#");
            strcpy(map[18] , "#****************************#");
            strcpy(map[19] , "#****************************#");
            strcpy(map[20] , "#****************************#");
            strcpy(map[21] , "#****************************#");
            strcpy(map[22] , "#****************************#");
            strcpy(map[23] , "#****************************#");
            strcpy(map[24] , "#****************************#");
            strcpy(map[25] , "#****************************#");
            strcpy(map[26] , "#****************************#");
            strcpy(map[27] , "#****************************#");
            strcpy(map[28] , "#****************************#");
            strcpy(map[29] , "##############################");
            break;
        case 2:
            strcpy(map[0] , "##############################");
            strcpy(map[1] , "###**********************#####");
            strcpy(map[2] , "###**********************#####");
            strcpy(map[3] , "#####**********************###");
            strcpy(map[4] , "#####**********************###");
            strcpy(map[5] , "###******************#########");
            strcpy(map[6] , "###******************#########");
            strcpy(map[7] , "#######********************###");
            strcpy(map[8] , "#######********************###");
            strcpy(map[9] , "#######************###########");
            strcpy(map[10] , "#######************###########");
            strcpy(map[11] , "###**********************#####");
            strcpy(map[12] , "###**********************#####");
            strcpy(map[13] , "#####**********************###");
            strcpy(map[14] , "#####**********************###");
            strcpy(map[15] , "###******************#########");
            strcpy(map[16] , "###******************#########");
            strcpy(map[17] , "#######********************###");
            strcpy(map[18] , "#######********************###");
            strcpy(map[19] , "#######************###########");
            strcpy(map[20] , "#######************###########");
            strcpy(map[21] , "###**********************#####");
            strcpy(map[22] , "###**********************#####");
            strcpy(map[23] , "#####**********************###");
            strcpy(map[24] , "#####**********************###");
            strcpy(map[25] , "###******************#########");
            strcpy(map[26] , "###******************#########");
            strcpy(map[27] , "#######********************###");
            strcpy(map[28] , "#######********************###");
            strcpy(map[29] , "##############################");
            break;
        case 3:
            strcpy(map[0] , "##############################");
            strcpy(map[1] , "##*********######***********##");
            strcpy(map[2] , "##*##***********************##");
            strcpy(map[3] , "##*******####*******###*****##");
            strcpy(map[4] , "##*******####*******###*****##");
            strcpy(map[5] , "##*******####*******###*****##");
            strcpy(map[6] , "##*******####*******###*****##");
            strcpy(map[7] , "##*******####*******###*****##");
            strcpy(map[8] , "##**************************##");
            strcpy(map[9] , "############************######");
            strcpy(map[10] , "#######**************#########");
            strcpy(map[11] , "#####**********#######******##");
            strcpy(map[12] , "#####******########*********##");
            strcpy(map[13] , "#####******####***********####");
            strcpy(map[14] , "######***********#############");
            strcpy(map[15] , "#######*********##############");
            strcpy(map[16] , "####*********#################");
            strcpy(map[17] , "####*****###***********#######");
            strcpy(map[18] , "####***********###############");
            strcpy(map[19] , "####**#*****************######");
            strcpy(map[20] , "###**************#############");
            strcpy(map[21] , "###******####**************###");
            strcpy(map[22] , "###************************###");
            strcpy(map[23] , "###************************###");
            strcpy(map[24] , "######***********#############");
            strcpy(map[25] , "#######*********##############");
            strcpy(map[26] , "####*********#################");
            strcpy(map[27] , "####*****###***********#######");
            strcpy(map[28] , "####***********####****#######");
            strcpy(map[29] , "##############################");
            break;
        default:
            break;
    }
};

void Record() {
    FILE *fp;
    char *s;
    int max,num[255],i,j,flag=0;
    cJSON *root,*layer1,*layer2;
    fp = fopen("rank.snake", "r");
    if (fp == NULL) {
        printf("file error");
        return;
    }
    fseek(fp, 0, SEEK_END);
    unsigned len = ftell(fp);
    s = (char *) malloc(len + 1);
    rewind(fp);
    fread(s, 1, len, fp);
    s[len] = 0;
    root=cJSON_Parse(s);
    if(root==NULL) {
        root = cJSON_CreateArray();
        layer1 = cJSON_CreateNumber(score);
        cJSON_AddItemToArray(root, layer1);
        s=cJSON_Print(root);
    }else {
        max = cJSON_GetArraySize(root);
        i=0;
        j=0;
        while (i<= max) {
            layer1 = cJSON_GetArrayItem(root, j);
            if (score > layer1->valueint&&flag==0) {
                num[i] = score;
                i++;
                num[i]=layer1->valueint;
                j++;
                i++;
                flag=1;
            } else {
                num[i] = layer1->valueint;
                i++;
                j++;
            }
        }
        fclose(fp);
        fp = fopen("rank.snake", "w");
        layer2=cJSON_CreateIntArray(num,max+1);
        s=cJSON_Print(layer2);
    }
    fprintf(fp,"%s",s);
    fclose(fp);
}

void difficult() {
    int location = 3;
    int ch=0, which = 0;
    char str[3][10] = {"EASY", "NORMAL", "HARD"};
    clear();
    get_food(&food_x, &food_y);
    while (ch != '\n') {
        ch = getch();
        switch (ch) {
            case (KEY_UP):
                if (location <= 7 && location > 3) {
                    location -= 2;
                    which--;
                }
                break;
            case (KEY_DOWN):
                if (location < 7 && location >= 3) {
                    location += 2;
                    which++;
                }
                break;
            default:
                break;
        }

        mvprintw(1, 1, "CHOOSE DIFFICULT");
        mvprintw(3, 1, "EASY");
        mvprintw(5, 1, "NORMAL");
        mvprintw(7, 1, "HARD");
        attron(A_REVERSE);
        mvprintw(location, 1, "%s", str[which]);
        attroff(A_REVERSE);
        refresh();
    }
    switch (which) {
        case 0:
            gift = 10;
            delay.tv_nsec = 200000000;
            diff = "EASY";
            break;
        case 1:
            gift = 20;
            delay.tv_nsec = 150000000;
            diff = "NORMAL";
            break;
        case 2:
            gift = 30;
            diff = "HARD";
            delay.tv_nsec = 100000000;
            break;
        default:
            break;
    }

}
