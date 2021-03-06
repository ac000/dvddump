/*
 * dvddump.c - A simple wrapper around dd_rescue.
 *
 * Copyright (C) 2015, 2019	Andrew Clayton <andrew@digital-domain.net>
 *
 * Released under the GNU General Public License version 2
 * See COPYING
 */

#define _XOPEN_SOURCE		/* for sigaction(2) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include <dvdread/dvd_reader.h>

static void reaper(int signo __attribute__ ((unused)))
{
	int status;

	wait(&status);
}

int main(int argc, char *argv[])
{
	dvd_reader_t *dvd;
	struct sigaction sa;
	pid_t pid;

	if (argc < 3) {
		printf("Usage: dvddump <dvd-device> <outfile>\n");
		exit(EXIT_FAILURE);
	}

	do {
		dvd = DVDOpen(argv[1]);
		if (!dvd) {
			printf("Trying to open device...\n");
			sleep(1);
		}
	} while (!dvd);

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = reaper;
	sigaction(SIGCHLD, &sa, NULL);

	pid = fork();
	if (pid == 0) {
		int err;

		err = execlp("ddrescue", "ddrescue", "-B", "-d", "-r", "1",
			     "-n", "-N", "-b", "2048", argv[1], argv[2],
			     (char *)NULL);
		if (err) {
			perror("execlp: dd_rescue");
			exit(EXIT_FAILURE);
		}
	}
	pause();

	DVDClose(dvd);

	exit(EXIT_SUCCESS);
}
