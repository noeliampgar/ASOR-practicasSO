#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*Ejercicio 2. Para la comunicación bi-direccional, es necesario crear dos tuberías, una para cada
sentido: p_h y h_p . Escribir un programa que implemente el mecanismo de sincronización de parada
y espera:
    - El padre leerá de la entrada estándar (terminal) y enviará el mensaje al proceso hijo,
    escribiéndolo en la tubería p_h . Entonces permanecerá bloqueado esperando la
    confirmación por parte del hijo en la otra tubería, h_p .
    - El hijo leerá de la tubería p_h , escribirá el mensaje por la salida estándar y esperará 1
    segundo. Entonces, enviará el carácter ‘l’ al proceso padre, escribiéndolo en la tubería
    h_p , para indicar que está listo. Después de 10 mensajes enviará el carácter ‘q’ para indicar
    al padre que finalice.*/

int main(int argc, char *argv[])
{
    int p_h[2];
    int h_p[2];

    pid_t pid;
    char buf;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(p_h) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(h_p) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)      //HIJOOOOOOOOOOOOOOOOOOOOOO
    {                  /* Child reads from pipe */
        close(p_h[1]); /* Close unused write end */
        close(h_p[0]); /* Close unused write end */
        printf("Hijo está escribiendo lo enviado por el padre.\n");

        while (read(p_h[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        close(h_p[0]); /* Close unused write end */

        write(STDOUT_FILENO, "\n", 1);

        sleep(1);
        char *l = "l";
        char *m = "m";

        char *q = "q";

        write(h_p[1], l, 1);
        for (int i = 0; i < 10; ++i)
            write(h_p[1], m, 1);
        write(h_p[1], q, 1);
        close(h_p[1]); /* Close unused write end */

        _exit(EXIT_SUCCESS);
    }
    else               //PADREEEEEEEEEEEEEEEEEEEEE
    {                  /* Parent writes argv[1] to pipe */
        close(p_h[0]); /* Close unused read end */
        close(h_p[1]); /* Reader will see EOF */

        write(p_h[1], argv[1], strlen(argv[1]));
        printf("Padre está escribiendo lo enviado por el hijo.\n");
        close(p_h[1]); /* Close unused write end */

        while (read(h_p[0], &buf, 1) > 0 && buf != 'q')
            write(1, &buf, 1);
        close(h_p[0]); /* Reader will see EOF */

        sleep(1);
        exit(EXIT_SUCCESS);
    }
}
