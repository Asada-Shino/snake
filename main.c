/*
 * @Author       : KnightZJ
 * @LastEditTime : 2022-01-26 19:37:00
 * @LastEditors  : KnightZJ
 * @Description  : main file
 */

#include "snake.h"
#include <stdio.h>

int main() {
    board b;
    snake s1, s2;
    init_board(&b, 32, 8);
    init_snake(&s1, &b, 1, 1, UP);
    init_snake(&s2, &b, 30, 6, DOWN);
    print_board(&b);
    while(1) {
        if(b.food_cnt == 0)
            add_food(&b);
        char ch = getchar();
        Status status;
        switch(ch) {
            case 'w': status = move(&b, &s1, UP); break;
            case 'a': status = move(&b, &s1, LEFT); break;
            case 's': status = move(&b, &s1, DOWN); break;
            case 'd': status = move(&b, &s1, RIGHT); break;
            case 'i': status = move(&b, &s2, UP); break;
            case 'j': status = move(&b, &s2, LEFT); break;
            case 'k': status = move(&b, &s2, DOWN); break;
            case 'l': status = move(&b, &s2, RIGHT); break;
        }
        print_board(&b);
        printf("%d\n", status);
    }
    release_snake(&s1);
    release_snake(&s2);
    release_board(&b);
    return 0;
}
