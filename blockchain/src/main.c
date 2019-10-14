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

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

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
    fprintf(stderr, " p2p_server getaddrinfo: %s\n", gai_strerror(rv));
    *(int *)err = 1;
    pthread_exit(err);
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, 
      p->ai_protocol)) == -1) {
        perror("p2p_server: socket");
        continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, 
      sizeof(int)) == -1) {
        perror("p2p_server setsockopt");
        exit(1);
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("p2p_server: bind");
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "p2p_server: failed to bind\n");
    exit(1);
  }
  if (listen(sockfd, SERV_BACKLOG) == -1) {
    perror(" p2p_server listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // bury zombie processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("p2p_server sigaction");
    exit(1);
  }

  printf("p2p_server: waiting for connections...\n");
  //while (1) {
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("p2p_server accept");
      exit(1);
      //continue;
    }

    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("p2p_server: got connection from %s\n", s);

    // TODO multithreaded server
    if (!fork()) { // this is the child process
      close(sockfd); // child doesn't need the listener
      if (send(new_fd, "Hello, world!", 13, 0) == -1)
        perror("send");
      close(new_fd);
      exit(0);
    }
    close(new_fd);  // parent doesn't need this
    
  //}

  pthread_exit(err);
}

void *p2p_client(void *err)
{
  int sockfd, numbytes;
  char buf[100];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];  

  *(int *)err = 0; // this will change in case of errors
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // TODO server hostname is currently hardcoded as "server" in 
  // first argument
  if ((rv = getaddrinfo("localhost", SERV_PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "p2p_client getaddrinfo: %s\n", gai_strerror(rv));
    *(int *)err = 1;
    pthread_exit(err);
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
      p->ai_protocol)) == -1) {
        perror("p2p_client: socket");
        continue;
      }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("p2p_client: connect");
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "p2p_client: failed to connect\n");
    *(int *)err = 2;
    pthread_exit(err);
  }

  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
    s, sizeof s);
  printf("p2p_client: connecting to %s\n", s);

  freeaddrinfo(servinfo);

  if ((numbytes = recv(sockfd, buf, 100-1, 0)) == -1) {
    perror("p2p_client recv");
    exit(1); // fatal
  }

  buf[numbytes] = '\0';

  printf("p2p_client: received'%s'\n", buf);
  close(sockfd);


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
    printf("p2p_server exit code: %d\n", serv_res);
  }
  else {
    fprintf(stderr, "Error code in p2p_server thread return\n");
  }

  if (!(*(int *)cli_err)) {
    printf("p2p_client exit code: %d\n", cli_res);
  }
  else {
    fprintf(stderr, "Error code in p2p_client thread return\n");
  }

  return 0;
}
