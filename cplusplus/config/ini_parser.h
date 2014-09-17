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
#ifndef __INI_PARSER_HEADER_H__
#define __INI_PARSER_HEADER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>

class IniParser {
  enum {BSIZE = 256};
  typedef std::map<std::string, std::string> ValueMap;

  int         pos_;
  bool        error_;
  FILE*       file_;
  int         length_;
  char        buffer_[BSIZE];
  std::string section_;
  ValueMap    values_;
public:
  IniParser(void);
  ~IniParser(void);

  bool Open(const char* fname);
  void Close(void);

  bool Parse(void);

  std::string Get(const std::string& section, const std::string& key);
private:
  int GetChar(void);
  void UngetChar(void);

  void ParseSection(void);
  void ParseValue(void);
  const std::string ParseValueKey(void);
  const std::string ParseValueValue(void);
};

#endif  // __INI_PARSER_HEADER_H__
