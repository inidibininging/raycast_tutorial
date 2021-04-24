#include <stdio.h>
#include "./constants.h"
#include "./objects.h"

void go_mov_init(go_mov* obj) {
  obj->direction = 0;
  obj->mv_step = 3;
  vec2 position;
  position.x = MAP_CENTER_X;
  position.y = MAP_CENTER_Y;
  obj->position = position; 
}
