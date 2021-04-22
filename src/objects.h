#include "./constants.h"

#ifndef OBJECTS_H_
#define OBJECTS_H_
typedef struct color_t {
  byte r, g, b , a;
} color;

typedef struct rect_t {
	float x;
	float y;
	float width;
	float height;
} rect;

typedef struct line_t {
  float x1, y1, x2, y2;
} line;

typedef struct vec2_t {
  float x;
  float y;
} vec2;

typedef struct vec2_it {
  int x;
  int y;
} vec2_i;

void vec2_add (vec2* a, vec2* b);
void vec2_subs (vec2* a, vec2* b);
void vec2_mul (vec2* a, vec2* b);
void vec2_div (vec2* a, vec2* b);

typedef struct gameobject_movable_t {
  int direction;
  int mv_step;
  vec2 position;
  float rotation;
} go_mov;

void go_mov_init(go_mov* instance);

#endif
