#include "./constants.h"
#include "./objects.h"

#ifndef MAP_H_
#define MAP_H_
#define CELL_WIDTH 32
#define CELL_HEIGHT 32

enum texture_t
{
  ColumnRed = 2,
  ColumnGreen = 3,
  ColumnBlue = 4,
  ColumnWhite = 1,
  ColumnBlack = 0,
};

typedef struct cell_t {
  vec2_i position;
  enum texture_t tex;
} cell;

typedef struct map_t
{
  int columns;
  int rows;
  cell** cells; 
} map;

void map_init(char* path, map* map);
int map_demo(map* map);
void map_render_player(go_mov* player);
rect_ll* map_get_rects(map* map);
void map_render(map* map);
void map_destroy(map* map);

#endif
