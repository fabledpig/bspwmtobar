#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tile.h"

#define BUF_SIZE 512
#define FORMAT_LENGTH 32

#define ALIGN_LEFT_ARG "-l"
#define ALIGN_CENTER_ARG "-c"
#define ALIGN_RIGHT_ARG "-r"
#define OC_ARG "--occupied"
#define OAC_ARG "--occupied-active"
#define FC_ARG "--free"
#define FAC_ARG "--free-active"
#define UC_ARG "--urgent"
#define UAC_ARG "--urgent-active"

typedef enum {ALIGN_ARG, FORMAT_ARG, MISC} arg_type;

char occupied_format[FORMAT_LENGTH], occupied_a_format[FORMAT_LENGTH],
			free_format[FORMAT_LENGTH], free_a_format[FORMAT_LENGTH],
			urgent_format[FORMAT_LENGTH], urgent_a_format[FORMAT_LENGTH];

static arg_type get_arg_type(const char *argv)
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
		return FORMAT_ARG;

	return MISC;
}

static unsigned int count_necessary_tiles(int argc, char *argv[])
{
	unsigned int cnt = 0;

	for(int i = 1; i < argc; ++i) {
		arg_type at = get_arg_type(argv[i]);
		if(at == MISC)
			++cnt;
		else if(at == FORMAT_ARG)
			++i; //next one is the format string so we skip it
	}

	return cnt;
}

static void process_format_arg(const char *format_type, const char *format)
{
	if(!strcmp(format_type, OC_ARG))
		strcpy(occupied_format, format);
	else if(!strcmp(format_type, OAC_ARG))
		strcpy(occupied_a_format, format);
	else if(!strcmp(format_type, FC_ARG))
		strcpy(free_format, format);
	else if(!strcmp(format_type, FAC_ARG))
		strcpy(free_a_format, format);
	else if(!strcmp(format_type, UC_ARG))
		strcpy(urgent_format, format);
	else if(!strcmp(format_type, UAC_ARG))
		strcpy(urgent_a_format, format);
}

static void set_empty_format_args(void)
{
	if(strlen(free_format) == 0)
		strcpy(free_format, "%{F#FFFFFF}");
	if(strlen(free_a_format) == 0)
		strcpy(free_a_format, "%{F#FF0000}");

	if(strlen(occupied_format) == 0)
		strcpy(occupied_format, free_format);
	if(strlen(occupied_a_format) == 0)
		strcpy(occupied_a_format, free_a_format);
	if(strlen(urgent_format) == 0)
		strcpy(urgent_format, free_format);
	if(strlen(urgent_a_format) == 0)
		strcpy(urgent_a_format, free_a_format);
}

static tile *process_args(int argc, char *argv[], unsigned int *size)
{
	*size = count_necessary_tiles(argc, argv);
	tile *tile_array = malloc(sizeof(tile) * *size);
	char pos = 'l';
	unsigned int index = 0;

	for(int i = 1; i < argc; ++i) {
		arg_type at = get_arg_type(argv[i]);
		if(at == ALIGN_ARG)
			pos = argv[i][1];
		else if(at == FORMAT_ARG) {
			if(i >= argc - 1)
				fprintf(stderr, "Error! Not enough arguments!");
			else {
				process_format_arg(argv[i], argv[i + 1]);
				++i;
			}
		}
		else
			tile_array[index++] = create_tile(argv[i], pos);
	}

	set_empty_format_args();

	return tile_array;
}

static void print_tile_array(const tile *tile_array, unsigned int size)
{
	for(unsigned int i = 0; i < size; ++i)
		printf("%%{%c}%s", tile_array[i].pos, tile_array[i].str);

	printf("\n");
	fflush(stdout);
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
		buf[bytes_read - 1] = 0; //cut down newline

		for(char *pch = strtok(buf, "\n"); pch != NULL; pch = strtok(NULL, "\n"))
			update_tile_array(tile_array, tile_array_size, pch);

		print_tile_array(tile_array, tile_array_size);
	}

	free_tile_array(tile_array, tile_array_size);
	close(fd);
	unlink(fifo_path);
	return 0;
}
