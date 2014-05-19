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
#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define NUMPACKS 5
#define PORT 12345
#define SRV_IP "127.0.0.1"


#define EXIT_FAILURE 1

void diep(char *s)
{
        perror(s);
        exit(1);
}

int
main (int argc, char **argv)
{
	send_stamp send_buf;
	const void *pointr;

        struct sockaddr_in si_other;
        int s, i, slen=sizeof(si_other);

        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
                diep("socket");

        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(PORT);
        if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
                fprintf(stderr, "inet_aton() failed\n");
                exit(1);
        }

        for (i=0; i<NUMPACKS; i++) {
                printf("Sending packet %d\n", i);
                send_buf.T1 = 123456;
                send_buf.T2 = 0x0;
                send_buf.T3 = 0x0;
                send_buf.T4 = 0x2348CA;
		pointr = &send_buf;
		
                if (sendto(s, pointr, 16, 0, &si_other, slen)==-1)
                        diep("sendto()");
        }

        close(s);

        return 0;
}
