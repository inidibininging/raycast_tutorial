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

int setup() {
	game_ball.x = WINDOW_WIDTH / 2;
	game_ball.y = WINDOW_HEIGHT / 2;
	game_ball.width = 10;
	game_ball.height = 10;
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
			break;
	}
}

 

void update() {
	bg_color_rgba[BG_RED_INDEX] = bg_color_rgba[BG_RED_INDEX] - 1;
	if(bg_color_rgba[BG_RED_INDEX == 0])
		bg_color_rgba[BG_RED_INDEX] = 255;
}

void render() {
	SDL_SetRenderDrawColor(
			renderer, 
			bg_color_rgba[BG_RED_INDEX], 
			bg_color_rgba[BG_GREEN_INDEX], 
			bg_color_rgba[BG_BLUE_INDEX], 
			bg_color_rgba[BG_ALPHA_INDEX]);
	SDL_RenderClear(renderer);
	SDL_Rect ball_rect = {
		game_ball.x,
		game_ball.y,
		game_ball.width,
		game_ball.height
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball_rect);
	
	//start drawing game objects
	SDL_RenderPresent(renderer);
}


