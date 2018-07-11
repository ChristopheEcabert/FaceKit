/**
 *  @file   memory.hpp
 *  @brief Low level allocation helper tools
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_MEMORY__
#define __FACEKIT_MEMORY__

#include <cstdlib>
#include <cassert>

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @namespace  Mem
 *  @brief      Memory management scope
 */
namespace Mem {
  
/**
 *  @name   AlignPointer
 *  @fn     void* AlignPointer(const size_t& alignment, void* ptr)
 *  @brief  Aligns a pointer to the specified number of bytes. 
 *          Stripped from OpenCV
 *  @param[in] alignment Alignment size that must be a power of two.
 *  @param[in] ptr Aligned pointer.
 *  @return Aligned pointer
 *  @ingroup core
 */
template<typename T>
T* AlignPointer(const size_t& alignment, T* ptr) {
  assert((alignment & (alignment-1)) == 0);
  return (T*)(((size_t)ptr + alignment - 1) & -(int)alignment);
}
  
/**
 *  @name   MallocAligned
 *  @fn     void* MallocAligned(const size_t& size, const size_t& alignment)
 *  @brief  Allocate a block of memory of a given size with a specific 
 *          alignement, `alignment` must be a power of two and at least of
 *          sizeof(void*).
 *          If alignment requirement are not met, fall back to unaligned malloc.
 *  @param[in]  size      Number of bytes needed
 *  @param[in]  alignment Desired alignment
 *  @return Pointer to the allocated memory or nullptr if failed
 *  @ingroup core
 */
void* MallocAligned(const size_t& size, const size_t& alignment);
  
/**
 *  @name   FreeAligned
 *  @fn     void FreeAligned(void* ptr)
 *  @brief  Release memory block pointed by `ptr`
 *  @param[in]  ptr Memory block to release
 *  @ingroup core
 */
void FreeAligned(void* ptr);
  
/**
 *  @name   Malloc
 *  @fn     void* Malloc(const size_t& size)
 *  @brief  Allocate a block of memory of a given size
 *  @param[in]  size      Number of bytes needed
 *  @return Pointer to the allocated memory or nullptr if failed
 *  @ingroup core
 */
void* Malloc(const size_t& size);
  
/**
 *  @name   Realloc
 *  @fn     void Realloc(const size_t& new_size, void* ptr)
 *  @brief  Changes the size of the memory block pointed to by `ptr`.
 *  @param[in] new_size New size for the memory block, in bytes.
 *  @param[in] ptr      Pointer to a memory block previously allocated
 *  @return A pointer to the reallocated memory block 
 *  @ingroup core
 */
void* Realloc(const size_t& new_size, void* ptr);
  
/**
 *  @name   Free
 *  @fn     void Free(void* ptr)
 *  @brief  Deallocated a block of memory pointed by `ptr`
 *  @param[in] ptr  Pointer to a memory block previously allocated
 *  @ingroup core
 */
void Free(void* ptr);
  
/**
 *  @class  ScopedBuffer
 *  @brief  Helper class to release (i.e. free) a buffer allocated with `malloc`
 *          when the out-of-scope
 *  @author Christophe Ecabert
 *  @date   10.06.18
 *  @ingroup core
 */
class ScopedBuffer {
 public:
  
  /**
   *  @name   ScopedBuffer
   *  @fn     ScopedBuffer(void* buffer)
   *  @brief  Constructor
   *  @param[in] buffer Address of the buffer to transfer ownership
   */
  ScopedBuffer(void* buffer) : buffer_(buffer) {}
  
  /**
   *  @name   ScopedBuffer
   *  @fn     ScopedBuffer(const ScopedBuffer& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  ScopedBuffer(const ScopedBuffer& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     ScopedBuffer& operator=(const ScopedBuffer& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  ScopedBuffer& operator=(const ScopedBuffer& rhs) = delete;
  
  /**
   *  @name   ~ScopedBuffer
   *  @fn     ~ScopedBuffer(void)
   *  @brief  Destructor
   */
  ~ScopedBuffer(void) {
    if (buffer_) {
      std::free(buffer_);
    }
  }
  
 private:
  /** Buffer to manage */
  void* buffer_;
};
  
}  // namespace Mem
}  // namespace FaceKit
#endif /* __FACEKIT_MEMORY__ */
