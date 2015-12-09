#include<curses.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>


void draw();
void  menu();
void dead();
void Play();
void test();
void delayed(unsigned ms);
void difficult();
void get_random_position_s(char *x, char *y,char n);
void get_random_position(char *x, char *y);
void Save();
void Pause();
void Load();
void Rank();
void Record();

struct timespec delay;
struct timespec dummy;

typedef struct snake {
    char x;
    char y;
    struct snake *previous;
    struct snake *next;
} snake;

typedef struct rank{
    int score;
    char name[100];
    struct rank *next;
} rank;
//basic snake pointer var
snake *head, *tail;

rank *all;

//food position
char food_x, food_y;

char map[30][30]={"##############################","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","#****************************#","##############################"};

char poison[2][10];

char bomb_x, bomb_y;

int game_on=1,level=1,score=0,timer=1,dir=4,food=0,bomb=0,gift,node=1;

char *diff;



void create_snake(char x, char y) {
    snake *temp;
    temp=(snake *)malloc(sizeof(snake));
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
    snake *temp,*p1;
    char x,y;
    temp=(snake *)malloc(sizeof(snake));
    if (!temp) {
        printf("memory error\n");
        return;
    }

    x=head->x;
    y=head->y;
    switch (mode) {
        case 1:
//up
            temp->x = x;
            temp->y = y-1;
            break;
        case 2:
//down
            temp->x = head->x;
            temp->y =y+1;
            break;
        case 3:
//left
            temp->x = x- 1;
            temp->y = y;
            break;
        case 4:
//right
            temp->x = x+1;
            temp->y =y;
            break;
        default:
            break;
    }

    temp->next = head;
    head->previous = temp;

    p1=tail->previous;
    head = temp;
    free(tail);
    tail=p1;
}

void eat_food() {
    snake *temp;
    temp=(snake *)malloc(sizeof(snake));
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
    snake *p1, *p2,*p3;
    p1=head;
    p2=tail;
    while (p1 != p2) {
        p1 = p1->next;
        if (p1 == p2)
            break;
        p2 = p2->previous;
    }
    p3=tail;
    tail=p1->previous;
    while (p1 != p3) {
        p1 = p1->next;
        free(p1->previous);
    }
}

void get_food(char *x, char *y) {
    srand((unsigned) time(NULL)+2); /*随机种子*/
    do{
    food_x = rand() %29;
    food_y = rand() %29;}
    while(map[food_x][food_y]=='#');

    *x = food_x;
    *y = food_y;
}

void get_random_position(char *x, char *y) {
    srand((unsigned) time(NULL)); /*随机种子*/
    do{
    *x = rand() %29;
    *y = rand() %29;
    }
    while(map[*y][*x]=='#');
}

void get_random_position_s(char *x, char *y,char n) {
    srand((unsigned) time(NULL)+n*3); /*随机种子*/
    do{
    *x = rand() %29;
    *y = rand() %29;
    }
    while(map[*y][*x]=='#');
}

//init map
void init() {
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
    int i;
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_CYAN);
    attron(COLOR_PAIR(1));
    for (i = 0; i < 30; i++)
        mvaddch(0, i, '#');
    for (i = 0; i < 30; i++)
        mvaddch(i, 0, '#');
    for (i = 29; i >= 0; i--)
        mvaddch(29, i, '#');
    for (i = 29; i >= 0; i--)
        mvaddch(i, 29, '#'); //box(stdscr,'*','*');
    attroff(COLOR_PAIR(1));
    refresh();
}

void menu() {
    int location = 3;
    int ch2 , which = 0;
    char str[4][5] = {"Play", "Load", "Rank", "Exit"};
    while (ch2 != '\n') {

        switch (ch2) {
            case (KEY_UP):
                if (location <=9&&location>3) {
                    location -= 2;
                    which --;
                }
                break;
            case (KEY_DOWN):
                if (location <9&&location>=3) {
                    location += 2;
                    which ++;
                }
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
    switch (which){
        case 0:
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
    }
}

void load(){

}

void Rank(){
    int i=3;
    rank *p1;
    while(getch()!='\n') {
        clear();
        mvprintw(1, 1, "RANK");
        p1 = all;
        while (p1 != NULL) {
            mvprintw(i, 1, "Score:%d  NAME:%s", p1->score, p1->name);
            p1 = p1->next;
            i++;
        }
        attron(A_REVERSE);
        mvprintw(1, 10, "EXIT");
        attroff(A_REVERSE);
        refresh();
    }
    menu();
}



void Play(){
    int ch=0,j;
    unsigned i=9999;
    snake *p1,*p2;
    difficult();
    init();
    create_snake(5,5);

    while(game_on) {
        clear();
        draw();
        p1=head;
        if(food_x==(head->x)&&food_y==(head->y)) {
            food = 0;
            eat_food();
            score+=gift;
        }
        if(food==0) {
                get_food(&food_x, &food_y);
            food=1;
        }

        if(map[head->y][head->x]=='#'){
            dead();
            break;
        }
        if(bomb==0) {
            get_random_position(&bomb_x, &bomb_y);
            bomb = 1;
        }
        if(bomb_x==(head->x)&&bomb_y==(head->y)){
            bomb=0;
            eat_bomb();
            score-=50;
        }
        p2=head;
        while(p2!=NULL){
            if(p2!=head&&p2!=head->next&&p2->x==head->x&&p2->y==head->y)
                dead();
            if(p2==tail)
                break;
            p2=p2->next;

        }


        attron(A_REVERSE);
        init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
        attron(COLOR_PAIR(3));
        mvprintw(food_y, food_x, "^");
        attroff(COLOR_PAIR(3));
        mvprintw(0,30,"score:");
        init_pair(4, COLOR_RED, COLOR_RED);
        attron(COLOR_PAIR(4));
        mvprintw(bomb_y,bomb_x,"@");
        attroff(COLOR_PAIR(4));
        move(0,37);
        printw("%d",score);
        mvprintw(1,30,"DIFFICULT:%s",diff);
        mvprintw(2,30,"LEVEL:%d",level);
        mvprintw(3,30,"NODE:%d",node);
        attroff(A_REVERSE);

        i++;
        j=0;
        init_pair(5, COLOR_BLACK, COLOR_GREEN);

        attron(COLOR_PAIR(5));
        if(i%10000!=0) {
            if(i%3==0)
            while (j < level * 3) {
                mvprintw(poison[1][j], poison[0][j], "W");
                j++;
            }

        }
        else {
            while (j < level * 3) {
                get_random_position_s(&poison[0][j], &poison[1][j], i+j);
                j++;
            }
            i = 0;
        }
        attroff(COLOR_PAIR(5));
        j=0;
        while(j< level*2){
            if(head->x==poison[0][j]&&head->y==poison[1][j]) {
                eat_poison();
                while (j < level * 3) {
                    get_random_position_s(&poison[0][j], &poison[1][j], i+j);
                    j++;
                }
                score-=20;
                break;
            }
            j++;
        }



        init_pair(2, COLOR_BLUE, COLOR_BLUE);
        attron(COLOR_PAIR(2));
        node=0;
        while(p1!=NULL) {
            mvprintw(p1->y, p1->x, "O");
            node++;
            if(tail==p1)
                break;
            p1 = p1->next;
        }
        attroff(COLOR_PAIR(2));
        refresh();
        ch=getch();
        switch (ch){
            case KEY_UP:
                if(dir!=2)
                dir=1;
                beep();
                break;
            case KEY_DOWN:
                if(dir!=1)
                dir=2;
                break;
            case KEY_LEFT:
                if(dir!=4)
                dir=3;
                break;
            case KEY_RIGHT:
                if(dir!=3)
                dir=4;
                break;
            case 27:
                Pause();
            default:
                break;
        }
        move_snake(dir);
        nanosleep(&delay, &dummy);
        if(game_on==0)
            break;
    }
}

void dead(){
    int location = 3;
    int ch , which = 0;
    Record();
    char str[2][10] = {"EXIT", "Retry"};
    score=0;
    get_food(&food_x,&food_y);
    while (ch != '\n') {
        switch (ch) {
            case (KEY_UP):
                if (location <=5&&location>3) {
                    location -= 2;
                    which --;
                }
                break;
            case (KEY_DOWN):
                if (location <5&&location>=3) {
                    location += 2;
                    which ++;
                }
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
    switch (which){
        case 0:
            endwin();
            exit(1);
        case 1:
            Play();
            break;
    }

}

void Pause(){
    int location = 3;
    int ch , which = 0;
    char str[3][10] = {"BACK", "SAVE","EXIT"};
    score=0;
    get_food(&food_x,&food_y);
    while (ch != '\n') {
        switch (ch) {
            case (KEY_UP):
                if (location <=7&&location>3) {
                    location -= 2;
                    which --;
                }
                break;
            case (KEY_DOWN):
                if (location <7&&location>=3) {
                    location += 2;
                    which ++;
                }
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
    switch (which){
        case 0:
            break;
        case 1:
            Save();
            break;
        case 2:
            menu();
    }

}

void Save(){

}

void Load(){

}

void Record() {
    rank *temp, *p1, *p2;
    temp = (rank *)malloc(sizeof(rank));


    temp->score = score;
    score = 0;

    p1 = all;
    if (all == NULL)
        all = temp;
    else {
        while (temp->score < p1->score && p1->next != NULL) {
            p1 = p1->next;
        }

        if (temp->score < p1->score)
            p1->next = temp;
        else {
            p2 = all;
            while (p2->next != p1)
                p2 = p2->next;
            p2->next = temp;
            temp->next = p1;
        }

        if (p1 == all)
            all = temp;
    }
}

void difficult(){
    int location = 3;
    int ch , which = 0;
    char str[3][10] = {"EASY", "NORMAL","HARD"};
    clear();
    score=0;
    get_food(&food_x,&food_y);
    while (ch != '\n') {
        switch (ch) {
            case (KEY_UP):
                if (location <=7&&location>3) {
                    location -= 2;
                    which --;
                }
                break;
            case (KEY_DOWN):
                if (location <7&&location>=3) {
                    location += 2;
                    which ++;
                }
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
        ch = getch();
    }
    switch (which){
        case 0:
            gift=10;
            delay.tv_nsec = 200000000;
            diff="EASY";
            break;
        case 1:
            gift=20;
            delay.tv_nsec = 150000000;
            diff="NORMAL";
            break;
        case 2:
            gift=30;
            diff="HARD";
            delay.tv_nsec = 100000000;
            break;
    }

}

void test(){
    int ch;
    snake *p1;
    create_snake(5, 5);
    while(1) {
        p1=head;
        while (p1 != NULL) {
            printf("%d,%d", p1->x, p1->y);
            if (p1 == tail)
                break;
            p1 = p1->next;
        }
        printf("\n");
        //nanosleep(&delay, &dummy);
        ch=getch();
        switch (ch){
            case KEY_UP:
                dir=1;
                beep();
                break;
            case KEY_DOWN:
                dir=2;
                break;
            case KEY_LEFT:
                dir=3;
                break;
            case KEY_RIGHT:
                dir=4;
                break;
            case 27:
                exit(1);
            default:
                break;
        }
        move_snake(dir);
        delayed(10000);
    }


}

void delayed(unsigned ms){
    int i,m;
    for(i=0;i<ms;i++)
        for(m=0;m<1000;m++);
}