#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desktop.h"

#include "tile.h"

desktop *desktop_array = NULL;
unsigned int desktop_array_size = 0;

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
void sort_tile_array(tile *tile_array, unsigned int size)
{
	for(unsigned int i = 0; i < size - 1; ++i) {
		for(unsigned int j = i + 1; j < size; ++j) {
			if(align_left(tile_array[j], tile_array[i])) {
				tile tmp = tile_array[i];
				tile_array[i] = tile_array[j];
				tile_array[j] = tmp;
				break;
			}
		}
	}
}

tile create_tile(const char *prefix, char pos)
{
	tile t = {.prefix = prefix, .str = NULL, .pos = pos};

	return t;
}

void update_tile_array(tile *tile_array, unsigned int size, const char *fifo_output)
{
	const char *prefix;
	char *str;
	char *tmp = NULL;

	if(fifo_output[0] == 'W') {
		prefix = "W";
		build_desktop_array(fifo_output, &desktop_array, &desktop_array_size);
		str = desktop_array_to_string(desktop_array, desktop_array_size);
	} else {
		if(strchr(fifo_output, ':') == NULL) //invalid format
			return;

		tmp = malloc(strlen(fifo_output) + 1);
		strcpy(tmp, fifo_output);
		prefix  = strtok(tmp, ":");

		str = malloc(strlen(fifo_output) - strlen(prefix));
		strcpy(str, fifo_output + strlen(prefix) + 1);
	}

	int found = 0;
	for(unsigned int i = 0; i < size; ++i) {
		if(!strcmp(tile_array[i].prefix, prefix)) {
			if(tile_array[i].str)
				free(tile_array[i].str);

			found = 1;
			tile_array[i].str = str;
		}
	}

	if(!found)
		free(str);

	if(tmp)
		free(tmp);
}

void free_tile_array(tile *tile_array, unsigned int size)
{
	for(unsigned int i = 0; i < size; ++i)
		free(tile_array[i].str);

	free(tile_array);

	//gets freed here, as it is decleared in this file
	if(desktop_array) {
		free_desktop_array(desktop_array, desktop_array_size);
		desktop_array = NULL; //just to be sure
	}
}
