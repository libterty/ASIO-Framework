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
        const T& front() {
          // avoid dead lock
          std::scoped_lock lock(mux_queue);
          return deq_queue.front();
        }

        const T& back() {
          std::scoped_lock lock(mux_queue);
          return deq_queue.back();
        }

        T pop_front() {
          std::scoped_lock lock(mux_queue);
          auto t = std::move(deq_queue.back());
          deq_queue.pop_back();
          return t;
        }

        void push_back(const T& item) {
          std::scoped_lock lock(mux_queue);
          deq_queue.emplace_back(std::move(item));

          std::unique_lock<std::mutex> ul(mux_blocking);
          cv_blocking.notify_one();
        }

        void push_front(const T& item) {
          std::scoped_lock lock(mux_queue);
          deq_queue.emplace_front(std::move(item));

          std::unique_lock<std::mutex> ul(mux_blocking);
          cv_blocking.notify_one();
        }

        bool empty() {
          std::scoped_lock lock(mux_queue);
          return deq_queue.size();
        }

        void clear() {
          std::scoped_lock lock(mux_queue);
          deq_queue.clear();
        }

        void wait() {
          while (empty()) {
            std::unique_lock<std::mutex> ul(mux_blocking);
            cv_blocking.wait(ul);
          }
        }

      protected:
        std::mutex mux_queue;
        std::deque<T> deq_queue;
        std::condition_variable cv_blocking;
        std::mutex mux_blocking;
    }
  }
}