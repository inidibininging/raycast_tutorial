#include <stdlib.h>
#include <stdio.h>
#include "./constants.h"
#include "./objects.h"
#include "./map.h"

void map_init(char* path, map* map) 
{

}

void map_destroy(map* map)
{
  for (int y =0; y < map->rows; y++)
  {
    free(map->cells[y]);
  }
  free(map->cells);
}

rect_ll* map_get_rects(map* map)
{
  rect_ll* list = (rect_ll*)malloc(sizeof(rect_ll));
  rect_ll* head = list;
  list->next = NULL;
  list->current = NULL;
  for(int y = 0; y < map->rows; y++)
  {
    for(int x = 0; x < map->columns; x++)
    {
      if(map->cells[y][x].tex != ColumnBlack)
        continue;

      //list.next is always NULL if the node is new
      if(list->current == NULL)
      {
        list->current = (rect*)malloc(sizeof(rect));
        list->current->x = map->cells[y][x].position.x;
        list->current->y = map->cells[y][x].position.y;
        list->current->width = CELL_WIDTH;
        list->current->height = CELL_HEIGHT;
        
        list->next = (rect_ll*)malloc(sizeof(rect_ll));
        list->next->next = NULL;
        list->next->current = NULL;
        list = list->next;
      }
      
    }
  }
  list = NULL;
  return head;
}

int map_demo(map* map)
{
  map->rows = MAP_WIDTH;
  map->columns = MAP_HEIGHT;
  map->cells = (cell**)malloc(sizeof(cell) * MAP_HEIGHT);

  if(map->cells == NULL)
  {
    printf("cannot create cells for demo map. malloc fails");
    return FALSE;
  }

  for(int y = 0; y < map->rows; y++)
  {
    map->cells[y] = (cell*)malloc(sizeof(cell) * MAP_HEIGHT);
    if(map->cells[y] == NULL)
    {
      printf("cannot create cell for y: %i", y);
      return FALSE;
    }
    for(int x = 0; x < map->columns; x++)
    {
      map->cells[y][x].position.y = y * CELL_HEIGHT;
      map->cells[y][x].position.x = x * CELL_WIDTH;
      map->cells[y][x].tex = x % 3 == 0 ? ColumnWhite : ColumnBlack;
    }
  }
  return TRUE;
}

void map_render_player(go_mov* player)
{
  
}

void map_render_cell(cell* cell)
{

}

void map_render(map* map)
{
  for(int y = 0; y <= map->columns; y++)
  {
    for(int x = 0; x <= map->rows; x++)
    {
        
    }
  }
}
