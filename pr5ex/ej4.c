#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

#define BUF_SIZE 2
#define BUF_SIZE_CL 30
/*
Ejercicio 4. Modificar el servidor para que, además de poder recibir comandos por red, los pueda
recibir directamente por el terminal, leyendo dos caracteres (el comando y ‘\n’) de la entrada
estándar. Multiplexar el uso de ambos canales usando select(2).
*/
int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int fdr, s, s1;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

    char bufcl[BUF_SIZE_CL];
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully bind(2).
        If socket(2) (or bind(2)) fails, we (close the socket
        and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        fdr = socket(rp->ai_family, rp->ai_socktype,
                     0);
        if (fdr == -1)
            continue;

        char host1[NI_MAXHOST],
            service1[NI_MAXSERV];
        s1 = getnameinfo(rp->ai_addr,
                         rp->ai_addrlen, host1, NI_MAXHOST,
                         service1, NI_MAXSERV, NI_NUMERICHOST);
        if (s1 != 0)
        {
            printf("Error getname");
            exit(EXIT_FAILURE);
        }
        printf("IP numérica: %s        ", host1);
        printf("Familia de protocolos : %i        ", rp->ai_family);
        printf("Tipo de socket: %i\n", rp->ai_socktype);

        if (bind(fdr, (struct sockaddr *)rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break;
        }
        /* Success */

        close(fdr);
    }
    if (rp == NULL)
    { /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result); /* No longer needed */

    /* Read datagrams and echo them back to sender */

    while (1)
    {
        fd_set rfds;
        struct timeval tv;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(fdr, &rfds);

        /* Wait up to five seconds. */
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        retval = select(fdr + 1, &rfds, NULL, NULL, &tv);
        /* Don't rely on the value of tv now! */
        if (retval == -1)
            perror("select()");
        else if (retval)
        {
            if (FD_ISSET(0, &rfds))
            {

                printf("Received message from entrada estandar \n");

                read(0, buf, 2);
                if (strcmp(buf, "t\n") == 0)
                {
                    printf("Received t\n");
                    time_t tiempo = time(NULL);
                    struct tm *local;
                    local = localtime(&tiempo);

                    size_t a = strftime(bufcl, BUF_SIZE_CL, "%H:%M:%S",
                                        local);
                    printf("%s\n", bufcl);
                }
                else if (strcmp(buf, "d\n") == 0)
                {
                    printf("Received d\n");
                    time_t tiempo = time(NULL);
                    struct tm *local;
                    local = localtime(&tiempo);

                    size_t a = strftime(bufcl, BUF_SIZE_CL, " %d/%m/%y",
                                        local);
                    printf("%s\n", bufcl);
                }
                else if (strcmp(buf, "q\n") == 0)
                {
                    printf("Received q\n");
                    break;
                }
                else
                {
                    printf("Nothing important\n");
                }
            }
            if (FD_ISSET(fdr, &rfds))
            {
                printf("Net gives data.\n");

                peer_addr_len = sizeof(struct sockaddr_storage);
                nread = recvfrom(fdr, buf, BUF_SIZE, 0,
                                 (struct sockaddr *)&peer_addr, &peer_addr_len);
                if (nread == -1)
                    continue; /* Ignore failed request */

                char host[NI_MAXHOST], service[NI_MAXSERV];

                s = getnameinfo((struct sockaddr *)&peer_addr,
                                peer_addr_len, host, NI_MAXHOST,
                                service, NI_MAXSERV, NI_NUMERICSERV);
                if (s == 0)
                {
                    printf("Received %ld bytes from %s:%s\n",
                           (long)nread, host, service, buf);
                    if (strcmp(buf, "t\n") == 0)
                    {
                        printf("Received t\n");
                        time_t tiempo = time(NULL);
                        struct tm *local;
                        local = localtime(&tiempo);

                        size_t a = strftime(bufcl, BUF_SIZE_CL, "%H:%M:%S",
                                            local);
                        if (sendto(fdr, bufcl, strlen(bufcl) + 1, 0,
                                   (struct sockaddr *)&peer_addr,
                                   peer_addr_len) != strlen(bufcl) + 1)
                            fprintf(stderr, "Error sending response\n");
                    }
                    else if (strcmp(buf, "d\n") == 0)
                    {
                        printf("Received d\n");
                        time_t tiempo = time(NULL);
                        struct tm *local;
                        local = localtime(&tiempo);

                        size_t a = strftime(bufcl, BUF_SIZE_CL, " %d/%m/%y",
                                            local);
                        if (sendto(fdr, bufcl, strlen(bufcl) + 1, 0,
                                   (struct sockaddr *)&peer_addr,
                                   peer_addr_len) != strlen(bufcl) + 1)
                            fprintf(stderr, "Error sending response\n");
                    }
                    else if (strcmp(buf, "q\n") == 0)
                    {
                        printf("Received q\n");
                        break;
                    }
                    else
                    {
                        printf("Nothing important\n");
                    }
                }

                else
                    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
            }
        }
        else
            printf("No data within five seconds.\n");
    }
    exit(EXIT_SUCCESS);
}
