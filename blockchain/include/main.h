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

#define _POSIX_C_SOURCE 200809L // must be defined first to keep vscode happy

#include <arpa/inet.h> // definition of struct sockaddr
#include <ev.h>
#include <pthread.h>

#define LISTEN_PORT             "51221" // this has to be configurable
#define SERV_BACKLOG            10
#define MAX_CONCURRENT_REQUESTS 10

// at a given index, the activity table will store a 1 if that thread is active
// or 0 if that thread is inactive. The socket table stores the file descriptor
// that that thread is using for its connection to the client. request
typedef struct {
    int status;
    int sock;
    int id;
} ClientIO;

// globals
ClientIO cio_table[MAX_CONCURRENT_REQUESTS]; 

// startup routines
int startup(int argc, char *argv[]);
int process_cmd_args(int argc, char *argv[]); 

// signal handling
int global_sig_attach(void);
void sigint_handler(int s); // still need to implement SIGINT handler
void sigchld_handler(int s);

// event callbacks
void accept_cb(struct ev_loop *loop, ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, ev_io *watcher, int revents);


// networking
int get_client_id(ClientIO *cio_table, int sz);
int get_listener_socket();
void *get_in_addr(struct sockaddr *sa);
void *request_handler(void *arg);
// void *client(void *err);

#endif
