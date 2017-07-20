#ifndef DESKTOP_H
#define DESKTOP_H

typedef struct {
	char *name;
	char status;
} desktop;

unsigned int count_desktops(const char *bspwm_output);

void build_desktop_array(const char *bspwm_output, desktop **desktop_array, unsigned int *size);

char **get_desktop_info(const char *bspwm_output);

desktop create_desktop(const char *desktop_info);

#endif
