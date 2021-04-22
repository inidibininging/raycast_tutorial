#include <stdlib.h>
#include "./objects.h"
#include "./map.h"

void map_init(char* path, map* map) 
{

}

void map_destroy(map* map)
{

}

void map_demo(map* map)
{
  map->rows = MAP_WIDTH;
  map->columns = MAP_HEIGHT;
  map->cells = (cell**)malloc(sizeof(cell) * MAP_WIDTH * MAP_HEIGHT);
  for(int h = 0; h < MAP_HEIGHT; h += CELL_WIDTH)
  {
    for(int w = 0; w < MAP_WIDTH; w += CELL_HEIGHT)
    {
        
    }
  }
}

void map_render_player(go_mov* player)
{

}

void map_render_cell(cell* cell)
{

}

void map_render(map* map)
{

}
