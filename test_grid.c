#include <assert.h>
#include <stdlib.h>
#include <limits.h>

#include "grid.h"

int main(void) {
	/* Test creation */
	grid_t *grid = grid_new(0, 0, 0);
	assert(grid->height == 0);
	assert(grid->width  == 0);
	assert(grid->bombs  == 0);

	grid_free(grid);
	
	grid = grid_new(10, 10, 0);
	assert(grid->height == 10);
	assert(grid->width  == 10);
	assert(grid->bombs  == 0);
	assert(grid->data   != NULL);

	/* Ensure whole grid is initialized to SQUARE_EMPTY */
	for (int row = 1; row <= 10; row++) {
		for (int col = 1; col <= 10; col++) {
			assert(square_eq(SQUARE_EMPTY, grid_get(grid, col, row)));
		}
	}

	/* Test set + get */
	grid_set(grid, 5, 5, SQUARE_BOMB);
	assert(square_is_bomb(grid_get(grid, 5, 5)));
	assert(square_value(grid_get(grid, 5, 6)) == 1);
	assert(square_value(grid_get(grid, 4, 4)) == 1);
	grid_set(grid, 5, 6, SQUARE_BOMB);
	assert(square_value(grid_get(grid, 4, 5)) == 2);

	grid_free(grid);

	grid = grid_new(16, 16, 0);
	grid_set(grid, 16, 16, SQUARE_BOMB);

	grid_free(grid);

	grid = grid_new(50, 50, 0);
	
	/* Ensure new squares are hidden */
	for (int x = 1; x <= 50; x++) {
		for (int y = 1; y <= 50; y++) {
			assert(square_is_hidden(grid_get(grid, x, y)));
		}
	}

	/* Revealing a grid of 0's should reveal every square */
	grid_reveal(grid, 1, 1);
	for (int x = 1; x <= 50; x++) {
		for (int y = 1; y <= 50; y++) {
			assert(!square_is_hidden(grid_get(grid, x, y)));
		}
	}

	grid_free(grid);

	grid = grid_new(10, 10, 10);
	assert(grid->bombs == 10);
	int bombs = 0;

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 10; y++) {
			int res = grid_reveal(grid, x, y);
			square_t sq = grid_get(grid, x, y);
			assert(!square_is_hidden(sq));
			if (square_is_bomb(sq)) {
				bombs++;
				assert(res < 0);
				assert(res == -(square_value(sq)+1));
			} else {
				assert(res == square_value(sq));
			}
		}
	}

	assert(bombs == 10);

	grid_free(grid);

	return 0;
}
