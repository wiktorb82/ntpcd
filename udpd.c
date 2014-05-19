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
#include "ntpdc.h"
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
