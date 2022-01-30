/*Fuente del comportamiento cliente-servidor: https://github.com/juandiegov16/proyecto-fileserver (Proyecto MÍO de Programación de Sistemas*/

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

void *thread(void *vargp);
sem_t mutex;

void print_help(char *command);
void atender_cliente(int connfd);
void recoger_hijos(int signal);
void connection_error(int connfd);

int main(int argc, char *argv[])
{
     int opt;
     //Sockets
     int listenfd, *connfd;
     unsigned int clientlen;
     //Direcciones y puertos
     struct sockaddr_in clientaddr;
     char *port;
     pthread_t tid;

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

          default:
               fprintf(stderr, "Uso:\n");
               fprintf(stderr, " %s <port>\n", argv[0]);
               fprintf(stderr, " %s -h\n", argv[0]);
               return -1;
          }
     }

     if (argc < 2)
     {
          fprintf(stderr, "Uso:\n");
          fprintf(stderr, " %s <puerto>\n", argv[0]);
          fprintf(stderr, " %s -h\n", argv[0]);
          exit(0);
     }
     port = argv[1];

     //Valida el puerto
     int port_n = atoi(port);
     if (port_n <= 0 || port_n > USHRT_MAX)
     {
          fprintf(stderr, "Puerto %s es inválido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
          return -1;
     }

     signal(SIGCHLD, recoger_hijos);

     listenfd = Open_listenfd(port);

     if (listenfd < 0)
          connection_error(listenfd);

     printf("\nServidor escuchando en puerto %s...\n\n", port);
     Sem_init(&mutex, 0, 1);

     while (1)
     {
          clientlen = sizeof(clientaddr);
          connfd = Malloc(sizeof(int));
          *connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
          Pthread_create(&tid, NULL, thread, connfd);
     }
}

void print_help(char *command)
{
     printf("Uso:\n");
     fprintf(stderr, " %s <port>\n", command);
     printf(" %s -h\n", command);
     printf("Opciones:\n");
     printf(" -h\t\t\tAyuda, muestra este mensaje.\n");
}

void connection_error(int connfd)
{
     fprintf(stderr, "Error de conexión: %s\n", strerror(errno));
     Close(connfd);
     exit(-1);
}

void *thread(void *vargp)
{
     int connfd = *((int *)vargp);
     pthread_detach(pthread_self());
     Free(vargp);
     atender_cliente(connfd);
     Close(connfd);
     return NULL;
}

void atender_cliente(int connfd)
{
     printf("Cliente en atención.\n");
}

void recoger_hijos(int signal)
{
     while (waitpid(-1, 0, WNOHANG) > 0)
          ;

     return;
}