gcc -g -o test_grid test_grid.c grid.c
gcc -g -DNDEBUG -O1 -o bench_grid bench_grid.c grid.c
