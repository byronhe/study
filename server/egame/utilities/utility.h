// Copyright (c) 2014 ASMlover. All rights reserved.
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
#ifndef __UTIL_UTILITY_HEADER_H__
#define __UTIL_UTILITY_HEADER_H__


#include "types.h"

// System interfaces header
#if defined(PLATFORM_WIN)
# include <windows.h>
# include <process.h>
#elif defined(PLATFORM_LINUX)
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>

# define MAX_PATH PATH_MAX
#endif


// ANSI C header
#include <sys/timeb.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// ANSI C++ header
#include <memory>
#include <string>
#include <stdexcept>


// STL header
#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <vector>

// Have our own assert, so we are sure it dose not get 
// optomized away in a release build.
#ifndef UTIL_ASSERT
# define UTIL_ASSERT(expr)\
  do {\
    if (!(expr)) {\
      fprintf(stderr, \
          "Assertion failed in %s on %d : %s\n", \
          __FILE__, \
          __LINE__, \
          #expr);\
      fflush(stderr);\
      abort();\
    }\
  } while (0)
#endif



// utilities header
#include "uncopyable.h"
#include "static_assert.h"
#include "locker.h"
#include "singleton.h"
#include "auto_ptr.h"
#include "auto_array.h"
#include "ref_counter.h"
#include "smart_ptr.h"
#include "smart_array.h"
#include "object_pool.h"
#include "object_mgr.h"
#include "thread.h"




#endif  // __UTIL_UTILITY_HEADER_H__
