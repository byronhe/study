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
#ifndef __SOCKET_HEADER_H__
#define __SOCKET_HEADER_H__

#include "buffer.h"

class Address;
class Packet;
class Socket {
  int fd_;
  Buffer rbuf_;
  Buffer wbuf_;

  Socket(const Socket&);
  Socket& operator =(const Socket&);
public:
  explicit Socket(void);
  ~Socket(void);

  inline int fd(void) const
  {
    return fd_;
  }

  inline void Attach(int fd)
  {
    fd_ = fd;
  }

  inline int Detach(void) 
  {
    int fd = fd_;
    fd_ = -1;
    return fd;
  }
public:
  bool SetTcpNoDelay(bool nodelay);
  bool SetReuseAddr(bool reuse);
  bool SetKeepAlive(bool keep);
  bool SetReadBuffer(int bytes);
  bool SetWriteBuffer(int bytes);
  bool SetNonBlock(void);
  bool SetSelfReadBuffer(int bytes);
  bool SetSelfWriteBuffer(int bytes);

  bool Open(void);
  void Close(void);
  bool Bind(const char* ip, unsigned short port);
  bool Listen(void);

  bool Accept(Socket* s, Address* addr);
  bool Connect(const char* ip, unsigned short port);
  int Read(int length, char* buffer);
  int Write(const char* buffer, int length);
  int ReadBlock(int length, char* buffer);
  int WriteBlock(const char* buffer, int length);
  bool ReadPacket(Packet* packet);
  bool WritePacket(const char* buffer, int length);
  bool WritePacket(Packet* packet);

  int DealWithSyncRead(void);
  int DealWithSyncWrite(void);
private:
  bool SetSockOpt(int level, int optname, int optval);
};

#endif  //! __SOCKET_HEADER_H__
