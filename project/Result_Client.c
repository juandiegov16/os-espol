/*Fuente del comportamiento cliente-servidor y ciertos comandos auxiliares: https://github.com/juandiegov16/proyecto-fileserver (Proyecto MÍO de Programación de Sistemas*/

/* Linux Headers */
#include <sys/mman.h> /* Includes mmap(), unmmap(). */
#include <sys/stat.h>

#include <fcntl.h> /* Contains O_ constants. */
#include <unistd.h>
#include <semaphore.h>
#include <libgen.h>

/* C Standard Headers */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Provides access to more features to program_invocation_short_name
 * in errno.h and psignal() in signal.h.
 */
#define __GNU_SOURCE
#include <signal.h>
#include <errno.h>
#include "dijsktra.h"
#include "csapp.h"
#include <limits.h>
#include <stdbool.h>
#include <getopt.h>

bool hflag = false;

void print_help(char *command);
void connection_error(int connfd);

int main(int argc, char *argv[])
{
     int opt;
     //Socket
     int clientfd;
     //Direcciones y puertos
     char *hostname, *port;

     while ((opt = getopt(argc, argv, "h")) != -1)
     {
          switch (opt)
          {
          case 'h':
               print_help(argv[0]);
               return 0;

          case '?':
               fprintf(stderr, "Opción inválida.\n");
               return 0;
               break;

          default:
               fprintf(stderr, "Uso:\n");
               fprintf(stderr, " %s <ip> <puerto>\n", argv[0]);
               fprintf(stderr, " %s -h\n", argv[0]);
               return -1;
          }
     }

     if (argc != 3 && hflag == false)
     {
          fprintf(stderr, "Uso:\n");
          fprintf(stderr, " %s <ip> <puerto>\n", argv[0]);
          fprintf(stderr, " %s -h\n", argv[0]);
          exit(0);
     }
     else
     {
          hostname = argv[1];
          port = argv[2];
     }

     //Valida el puerto
     int port_n = atoi(port);
     if (port_n <= 0 || port_n > USHRT_MAX)
     {
          fprintf(stderr, "Puerto %s es inválido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
          return -1;
     }
     clientfd = open_clientfd(hostname, port);
     if (clientfd < 0)
          connection_error(clientfd);

     printf("\nConectado exitosamente a %s en puerto %s.\n\n", hostname, port);
}

void print_help(char *command)
{
     printf("%s se conecta a un servidor de donde obtenemos una matriz, la cual se puede visualizar y modificar.\n\n", command);
     printf("Uso:\n");
     printf(" %s <ip> <puerto>\n", command);
     printf(" %s -h\n", command);
     printf("Opciones:\n");
     printf(" -h\t\t\tAyuda, mostrar este mensaje.\n");
}

void connection_error(int connfd)
{
     fprintf(stderr, "Error de conexión: %s\n", strerror(errno));
     close(connfd);
     exit(-1);
}