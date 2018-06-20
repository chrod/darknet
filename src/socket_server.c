#include "socket_server.h"

#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8750

#define TRUE 1

void sock_error(char *msg)
{
    perror(msg);
    exit(1);
}

//int main(int argc, char *argv[]) {
int socket_server(char* data_str) {
    setlinebuf(stdout);

    int sockfd, newsockfd, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
      sock_error("ERROR opening socket");
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    printf("Server listening on %d\n", PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      sock_error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Waiting to accept client connections\n");
    while (TRUE) {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

      if (newsockfd < 0) {
        sock_error("ERROR on accept");
      }

      printf("New client connection\n");

      pid = fork();
      if (pid < 0) {
          sock_error("ERROR on fork");
      }

      if (pid == 0)  {
          handle_client_comm(newsockfd, data_str);
          close(newsockfd);
      } else {
        close(newsockfd);
      }
    }

    return(0);
}

void handle_client_comm(int sock, char* data_str) {
  char c[20];
  int n;

  //for (int ix=100; ix > 0; ix--) {
  //  sprintf(c, "%d", ix);
  n = write(sock, &data_str, sizeof(data_str));

   // if (n < 0) {
   //   break;
   // }

  //  sleep(1);
  //}

  //printf("Finished streaming data to a client, closing socket\n");
  if (n < 0) {
    close(sock);
  }
}
