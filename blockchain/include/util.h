/*
util.h: utility function declarations
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


#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

// meta data
#define PROG_NAME "blockchainos"
#define COPYRIGHT_YEAR "2019"
#define MAINTAINER "Carlos WM"

// command line arguments
void util_cmd_hash(const char *str);

// buffer utilities
void util_buf_print_hex(uint8_t *buf, uint64_t buf_sz, 
                        const char *label, const int newline);
void util_buf_hash(uint8_t *buf, uint64_t buf_sz, uint8_t *hash);
void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len);
int util_buf_write_raw(const uint8_t *, int, const char *pathname);


int util_print_license(void);

#endif
