#ifndef GRID_H
#define GRID_H

typedef struct square_t {
	unsigned int hidden:1; /* 1 for hidden, 0 for revealed */
	unsigned int value:4; /* Number of bomb neighbors */
	unsigned int bomb:1; /* 1 for bomb, 0 for empty */
} square_t;

#define SQUARE_BOMB ((square_t){.hidden = 1, .value = 0, .bomb=1})
#define SQUARE_EMPTY ((square_t){.hidden = 1, .value = 0, .bomb=0})

#define square_value(square) ((square).value)
#define square_is_bomb(square) ((square).bomb)
#define square_is_hidden(square) ((square).hidden)

typedef struct grid {
	unsigned int height, width, bombs;
	square_t *data;
} grid_t;

grid_t *grid_new(int height, int width, unsigned int bombs);
void grid_free(grid_t *grid);
square_t grid_get(grid_t *grid, int x, int y);
void grid_set(grid_t *grid, int x, int y, square_t square);
void grid_add_bombs(grid_t *grid, unsigned int bombs);
int grid_reveal(grid_t *grid, int x, int y);
int square_eq(square_t one, square_t two);

#endif
