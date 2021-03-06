//! Copyright (c) 2014 ASMlover. All rights reserved.
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
#ifndef __IMPORT_HEADER_H__
#define __IMPORT_HEADER_H__

#include "config.h"

#if defined(PLATFORM_WIN)
# ifndef _WINDOWS_
#   include <winsock2.h>
# endif
# include <windows.h>
# include <process.h>
# include <io.h>
# include <direct.h>

# define  PATH_MAX  MAX_PATH
# define  EAGAIN    WSAEWOULDBLOCK
# define  NERRNO()  WSAGetLastError()

  typedef int socklen_t;
#elif defined(PLATFORM_POSIX)
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>
# include <limits.h>
# include <errno.h>

# define  NERRNO()  errno
#endif
#include <sys/timeb.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <map>
#include <queue>
#include <vector>

#if defined(PLATFORM_POSIX)
# include "posix_tools.h"
#endif


#include "uncopyable.h"
#include "locker.h"
#include "singleton.h"
#include "thread.h"
#include "net.h"

#endif  //! __IMPORT_HEADER_H__
