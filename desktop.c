#include <stdlib.h>
#include <string.h>

#include "desktop.h"

void add_desktop(desktop_list dl, desktop *d)
{
	d->next = NULL;

	if(dl == NULL) {
		dl = d;
		return;
	}

	desktop *i;
	for(i = dl; i->next != NULL; i = i->next);

	i->next = d;
}

void remove_last(desktop_list dl)
{
	if(dl == NULL)
		return;

	if(dl->next == NULL) {
		dl = NULL;
		return;
	}

	desktop *i;
	for(i = dl; i->next->next != NULL; i = i->next);

	free(i->next);
	i->next = NULL;
}

desktop *create_desktop(const char *name, char status)
{
	desktop *d = malloc(sizeof(desktop));
	d->name = malloc(strlen(name) + 1);
	strcpy(d->name, name);
	d->status = status;
	d->next = NULL;

	return d;
}

void change_desktop(desktop *d, const char *name, char status)
{
	if(strcmp(desktop->name, name)) {
		free(desktop->name);
		d->name = malloc(strlen(name) + 1);
		strcpy(d->name, name);
	}

	d->status = status;
}
