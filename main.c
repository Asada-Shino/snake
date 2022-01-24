#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 30
#define HEIGHT 15

#define EMPTY ' '
#define BODY '#'
#define HEAD '@'
#define FOOD '$'

#define INVALID -1
#define DEAD -2
#define OK 0

struct NODE{
    int x;
    int y;
    struct NODE* last;
    struct NODE* next;
};

typedef struct NODE node;

char maze[HEIGHT][WIDTH];
int length;
int food_cnt;
node* head, *tail;

int add_food() {
    int area = WIDTH * HEIGHT - length - food_cnt;
    if(area == 0)
        return 0;
    int i = rand()%area, x = 0, y = 0;
    printf("%d\n", i);
    while(1) {
        if(maze[y][x] == EMPTY) {
            if(--i < 0)
                break;
        }
        ++x;
        if(x >= WIDTH) {
            x = 0;
            ++y;
        }
    }
    maze[y][x] = FOOD;
    ++food_cnt;
    return area;
}

void print_maze() {
    system("clear"); 
    printf("|");
    for(int j = 0; j < WIDTH; ++j) {
        printf("-");
    }
    printf("|\n");
    for(int i = HEIGHT - 1; i >= 0; --i) {
        printf("|");
        for(int j = 0; j < WIDTH; ++j) {
            printf("%c", maze[i][j]);
        }
        printf("|\n");
    }
    printf("|");
    for(int j = 0; j < WIDTH; ++j) {
        printf("-");
    }
    printf("|\n");
}

void update_board(int is_to_add_food) {
    for(int i = HEIGHT - 1; i >= 0; --i) {
        for(int j = 0; j < WIDTH; ++j) {
            if(is_to_add_food || maze[i][j] != FOOD)
                maze[i][j] = EMPTY;
        }
    }
    maze[head->y][head->x] = HEAD;
    node* body = head->next;
    while(body != NULL) {
        maze[body->y][body->x] = BODY;
        body = body->next;
    }
    if(is_to_add_food)
        add_food();
}

int update() {
    char ch = getchar();
    int x = head->x, y = head->y, is_to_add_food = 0;
    switch(ch) {
        case 'w':
            y += 1;
            break;
        case 'a':
            x -= 1;
            break;
        case 's':
            y -= 1;
            break;
        case 'd':
            x += 1;
            break;
        default:
            return INVALID;
    }
    if(x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return DEAD;
    if(maze[y][x] == FOOD) {
        node* new_head = malloc(sizeof(node));
        new_head->next = head;
        head->last = new_head;
        head = new_head;
        head->x = x;
        head->y = y;
        is_to_add_food = 1;
        --food_cnt;
    }
    else if(head != tail){
        node* temp = tail->last;
        tail->last->next = NULL;
        tail->last = NULL;
        tail->next = head;
        head->last = tail;
        head = tail;
        tail = temp;
        head->x = x;
        head->y = y;
    }
    else {
        head->x = x;
        head->y = y;
    }
    node* body = head->next;
    while(body != NULL) {
        if(head->x == body->x && head->y == body->y)
            return DEAD;
        body = body->next;
    }
    update_board(is_to_add_food);
    return OK;
}

int main() {
    srand(time(0));
    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) {
            maze[i][j] = EMPTY;
        }
    }
    maze[0][0] = HEAD;
    head = malloc(sizeof(node));
    head->x = head->y = 0;
    head->last = head->next = NULL;
    tail = head;
    length = 1;
    add_food();
    while(update() != DEAD) {
        print_maze();
    }
    printf("DEAD!\n");
    return 0;
}