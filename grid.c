/**
 * @file grid.c
 * @brief Creating, manipulating, and destroying Minesweeper grids
 *
 * @note All locations are 1-based when dealing with the grid!
 */
#define _XOPEN_SOURCE 500
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "grid.h"

#define cell_at(x, y) (grid->data + ((y-1) * grid->width) + (x-1))

static inline void incr_cell_f(grid_t *grid, int x, int y) {
	/* 0-1 is UINT_MAX, so when setting cells on the edges of the grid, this
	 * won't modify other rows or, even worse, unallocated pages.
	 * Not an assert because it's vital to proper functioning.
	 */
	assert(grid != NULL);
	assert(grid->data != NULL);

	if (x >= grid->width  || x <= 0) {
		return;
	}
	if (y >= grid->height || y <= 0) {
		return;
	}

	if (!square_is_bomb(*cell_at(x, y))) {
		square_value(*cell_at(x, y)) += 1;
	}
}

#define incr_cell(x, y) (incr_cell_f(grid, (x), (y)))

/**
 * Create a new grid.
 *
 * @param height Number of rows
 * @param width Number of columns
 * @param bombs Number of bombs to be placed on the grid
 *
 * @returns New grid
 */
grid_t *grid_new(int height, int width, unsigned int bombs) {
	if (width < 0 || height < 0) {
		return NULL;
	}

	assert(bombs <= width * height);
	unsigned long int mem_needed = sizeof(square_t) * width * height;

	grid_t *grid = (grid_t*)malloc(sizeof(grid_t));
	grid->height = height;
	grid->width = width;
	grid->bombs = bombs;

	grid->data = (square_t*)malloc(mem_needed);

	for (int i = 0; i < width * height; i++) {
		*(grid->data + i) = SQUARE_EMPTY;
	}

	grid_add_bombs(grid, bombs);
	return grid;
}

/**
 * Delete a grid and free its associated memory.
 *
 * @param grid Grid to delete
 */
void grid_free(grid_t *grid) {
	assert(grid != NULL);
	assert(grid->data != NULL);

	free(grid->data);
	free(grid);
}

/**
 * Get the value of a grid cell.
 *
 * @param grid Grid to get cell from
 * @returns The square at (x,y)
 */
square_t grid_get(grid_t *grid, int x, int y) {
	assert(grid != NULL);
	assert(x <= grid->width);
	assert(y <= grid->height);
	assert(x > 0);
	assert(y > 0);
	assert(grid->data != NULL);

	return *cell_at(x, y);
}

/**
 * Set the value of a grid cell.
 *
 * @param grid Grid to set cell on
 * @param square Value to set. Valid inputs: @a SQUARE_BOMB, @a SQUARE_EMPTY
 */
void grid_set(grid_t *grid, int x, int y, square_t square) {
	assert(square_is_bomb(square) || square_value(square) == 0);
	assert(grid != NULL);
	assert(grid->data != NULL);
	assert(x != 0);
	assert(y != 0);

	if (x > grid->width || y > grid->height) {
		/* Don't overflow into wrong row or, worse, unallocated memory */
		return;
	}
	*cell_at(x, y) = square;

	/* Increment adjacent cells */
	incr_cell(x-1, y-1);
	incr_cell(x, y-1);
	incr_cell(x+1, y-1);

	incr_cell(x-1, y);
	incr_cell(x+1, y);

	incr_cell(x-1, y+1);
	incr_cell(x, y+1);
	incr_cell(x+1, y+1);
}

/**
 * Randomly place bombs on a grid.
 *
 * @param grid Grid to place bombs on
 * @param bombs Number of bombs to place
 */
void grid_add_bombs(grid_t *grid, unsigned int bombs) {
	int x, y;
	while (bombs != 0) {
		do {
			x = (random() % grid->width) + 1;
			y = (random() % grid->height) + 1;
		} while (square_is_bomb(*cell_at(x, y)));

		grid_set(grid, x, y, SQUARE_BOMB);
		bombs--;
	}
}

/**
 * Reveal a sqaure.
 *
 * @param grid Grid to reveal cell on
 * @returns Value of the cell at (x,y). Result will be -(value + 1) if there
 * is a bomb in the cell
 */
int grid_reveal(grid_t *grid, int x, int y) {
	assert(grid != NULL);
	assert(grid->data != NULL);

	if (x > grid->width || y > grid->height) {
		return 0;
	}
	if (x < 1 || y < 1) {
		return 0;
	}

	square_t *cell = cell_at(x, y);
	int old_hidden = cell->hidden;
	cell->hidden = 0;

	if (cell->bomb) {
		return -(cell->value+1);
	}

	if (old_hidden == 0) {
		return cell->value;
	}

	/* Don't do a potentially expensive operation */

	if(cell->value == 0) {
		/* 0 cells have their neighbors revealed as well
		 * XXX: this code has a pathlogical case of large contiguous
		 * blocks of 0's. grid_reveal will be called for each square
		 * mulptile times. This is inefficient, and also has a high
		 * probability of blowing the stack for large grids.
		 */
		grid_reveal(grid, x-1, y-1);
		grid_reveal(grid, x, y-1);
		grid_reveal(grid, x+1, y-1);

		grid_reveal(grid, x-1, y);
		grid_reveal(grid, x+1, y);

		grid_reveal(grid, x-1, y+1);
		grid_reveal(grid, x, y+1);
		grid_reveal(grid, x+1, y+1);
	}

	return cell->value;
}

/**
 * Compare two squares for equality.
 *
 * Squares are considered equal if they have the same value and same
 * bomb-carrying status. Visibility does not come into play.
 */
int square_eq(square_t one, square_t two) {
	if (one.value == two.value && one.bomb == two.bomb) {
		return 1;
	}
	return 0;
}
