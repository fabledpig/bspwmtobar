#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "tile.h"

#define BUF_SIZE 512
#define COLOR_LENGTH 10

char occupied_color[COLOR_LENGTH], occupied_a_color[COLOR_LENGTH],
			free_color[COLOR_LENGTH], free_a_color[COLOR_LENGTH],
			urgent_color[COLOR_LENGTH], urgent_a_color[COLOR_LENGTH];

int main(int argc, char *argv[])
{
	const char *fifo_path = "/tmp/bspwmtobar-fifo";
	mkfifo(fifo_path, 0666);
	int fd = open(fifo_path, O_RDONLY);

	tile *tile_array = malloc(sizeof(tile)); //this part is hardcoded, just for testing
	unsigned int size = 1;
	tile_array[0] = create_tile("test", 'c');
	sort_tile_array(tile_array, size);

	char buf[BUF_SIZE];
	int bytes_read;
	while((bytes_read = read(fd, buf, BUF_SIZE - 1)) > 0) {
		buf[bytes_read] = 0;
		update_tile_array(tile_array, size, buf);

		if(tile_array[0].str)
			printf("%s\n", tile_array[0].str);
	}

	free_tile_array(tile_array, size);
	close(fd);
	unlink(fifo_path);
	return 0;
}
