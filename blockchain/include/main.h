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

#define LISTEN_PORT             "51218" // this has to be configurable
#define SERV_BACKLOG            100
#define MAX_CONCURRENT_REQUESTS 100

typedef struct {
    int id;
    int sock;
} ThreadData;

// NOTE: this is a global variable!!
int p2p_thread_table[MAX_CONCURRENT_REQUESTS]; // global thread table
pthread_t p2p_request[MAX_CONCURRENT_REQUESTS];

// startup routines
int startup(int argc, char *argv[]);
int process_cmd_args(int argc, char *argv[]); 

// signal handling
int global_sig_attach(void);
void sigint_handler(int s); // still need to implement SIGINT handler
void sigchld_handler(int s);

// networking
int get_listener_socket();
void *get_in_addr(struct sockaddr *sa);
void *p2p_request_handler(int *threadIndex);
// void *p2p_client(void *err);

// threading
void p2p_request_cleanup(int *arg);
int p2p_get_thread_index(int *table, int sz);

#endif
