#include <assert.h>
#include <stdlib.h>
#include <limits.h>

#include "grid.h"

int main(void) {
	grid_t *grid = grid_new(0, 0, 0);
	assert(grid->height == 0);
	assert(grid->width  == 0);
	assert(grid->bombs  == 0);
/*	assert(grid->data   == NULL); */

	grid_del(grid);
	
	grid = grid_new(10, 10, 0);
	assert(grid->height == 10);
	assert(grid->width  == 10);
	assert(grid->bombs  == 0);
	assert(grid->data   != NULL);

	grid_set(grid, coord(5, 5), SQUARE_BOMB);
	assert(square_is_bomb(grid_get(grid, coord(5, 5))));
	assert(square_value(grid_get(grid, coord(5, 6))) == 1);
	assert(square_value(grid_get(grid, coord(4, 4))) == 1);
	grid_set(grid, coord(5, 6), SQUARE_BOMB);
	assert(square_value(grid_get(grid, coord(4, 5))) == 2);
	grid_set(grid, coord(0, 0), SQUARE_BOMB);
	assert(square_value(grid_get(grid, coord(0, 1))) == 1);

	grid_del(grid);

	/* Mother of all memory hogs. 2^64 * 4 bytes. Counting on linux to do some
	 * sort of CoW or lazy allocation... we'll see */

	/* Note: it's not actually possible to test this... need to wait for
	 * 128bit processors come along!
	 */

	/*
	 *grid = grid_new(UINT_MAX - 1, UINT_MAX - 1, 0);
	 *grid_set(grid, coord(UINT_MAX - 2, UINT_MAX - 2), SQUARE_BOMB);
	 *assert(square_is_bomb(grid_get(grid, coord(UINT_MAX - 2, UINT_MAX - 2))));
	 */

	grid = grid_new(16, 16, 0);
	grid_set(grid, coord(16, 16), SQUARE_BOMB);

	grid_del(grid);

	return 0;
}
