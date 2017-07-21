#include "tile.h"

//returns true if t1 should be aligned before t2
int align_left(tile t1, tile t2)
{
	if(t1.pos == t2.pos)
		return 0;

	if(t1.pos == 'l')
		return 1;

	if(t1.pos == 'c' && t2.pos == 'r')
		return 1;

	return 0;
}

//we put the elements that are aligned left to the front, thent those that are aligned center etc.
void sort_tile_array(tile **tile_array, unsigned int size)
{
	for(unsigned int i = 0; i < size - 1; ++i) {
		for(unsigned int j = i + 1; j < size; ++j) {
			if(align_left((*tile_array)[j], (*tile_array)[i])) {
				tile tmp = (*tile_array)[i];
				(*tile_array)[i] = (*tile_array)[j];
				(*tile_array)[j] = tmp;
				break;
			}
		}
	}
}
