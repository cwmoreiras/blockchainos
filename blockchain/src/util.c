/*
util.c: global utility functions
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

#include "util.h"

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// openssl header files
#include <openssl/crypto.h>
#include <openssl/sha.h>

void util_cmd_hash(const char *str) 
// -----------------------------------------------------------------------------
// Func: 
// Args:
// Retn:
// -----------------------------------------------------------------------------
{
  uint8_t hash[SHA256_DIGEST_LENGTH];
  int sz = strlen(str)+1; // include the null terminator

  util_buf_write_raw((uint8_t *)str, sz, "test/raw");
  util_buf_print_hex((uint8_t *)str, sz, "strng", 1);
  util_buf_hash((uint8_t*) str, sz, hash);
  util_buf_print_hex(hash, SHA256_DIGEST_LENGTH, "hash ", 1);

}

void util_buf_print_hex(uint8_t *buf, uint64_t buf_sz, 
                        const char *label, const int newline)
// -----------------------------------------------------------------------------
// Func: 
// Args:
// Retn:
// -----------------------------------------------------------------------------
{
  uint64_t i;

  if (label != NULL)
    printf("%s: 0x", label);
  else 
    printf("0x");

  for (i = 0; i < buf_sz; i++) 
    printf("%02x", buf[i]);

  if (newline)
    printf("\n");
}

void util_buf_hash(uint8_t *buf, uint64_t buf_sz, uint8_t *hash) 
// -----------------------------------------------------------------------------
// Func: Hash any buffer
// Args: buf - the buffer we want to hash
//       buf_sz - the size of the buffer 
//       hash - the hash of the buffer
// Retn:
// -----------------------------------------------------------------------------
{
  SHA256_CTX sha;
  SHA256_Init(&sha);
  SHA256_Update(&sha, buf, buf_sz);
  SHA256_Final(hash, &sha);
}

int util_print_license(void) 
// -----------------------------------------------------------------------------
// Func: 
// Args:
// Retn:
// -----------------------------------------------------------------------------
{
  printf("%s\nCopyright (C) %s\nmaintainer: %s\n", 
    PROG_NAME, COPYRIGHT_YEAR, MAINTAINER);
  printf("This program comes with ABSOLUTELY NO WARRANTY; ");
  printf("for details type 'show w'\n");
  printf("This is free software, and you are welcome to redisribute it\n");
  printf("under certain conditions; type 'show c' for details\n");
  printf("-------------------------------------------------------------------------\n");


  return 0; // todo error codes
}

int util_buf_write_raw(const uint8_t *buf, int n, const char *pathname) 
// -----------------------------------------------------------------------------
// Func: Write the contents of the buffer out to a file
// Args: buf - pointer to an array of bytes
//       n - the number of bytes 
//       pathname - the pathname of the file which will be created
// Retn: 0 in case of no errors, otherwise return a nonzero number
// -----------------------------------------------------------------------------
{
  // see man page for open to read about O_CLOEXEC in multithreaded
  // programs
  int fd = open(pathname, O_CREAT|O_WRONLY|O_TRUNC, 0666); //rw permissions

  if (fd < 0) {
    return fd;
  }

  if (write(fd, buf, n) != n) {
    return -1; // TODO how to do errors
  }

  close(fd);

  return 0;
}

// time  - O(n) array size is tight bound
// space - O(n), exactly 2n
// use temporary array so that the caller can use the same buffer
// for dest and src
void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len)
// -----------------------------------------------------------------------------
// Func: Reverse the src buffer and store the result in dest
// Args: dest - the address of the new reversed buffer 
//       src - points to the buffer object that will be reversed
//       len - the number of bytes in the buffer
// Retn: None
// -----------------------------------------------------------------------------
{
  int i;
  uint8_t *temp;

  temp = malloc(len);

  for (i = 0; i < len; i++)
    temp[len-1-i] = src[i]; // reverse the array
  for (i = 0; i < len; i++)
    dest[i] = temp[i]; // copy the reversed array into dest

  free(temp);
}
