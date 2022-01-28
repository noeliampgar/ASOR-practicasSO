
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/*Ejercicio 5. Escribir un programa que muestre los identificadores del proceso: identificador de
proceso, de proceso padre, de grupo de procesos y de sesión. Mostrar además el número máximo
de ficheros que puede abrir el proceso y el directorio de trabajo actual.*/

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { /* Code executed by child */

        /*La salida estándar al fichero /tmp/daemon.out.
          La salida de error estándar al fichero /tmp/daemon.err.
          La entrada estándar a /dev/null.*/
        pid_t sesion = setsid();
        if (sesion == -1)
        {
            perror("setsid");
            exit(1);
        }
        int op1 = open("/tmp/daemon.out", O_WRONLY | O_CREAT, 0666);
        int op2 = open("/tmp/daemon.err", O_WRONLY | O_CREAT, 0666);
        int op0 = open("/dev/null", O_WRONLY | O_CREAT, 0666);
        if (op1 == -1 || op2 == -1 || op0 == -1)
        {

            perror("open");
            exit(1);
        }
        int d21 = dup2(op1, 1);
        int d22 = dup2(op2, 2);
        int d20 = dup2(op0, 0);
        if (d21 == -1 || d22 == -1 || d20 == -1)
        {
            perror("dup2");
            exit(1);
        }
        pid_t pid = getpid();

        printf("Proceso actual: %i\n", pid);
        printf("Proceso padre: %i\n", getppid());
        /*char *newargv[] = {"bash", "-c", NULL, NULL};

        newargv[2] = argv[1];

        execvp("/bin/bash", newargv);
        /*printf("El comando termino de ejecutarse\n");*/

        execvp(argv[1], argv + 1);

        fprintf(stderr, "Error execvp actual: %i\n", pid);

        perror("execvp"); /* execvp() returns only on error */
        exit(EXIT_FAILURE);
    }
    else
    { /* Code executed by parent */
        printf("Parent PID is %ld\n", (long)getpid());
        sleep(30);
    }
}
