#ifndef _SNAKE_H_
#define _SNAKE_H_

// snake's direction
// 朝向枚举
enum Direction {
    Left,
    Right,
    Up,
    Down,
};

// cell type
// 棋盘结点类型枚举
enum CellType {
    None,
    Body,
    Food
};

// snake body definition
// 蛇身节点定义
struct body {
    int x;
    int y;
    Direction dir;
    struct body* prior;
    struct body* next;
};

// snake definition
// 整条蛇定义
struct snake {
    int length;
    struct body* head;
    struct body* tail;
};

// board node
// 棋盘结点
struct cell {
    CellType type;
    int reserved;
};

// 棋盘
struct board {
    int width;
    int height;
    cell** cells;
};


#endif