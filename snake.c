/*
 * @Author       : KnightZJ
 * @LastEditTime : 2022-01-26 19:35:41
 * @LastEditors  : KnightZJ
 * @Description  : snake source file
 */

#include "snake.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

Status init_board(board* b, int width, int height) {
    assert(width > 2 && height > 2 && b != NULL);
    b->cells = (cell*)malloc(sizeof(cell)*width*height);
    assert(b->cells != NULL);
    b->width = width;
    b->height = height;
    b->area = width*height;
    b->food_cnt = 0;
    for(int i=0; i < width; ++i)
        at(b, i, 0)->cell_type =  at(b, i, height-1)->cell_type = WALL;
    for(int i=0; i < height; ++i)
        at(b, 0, i)->cell_type = at(b, width-1, i)->cell_type = WALL;
    srand(time(0));
    return OK;
}

Status init_snake(snake* s, board* b, int x, int y, Direction dir) {
    assert(s != NULL && b != NULL && x > 0 && x < b->width && y > 0 && y < b->height && at(b, x, y)->cell_type == NONE);
    s->head = (body*)malloc(sizeof(body));
    assert(s->head!=NULL);
    s->head->next = s->head->prior = NULL;
    s->head->x = x;
    s->head->y = y;
    s->head->dir = dir;
    s->tail = s->head;
    s->length = 1;
    cell* dest = at(b, x, y);
    dest->cell_type = SNAKE;
    dest->snake_id = s->id;
    dest->snake_type = HEAD;
    return OK;
}

cell* at(board* b, int x, int y) {
    assert(b!=NULL && b->cells != NULL && x >= 0 && y >= 0 && x < b->width && y < b->height);
    return &b->cells[y*b->width + x];
}

cell* index(board* b, int id) {
    assert(b!=NULL && b->cells != NULL && id >= 0 && id < b->area);
    return &b->cells[id];
}

Status add_food(board* b) {
    assert(b!=NULL);
    int room = 0;
    for(int i = 0; i < b->area; ++i)
        if(index(b, i)->cell_type == NONE)
            ++room;
    if(room == 0)
        return FAILED;
    int pos = rand() % room;
    for(int i = 0; i < b->area; ++i) {
        if(index(b, i)->cell_type == NONE && --pos < 0) {
            index(b, i)->cell_type = FOOD;
            b->food_cnt += 1;
            return OK;
        }
    }
    return FAILED;
}

Status move(board* b, snake* s, Direction dir) {
    assert(b!=NULL && s!=NULL && s->head!=NULL);
    int x = s->head->x, y = s->head->y;
    if(s->head->dir + dir == 3)
        return INVALID_MOVEMENT;
    switch(dir) {
        case LEFT:  x -= 1; break;
        case RIGHT: x += 1;  break;
        case UP:    y += 1;  break;
        case DOWN:  y -= 1; break;
    }
    cell* dest = at(b, x, y);
    if(dest->cell_type == NONE) {
        at(b, s->tail->x, s->tail->y)->cell_type = NONE;
        at(b, s->head->x, s->head->y)->snake_type = BODY;
        s->tail->dir = dir;
        s->tail->x = x;
        s->tail->y = y;
        dest->cell_type = SNAKE;
        dest->snake_id = s->id;
        dest->snake_type = HEAD;
        if(s->head == s->tail)
            return OK;
        body* new_tail = s->tail->prior;
        s->tail->prior->next = NULL;
        s->tail->prior = NULL;
        s->tail->next = s->head;
        s->head->prior = s->tail;
        s->head = s->tail;
        s->tail = new_tail;
        while(new_tail->prior != NULL) {
            new_tail->dir = new_tail->prior->dir;
            new_tail = new_tail->prior;
        }
    }
    else if(dest->cell_type == FOOD) {
        body* new_head= (body*)malloc(sizeof(body));
        new_head->x = x;
        new_head->y = y;
        new_head->dir = dir;
        new_head->next = s->head;
        new_head->prior = NULL;
        at(b, s->head->x, s->head->y)->snake_type = BODY;
        dest->cell_type = SNAKE;
        dest->snake_type = HEAD;
        dest->snake_id = s->id;
        s->head->prior = new_head;
        s->head = new_head;
        b->food_cnt -= 1;
        s->length += 1;
    }
    else if(dest->cell_type == WALL)
        return DEAD;
    else if(dest->cell_type == SNAKE) {
        if(dest->snake_id != s->id)
            return DEAD;
        if(s->tail->x != x || s->tail->y != y)
            return DEAD;
        at(b, s->head->x, s->head->y)->snake_type = BODY;
        dest->snake_type = HEAD;
        s->head->prior = s->tail;
        s->tail->next = s->head;
        s->head = s->tail;
        s->tail = s->tail->prior;
        s->head->prior = NULL;
        s->tail->next = NULL;
        s->head->dir = dir;
        body* new_tail = s->tail;
        while(new_tail->prior != NULL) {
            new_tail->dir = new_tail->prior->dir;
            new_tail = new_tail->prior;
        }
    }
    return OK;
}

void print_board(board* b) {
    static char ch[] = " #$|";
    for(int i = b->height - 1; i >= 0; --i) {
        for(int j = 0; j < b->width; ++j) {
            if(at(b, j, i)->cell_type == SNAKE) {
                printf("%c", at(b, j, i)->snake_type == HEAD ? '@' : '#');
            }
            else {
                printf("%c", ch[at(b, j, i)->cell_type]);
            }
        }
        printf("\n");
    }
}

void release_snake(snake* s) {
    assert(s!=NULL);
    body* bd = s->head;
    while(bd != NULL) {
        bd = bd->next;
        free(s->head);
        s->head = bd;
    }
    s->tail = s->head;
    s->length = 0;
}

void release_board(board* b) {
    assert(b!=NULL);
    free(b->cells);
    b->food_cnt = b->width = b->height = b->area = 0;
    b->cells = NULL;
}