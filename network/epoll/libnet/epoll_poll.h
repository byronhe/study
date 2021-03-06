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
#ifndef __EPOLL_POLL_HEADER_H__
#define __EPOLL_POLL_HEADER_H__


#include <map>
#include "locker.h"

struct epoll_event;
class Socket;
struct EventHandler;
class EpollPoll {
  enum {
    kDefaultBufferLen   = 16 * 1024, 
    kDefaultEventCount  = 4096, 
  };
  int fd_;
  int rbytes_;
  int wbytes_;
  int event_count_;
  epoll_event* events_;
  EventHandler* handler_;
  SpinLock spinlock_;
  std::map<int, Socket*> connectors_;

  EpollPoll(const EpollPoll&);
  EpollPoll& operator =(const EpollPoll&);
public:
  explicit EpollPoll(void);
  ~EpollPoll(void);

  inline void Attach(EventHandler* handler) 
  {
    handler_ = handler;
  }

  inline void SetBuffer(
      int rbytes = kDefaultBufferLen, 
      int wbytes = kDefaultBufferLen)
  {
    rbytes_ = rbytes;
    wbytes_ = wbytes;
  }
public:
  void CloseAll(void);
  bool Insert(int fd, int ev);
  void Remove(int fd);
  bool AddEvent(int fd, int ev);
  bool DelEvent(int fd, int ev);

  Socket* GetConnector(int fd);

  bool Polling(int ev, int millitm = -1);
private:
  bool Init(void);
  void Destroy(void);
  bool UpdateEvent(int op, Socket* s, int ev);
};

#endif  //! __EPOLL_POLL_HEADER_H__
