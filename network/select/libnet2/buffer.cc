//! Copyright (c) 2013 ASMlover. All rights reserved.
//!
//! Redistribution and use in source and binary forms, with or without
//! modification, are permitted provided that the following conditions
//! are met:
//!
//!  * Redistributions of source code must retain the above copyright
//!    notice, this list ofconditions and the following disclaimer.
//!
//!  * Redistributions in binary form must reproduce the above copyright
//!    notice, this list of conditions and the following disclaimer in
//!    the documentation and/or other materialsprovided with the
//!    distribution.
//!
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//! "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//! LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//! FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//! COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//! INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//! BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//! LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//! CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//! LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//! ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//! POSSIBILITY OF SUCH DAMAGE.
#include <stdlib.h>
#include <string.h>
#include "buffer.h"




Buffer::Buffer(void)
  : buffer_(NULL)
  , length_(0)
  , storage_(kDefaultStorage)
{
}

Buffer::~Buffer(void)
{
  Destroy();
}

bool 
Buffer::Init(int storage)
{
  storage_ = (storage > kDefaultStorage ? storage : kDefaultStorage);

  length_ = 0;
  buffer_ = (char*)malloc(storage_);
  if (NULL == buffer_) {
    abort();
    return false;
  }

  return true;
}

void 
Buffer::Destroy(void)
{
  if (NULL != buffer_) {
    free(buffer_);
    buffer_ = NULL;
  }
  length_ = 0;
  storage_ = kDefaultStorage;
}

int 
Buffer::Put(const char* buffer, int bytes)
{
  if (length_ + bytes > storage_) {
    if (!Regrow())
      return 0;
  }

  memcpy(buffer_ + length_, buffer, bytes);
  length_ += bytes;

  return bytes;
}

int 
Buffer::Get(int bytes, char* buffer)
{
  int copy_bytes = (length_ <= bytes ? length_ : bytes);
  if (0 == copy_bytes)
    return 0;

  memcpy(buffer, buffer_, copy_bytes);
  if (copy_bytes != length_)
    memmove(buffer_, buffer_ + copy_bytes, length_ - copy_bytes);
  length_ -= copy_bytes;

  return copy_bytes;
}

int 
Buffer::AddWritePosition(int bytes)
{
  if (length_ + bytes > storage_) {
    if (!Regrow())
      return 0;
  }
  
  length_ += bytes;
  return bytes;
}

int 
Buffer::AddReadPosition(int bytes)
{
  if (bytes > length_) {
    abort();
    return -1;
  }

  if (bytes < length_) 
    memmove(buffer_, buffer_ + bytes, length_ - bytes);
  length_ -= bytes;

  return bytes;
}

int 
Buffer::SubWritePosition(int bytes)
{
  int sub_bytes = (length_ <= bytes ? length_ : bytes);
  length_ -= sub_bytes;

  return sub_bytes;
}



bool 
Buffer::Regrow(void)
{
  int new_storage = (0 != storage_ ? 2 * storage_ : kDefaultStorage);
  buffer_ = (char*)realloc(buffer_, new_storage);
  if (NULL == buffer_) {
    abort();
    return false;
  }

  storage_ = new_storage;

  return true;
}
