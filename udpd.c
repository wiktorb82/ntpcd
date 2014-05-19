/*
   udpdc - server and client

   Copyright (C) 2014 virtualbox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include <termios.h>
#include <grp.h>
#include <pwd.h>
*/

#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>
#include "system.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define NUMPACKS 5
#define PORT 12345


#define EXIT_FAILURE 1

char *xmalloc ();
char *xrealloc ();
char *xstrdup ();


static void usage (int status);

/* The name the program was run with, stripped of any leading path. */
char *program_name;

/* getopt_long return codes */
enum {DUMMY_CODE=129
     };

/* Option flags and variables */


static struct option const long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {NULL, 0, NULL, 0}
};

static int decode_switches (int argc, char **argv);
void diep(char *s)
{
        perror(s);
        exit(1);
}

int
main (int argc, char **argv)
{
	send_stamp recv_buf;
	const void *point_r=&recv_buf;

        struct sockaddr_in si_me, si_other;
        int s, i, slen=sizeof(si_other);
        char buf[BUFLEN];

        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	{
		diep("bind");
	}

        memset((char *) &si_me, 0, sizeof(si_me));
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(PORT);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(s, &si_me, sizeof(si_me))==-1)
	{
		diep("bind");
	}

        for (i=0; i<NUMPACKS; i++) {
                if (recvfrom(s, point_r, 16, 0, &si_other, &slen)==-1)
		{
			diep("recvfrom()");
		}

                printf("Received packet from %s:%d\n",
			inet_ntoa(si_other.sin_addr),
			ntohs(si_other.sin_port));
		printf("Data: T1:%f\nT2:%f\nT3:%f\nT4:%f\n",
			recv_buf.T1,
			recv_buf.T2,
			recv_buf.T3,
			recv_buf.T4
			);
        }

        close(s);
        return 0;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

static int
decode_switches (int argc, char **argv)
{
        int c;


        while ((c = getopt_long (argc, argv,
                                 "h"	/* help */
                                 "V",	/* version */
                                 long_options, (int *) 0)) != EOF) {
                switch (c) {
                case 'V':
                        printf ("udpdc %s\n", VERSION);
                        exit (0);

                case 'h':
                        usage (0);

                default:
                        usage (EXIT_FAILURE);
                }
        }

        return optind;
}


static void
usage (int status)
{
        printf (_("%s - \
server and client\n"), program_name);
        printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
        printf (_("\
Options:\n\
  -h, --help                 display this help and exit\n\
  -V, --version              output version information and exit\n\
"));
        exit (status);
}
