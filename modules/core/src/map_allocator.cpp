/**
 *  @file   map_allocator.cpp
 *  @brief Allocator acting as a ghost allocator in order to map existing raw
 *          buffer for NDArray container.
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   24.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/mem/map_allocator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   MapAllocator
 *  @fn     explicit MapAllocator(void* ptr)
 *  @brief  Constructor
 *  @param[in] ptr  Buffer to map
 */
MapAllocator::MapAllocator(void* ptr) : buffer_(ptr) {}
  
/*
 *  @name   AllocateRaw
 *  @fn     void* AllocateRaw(const size_t& size,
 *          const size_t& alignment) override
 *  @brief  Allocate a block of memory of a given size with specific
 *          alignment. Alignment must be a power of 2 and minimum of
 *          sizeof(void*)
 *  @param[in]  size      Size of the memory block in bytes
 *  @param[in]  alignment Desired memory alignment
 *  @return Pointer to memory block or nullptr if failed.
 */
void* MapAllocator::AllocateRaw(const size_t& size,
                                const size_t& alignment) {
  return buffer_;
}
  
/*
 *  @name   DeallocateRaw
 *  @fn     void DeallocateRaw(void* ptr) override
 *  @brief  Release a block of memory pointed by `ptr`
 *  @param[in]  size  Size of the memory block in bytes
 *  @param[in]  ptr   Pointer to memory block
 */
void MapAllocator::DeallocateRaw(const size_t& size, void* ptr) {
  // no-op
}
}  // namespace FaceKit
