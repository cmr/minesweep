#ifndef GRID_H
#define GRID_H

typedef struct square_t {
	unsigned char hidden; /* 1 for hidden, 0 for revealed */
	signed char value; /* -1 for bomb */
} square_t;

#define SQUARE_BOMB ((square_t){.hidden = 1, .value = -1})
#define SQUARE_EMPTY ((square_t){.hidden = 1, .value = 0})

#define square_value(square) ((square).value)
#define square_is_bomb(square) ((square).value == -1)
#define square_hidden(square) ((square).hidden)
#define square_cmp(sq1, sq2) ((sq1).value == (sq2).value)

#define coord(x_, y_) ((coord_t){.x = (x_), .y = (y_)})

typedef struct grid {
	unsigned int height, width, bombs;
	square_t *data;
} grid_t;

typedef struct coord {
	unsigned int x;
	unsigned int y;
} coord_t;

grid_t *grid_new(unsigned int height, unsigned int width, unsigned int bombs);
void grid_del(grid_t *grid);
square_t grid_get(grid_t *grid, coord_t location);
void grid_set(grid_t *grid, coord_t location, square_t square);

#endif
