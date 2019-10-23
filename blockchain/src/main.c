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
#include <ifaddrs.h>
#include <limits.h>

int net_get_localip() 
{
  char host_buf[HOST_NAME_MAX];
  char *ip_buf;
  struct hostent *host_entry;

  if (gethostname(host_buf, sizeof host_buf) == -1) {
    perror("net_get_localip gethostname");
    return -1;
  }
  if ((host_entry = gethostbyname(host_buf)) == NULL) {
    perror("net_get_localip gethostbyname");
    return -1;
  }
  ip_buf = inet_ntoa(*((struct in_addr*) 
            host_entry->h_addr_list[0])); 

  printf("Hostname: %s\n", host_buf);
  printf("Host IP : %s\n", ip_buf);
  return 0;
}

int main_startup(int argc, char *argv[]) 
{
  net_get_localip();

  #ifndef DEBUG
  util_print_license();
  #endif
  if (main_process_cmd_args(argc, argv)) {
    return 0;
  }
  return 0;
}

int main(int argc, char *argv[]) 
// -----------------------------------------------------------------------------
// Func: check command line arguments and start p2p server and client threads
// Args: argc - number of command line arguments
//       argv - command line arguments
// Retn: 0 unless there is an error
// -----------------------------------------------------------------------------
{
  SockData sock_data;
  // event handling variables
  struct ev_loop *mainloop = EV_DEFAULT;
  ev_io accept_watcher;
  ev_io sigint_watcher;
  // networking variables
  int lsock;

  // set up pipe between sigint handler and main thread
  if (pipe(sig_pipe) == -1) {
    perror("main pipe");
    return -1;
  }

  main_startup(argc, argv); // handles command line arguments, print license logic
  sig_attach_handler(); // attach all signal handlers

  lsock = net_get_listener(LISTEN_PORT); // get a listener socket 
  if (lsock == -1) {
    perror("main get_listener_socket");
    return -1;
  }

  // event loop will need all the socket information so that it can
  // cleanly shut down the server
  memset(&sock_data, 0, sizeof sock_data);
  sock_data.listener = lsock;

  if (listen(lsock, SERV_BACKLOG) == -1) {
    perror("main listen");
    return -1;
  }

  accept_watcher.data = sock_data.cio; // give the accept callback access to client table
  // Initialize and start a watcher to accept client requests
  // attach function cb_acceptt as the callback with the listen socket as an arg
  ev_io_init(&accept_watcher, cb_accept, lsock, EV_READ);
  ev_io_start(mainloop, &accept_watcher);

  // sigint gets a copy of all the socket info, so the callback can shut down
  // the server
  sigint_watcher.data = &sock_data;
  ev_io_init(&sigint_watcher, cb_sigint, sig_pipe[0], EV_READ);
  ev_io_start(mainloop, &sigint_watcher);

  printf("Started server event loop\n");
  ev_run(mainloop, 0); // run listener callback under main event loop

  ev_io_stop(mainloop, &sigint_watcher);
  ev_io_stop(mainloop, &accept_watcher);

  return 0;
}

void cb_sigint(struct ev_loop *loop, ev_io *watcher, int revents)
{
  int i;
  char ofbuf[RD_SZ]; // overflow buffer
  SockData *sd =   (SockData *)watcher->data;
  ClientIO *cio = ((SockData *)watcher->data)->cio;
  if (EV_ERROR & revents) {
    perror("got invalid sigint event");
    return;
  }
  
  // sigint has been received, need to clean things up
  // TODO clean shutdown routine
  // https://stackoverflow.com/questions/9681531/graceful-shutdown-server-socket-in-linux
  // https://stackoverflow.com/questions/11487900/best-pratice-to-free-allocated-memory-on-sigint
  /*
  1. prevent accept from adding more clients
  2. use a list of open sockets to know when a socket is properly closed
      - use shutdown to tell client im closing the socket 
      - call read for a while to make sure that all the client data has been received
      - use close to free each client socket 
  3. close the listen socket
  
  use a timeout to make sure idle connections wont last forever
 */
  printf("server: Shutting down\n");
  for (i = 0; i < MAX_CONCURRENT_REQUESTS; i++) {
    if (cio[i].status == 1) { // socket is active
      printf("server: Dropping peer connection on socket %d\n", cio[i].sock);
      net_disconnect_peer(&cio[i], ofbuf, RD_SZ);
      printf("server: Closed connection to peer on socket %d\n", cio[i].sock);
    }
  } 
  printf("server: Closing listener on socket %d\n", sd->listener);
  shutdown(sd->listener, SHUT_RDWR); // TODO error checking
  close(sd->listener);

  ev_break(loop, EVBREAK_ALL);
}

// event-driven wrapper for accept
// ensures that we're not blocked on the accept call
void cb_accept(struct ev_loop *loop, ev_io *watcher, int revents) 
{
  struct sockaddr_storage their_addr;
  socklen_t sin_size = sizeof their_addr;
  char s [INET6_ADDRSTRLEN];
  int index;
  ev_io *recv_watcher;
  ClientIO *cio = watcher->data;

  memset(s, 0, sizeof s);

  if (EV_ERROR & revents) {
    perror("got invalid listener event");
    return;
  }

  // find the lowest usable index for this client's entry in the
  // client table
  index = net_get_peer_id(cio, MAX_CONCURRENT_REQUESTS);

  cio[index].status = 1;
  cio[index].sock = accept4(watcher->fd, (struct sockaddr *)&their_addr, 
        &sin_size, SOCK_CLOEXEC);
  if (cio[index].sock == -1) {
    perror("cb_accept accept4");
  }
  // fcntl(cio_table[index].sock, F_SETFL, O_NONBLOCK)
  fcntl(cio[index].sock, 4, 04000);
  
  if (cio[index].sock == -1) {
    perror("cb_accep");
    return;
  }
  cio[index].id = index; // not being used for anything

  inet_ntop(their_addr.ss_family,
    net_get_inaddr((struct sockaddr *)&their_addr),
    s, sizeof s);
  printf("server: accepted connection from %s\n", s);

  recv_watcher = malloc(sizeof(ev_io));
  // recv_watcher->data = malloc(sizeof cio[index]);
  recv_watcher->data = (void *)&cio[index]; // so that callback can access client info through watcher
  
  ev_io_init(recv_watcher, cb_recv, cio[index].sock, EV_READ);
  ev_io_start(loop, recv_watcher); // add it to this loop
}

void cb_recv(struct ev_loop *loop, ev_io *watcher, int revents) 
{
  ClientIO *cio = (ClientIO *)watcher->data; // get all the info on this client

  // TODO do stuff here
  // depending on what type of request has been made, we may have to do 
  // some different things
  char buf[RD_SZ];
  char ofbuf[RD_SZ]; // overflow buffer

  memset(buf, 0, sizeof buf);

  if (EV_ERROR & revents) {
    perror("got invalid client event");
    return;
  }

  recv(cio->sock, buf, RD_SZ, 0); // TODO flags, error checking, read in loop
  printf("%s", buf);

  net_disconnect_peer(cio, ofbuf, RD_SZ);

  ev_io_stop(loop, watcher);
  free(watcher); 
  printf("server: Disconnected peer\n");
}

int net_disconnect_peer(ClientIO *cio, char *ofbuf, int sz) {
  int rv = 0;
  if (shutdown(cio->sock, SHUT_RDWR)) { // tell client im closing socket
    rv = -1;
    // shutdown failed
  }
  if (recv(cio->sock, ofbuf, sz, 0) == sz) { // one last read from socket
    rv = -2; // we may not have gotten all the data
  }
  if (!close(cio->sock)) {
    rv = -3;
    // TODO fatal error?
  }
  cio->id = 0;
  cio->status = 0;

  return rv;
}

int net_get_peer_id(ClientIO *cio_table, int sz) 
{
  int i;

  for (i = 0; i < sz; i++) {
    if(cio_table[i].status == 0) {
      return i;
    }

  }

  return -1;
}

int net_get_listener(const char *port) 
{
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int lsock;
  int yes = 0;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "server getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((lsock = socket(p->ai_family, p->ai_socktype, 
      p->ai_protocol)) == -1) {
        perror("server: socket");
        continue;
    }

    // TODO is this not portable?
    // fcntl(lsock, F_SETFL, O_NONBLOCK)
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

void *net_get_inaddr(struct sockaddr *sa) 
{
  return sa->sa_family == AF_INET
    ? (void *) &(((struct sockaddr_in*)sa)->sin_addr)
    : (void *) &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int sig_attach_handler(void) {
  struct sigaction action; // normal way to shut down server

  // TODO sigint should be normal way to shut down cleanly
  
  action.sa_handler = sig_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &action, NULL) == -1) {
    perror("global_sig_attach sigaction");
    return -1;
  }
  
  return 0;

}

void sig_handler(int s) 
{ // program shuts down nicely when sigint is received 
  switch (s) {
  case SIGINT: sig_int_handler(); break;
  }
}

void sig_int_handler()
{
  char tmp = MAIN_SHUTDOWN;
  // write a byte to the pipe so the main callback loop
  // will trigger the sigint event
  if (write(sig_pipe[1], &tmp, 1) == -1) {
    perror("sigint_handler write");
  }
}

int main_process_cmd_args(int argc, char *argv[]) 
{
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