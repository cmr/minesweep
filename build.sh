if [ "$CC" = "" ] ; then
	CC=gcc
fi

echo "$*" | grep debug > /dev/null

if [ $? -eq 0 ]; then
	CFLAGS+=-g3
fi

$CC $CFLAGS -std=c99 -o test_grid test_grid.c grid.c
$CC $CFLAGS -std=c99 -DNDEBUG -O1 -o bench_grid bench_grid.c grid.c
$CC $CFLAGS -std=c99 -o ui ui.c grid.c
