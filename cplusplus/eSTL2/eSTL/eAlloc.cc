// Copyright (c) 2015 ASMlover. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "eAlloc.h"

namespace estl {

char* Alloc::start_free_ = nullptr;
char* Alloc::finish_free_ = nullptr;
size_t Alloc::heap_size_ = 0;
Alloc::Obj* Alloc::free_list_[Alloc::FreeLists::NFREELISTS] = {0};

void* Alloc::ReFill(size_t bytes) {
  size_t nobjs = Objs::NOBJS;
  char*  chunk = ChunkAlloc(bytes, nobjs);

  if (1 == nobjs) {
    return chunk;
  }
  else {
    Obj*  current_obj = nullptr;
    Obj*  next_obj = nullptr;
    Obj** free_list = free_list_ + FREELIST_INDEX(bytes);
    Obj*  result = (Obj*)chunk;
    *free_list = next_obj = (Obj*)(chunk + bytes);

    for (auto i = 1; ; ++i) {
      current_obj = next_obj;
      next_obj = (Obj*)((char*)next_obj + bytes);
      if (1 == nobjs - 1) {
        current_obj->next = nullptr;
        break;
      }
      else {
        current_obj->next = next_obj;
      }
    }

    return result;
  }
}

char* Alloc::ChunkAlloc(size_t bytes, size_t& nobjs) {
  return nullptr;
}

}
