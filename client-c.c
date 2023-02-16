/*****************************************************************************
 * client-c.c                                                                 
 * Name: Lucas Irwin
 * NetId: ljirwin 
 *****************************************************************************/

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
// Added required file 
#include <arpa/inet.h>

#define SEND_BUFFER_SIZE 2048


/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/

/* Method to create a socket and send a message
*  to the server from the client */
int client(char *server_ip, char *server_port) {
  int sockfd;  
  char message[SEND_BUFFER_SIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  /* allocting memory */
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  /* error handling for getaddrinfo method */
  if ((rv = getaddrinfo(server_ip, server_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
  
  /* Loops over linked list of results and connects to first possible */
  for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

  /* Error handling: checks if client connected to socket*/
   if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

   freeaddrinfo(servinfo); /*Frees from servinfo struct */
   
  /* Reads messages from StdIn of the required buffer size
  * and sends them to the server in chunks */ 
  int bytes; 
  bytes = fread(message, 1, SEND_BUFFER_SIZE, stdin); 
  while (bytes > 0){ 
    if ((send(sockfd, message, bytes, 0) == -1)){
     perror("send failed");
     exit(1); 
    }
    bytes = fread(message, 1, SEND_BUFFER_SIZE, stdin); 
  }
  
  /* Close the socket */
  close(sockfd); 
  return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv) {
  char *server_ip;
  char *server_port;

  if (argc != 3) {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];
  return client(server_ip, server_port);
}
