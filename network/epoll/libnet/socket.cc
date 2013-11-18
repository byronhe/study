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
#if defined(_WINDOWS_) || defined(_MSC_VER)
# ifndef _WINDOWS_
#   include <winsock2.h>
# endif
  typedef int socklen_t;
#elif defined(__linux__)
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <unistd.h>
# include <fcntl.h>
#endif
#include "net.h"
#include "address.h"
#include "socket.h"



Socket::Socket(void)
  : fd_(kNetTypeInval)
{
}

Socket::~Socket(void)
{
  Close();
}

bool 
Socket::SetOption(int level, int optname, int optval)
{
  if (kNetTypeInval == fd_)
    return false;

  if (kNetTypeError == setsockopt(fd_, level, 
        optname, 
#if defined(_WINDOWS_) || defined(_MSC_VER)
        (const char*)&optval, 
#elif defined(__linux__)
        (const void*)&optval, 
#endif
        sizeof(optval)))
    return false;

  return true;
}

bool 
Socket::SetNonBlock(void)
{
  if (kNetTypeInval == fd_)
    return false;

#if defined(_WINDOWS_) || defined(_MSC_VER)
  u_long mode = 1;
  if (kNetTypeError == ioctlsocket(fd_, FIONBIO, &mode))
    return false;
#elif defined(__linux__)
  int opt = fcntl(fd_, F_GETFL);
  if (kNetTypeError == fd_)
    return false;
  if (kNetTypeError == fcntl(fd_, F_SETFL, opt | O_NONBLOCK))
    return false;
#endif

  return true;
}

bool 
Socket::SetTcpNoDelay(bool nodelay)
{
  return SetOption(IPPROTO_TCP, TCP_NODELAY, (nodelay ? 1 : 0));
}

bool 
Socket::SetReuseAddr(bool reuse)
{
  return SetOption(SOL_SOCKET, SO_REUSEADDR, (reuse ? 1 : 0));
}

bool 
Socket::SetKeepAlive(bool keep)
{
  return SetOption(SOL_SOCKET, SO_KEEPALIVE, (keep ? 1 : 0));
}

bool 
Socket::SetReadBuffer(int bytes)
{
  return SetOption(SOL_SOCKET, SO_RCVBUF, bytes);
}

bool 
Socket::SetWriteBuffer(int bytes)
{
  return SetOption(SOL_SOCKET, SO_SNDBUF, bytes);
}

bool 
Socket::SetSelfReadBuffer(int bytes)
{
  return rbuf_.Init(bytes);
}

bool 
Socket::SetSelfWriteBuffer(int bytes)
{
  return wbuf_.Init(bytes);
}


bool 
Socket::Open(void)
{
  fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (kNetTypeInval == fd_)
    return false;

  return true;
}

void 
Socket::Close(void)
{
  if (kNetTypeInval == fd_) {
#if defined(_WINDOWS_) || defined(_MSC_VER)
    shutdown(fd_, SD_BOTH);
    closesocket(fd_);
#elif defined(__linux__)
    shutdown(fd_, SHUT_RDWR);
    close(fd_);
#endif
    fd_ = kNetTypeInval;
  }
}
