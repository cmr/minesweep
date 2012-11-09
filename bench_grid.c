#include <stdlib.h>
#include <limits.h>

#include "grid.h"

int main(void) {
	grid_t *grid = grid_new(4096, 4096, 4096 * 1024);
	return grid->height;
}
