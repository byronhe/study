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
#ifndef __COMMON_HEADER_H__
#define __COMMON_HEADER_H__

class CommonLib {
  static bool loaded_;

  CommonLib(const CommonLib&);
  CommonLib& operator =(const CommonLib&);
public:
  static CommonLib& Singleton(void);
  void Init(void);
  void Destroy(void);
};


#define NetErrno()  WSAGetLastError()
#define Errno()     GetLastError()

enum LoggingMode {
  kLoggingModeDebug = 0, 
  kLoggingModeMessage, 
  kLoggingModeWarning, 
  kLoggingModeError, 
  kLoggingModeFailed, 
};
extern void CommonLog(int severity, 
    const char* file, int line, const char* format, ...);
#if defined(_MSC_VER) && (_MSC_VER < 1400)
# error "Your compiler version is too low."
#endif
#define LOG_DEBUG(fmt, ...)\
  CommonLog(kLoggingModeDebug, __FILE__, __LINE__, (fmt), ##__VA_ARGS__)
#define LOG_MSG(fmt, ...)\
  CommonLog(kLoggingModeMessage, __FILE__, __LINE__, (fmt), ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)\
  CommonLog(kLoggingModeWarning, __FILE__, __LINE__, (fmt), ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)\
  CommonLog(kLoggingModeError, __FILE__, __LINE__, (fmt), ##__VA_ARGS__)
#define LOG_FAIL(fmt, ...)\
  CommonLog(kLoggingModeFailed, __FILE__, __LINE__, (fmt), ##__VA_ARGS__)

#endif  //! __COMMON_HEADER_H__
