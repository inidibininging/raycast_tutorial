#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "./constants.h"
#include "./objects.h"
#include "./map.h"
#include "./game.h"
float PI = 3.14;
int game_running = TRUE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

go_mov player;
map player_map;
rect_ll *walls = NULL;

//player view
rect r;
line l;
line h;
line *rays = NULL;

byte bg_color_rgba[4] = {
    0,
    0,
    0,
    255};

int setup()
{
  go_mov_init(&player);
  map_demo(&player_map);
  walls = map_get_rects(&player_map);
  rays = (line *)malloc(sizeof(line) * WINDOW_WIDTH);

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

int initialize_window(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
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
  if (!window)
  {
    fprintf(stderr, "Error creating SDL Window\n");
    return FALSE;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  //-1 => default driver
  if (!renderer)
  {
    fprintf(stderr, "Error creating SDL Renderer\n");
    return FALSE;
  }

  return TRUE;
}

void destroy_window()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

float normalize_rotation(float rotation)
{
  rotation = fmod(rotation, (2 * PI));
  if (rotation < 0)
    rotation += (2 * PI);
  return rotation;
}

void update()
{
  r.x = player.position.x - (r.width / 2);
  r.y = player.position.y - (r.height / 2);

  l.x1 = player.position.x;
  l.y1 = player.position.y;
  l.x2 = player.position.x + (cos(player.rotation) * player.mv_step * FOV);
  l.y2 = player.position.y + (sin(player.rotation) * player.mv_step * FOV);

  normalize_rotation(player.rotation);
}

void rays_destroy()
{
  //this is so metal
  free(rays);
}

int main()
{
  game_running = initialize_window();
  setup();
  while (game_running)
  {
    process_input();
    update();
    render();
  }
  map_destroy(&player_map);
  rays_destroy();
  destroy_window();
  return FALSE;
}

int collides(rect *a, rect *b)
{

  if (a->x <= b->x &&
      a->x + a->width >= b->x &&
      a->y <= b->y &&
      a->y + a->height >= b->y)
  {
    //a intersects with b
    return TRUE;
  }
  return FALSE;
}

void process_input()
{
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type)
  {
  //clicking the X button of the window
  case SDL_QUIT:
    game_running = FALSE;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
    {
      game_running = FALSE;
    }
    if (event.key.keysym.sym == SDLK_LEFT)
    {
      player.rotation -= 0.1 * player.direction;
    }
    if (event.key.keysym.sym == SDLK_RIGHT)
    {
      player.rotation += 0.1 * player.direction;
    }
    if (event.key.keysym.sym == SDLK_UP)
    {
      player.position.x += cos(player.rotation) * player.mv_step * player.direction;
      player.position.y += sin(player.rotation) * player.mv_step * player.direction;
    }
    if (event.key.keysym.sym == SDLK_DOWN)
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
  if (bg_color_rgba[BG_RED_INDEX == 0])
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

void render_bg()
{
  SDL_SetRenderDrawColor(
      renderer,
      bg_color_rgba[BG_RED_INDEX],
      bg_color_rgba[BG_GREEN_INDEX],
      bg_color_rgba[BG_BLUE_INDEX],
      bg_color_rgba[BG_ALPHA_INDEX]);
  SDL_RenderClear(renderer);
}

void render_rect(rect *rect, color *color)
{
  SDL_Rect sdlrect = {
      rect->x,
      rect->y,
      rect->width,
      rect->height};
  if (color == NULL)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  else
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderFillRect(renderer, &sdlrect);
}

void render_line(line *line, color *color)
{
  if (color == NULL)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  else
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
  SDL_RenderDrawLineF(renderer, line->x1, line->y1, line->x2, line->y2);
}

void render_view(go_mov *p)
{

  //FOV / 2 because the player looks at the center. so the max position is related to FOV / 2
  float ray_angle = p->rotation - ((FOV_HALF * PI) / 180);
  float intersect_y = floor(p->position.y / CELL_WIDTH) * CELL_WIDTH;
  float x_step = CELL_WIDTH;
  int column = 0;

  static color lecolour;
  lecolour.r = 255;
  lecolour.g = 0;
  lecolour.b = 0;
  lecolour.a = 0;

  // cast rays
  while (column < WINDOW_WIDTH)
  {
    float intersect_x = p->position.x + ((intersect_y - p->position.y) / tan(ray_angle));
    float y_step = CELL_WIDTH;

    //get grid, where player interesects
    rays[column].x1 = p->position.x;
    rays[column].y1 = p->position.y;
    rays[column].x2 = p->position.x + cos(ray_angle) * p->mv_step * FOV;
    rays[column].y2 = p->position.y + sin(ray_angle) * p->mv_step * FOV;

    int facing_down = ray_angle > 0 && ray_angle < PI;
    int facing_up = !facing_down;
    int facing_right = ray_angle < 0.5 * PI || ray_angle > (1.5 * PI);
    int facing_left = !facing_right;

    int found_wall_hit = FALSE;
    int ray_horizontal_direction = facing_left ? -1 : 1;
    int ray_vertical_direction = facing_up ? 1 : -1;

    //get player grid corresponding to grid
    int ray_map_x = (int)floor(intersect_x * 2 / CELL_WIDTH);
    int ray_map_y = (int)floor(intersect_y * 2 / CELL_HEIGHT);
    y_step *= facing_up ? 1 : -1;
    
    if(ray_map_x > player_map.columns)
      ray_map_x = player_map.columns - 1;
    if(ray_map_y > player_map.rows)
      ray_map_y = player_map.rows - 1;

    //enlarge ray till it hits a wall
    while (found_wall_hit == FALSE && 
        (ray_map_y < player_map.columns && ray_map_y > 0) && 
        (ray_map_x < player_map.rows && ray_map_x > 0))
    {
      //go into the direction where the player is looking
      //check if the a grid is a wall
      if (player_map.cells[ray_map_y][ray_map_x].tex != ColumnBlack)
      {
        found_wall_hit = TRUE;
        break;
      }

      x_step = CELL_WIDTH / tan(ray_angle);
      x_step *= facing_left && x_step > 0 ? -1 : 1;
      x_step *= facing_right && x_step < 0 ? 1 : -1;
      
      y_step += CELL_HEIGHT;
      y_step *= facing_up && y_step > 0 ? 1 : -1;
      y_step *= facing_down && y_step > 0 ? 1 : -1;

      ray_map_x = (int)floor(intersect_x + x_step / CELL_WIDTH);
      ray_map_y = (int)floor(intersect_y + y_step / CELL_HEIGHT);

    }

    static rect face_indicator;
    face_indicator.x = 0;
    face_indicator.y = 0;
    face_indicator.width = 32;
    face_indicator.height = 32;

    static color color_indicator;
    color_indicator.r = facing_down ? 255 : 0;
    color_indicator.g = facing_right ? 255 : 0;
    color_indicator.b = facing_up ? 255 : 0;

    static color ri;
    ri.r = 0;
    ri.g = facing_left ? 255 : 0;
    ri.b = facing_right ? 255 : 0;

    if (found_wall_hit == TRUE)
      render_rect(&face_indicator, &ri);

    if (ray_map_x >= 0 && ray_map_y >= 0) {
      //take a cell
      //store that the player is inside this grid
      static line line_of_sight;
      line_of_sight.x1 = p->position.x;
      line_of_sight.x2 = intersect_x + x_step;
      line_of_sight.y1 = p->position.y;
      line_of_sight.y2 = intersect_y + y_step;

      if (line_of_sight.x2 > WINDOW_WIDTH)
        line_of_sight.x2 = WINDOW_WIDTH;
      if (line_of_sight.y2 > WINDOW_HEIGHT)
        line_of_sight.y2 = WINDOW_HEIGHT;
      if (line_of_sight.x2 < 0)
        line_of_sight.x2 = 0;
      if (line_of_sight.y2 < 0)
        line_of_sight.y2 = 0;

      render_line(&line_of_sight, &lecolour);
    }
    render_line(&rays[column], &color_indicator);
    float radadd = (((float)FOV / ((float)WINDOW_WIDTH) * PI) / 180);
    ray_angle += radadd; //WINDOW_WIDTH_HALF stands for the number of rays to cast (resolution)
    ray_angle = normalize_rotation(ray_angle);
    column++;
  }
}

void render()
{
  render_bg();
  render_rect(&r, NULL);
  render_line(&l, NULL);
  render_view(&player);
  //start drawing game objects
  SDL_RenderPresent(renderer);
}
