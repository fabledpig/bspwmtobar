#ifdef DESKTOP_H
#define DESKTOP_H

typedef struct desktop_ {
	char *name;
	char status;
	desktop_ *next;
} desktop, *desktop_list;

void add_desktop(desktop_list dl, desktop *d);

void remove_last(desktop_list dl);

desktop *create_desktop(const char *name, char status);

void change_desktop(desktop *d, const char *name, char status)

#endif
