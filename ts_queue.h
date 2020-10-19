#pragma once

#include "net_common.h"

namespace olc {
  namespace net {
    template<typename T>
    class TSQUEUE {
      public:
        TSQUEUE() = default;
        TSQUEUE(const TSQUEUE<T>&) = delete;
        virtual ~TSQUEUE() {
          clear();
        }

      public:

      protected:
    }
  }
}