#include <stdlib.h>
#include <limits.h>
#include <malloc.h>

#include "grid.h"

int main(void) {
	grid_t *grid = grid_new(4096, 4096, 4096 * 2048);
	malloc_stats();
	return grid->height;
}
