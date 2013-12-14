#ifndef __CONNECTOR_HEADER_H__
#define __CONNECTOR_HEADER_H__

#include "locker.h"
#include "buffer.h"

class Connector : public Socket {
  SpinLock spinlock_;
  Buffer rbuf_;
  Buffer wbuf_;

  Connector(const Connector&);
  Connector& operator =(const Connector&);
public:
  explicit Connector(void);
  ~Connector(void);

  inline bool SetReadBuffer(uint32_t bytes)
  {
    return rbuf_.Init(bytes);
  }

  inline bool SetWriteBuffer(uint32_t bytes)
  {
    return wbuf_.Init(bytes);
  }
};

#endif  //! __CONNECTOR_HEADER_H__
