// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#pragma once

#include <atomic>
#include <cstdint>
#include "address.h"
#include "async.h"
#include "native_buffer_pool.h"

#ifdef _WIN32
#include <concurrent_queue.h>

template <typename T>
using concurrent_queue = concurrency::concurrent_queue<T>;
#endif

namespace fishstore {
namespace core {

class AsyncIOContext : public IAsyncContext {
 public:
  AsyncIOContext(void* fishstore_, Address address_,
                 IAsyncContext* caller_context_,
                 concurrent_queue<AsyncIOContext*>* thread_io_responses_,
                 uint64_t io_id_)
    : fishstore{ fishstore_ }
    , address{ address_ }
    , caller_context{ caller_context_ }
    , thread_io_responses{ thread_io_responses_ }
    , io_id{ io_id_ }
    , kpt_offset{ 0 } {
  }
  /// No copy constructor.
  AsyncIOContext(const AsyncIOContext& other) = delete;
  /// The deep-copy constructor.
  AsyncIOContext(AsyncIOContext& other, IAsyncContext* caller_context_)
    : fishstore{ other.fishstore }
    , address{ other.address }
    , caller_context{ caller_context_ }
    , thread_io_responses{ other.thread_io_responses }
    , record{ std::move(other.record) }
    , io_id{ other.io_id }
    , kpt_offset{ other.kpt_offset } {
  }
 protected:
  Status DeepCopy_Internal(IAsyncContext*& context_copy) final {
    return IAsyncContext::DeepCopy_Internal(*this, caller_context, context_copy);
  }
 public:
  void* fishstore;
  Address address;
  IAsyncContext* caller_context;
  concurrent_queue<AsyncIOContext*>* thread_io_responses;
  uint64_t io_id;
  uint32_t kpt_offset;

  SectorAlignedMemory record;
};

}
} // namespace fishstore::core
