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
rect_ll* walls = NULL;

//player view
rect r;
line l;
line h;
line* rays = NULL;

byte bg_color_rgba[4] = {
				0,
				0,
				0,
				255
			};

int setup() {
  go_mov_init(&player);
  map_demo(&player_map);
  walls = map_get_rects(&player_map);
  rays = (line*)malloc(sizeof(line) * WINDOW_WIDTH);

  player.direction = ROTATION_RIGHT;
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
  l.x2 = player.position.x + (cos(player.rotation) * player.mv_step * FOV);
  l.y2 = player.position.y + (sin(player.rotation) * player.mv_step * FOV);

  
}

void rays_destroy()
{
  //this is so metal
  free(rays);
}

int main() {
	game_running = initialize_window();
	setup();
	while(game_running){
		process_input();
		update();
		render();		
	}
  map_destroy(&player_map);
  rays_destroy();
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
        player.rotation -= 0.1 * player.direction;
      }
      if(event.key.keysym.sym == SDLK_RIGHT)
      {
        player.rotation += 0.1 * player.direction;
      }
      if(event.key.keysym.sym == SDLK_UP)
      {
        player.position.x += cos(player.rotation) * player.mv_step * player.direction;
        player.position.y += sin(player.rotation) * player.mv_step * player.direction;
      }
      if(event.key.keysym.sym == SDLK_DOWN)
      {
        player.position.x += cos(player.rotation) * player.mv_step * player.direction * -1;
        player.position.y += sin(player.rotation) * player.mv_step * player.direction * -1;
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
  if(color == NULL)
	  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  else
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
	SDL_RenderFillRect(renderer, &sdlrect);
}

void render_line(line* line, color* color) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLineF(renderer, line->x1, line->y1, line->x2, line->y2);
}

void render_view(go_mov* p)
{

  //FOV / 2 because the player looks at the center. so the max position is related to FOV / 2
  float ray_angle = p->rotation - ((FOV_HALF * 3.14) / 180);  
  int column = 0; 
  while(column < WINDOW_WIDTH)
  {
    //cast a ray
    //get grid, where player interesects
    rays[column].x1 = p->position.x;
    rays[column].y1 = p->position.y;
    rays[column].x2 = p->position.x + cos(ray_angle) * p->mv_step * FOV;
    rays[column].y2 = p->position.y + sin(ray_angle) * p->mv_step * FOV;
   
    for(int map_y = 0; map_y < player_map.rows; map_y++)
    {
      for(int map_x = 0; map_x < player_map.columns; map_x++)
      {

        if(p->position.x > player_map.cells[map_y][map_x].position.x &&
           p->position.x < player_map.cells[map_y][map_x].position.x + CELL_WIDTH &&
           p->position.y > player_map.cells[map_y][map_x].position.y &&
           p->position.y < player_map.cells[map_y][map_x].position.y + CELL_HEIGHT)
        {

          static rect lecell;
          lecell.x = player_map.cells[map_y][map_x].position.x;
          lecell.y = player_map.cells[map_y][map_x].position.y;
          lecell.width = CELL_WIDTH;
          lecell.height = CELL_HEIGHT;
          
          static color lecolour;
          lecolour.r = 0;
          lecolour.g = 128;
          lecolour.b = 128;
          lecolour.a = 128;

          render_rect(&lecell, &lecolour);
          
          float intersect_x = 0;
          float intersect_y = 0;
          
          //right
          if(rays[column].x2 > player_map.cells[map_y][map_x].position.x + CELL_WIDTH)
          {
            intersect_x = player_map.cells[map_y][map_x].position.x + CELL_WIDTH;
            intersect_y = p->position.y + (tan(ray_angle) * (player_map.cells[map_y][map_x].position.x + CELL_WIDTH - p->position.x)); 
          }

          //left
          if(rays[column].x2 < player_map.cells[map_y][map_x].position.x)
          {
            //intersect_x = rays[column].x2 - player_map.cells[map_y][map_x].position.x;
            //intersect_y = rays[column].y2 - player_map.cells[map_y][map_x].position.y;
          }

          //bottom
          if(rays[column].y2 > player_map.cells[map_y][map_x].position.y + CELL_HEIGHT)
          {
            //intersect_y = rays[column].y2 - player_map.cells[map_y][map_x].position.y + CELL_HEIGHT;
            //intersect_x = rays[column].x2 - player_map.cells[map_y][map_x].position.x + CELL_WIDTH;
          }

          //top
          if(rays[column].y2 < player_map.cells[map_y][map_x].position.y)
          {
            //intersect_y = rays[column].y2 - player_map.cells[map_y][map_x].position.y;
            //intersect_x = rays[column].x2 - player_map.cells[map_y][map_x].position.x;
          }

          if(intersect_x != 0 ||
             intersect_y != 0)
          {
            static rect interect;
            interect.x = intersect_x;
            interect.y = intersect_y;
            interect.width = 2;
            interect.height = 2;

            static color red;
            red.r = 255;
            red.g = 0;
            red.b = 0;
            red.a = 255;

            render_rect(&interect, &red);
          }
          break;
        } 
      }  
    }
  
    render_line(&rays[column], NULL);
   
    ray_angle += (((float)FOV / ((float)WINDOW_WIDTH) * 3.14) / 180); //WINDOW_WIDTH_HALF stands for the number of rays to cast (resolution)
    column++;
  } 
}

void render() {
  render_bg();
  render_rect(&r, NULL);
  render_line(&l, NULL); 
  render_view(&player);
  //start drawing game objects
	SDL_RenderPresent(renderer);
}
