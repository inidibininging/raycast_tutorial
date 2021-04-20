#define CELL_WIDTH 10
#define CELL_HEIGHT 10

enum texture_t
{
  ColumnRed = 2,
  ColumnGreen = 3,
  ColumnBlue = 4,
  ColumnWhite = 1,
  ColumnBlack = 0,
};

typedef struct cell_t {
  struct vec2_it pos;
  enum texture_t tex;
} cell;

typedef struct map_t
{
  int columns;
  int rows;
  cell** cells; 
} map;

void init_map(char* path, map* map);
map create_map();
void destroy_map(map* map);

