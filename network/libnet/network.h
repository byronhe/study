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
#ifndef __NETWORK_HEADER_H__
#define __NETWORK_HEADER_H__

class Worker;
class Listener;
class ConnectorDispatcher;
class Network {
  enum {
    kDefaultWorkerCount = 1, 
    kDefaultBufferSize  = 16 * 1024, 
  };
  ConnectorDispatcher* dispatcher_;
  int worker_count_;
  Worker* workers_;
  Listener* listener_;
  int suitable_worker_;
  EventHandler* handler_;

  Network(const Network&);
  Network& operator =(const Network&);
public:
  explicit Network(void);
  ~Network(void);

  inline void Attach(EventHandler* handler) 
  {
    handler_ = handler;
  }
public:
  bool Init(
      int worker_count = kDefaultWorkerCount, 
      uint32_t rbytes = kDefaultBufferSize, 
      uint32_t wbytes = kDefaultBufferSize);
  void Destroy(void);

  Worker* SuitableWorker(void);
  void MarkNextSuitableWorker(void);

  bool Listen(const char* ip = "0.0.0.0", uint16_t port = 5555);
};

#endif  //! __NETWORK_HEADER_H__