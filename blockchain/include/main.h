/*
main.h: 
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


#ifndef MAIN_H
#define MAIN_H

// #define _POSIX_C_SOURCE 200809L // must be defined first to keep vscode happy
#define _GNU_SOURCE

#include <arpa/inet.h> // definition of struct sockaddr
#include <ev.h>
#include <pthread.h>

#define LISTEN_PORT             "51223" // this has to be configurable
#define SERV_BACKLOG            10
#define MAX_CONCURRENT_REQUESTS 10
#define MAIN_SHUTDOWN           'x'
#define RD_SZ                   100 // number of bytes per recv

// global variables
int sig_pipe[2]; // used in shutdown routine

typedef struct {
    int status;
    int sock;
    int id; // currently not being used
} ClientIO;

typedef struct {
    int listener;
    ClientIO cio[MAX_CONCURRENT_REQUESTS];
} SockData;

// startup routines
int main_startup(int argc, char *argv[]);
int main_process_cmd_args(int argc, char *argv[]); 

// signal handling
int sig_attach_handler(void);
void sig_handler(int s);
void sig_int_handler();

// event callbacks
void cb_accept(struct ev_loop *loop, ev_io *watcher, int revents);
void cb_recv(struct ev_loop *loop, ev_io *watcher, int revents);
void cb_sigint(struct ev_loop *loop, ev_io *watcher, int revents);


// networking
int net_get_localip();
int net_disconnect_peer(ClientIO *cio, char *ofbuf, int sz);
int net_get_peer_id(ClientIO *cio_table, int sz);
int net_get_listener();
void *net_get_inaddr(struct sockaddr *sa);

#endif
