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
Ejercicio 2. Escribir un servidor UDP de hora de forma que:
    ● La dirección y el puerto son el primer y segundo argumento del programa. Las direcciones
    pueden expresarse en cualquier formato (nombre de host, notación de punto…). Además,
    el servidor debe funcionar con direcciones IPv4 e IPv6 .
    ● El servidor recibirá un comando (codificado en un carácter), de forma que ‘t’ devuelva la
    hora, ‘d’ devuelve la fecha y ‘q’ termina el proceso servidor.
    ● En cada mensaje el servidor debe imprimir el nombre y puerto del cliente, usar
    getnameinfo(3) .
Probar el funcionamiento del servidor con la herramienta Netcat (comando nc o ncat ) como
cliente.
Nota: Dado que el servidor puede funcionar con direcciones IPv4 e IPv6, hay que usar struct
sockaddr_storage para acomodar cualquiera de ellas, por ejemplo, en recvfrom(2) .

*/
int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, s1;
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
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     0);
        if (sfd == -1)
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

        if (bind(sfd, (struct sockaddr *)rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break;
        }
        /* Success */

        close(sfd);
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
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
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
            }
            else if (strcmp(buf, "d\n") == 0)
            {
                printf("Received d\n");
                time_t tiempo = time(NULL);
                struct tm *local;
                local = localtime(&tiempo);

                size_t a = strftime(bufcl, BUF_SIZE_CL, " %d/%m/%y",
                                    local);
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

        if (sendto(sfd, bufcl, strlen(bufcl) + 1, 0,
                   (struct sockaddr *)&peer_addr,
                   peer_addr_len) != strlen(bufcl) + 1)
            fprintf(stderr, "Error sending response\n");
    }
}
