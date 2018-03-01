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
T* AlignPointer(const size_t& alignment, T* ptr);
  
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
 *  @fn     void* Malloc(const size_t& size, const size_t& alignment)
 *  @brief  Allocate a block of memory of a given size with a given alignment 
 *          requirement (Done by hand)
 *  @param[in]  size      Number of bytes needed
 *  @param[in]  alignment Desired alignment
 *  @return Pointer to the allocated memory or nullptr if failed
 *  @ingroup core
 */
void* Malloc(const size_t& size, const size_t& alignment);
  
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
  
#pragma mark -
#pragma mark Implementation
  
/*
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
inline T* AlignPointer(const size_t& alignment, T* ptr) {
  assert((alignment & (alignment-1)) == 0);
  return (T*)(((size_t)ptr + alignment - 1) & -(int)alignment);
}
  
}  // namespace Mem
}  // namespace FaceKit
#endif /* __FACEKIT_MEMORY__ */
