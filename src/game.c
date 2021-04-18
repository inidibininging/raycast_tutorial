#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include "./objects.h"

int game_running = TRUE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

byte bg_color_rgba[4] = {
				0,
				0,
				0,
				255
			};

ball game_ball;
rect game_pad_left;
rect game_pad_right;

vec2 game_ball_vel;
vec2 game_pad_left_vel;
vec2 game_pad_right_vel;

int setup() {
	game_ball.x = WINDOW_WIDTH / 2;
	game_ball.y = WINDOW_HEIGHT / 2;
	game_ball.width = 10;
	game_ball.height = 10;

  game_ball_vel.x = -2;
  game_ball_vel.y = 0;

  game_pad_left.x = 0;
  game_pad_left.y = WINDOW_HEIGHT / 2;
  game_pad_left.width = 20;
  game_pad_left.height = 64;

  game_pad_right.x = WINDOW_WIDTH - 20;
  game_pad_right.y = WINDOW_HEIGHT / 2;
  game_pad_right.width = 20;
  game_pad_right.height = 64;


	return TRUE;
}

int initialize_window(void) {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}
	window = SDL_CreateWindow(
			NULL, 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			WINDOW_WIDTH, 
			WINDOW_HEIGHT,
			SDL_WINDOW_BORDERLESS);
	if(!window) {
		fprintf(stderr, "Error creating SDL Window\n");
		return FALSE;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	//-1 => default driver
	if(!renderer) {
		fprintf(stderr, "Error creating SDL Renderer\n");
		return FALSE;
	}

	return TRUE;
}

void destroy_window(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void process_input();
void update();
void render();

int main() {
	game_running = initialize_window();
	setup();
	while(game_running){
		process_input();
		update();
		render();		
	}

	destroy_window();
	return FALSE;
}

void apply_vel() {
  game_ball.x += game_ball_vel.x;
  game_ball.y += game_ball_vel.y;

  game_pad_left.x += game_pad_left_vel.x;
  game_pad_left.y += game_pad_left_vel.y;
  
  game_pad_right.x += game_pad_right_vel.x;
  game_pad_right.y += game_pad_right_vel.y;

}

int collides(rect* a, rect* b){

  if(a->x <= b->x && 
     a->x + a->width >= b->x &&
     a->y <= b->y && 
     a->y + a->height >= b->y)
  {
    //a intersects with b
    return TRUE;
  } 
  return FALSE;
}

void update_ball_bounce()
{
  if(game_ball.x < 0)
  {
    game_ball_vel.x *= -1;
  }
  if(game_ball.x > WINDOW_WIDTH)
  {
    game_ball_vel.x *= -1;
  }
  if(game_ball.y > WINDOW_HEIGHT)
  {
    game_ball_vel.y *= -1;
  }
  if(game_ball.y < 0)
  {
    game_ball_vel.y *= -1;
  }
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type) {
		//clicking the X button of the window
		case SDL_QUIT:
			game_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				game_running = FALSE;
      
      if(event.key.keysym.sym == SDLK_w)
      {
        game_pad_right_vel.y = -2;
      }
      if(event.key.keysym.sym == SDLK_s)
      {
        game_pad_right_vel.y = 2;
      }
      if(event.key.keysym.sym == SDLK_UP)
      {
        game_pad_left_vel.y = -2;
      }
      if(event.key.keysym.sym == SDLK_DOWN)
      {
        game_pad_left_vel.y = 2;
      }
			break;
	}
}

void blink()
{
	bg_color_rgba[BG_RED_INDEX] = bg_color_rgba[BG_RED_INDEX] - 10;
	if(bg_color_rgba[BG_RED_INDEX == 0])
		bg_color_rgba[BG_RED_INDEX] = 255;
}

void ouch_bg()
{
  bg_color_rgba[BG_RED_INDEX] = 255;
}
void no_ouch_bg()
{
  bg_color_rgba[BG_RED_INDEX] = 0;
}


void update_collision()
{
  if(collides(&game_pad_left, &game_ball) == TRUE){
    ouch_bg();
    game_ball_vel.x *= -1;
    game_ball_vel.y = game_pad_left_vel.y;
  }
  else{
    no_ouch_bg();
  }
  if(collides(&game_pad_right, &game_ball) == TRUE){
    ouch_bg();
    game_ball_vel.x *= -1;
    game_ball_vel.y = game_pad_right_vel.y;
  }
  else{
    no_ouch_bg();
  }
}

void stop_left_pad(){
  if(game_pad_left.y + game_pad_left.height > WINDOW_HEIGHT)
    game_pad_left.y = WINDOW_HEIGHT - game_pad_left.height;
  if(game_pad_left.y < 0)
    game_pad_left.y = 0;
}

void stop_right_pad(){
  if(game_pad_right.y + game_pad_right.height > WINDOW_HEIGHT)
    game_pad_right.y = WINDOW_HEIGHT - game_pad_right.height;
  if(game_pad_right.y < 0)
    game_pad_right.y = 0;
}

void update() {
  apply_vel();
  stop_left_pad();
  stop_right_pad();
  update_collision();  
  update_ball_bounce();
}


void render_bg() {
	SDL_SetRenderDrawColor(
			renderer, 
			bg_color_rgba[BG_RED_INDEX], 
			bg_color_rgba[BG_GREEN_INDEX], 
			bg_color_rgba[BG_BLUE_INDEX], 
			bg_color_rgba[BG_ALPHA_INDEX]);
	SDL_RenderClear(renderer);
  
}

void render_ball() {
	SDL_Rect ball_rect = {
		game_ball.x,
		game_ball.y,
		game_ball.width,
		game_ball.height
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball_rect);
	
}

void render_pad_left(){
  SDL_Rect pad_left = {
    game_pad_left.x,
    game_pad_left.y,
    game_pad_left.width,
    game_pad_left.height
  };
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &pad_left);
}

void render_pad_right(){
  SDL_Rect pad_right = {
    game_pad_right.x,
    game_pad_right.y,
    game_pad_right.width,
    game_pad_right.height
  };
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &pad_right);
}


void render() {
  render_bg();
  render_ball();
  render_pad_left();
  render_pad_right();
	
  //start drawing game objects
	SDL_RenderPresent(renderer);
}
