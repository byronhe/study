// Copyright (c) 2016 ASMlover. All rights reserved.
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
#ifndef __TYR_BASIC_WIN_PLATFORMWINDOWS_HEADER_H__
#define __TYR_BASIC_WIN_PLATFORMWINDOWS_HEADER_H__

#if !defined(_WINDOWS_)
# include <Winsock2.h>
#endif

#define TYR_DECLARRAY(type, name, count) type* name = (type*)_alloca(sizeof(char) * (count))

typedef int                 pid_t;
typedef CONDITION_VARIABLE  KernCond;
typedef DWORD               KernThreadKey;

struct KernMutex {
  CRITICAL_SECTION cs;
  pid_t tid{-1};
  int count{0};

  KernMutex(void) {
    InitializeCriticalSectionEx(&cs, 4000, 0);
  }

  ~KernMutex(void) {
    DeleteCriticalSection(&cs);
  }

  void lock(void) {
    EnterCriticalSection(&cs);
  }

  bool try_lock(void) {
    return TRUE == TryEnterCriticalSection(&cs);
  }

  void unlock(void) {
    LeaveCriticalSection(&cs);
  }
};

struct KernThread {
  HANDLE start_event;
  HANDLE thrd_handle;

  KernThread(void)
    : start_event(nullptr)
    , thrd_handle(nullptr) {
  }

  KernThread(std::nullptr_t)
    : start_event(nullptr)
    , thrd_handle(nullptr) {
  }

  KernThread& operator=(std::nullptr_t) {
    start_event = nullptr;
    thrd_handle = nullptr;
  }
};

namespace tyr { namespace basic {

#if !defined(__builtin_expect)
# define __builtin_expect(exp, c) (exp)
#endif
#define gmtime_r(timep, result)         gmtime_s((result), (timep))
#define strerror_r(errnum, buf, buflen) strerror_s((buf), (buflen), (errnum))
#define timegm                          _mkgmtime

struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};

int gettimeofday(struct timeval* tv, struct timezone* tz);

}}

#endif // __TYR_BASIC_WIN_PLATFORMWINDOWS_HEADER_H__
