/*** 
 * @Author       : KnightZJ
 * @LastEditTime : 2022-01-26 19:07:03
 * @LastEditors  : KnightZJ
 * @Description  : snake header file
 */

#ifndef _SNAKE_H_
#define _SNAKE_H_

/********* Definitions *********/

// snake's direction
// 朝向枚举
typedef enum {
    LEFT,
    UP,
    DOWN,
    RIGHT,
} Direction;

// cell type
// 棋盘结点类型枚举
typedef enum {
    NONE,
    SNAKE,
    FOOD,
    WALL
} CellType;

typedef enum {
    HEAD,
    TAIL,
    BODY
} SnakeType;

// game status
// 行为状态
typedef enum {
    FAILED = -1,
    OK,
    INVALID_MOVEMENT,
    DEAD
} Status;

// snake body definition
// 蛇身节点定义
typedef struct body {
    int x;
    int y;
    Direction dir;
    struct body* prior;
    struct body* next;
} body;

// snake definition
// 整条蛇定义
typedef struct {
    int length;
    int id;
    body* head;
    body* tail;
} snake;

// board node
// 棋盘结点
typedef struct{
    CellType cell_type;
    SnakeType snake_type;
    int snake_id;
} cell;

// 棋盘
typedef struct {
    int width;
    int height;
    int area;
    int food_cnt;
    cell* cells;
} board;

/******** Functions *********/

/*** 
 * @description: init the board
 * @param      {board*} bd
 * @param      {int} width  NOTICE: width > 2, including boundary
 * @param      {int} height NOTICE: Height > 2, including boundary
 * @return     {STATUS} 
 */
Status init_board(board* b, int width, int height);

/*** 
 * @description: inti snake with given board and position
 * @param      {snake*} s
 * @param      {board*} b
 * @param      {int} x
 * @param      {int} y
 * @param      {Direction} dir is the direction of the snake's head
 * @return     {STATUS}
 */
Status init_snake(snake* s, board* b, int x, int y, Direction dir);

/*** 
 * @description: get b[x][y]'s cell
 * @param      {board*} b
 * @param      {int} x
 * @param      {int} y
 * @return     {cell*} the requested cell
 */
cell* at(board* b, int x, int y);

/*** 
 * @description: get the id th element of b->cells
 * @param      {board*} b
 * @param      {int} x
 * @param      {int} y
 * @return     {int}
 */
cell* index(board* b, int id);

/*** 
 * @description: add food to the board
 * @param      {board*} b
 * @return     {STATUS}
 */
Status add_food(board* b);

/*** 
 * @description: 
 * @param      {board*} b
 * @param      {snake*} s
 * @param      {Direction} dir
 * @return     {Status} return INVALID_MOVEMENT or DEAD if the snake hit a snake or wall
 */
Status move(board* b, snake* s, Direction dir);


/*** 
 * @description: print the whole board
 * @param      {board*} b
 * @return     {void}
 */
void print_board(board* b);

void release_snake(snake* s);

void release_board(board* b);

#endif