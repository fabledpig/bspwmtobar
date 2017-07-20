#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "desktop.h"

#define BUF_SIZE 512

int main(int argc, char *argv[])
{
	const char *fifo_path = "/tmp/bspwmtobar-fifo";
	mkfifo(fifo_path, 0666);
	int fd = open(fifo_path, O_RDONLY);

	char buf[BUF_SIZE];
	int bytes_read;
	while((bytes_read = read(fd, buf, BUF_SIZE - 1)) > 0) {
		buf[bytes_read] = 0;
		printf("%s", buf);
	}

	close(fd);
	unlink(fifo_path);
	return 0;
}
