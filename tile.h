#ifndef TILE_H
#define TILE_H

typedef struct {
	const char *prefix;
	char *str;
	char pos;
} tile;

int align_left(tile t1, tile t2);

void sort_tile_array(tile *tile_array, unsigned int size);

tile create_tile(const char *prefix, char pos);

void update_tile_array(tile *tile_array, unsigned int size, const char *fifo_output);

void free_tile_array(tile *tile_array, unsigned int size);

#endif
