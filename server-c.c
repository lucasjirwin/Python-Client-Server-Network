/*****************************************************************************
 * server-c.c                                                                 
 * Name: Lucas Irwin
 * NetId: ljirwin 
 *****************************************************************************/

/* Imports */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

/* Defining constants */
#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048


/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(char *server_port) {

  int sockfd, new_fd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t sin_size; 
  int yes=1; 
  char s[INET6_ADDRSTRLEN];
  int rv;
  char message[RECV_BUFFER_SIZE];

  memset(&hints, 0, sizeof hints); 
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; 

  if ((rv = getaddrinfo("localhost", server_port, &hints, &servinfo) != 0)){
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));
    exit(1);
}

  for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            return 1;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

  freeaddrinfo(servinfo);

  if (p == NULL)  {
    fprintf(stderr, "server: failed to bind\n");
     exit(1);
    }
  
  /* Listens to a maximum number of clients euqal to the Queue length
  */ 
  if (listen(sockfd, QUEUE_LENGTH) == -1) {
     perror("listen");
     exit(1);
    }
  
  /*Loops over client sockets while they exist 
  * and writes sent messages to stdout 
  */ 
  while(1) {  
        sin_size = sizeof their_addr;

        /* Error handling for accepting sockets
         */
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
   
         /* Writes messages sent from clients to the server to stdout 
         * with message lengths equal to the required buffer size 
         */
        int bytes_received;
        while ((bytes_received = recv(new_fd, message, RECV_BUFFER_SIZE - 1, 0)) > 0) {
         message[bytes_received] = '\0';
         fwrite(message, sizeof(char), bytes_received, stdout);
         fflush(stdout); }

        /*Closes the socket for the current client */
        close(new_fd);
    }

  
    return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) {
  char *server_port;
  server_port = argv[1];
  if (argc != 2) {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  }

  return server(server_port);
}
