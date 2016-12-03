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
#ifndef CHAOS_OS_POSIX_OS_H
#define CHAOS_OS_POSIX_OS_H

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <string>

namespace Chaos {

inline struct tm* kern_gmtime(const time_t* timep, struct tm* result) {
  return gmtime_r(timep, result);
}

inline time_t kern_timegm(struct tm* timep) {
  return timegm(timep);
}

inline pid_t kern_getpid(void) {
  return getpid();
}

inline pid_t kern_getppid(void) {
  return getppid();
}

int kern_backtrace(std::string& bt);

// posix thread methods wrapper
typedef pthread_t _Thread_t;

inline int kern_thread_create(_Thread_t* thread, void* (*start_routine)(void*), void* arg) {
  return pthread_create(thread, nullptr, start_routine, arg);
}

inline int kern_thread_join(_Thread_t thread) {
  return pthread_join(thread, nullptr);
}

inline int kern_thread_detach(_Thread_t thread) {
  return pthread_detach(thread);
}

}

#endif // CHAOS_OS_POSIX_OS_H
