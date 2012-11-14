#ifndef GRID_H
#define GRID_H

typedef struct square_t {
	unsigned int hidden:1; /* 1 for hidden, 0 for revealed */
	unsigned int value:4; /* Number of bomb neighbors */
	unsigned int bomb:1; /* 1 for bomb, 0 for empty */
	unsigned int guess:2; /* 0 for none, 1 for unknown, 2 for empty, 3 for bomb */
} square_t;

#define SQUARE_BOMB ((square_t){.hidden = 1, .value = 0, .bomb=1})
#define SQUARE_EMPTY ((square_t){.hidden = 1, .value = 0, .bomb=0})
#define GUESS_NONE 0
#define GUESS_UNKNOWN 1
#define GUESS_EMPTY 2
#define GUESS_BOMB 3
#define square_value(square) ((square).value)
#define square_is_bomb(square) ((square).bomb)
#define square_is_hidden(square) ((square).hidden)
#define square_cmp(sq1, sq2) ((sq1).value == (sq2).value)
#define square_flag(square, val) ((square).guess = (val))

#define coord_new(x_, y_) ((coord_t){.x = (x_), .y = (y_)})

typedef struct grid {
	unsigned int height, width, bombs;
	square_t *data;
} grid_t;

typedef struct coord {
	unsigned int x;
	unsigned int y;
} coord_t;

grid_t *grid_new(unsigned int height, unsigned int width, unsigned int bombs);
void grid_free(grid_t *grid);
square_t grid_get(grid_t *grid, coord_t location);
void grid_set(grid_t *grid, coord_t location, square_t square);
void grid_add_bombs(grid_t *grid, unsigned int bombs);
unsigned int grid_reveal(grid_t *grid, coord_t location);

#endif
