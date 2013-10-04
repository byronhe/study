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
#ifndef __LOGGING_HEADER_H__
#define __LOGGING_HEADER_H__


#include <map>
#include <stdio.h>
#include "timing.h"


struct LogFile {
  unsigned short year;
  unsigned char  mon;
  unsigned char  day;
  FILE*          stream;
};


class Logging {
  enum BufferStorage { DEF_BUFSIZE = 16 * 1024 };

  std::map<int, LogFile> file_list_;

  Logging(const Logging&);
  Logging& operator =(const Logging&);
public:
  enum SeverityType {
    ST_DEBUG    = 0, 
    ST_MESSAGE  = 1, 
    ST_WARNING  = 2, 
    ST_ERROR    = 3, 
    ST_FAIL     = 4,
  };
public:
  Logging(void);
  ~Logging(void);

  static Logging& Singleton(void);

  void Write(int severity, const char* format, ...);
private:
  const char* GetSeverityName(int severity);
  FILE* GetFileStream(int severity);
};

#endif  //! __LOGGING_HEADER_H__
