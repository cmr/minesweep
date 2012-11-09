#ifndef GRID_H
#define GRID_H

typedef int square_t;

#define SQUARE_BOMB -1
#define SQUARE_EMPTY 0

#define coord(x_, y_) ((struct coord){.x = (x_), .y = (y_)})

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
