/*
main.c: main program. launches whatever necessary data structures
        and calls test functions
Copyright (C) 2019 
maintainer: Carlos WM
email: cwmoreiras@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "main.h"
#include "blockchain.h"
#include "util.h"
#include "linkedlist.h"
#include "dynarray.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <ev.h>

void *get_in_addr(struct sockaddr *sa) {
  return sa->sa_family == AF_INET
    ? (void *) &(((struct sockaddr_in*)sa)->sin_addr)
    : (void *) &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *p2p_server(void *err)
{
  int sockfd, new_fd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s [INET6_ADDRSTRLEN];
  int rv;

  *(int *)err = 0; // this will change in case of errors
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, SERV_PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddringo: %s\n", gai_strerror(rv));
    *(int *)err = 1;
    pthread_exit(err);
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }
  if (listen(sockfd, SERV_BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  printf("server: waiting for connections...\n");
  while (1) {
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("server: got connection from %s\n", s);


  }

  pthread_exit(err);
}

void *p2p_client(void *err)
{
  *(int *)err = 0; // this will change in case of errors

  pthread_exit(err);
}

int main(int argc, char *argv[]) {

  #ifndef DEBUG
  util_print_license();
  #endif

  if (argc > 1) { // process flags
    if (strlen(argv[1]) == 2 && argv[1][0] == '-') {
      switch (argv[1][1]) {
        case 'h': util_cmd_hash(argv[2]); break;
        default: printf("Command line argument is not recognized\n");
      }
    }
    else {
      printf("Command line argument is malformed\n");
    }
    return 0;
  }

  pthread_t p2p_server_thread, p2p_client_thread;
  int serv_res, cli_res;
  void *serv_err = 0, *cli_err = 0;

  if (pthread_create(&p2p_server_thread, NULL, p2p_server, &serv_res)) {
    fprintf(stderr, "Error creating p2p_server thread\n");
  }
  if (pthread_create(&p2p_client_thread, NULL, p2p_client, &cli_res)) {
    fprintf(stderr, "Error creating client thread\n");
  }

  if (pthread_join(p2p_server_thread, &serv_err)) {
    fprintf(stderr, "Error joining p2p_client thread\n");
  }
  if (pthread_join(p2p_client_thread, &cli_err)) {
    fprintf(stderr, "Error joining p2p_client thread\n");
  }
  
  if (!(*(int *)serv_err)) {
    printf("%d\n", serv_res);
  }
  else {
    fprintf(stderr, "Error code in p2p_server thread return\n");
  }

  if (!(*(int *)cli_err)) {
    printf("%d\n", cli_res);
  }
  else {
    fprintf(stderr, "Error code in p2p_client thread return\n");
  }

  return 0;
}
