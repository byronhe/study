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
#include <stdio.h>
#include <time.h>
#include "TDate.h"

namespace tyr {

static_assert(sizeof(int) >= sizeof(int32_t), "require 32 bit integer at least");

int get_unix_day(int year, int month, int day) {
  int a = (14 - month) / 12;
  int y = year + 4800 - a;
  int m = month + 12 * a - 3;
  return day + (m * 153 + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
}

struct Date::InternalDate get_internal_date(int unix_day) {
  int a = unix_day + 32044;
  int b = (a * 4 + 3) / 146097;
  int c = a - ((b * 146097) / 4);
  int d = (c * 4 + 3) / 1461;
  int e = c - ((d * 1461) / 4);
  int m = (e * 5 + 2) / 153;
  Date::InternalDate ymd;
  ymd.year = b * 100 + d - 4800 + (m / 10);
  ymd.month = m + 3 - (m / 10) * 12;
  ymd.day = e - ((m * 153 + 2) / 5) + 1;
  return ymd;
}

const int Date::kUnixDay19700101 = get_unix_day(1970, 1, 1);

Date::Date(int year, int month, int day)
  : unix_day_(get_unix_day(year, month, day)) {
}

Date::Date(const struct tm& t)
  : unix_day_(get_unix_day(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday)) {
}

std::string Date::to_iso_string(void) const {
  char buf[32] = {0};
  InternalDate ymd(internal_date());
  snprintf(buf, sizeof(buf), "%4d-%02d-%02d", ymd.year, ymd.month, ymd.day);
  return std::string(buf);
}

Date::InternalDate Date::internal_date(void) const {
  return get_internal_date(unix_day_);
}

}
