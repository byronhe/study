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
#ifndef __SMART_ARRAY_HEADER_H__
#define __SMART_ARRAY_HEADER_H__

template <typename T>
class RefArrayDelete : public RefBase, private UnCopyable {
  T* ptr_;
public:
  explicit RefArrayDelete(T* p)
    : ptr_(p) {
  }

  ~RefArrayDelete(void) {
  }

  virtual void Destroy(void) {
    if (NULL != ptr_) 
      delete [] ptr_;
  }
};


template <typename T, typename D> 
class RefArrayDestructor : public RefBase, private UnCopyable {
  T* ptr_;
  D  dtor_;
public:
  explicit RefArrayDestructor(T* p, D d)
    : ptr_(p)
    , dtor_(d) {
  }

  ~RefArrayDestructor(void) {
  }

  virtual void Destroy(void) {
    if (NULL != ptr_)
      dtor_(ptr_);
  }
};


template <typename T>
class SmartArray {
  T*        ptr_;
  RefBase*  ref_array_;
  int*      ref_count_;
public:
  explicit SmartArray(T* p)
    : ptr_(p)
    , ref_array_(new RefArrayDelete<T>(ptr))
    , ref_count_(new int(1)) {
  }

  template <typename Y, typename D>
  explicit SmartArray(Y* p, D d) 
    : ptr_(p)
    , ref_array_(new RefArrayDestructor<Y, D>(ptr_, d))
    , ref_count_(new int(1)) {
  }

  ~SmartArray(void) {
    if (0 == --*ref_count_) {
      ref_array_->Destroy();
      delete ref_array_;

      delete ref_count_;
    }
  }

  SmartArray(const SmartArray& x) 
    : ptr_(x.ptr_)
    , ref_array_(x.ref_array_)
    , ref_count_(x.ref_count_) {
    ++*ref_count_;
  }

  SmartArray& operator=(const SmartArray& x) {
    if (this != &x) {
      if (0 == --*ref_count_) {
        ref_array_->Destroy();
        delete ref_array_;

        delete ref_count_;
      }

      ptr_ = x.ptr_;
      ref_array_ = x.ref_array_;
      ref_count_ = x.ref_count_;
      ++*ref_count_;
    }

    return *this;
  }
public:
  T& operator[](int i) const {
    return ptr_[i];
  }

  T* Get(void) const {
    return ptr_;
  }
};

#endif  // __SMART_ARRAY_HEADER_H__
