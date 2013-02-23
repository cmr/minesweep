#include <stdio.h>

#include "grid.h"

int main(int argc, char *argv[]) {
	grid_t *grid = grid_new(10, 10, 10);

	for (int row = 1; row <= 10; row++) {
		for (int col = 1; col <= 10; col++) {
			square_t sq = grid_get(grid, col, row);
			printf(",");
			if (square_is_bomb(sq)) {
				printf("X");
			} else {
				printf(" %d", square_value(sq));
			}
		}
		printf("\n");
	}

	return 0;
}
