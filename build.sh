if [ "$CC" = "" ] ; then
	CC=gcc
fi

echo "$*" | grep debug > /dev/null

if [ $? -eq 0 ]; then
	CFLAGS+=-g
fi

$CC $CFLAGS -o test_grid test_grid.c grid.c
$CC $CFLAGS -DNDEBUG -O1 -o bench_grid bench_grid.c grid.c
