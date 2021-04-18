typedef struct rect_t {
	float x;
	float y;
	float width;
	float height;
} rect;

typedef rect ball;

typedef struct vec2_t {
  float x;
  float y;
} vec2;

void ball_move(vec2* vel);

void vec2_add (vec2* a, vec2* b);
void vec2_subs (vec2* a, vec2* b);
void vec2_mul (vec2* a, vec2* b);
void vec2_div (vec2* a, vec2* b);

