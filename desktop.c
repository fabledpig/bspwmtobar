#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "desktop.h"

#define DESKTOP_SIZE 1024
#define DESKTOP_BUF_SIZE 128

unsigned int count_desktops(const char *bspwm_output)
{
	const char *types = "OoFfUu"; //desktop type specifiers

	unsigned int cnt = 0;
	for(unsigned int i = 0; i < strlen(bspwm_output); ++i) {
		if(bspwm_output[i] == ':' && strchr(types, bspwm_output[++i]) != NULL)
			++cnt;
	}

	return cnt;
}

void build_desktop_array(const char *bspwm_output, desktop **desktop_array, unsigned int *size)
{
	unsigned int new_size = count_desktops(bspwm_output);

	if(*desktop_array != NULL) {
		for(unsigned int i = 0; i < *size; ++i)
			free((*desktop_array)[i].name);
	}

	//if the number of desktops has changed
	if(*size != new_size) {
		free(*desktop_array);

		*size = new_size;
		*desktop_array = malloc(sizeof(desktop) * new_size);
	}

	//gets info describing desktops from bspwm_output
	char **desktop_info = get_desktop_info(bspwm_output);

	for(unsigned int i = 0; i < new_size; ++i) {
		(*desktop_array)[i] = create_desktop(desktop_info[i]);
		free(desktop_info[i]);
	}

	free(desktop_info);
}

char **get_desktop_info(const char *bspwm_output)
{
	unsigned int num_of_desktops = count_desktops(bspwm_output);
	char **desktop_info = malloc(sizeof(char *) * num_of_desktops);

	char *tmp = malloc(strlen(bspwm_output));
	strcpy(tmp, bspwm_output + 1); //we cut down the first character

	char *pch = strtok(tmp, ":");
	for(; pch != NULL && (pch[0] == 'M' || pch[0] == 'm'); pch = strtok(NULL, ":"));

	for(unsigned int i = 0; i < num_of_desktops; ++i, pch = strtok(NULL, ":")) {
		desktop_info[i] = malloc(strlen(pch) + 1);
		strcpy(desktop_info[i], pch);
	}

	free(tmp);

	return desktop_info;
}

desktop create_desktop(const char *desktop_info)
{
	desktop d;
	d.status = desktop_info[0];
	d.name = malloc(strlen(desktop_info));
	strcpy(d.name, desktop_info + 1);

	return d;
}

void free_desktop_array(desktop *desktop_array, unsigned int size)
{
	for(unsigned int i = 0; i < size; ++i)
		free(desktop_array[i].name);

	free(desktop_array);
}

const char *select_format(char status)
{
	switch(status) {
		case 'o':
			return occupied_format;
			break;
		case 'O':
			return occupied_a_format;
			break;
		case 'f':
			return free_format;
			break;
		case 'F':
			return free_a_format;
			break;
		case 'u':
			return urgent_format;
			break;
		case 'U':
			return urgent_a_format;
			break;
	}

	return occupied_format;
}

char *desktop_array_to_string(const desktop *desktop_array, unsigned int size)
{
	char *str = malloc(DESKTOP_SIZE);
	str[0] = 0;

	for(unsigned int i = 0; i < size; ++i) {
		char tmp[DESKTOP_BUF_SIZE];
		const char *format = select_format(desktop_array[i].status);
		sprintf(tmp, "%s%s%%{F-}%%{B-}%%{U-}%%{T-}%%{-u}%%{-o}", format, desktop_array[i].name); //reset every formatting that may have happened
		strcat(str, tmp);

		if(i < size - 1)
			strcat(str, " ");
	}

	return str;
}
