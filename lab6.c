#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#define N 100
#define True 1

int main (int argc, char *argv[]) {
	struct timeval tv1;
	long fileOffsets[N] = {0};
	int fileDescriptor;
	int lineLength[N] = {-1};
	int bufSize = 512;
	char *buf = (char *)malloc (sizeof (char) * bufSize);

	if (buf == NULL) {
		perror ("Error memory allocation ");
		exit (1);
	}
	if ((fileDescriptor = open ("test.txt", O_RDONLY)) == -1) {
		perror ("File doesn't open");
		free (buf);
		exit (1);
	}

	int i = 1, j = 0;
	int r = 1;
	int oldFileOffsets = 0;
	while (r) {
		r = read(fileDescriptor, buf, bufSize);
		if (r == -1) {
			if (errno == EAGAIN || errno == EINTR) {
				r = 1;
			} else {
				r = 0;
				fprintf(stderr, "Error read occurred\n");
			}
		}
		for (int k = 0; k < r; ++k) {
			++j;
			if(buf[k] == '\n') {
				lineLength[i] = j;
				fileOffsets[++i] = j + oldFileOffsets;
				oldFileOffsets += j;
				j = 0;
			}
		}
	}
	int lineNumber = 0;
	int retval;
	tv1.tv_sec = 5;
	tv1.tv_usec = 0;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(fileno (stdin), &readfds);
	while (True) {
		retval = select (fileno (stdin) + 1, &readfds, NULL, NULL, &tv1);
		if (retval == 0) {
			break;
		} else if (retval == -1) {
			fprintf (stderr, "Select error");
			free (buf);
			if (close (fileDescriptor) != 0) {
				fprintf (stderr, "Cannot close file (descriptor=%d)\n",
						 fileDescriptor);
			}
			exit (1);
		}
		scanf ("%d", &lineNumber);
		if (!lineNumber) {
			free (buf);
			if (close (fileDescriptor) != 0) {
				fprintf (stderr, "Cannot close file (descriptor=%d)\n",
						 fileDescriptor);
				exit (1);
			}
			exit (0);
		}

		if (lineNumber < 0 || lineNumber > (N - 1)|| (fileOffsets[lineNumber
		+ 1] == 0)) {
			fprintf (stderr, "wrong line number \n");
			continue;
		}

		lseek (fileDescriptor, fileOffsets[lineNumber], SEEK_SET);
		if (lineLength[lineNumber] > bufSize) {
			if (realloc (buf, lineLength[lineNumber] * sizeof (char))
				== NULL) {
				perror ("Error memory allocation");
				free (buf);
				if (close (fileDescriptor) != 0) {
					fprintf (stderr, "Cannot close file (descriptor=%d)\n",
							 fileDescriptor);
				}
				exit (1);
			}
			bufSize = lineLength[lineNumber];
		}

		r = 1;
		int w = -1;
		while (r) {
			r = read(fileDescriptor, buf, lineLength[lineNumber]);
			if (r == -1) {
				if (errno == EAGAIN || errno == EINTR) {
					r = 1;
				} else {
					r = 0;
					fprintf(stderr, "Error read occurred\n");
				}
			}
			while (w != lineLength[lineNumber]) {
				w = write (STDOUT_FILENO, buf, lineLength[lineNumber]);
				if (w == -1) {
					if (errno == EAGAIN || errno == EINTR) {
						continue;
					} else {
						w = lineLength[lineNumber];
						fprintf(stderr, "Error write occurred\n");
					}
				}
			}
		}
	}
	lseek (fileDescriptor, 0, SEEK_SET);
	r = 1;
	while (r != 0) {
		r = read(fileDescriptor, buf, bufSize);
		if (r == -1) {
			if (errno == EAGAIN || errno == EINTR) {
				r = 1;
			} else {
				r = 0;
				fprintf(stderr, "Error read occurred\n");
			}
		}
		int w = -1;
		while (w != r) {
			w = write (STDOUT_FILENO, buf, r);
			if (w == -1) {
				if (errno == EAGAIN || errno == EINTR) {
					continue;
				} else {
					w = lineLength[lineNumber];
					fprintf(stderr, "Error write occurred\n");
				}
			}
		}
	}

	free (buf);
	if (close (fileDescriptor) != 0) {
		fprintf (stderr, "Cannot close file (descriptor=%d)\n",
				 fileDescriptor);
		exit (1);
	}
	exit (0);
}#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#define N 100
#define True 1

int main (int argc, char *argv[]) {
	struct timeval tv1;
	long fileOffsets[N] = {0};
	int fileDescriptor;
	int lineLength[N] = {-1};
	int bufSize = 512;
	char *buf = (char *)malloc (sizeof (char) * bufSize);

	if (buf == NULL) {
		perror ("Error memory allocation ");
		exit (1);
	}
	if ((fileDescriptor = open ("test.txt", O_RDONLY)) == -1) {
		perror ("File doesn't open");
		free (buf);
		exit (1);
	}

	int i = 1, j = 0;
	int r = 1;
	int oldFileOffsets = 0;
	while (r) {
		r = read(fileDescriptor, buf, bufSize);
		if (r == -1) {
			if (errno == EAGAIN || errno == EINTR) {
				r = 1;
			} else {
				r = 0;
				fprintf(stderr, "Error read occurred\n");
			}
		}
		for (int k = 0; k < r; ++k) {
			++j;
			if(buf[k] == '\n') {
				lineLength[i] = j;
				fileOffsets[++i] = j + oldFileOffsets;
				oldFileOffsets += j;
				j = 0;
			}
		}
	}
	int lineNumber = 0;
	int retval;
	tv1.tv_sec = 5;
	tv1.tv_usec = 0;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(fileno (stdin), &readfds);
	while (True) {
		retval = select (fileno (stdin) + 1, &readfds, NULL, NULL, &tv1);
		if (retval == 0) {
			break;
		} else if (retval == -1) {
			fprintf (stderr, "Select error");
			free (buf);
			if (close (fileDescriptor) != 0) {
				fprintf (stderr, "Cannot close file (descriptor=%d)\n",
						 fileDescriptor);
			}
			exit (1);
		}
		scanf ("%d", &lineNumber);
		if (!lineNumber) {
			free (buf);
			if (close (fileDescriptor) != 0) {
				fprintf (stderr, "Cannot close file (descriptor=%d)\n",
						 fileDescriptor);
				exit (1);
			}
			exit (0);
		}

		if (lineNumber < 0 || lineNumber > (N - 1)|| (fileOffsets[lineNumber
		+ 1] == 0)) {
			fprintf (stderr, "wrong line number \n");
			continue;
		}

		lseek (fileDescriptor, fileOffsets[lineNumber], SEEK_SET);
		if (lineLength[lineNumber] > bufSize) {
			if (realloc (buf, lineLength[lineNumber] * sizeof (char))
				== NULL) {
				perror ("Error memory allocation");
				free (buf);
				if (close (fileDescriptor) != 0) {
					fprintf (stderr, "Cannot close file (descriptor=%d)\n",
							 fileDescriptor);
				}
				exit (1);
			}
			bufSize = lineLength[lineNumber];
		}

		r = 1;
		int w = -1;
		while (r) {
			r = read(fileDescriptor, buf, lineLength[lineNumber]);
			if (r == -1) {
				if (errno == EAGAIN || errno == EINTR) {
					r = 1;
				} else {
					r = 0;
					fprintf(stderr, "Error read occurred\n");
				}
			}
			while (w != lineLength[lineNumber]) {
				w = write (STDOUT_FILENO, buf, lineLength[lineNumber]);
				if (w == -1) {
					if (errno == EAGAIN || errno == EINTR) {
						continue;
					} else {
						w = lineLength[lineNumber];
						fprintf(stderr, "Error write occurred\n");
					}
				}
			}
		}
	}
	lseek (fileDescriptor, 0, SEEK_SET);
	r = 1;
	while (r != 0) {
		r = read(fileDescriptor, buf, bufSize);
		if (r == -1) {
			if (errno == EAGAIN || errno == EINTR) {
				r = 1;
			} else {
				r = 0;
				fprintf(stderr, "Error read occurred\n");
			}
		}
		int w = -1;
		while (w != r) {
			w = write (STDOUT_FILENO, buf, r);
			if (w == -1) {
				if (errno == EAGAIN || errno == EINTR) {
					continue;
				} else {
					w = lineLength[lineNumber];
					fprintf(stderr, "Error write occurred\n");
				}
			}
		}
	}

	free (buf);
	if (close (fileDescriptor) != 0) {
		fprintf (stderr, "Cannot close file (descriptor=%d)\n",
				 fileDescriptor);
		exit (1);
	}
	exit (0);
}
