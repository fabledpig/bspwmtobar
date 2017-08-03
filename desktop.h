#ifndef DESKTOP_H
#define DESKTOP_H

typedef struct {
	char *name;
	char status;
} desktop;

extern char occupied_format[], occupied_a_format[], free_format[], free_a_format[],
			urgent_format[], urgent_a_format[];

void build_desktop_array(const char *bspwm_output, desktop **desktop_array, unsigned int *size);

void free_desktop_array(desktop *desktop_array, unsigned int size);

char *desktop_array_to_string(const desktop *desktop_array, unsigned int size);

#endif
