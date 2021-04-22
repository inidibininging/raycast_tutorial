#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include "./objects.h"
#include "./map.h"
#include "./game.h"

int game_running = TRUE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
go_mov player;
map player_map;
rect r;
line l;

byte bg_color_rgba[4] = {
				0,
				0,
				0,
				255
			};

int setup() {
  go_mov_init(&player);
  map_demo(&player_map);
  
  player.direction = ROTATION_LEFT;
  player.position.x = WINDOW_WIDTH / 2;
  player.position.y = WINDOW_HEIGHT / 2;
   
  r.width = 16;
  r.height = 16;
  r.x = player.position.x;
  r.y = player.position.y;

  map_demo(&player_map);
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

void update()
{
  r.x = player.position.x - (r.width / 2);
  r.y = player.position.y - (r.height / 2);
  l.x1 = player.position.x;
  l.y1 = player.position.y; 
  l.x2 = player.position.x + (sin(player.rotation) * player.mv_step * 20);
  l.y2 = player.position.y + (cos(player.rotation) * player.mv_step * 20);
}

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
      {
				game_running = FALSE;
      }
      if(event.key.keysym.sym == SDLK_LEFT)
      {      
        player.rotation -= 1 * player.direction;
      }
      if(event.key.keysym.sym == SDLK_RIGHT)
      {
        player.rotation += 1 * player.direction;
      }
      if(event.key.keysym.sym == SDLK_UP)
      {
        player.position.x += sin(player.rotation) * player.mv_step * -1 * player.direction;
        player.position.y += cos(player.rotation) * player.mv_step * -1 * player.direction;
      }
      if(event.key.keysym.sym == SDLK_DOWN)
      {
        player.position.x += sin(player.rotation) * player.mv_step * player.direction;
        player.position.y += cos(player.rotation) * player.mv_step * player.direction;
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

void render_bg() {
	SDL_SetRenderDrawColor(
			renderer, 
			bg_color_rgba[BG_RED_INDEX], 
			bg_color_rgba[BG_GREEN_INDEX], 
			bg_color_rgba[BG_BLUE_INDEX], 
			bg_color_rgba[BG_ALPHA_INDEX]);
	SDL_RenderClear(renderer);
  
}

void render_rect(rect* rect, color* color) {
	SDL_Rect sdlrect = {
		rect->x,
		rect->y,
		rect->width,
		rect->height
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &sdlrect);
}

void render_line(line* line, color* color) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLineF(renderer, line->x1, line->y1, line->x2, line->y2);
}

void render() {
  render_bg();
  render_rect(&r, NULL);
  render_line(&l, NULL); 
  //start drawing game objects
	SDL_RenderPresent(renderer);
}
