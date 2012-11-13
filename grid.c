/**
 * @file grid.c
 * @brief Creating, manipulating, and destroying Minesweeper grids
 */

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "grid.h"

#define cell_at(x, y) (grid->data + (x) + (grid->width * (y)))

static inline void incr_cell_f(grid_t *grid, unsigned int x, unsigned int y) {
	/* 0-1 is UINT_MAX, so when setting cells on the edges of the grid, this
	 * won't modify other rows or, even worse, unallocated pages.
	 * Not an assert because it's vital to proper functioning.
	 */
	assert(grid != NULL);
	assert(grid->data != NULL);

	if (x >= grid->width  || x == UINT_MAX) {
		return;
	}
	if (y >= grid->height || y == UINT_MAX) {
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
grid_t *grid_new(unsigned int height, unsigned int width, unsigned int bombs) {
	unsigned long int mem_needed = sizeof(square_t) * width * height;
	assert(bombs <= width * height);

	if (width == UINT_MAX || height == UINT_MAX) {
		return NULL;
	}

	grid_t *grid = (grid_t*)malloc(sizeof(grid_t));
	grid->height = height;
	grid->width = width;
	grid->bombs = bombs;
	
	grid->data = (square_t*)malloc(mem_needed);
	memset(grid->data, 0, mem_needed);

	grid_add_bombs(grid, bombs);	
	return grid;
}

/**
 * Delete a grid and free its associated memory.
 *
 * @param grid Grid to delete
 */
void grid_del(grid_t *grid) {
	assert(grid != NULL);
	assert(grid->data != NULL);

	free(grid->data);
	free(grid);
}

/**
 * Get the value of a grid cell.
 *
 * @param grid Grid to get cell from
 * @param location (x,y) of cell
 * @returns The square at (x,y)
 */
square_t grid_get(grid_t *grid, coord_t location) {
	unsigned int x = location.x, y = location.y;
	assert(grid != NULL);
	assert(x < grid->width);
	assert(y < grid->height);
	assert(grid->data != NULL);

	return *cell_at(x, y);
}

/**
 * Set the value of a grid cell.
 *
 * @param grid Grid to set cell on
 * @param location (x,y) of cell
 * @param square Value to set. Valid inputs: @a SQUARE_BOMB, @a SQUARE_EMPTY
 */
void grid_set(grid_t *grid, coord_t location, square_t square) {
	unsigned int x = location.x, y = location.y;
	assert(square_is_bomb(square) || square_value(square) == 0);
	assert(grid != NULL);
	assert(grid->data != NULL);

	if (x >= grid->width || y >= grid->height) {
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
	/* XXX: don't need to check for duplicates because the period of
	 * random(2) is 8x bigger than UINT_MAX-1, the maximum number of bombs
	 * that can be placed. This IS platform dependant! If an unsigned int is
	 * any larger than 34 bits, duplicate checking will need to be done.
	 */
	for (/* EMPTY */; bombs != 0; bombs--) {
		grid_set(grid, coord(random() % grid->width, random() % grid->height), SQUARE_BOMB);
	}
}
