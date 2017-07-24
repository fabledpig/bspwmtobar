#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tile.h"

#define BUF_SIZE 512
#define COLOR_LENGTH 10

#define ALIGN_LEFT_ARG "-l"
#define ALIGN_CENTER_ARG "-c"
#define ALIGN_RIGHT_ARG "-r"
#define OC_ARG "--occupied"
#define OAC_ARG "--occupied-active"
#define FC_ARG "--free"
#define FAC_ARG "--free-active"
#define UC_ARG "--urgent"
#define UAC_ARG "--urgent-active"

typedef enum {ALIGN_ARG, COLOR_ARG, MISC} arg_type;

char occupied_color[COLOR_LENGTH], occupied_a_color[COLOR_LENGTH],
			free_color[COLOR_LENGTH], free_a_color[COLOR_LENGTH],
			urgent_color[COLOR_LENGTH], urgent_a_color[COLOR_LENGTH];

arg_type get_arg_type(const char *argv)
{
	if(!strcmp(argv, ALIGN_LEFT_ARG) ||
			!strcmp(argv, ALIGN_CENTER_ARG)  || !strcmp(argv, ALIGN_RIGHT_ARG))
		return ALIGN_ARG;

	if(!strcmp(argv, OC_ARG) ||
			!strcmp(argv, OAC_ARG) ||
			!strcmp(argv, FC_ARG) ||
			!strcmp(argv, FAC_ARG) ||
			!strcmp(argv, UC_ARG) ||
			!strcmp(argv, UAC_ARG))
		return COLOR_ARG;

	return MISC;
}

unsigned int count_necessary_tiles(int argc, char *argv[])
{
	unsigned int cnt = 0;

	for(int i = 1; i < argc; ++i) {
		arg_type at = get_arg_type(argv[i]);
		if(at == MISC)
			++cnt;
		else if(at == COLOR_ARG)
			++i; //next one is the color code so we skip it
	}

	return cnt;
}

void process_color_arg(const char *color_type, const char *color)
{
	if(!strcmp(color_type, OC_ARG))
		strcpy(occupied_color, color);
	else if(!strcmp(color_type, OAC_ARG))
		strcpy(occupied_a_color, color);
	else if(!strcmp(color_type, FC_ARG))
		strcpy(free_color, color);
	else if(!strcmp(color_type, FAC_ARG))
		strcpy(free_a_color, color);
	else if(!strcmp(color_type, UC_ARG))
		strcpy(urgent_color, color);
	else if(!strcmp(color_type, UAC_ARG))
		strcpy(urgent_a_color, color);
}

tile *process_args(int argc, char *argv[], unsigned int *size)
{
	*size = count_necessary_tiles(argc, argv);
	tile *tile_array = malloc(sizeof(tile) * *size);
	char pos = 'l';
	unsigned int index = 0;

	for(int i = 1; i < argc; ++i) {
		arg_type at = get_arg_type(argv[i]);
		if(at == ALIGN_ARG)
			pos = argv[i][1];
		else if(at == COLOR_ARG) {
			if(i >= argc - 1)
				fprintf(stderr, "Error! Not enough arguments!");
			else {
				process_color_arg(argv[i], argv[i + 1]);
				++i;
			}
		}
		else
			tile_array[index++] = create_tile(argv[i], pos);
	}

	return tile_array;
}

int main(int argc, char *argv[])
{
	unsigned int tile_array_size;
	tile *tile_array = process_args(argc, argv, &tile_array_size);

	const char *fifo_path = "/tmp/bspwmtobar-fifo";
	mkfifo(fifo_path, 0666);
	int fd = open(fifo_path, O_RDONLY);

	sort_tile_array(tile_array, tile_array_size);

	char buf[BUF_SIZE];
	int bytes_read;
	while((bytes_read = read(fd, buf, BUF_SIZE - 1)) > 0) {
		buf[bytes_read] = 0;
		update_tile_array(tile_array, tile_array_size, buf);

		if(tile_array[0].str)
			printf("%s\n", tile_array[0].str);
	}

	free_tile_array(tile_array, tile_array_size);
	close(fd);
	unlink(fifo_path);
	return 0;
}
