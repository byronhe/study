/*
 * Copyright (c) 2013 ASMlover. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 *
 *    notice, this list of conditions and the following disclaimer in
 *  * Redistributions in binary form must reproduce the above copyright
 *    the documentation and/or other materialsprovided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "file.h"




void 
file_readfat(int* fat, unsigned char* img)
{
  /* uncompress FAT of disk */
  int i, j = 0;
  for (i = 0; i < 2880; i += 2) {
    fat[i + 0] = (img[j + 0] | (img[j + 1] << 8)) & 0xfff;
    fat[i + 1] = ((img[j + 1] >> 4) | (img[j + 2] << 4)) & 0xfff;
    j += 3;
  }
}

void 
file_loadfile(int clustno, int size, char* buf, int* fat, char* img)
{
  int i;
  for ( ; ; ) {
    if (size <= 512) {
      for (i = 0; i < size; ++i)
        buf[i] = img[clustno * 512 + i];
      break;
    }

    for (i = 0; i < 512; ++i)
      buf[i] = img[clustno * 512 + i];
    size -= 512;
    buf += 512;
    clustno = fat[clustno];
  }
}

fileinfo_t* 
file_search(fileinfo_t* finfo, char* name, int max)
{
  int i, j;
  char buf[12];

  for (j = 0; j < 11; ++j)
    buf[j] = ' ';
  j = 0;
  for (i = 0; 0 != name[i]; ++i) {
    if (j >= 11) {
      /* do not found */
      return 0;
    }

    if ('.' == name[i] && j <= 8)
      j = 8;
    else {
      buf[j] = name[i];
      if ('a' <= buf[j] && buf[j] <= 'z')
        buf[j] -= 0x20;
      ++j;
    }
  }

  for (i = 0; i < max; ) {
    if (0x00 == finfo[i].name[0])
      break;
    if (0 == (finfo[i].type & 0x18)) {
      for (j = 0; j < 11; ++j) {
        if (finfo[i].name[j] != buf[j])
          goto __next;
      }

      return (finfo + i); /* found the file */
    }
__next:
    ++i;
  }

  return 0; /* do not found the file */
}
