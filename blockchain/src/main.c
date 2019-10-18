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
#include <ev.h>
#include <pthread.h>
#include <fcntl.h>

int main(int argc, char *argv[]) 
// -----------------------------------------------------------------------------
// Func: check command line arguments and start p2p server and client threads
// Args: argc - number of command line arguments
//       argv - command line arguments
// Retn: 0 unless there is an error
// -----------------------------------------------------------------------------
{
  int i;

  // event handling variables
  struct ev_loop *mainloop = EV_DEFAULT;
  ev_io accept_watcher;

  // networking variables
  int lsock;

  startup(argc, argv); // handles command line arguments, print license logic
  global_sig_attach(); // attach all signal handlers

  memset(cio_table, 0, sizeof cio_table);

  lsock = get_listener_socket(LISTEN_PORT); // get a listener socket 
  if (lsock == -1) {
    perror("main get_listener_socket");
    return -1;
  }

  if (listen(lsock, SERV_BACKLOG) == -1) {
    perror("main listen");
    return -1;
  }
  // Initialize and start a watcher to accept client requests
  // attach function accept_cb as the callback with the listen socket as an arg
  ev_io_init(&accept_watcher, accept_cb, lsock, EV_READ);
  ev_io_start(mainloop, &accept_watcher);


  printf("Started server event loop\n");
  ev_run(mainloop, 0); // run listener callback under main event loop

  return 0;
}

// event-driven wrapper for accept
// ensures that we're not blocked on the accept call
void accept_cb(struct ev_loop *loop, ev_io *watcher, int revents) {
  struct sockaddr_storage their_addr;
  socklen_t sin_size = sizeof their_addr;
  char s [INET6_ADDRSTRLEN];
  int index;
  ev_io *read_watcher;

  if (EV_ERROR & revents) {
    perror("got invalid listener event");
    return;
  }

  // find the lowest usable index for this client's entry in the
  // client table
  index = get_client_id(cio_table, MAX_CONCURRENT_REQUESTS);

  cio_table[index].status = 1;
  cio_table[index].sock = accept(watcher->fd, (struct sockaddr *)&their_addr, &sin_size);
  // fcntl(cio_table[index].sock, F_SETFL, O_NONBLOCK)
  fcntl(cio_table[index].sock, 4, 04000);
  
  if (cio_table[index].sock == -1) {
    perror("accept_cb");
    return;
  }
  cio_table[index].id = index;

  inet_ntop(their_addr.ss_family,
    get_in_addr((struct sockaddr *)&their_addr),
    s, sizeof s);
  printf("server: accepted connection from %s\n", s);

  // these will have to be freed in the read callback
  read_watcher = malloc(sizeof(ev_io));
  read_watcher->data = &cio_table[index]; // so that callback can access client info through watcher
  
  ev_io_init(read_watcher, read_cb, cio_table[index].sock, EV_READ);
  ev_io_start(loop, read_watcher); // add it to this loop
}

void read_cb(struct ev_loop *loop, ev_io *watcher, int revents) {
  ClientIO *cio = (ClientIO *)watcher->data; // get all the info on this client

  // TODO do stuff here
  // depending on what type of request has been made, we may have to do 
  // some different things
  char buf[100];
  read(cio->sock, buf, 100);
  printf("%s", buf);

  // cleanup
  close(cio->sock);
  cio->id = 0;
  cio->status = 0;

  ev_io_stop(loop, watcher); 
  free(watcher);
  printf("server: Disconnected peer\n");
}

int get_client_id(ClientIO *cio_table, int sz) {
  int i;

  for (i = 0; i < sz; i++) {
    if(cio_table[i].status == 0) {
      return i;
    }

  }

  return -1;
}

int get_listener_socket(const char *port) {
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int lsock;
  int yes = 0;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, " server getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((lsock = socket(p->ai_family, p->ai_socktype, 
      p->ai_protocol)) == -1) {
        perror("server: socket");
        continue;
    }

    // fcntl(lsock, F_SETFL, O_NONBLOCK)
    // TODO is this not portable?
    fcntl(lsock, 4, 04000);

    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, 
      sizeof(int)) == -1) {
        perror("server setsockopt");
        return -1;
    }
    if (bind(lsock, p->ai_addr, p->ai_addrlen) == -1) {
      close(lsock);
      perror("server: bind");
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);
  
  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    return -1;
  }

  return lsock;
}

void *get_in_addr(struct sockaddr *sa) 
{
  return sa->sa_family == AF_INET
    ? (void *) &(((struct sockaddr_in*)sa)->sin_addr)
    : (void *) &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int global_sig_attach(void) {
  struct sigaction child_action;
  //struct sigaction int_action; // normal way to shut down server

  // right now we're not multiprocessing
  child_action.sa_handler = sigchld_handler; // bury zombie processes
  sigemptyset(&child_action.sa_mask);
  child_action.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &child_action, NULL) == -1) {
    perror("sigaction SIGCHLD");
    return -1;
  } 

  // TODO sigint should be normal way to shut down cleanly
  /*
  int_action.sa_handler = sigint_handler;
  sigemptyset(&int_action.sa_mask);
  int_action.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &int_action, NULL) == -1) {
    perror("sigaction SIGINT");
    return -1;
  }
  */
  return 0;

}

void sigint_handler(int s) { // program shuts down nicely when sigint is received 
  s = s; // suppress compiler warning
  // TODO clean shutdown routine
}

void sigchld_handler(int s)
{
  s = s; // suppress compiler warning
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}

int startup(int argc, char *argv[]) 
{
  #ifndef DEBUG
  util_print_license();
  #endif
  if (process_cmd_args(argc, argv)) {
    return 0;
  }
  return 0;
}

int process_cmd_args(int argc, char *argv[]) {
  // check command line arguments
  // right now, if the argument is -h then argv[1][1] will be hashed
  // and the program will exit (for testing hashing)
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
    return 1;
  }

  return 0;
}