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

#define SERV_PORT       "51218" // this has to be configurable
#define SERV_BACKLOG    10

const int test_data = 101;

void sigint_handler(int s); // still need to implement SIGINT handler
void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);
void *p2p_server(void *err);
void *p2p_client(void *err);

#endif
