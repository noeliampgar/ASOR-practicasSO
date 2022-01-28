#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netdb.h>
extern int h_errno;

#define BUF_SIZE 500

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s node\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    char buf[BUF_SIZE];
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;   /* Allow IPv4 or IPv6 */
    hints.ai_socktype = (int)NULL; /*    Datagram socket */
    hints.ai_flags = (int)NULL;    /*   For wildcard IP address */
    hints.ai_protocol = 0;         /*   Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    s = getaddrinfo(argv[1], NULL, &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
               */
    // IP numérica, familia de protocolos y tipo de socket
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        /* sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);

        if (sfd == -1)
            continue;
        else
        {*/

        char host[NI_MAXHOST],
            service[NI_MAXSERV];
        s = getnameinfo(rp->ai_addr,
                        rp->ai_addrlen, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICHOST);
        if (s != 0)
        {
            printf("Error getname");
            exit(EXIT_FAILURE);
        }
        printf("IP numérica: %s        ", host);
        printf("Familia de protocolos : %i        ", rp->ai_family);
        printf("Tipo de socket: %i\n", rp->ai_socktype);
        // }

        close(sfd);
    }

    freeaddrinfo(result); /* No longer needed */

    exit(EXIT_SUCCESS);
}
