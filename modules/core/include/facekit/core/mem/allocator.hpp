/**
 *  @file   allocator.hpp
 *  @brief Memory allocator interface. Inspired from Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ALLOCATOR__
#define __FACEKIT_ALLOCATOR__

#include <cstdlib>
#include <string>
#include <limits>

#include "facekit/core/library_export.hpp"
#include "facekit/core/types.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  AllocatorStatistic
 *  @brief  Gather statistics for an allocator
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  @ingroup core
 */
class FK_EXPORTS AllocatorStatistic {
 public:
  
  /**
   *  @name   AllocatorStatistic
   *  @fn     AllocatorStatistic(void)
   *  @brief  Constructor
   */
  AllocatorStatistic(void);
  
  /**
   *  @name   Clear
   *  @fn     void Clear(void)
   *  @brief  Reset statistics
   */
  void Clear(void);
  
  /**
   *  @name   ToString
   *  @fn     std::string ToString(void) const
   *  @brief  Convert statistics to a readable string
   *  @return String formatted statistics
   */
  std::string ToString(void) const;
  
  /** Number of allocation performed by this allocator */
  size_t n_alloc;
  /** Total number of byte currently being used by this allocator */
  size_t used_bytes;
  /** Largest memory usage for this allocator */
  size_t max_used_bytes;
  /** Largest memory block allocated by this allocator */
  size_t max_alloc_size;
};
  
/**
 *  @class  Allocator
 *  @brief  Memory allocator interface
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  @ingroup core
 */
class FK_EXPORTS Allocator {
 public:
  
  /** Default allocation alignment */
  static constexpr size_t kDefaultAlignment = 32;
  
  /**
   *  @name   ~Allocator
   *  @fn     virtual ~Allocator(void)
   *  @brief  Destructor
   */
  virtual ~Allocator(void);
  
  /**
   *  @name   Name
   *  @fn     virtual std::string Name(void) const = 0
   *  @brief  Give allocator's name
   */
  virtual std::string Name(void) const = 0;
  
  /**
   *  @name   AllocateRaw
   *  @fn     virtual void* AllocateRaw(const size_t& size,
  const size_t& alignment) = 0
   *  @brief  Allocate a block of memory of a given size with specific 
   *          alignment. Alignment must be a power of 2 and minimum of 
   *          sizeof(void*)
   *  @param[in]  size      Size of the memory block in bytes
   *  @param[in]  alignment Desired memory alignment
   *  @return Pointer to memory block or nullptr if failed.
   */
  virtual void* AllocateRaw(const size_t& size, const size_t& alignment) = 0;
  
  /**
   *  @name   DeallocateRaw
   *  @fn     virtual void DeallocateRaw(void* ptr) = 0
   *  @brief  Release a block of memory pointed by `ptr`
   *  @param[in]  size  Size of the memory block in bytes
   *  @param[in]  ptr   Pointer to memory block
   */
  virtual void DeallocateRaw(const size_t& size, void* ptr) = 0;
  
  /**
   *  @name   Allocate
   *  @fn     T* Allocate(const size_t& n_element)
   *  @brief  Allocate a block of memory for a specific type with default
   *          alignment.
   *  @param[in] n_element  Number of element to allocate
   *  @tparam T Data type, must be a basic type (float, int, ...)
   *  @return Pointer to the memory block, nullptr if allocation failed.
   */
  template<typename T>
  T* Allocate(const size_t& n_element) {
    return this->Allocate<T>(n_element, kDefaultAlignment);
  }
  
  /**
   *  @name   Allocate
   *  @fn     T* Allocate(const size_t& n_element, const size_t& alignment)
   *  @brief  Allocate a block of memory for a specific type / alignment
   *  @param[in] n_element  Number of element to allocate
   *  @param[in] alignment  Desired alignment
   *  @tparam T Data type, must be a basic type (float, int, ...)
   *  @return Pointer to the memory block, nullptr if allocation failed.
   */
  template<typename T>
  T* Allocate(const size_t& n_element, const size_t& alignment) {
    // Ensure only supported type are allocated, this may change in the future
    static_assert(IsDataTypeValid<T>::value, "T is not a valid type");
    
    // Check if size is not to large
    if (n_element > std::numeric_limits<size_t>::max() / sizeof(T)) {
      return nullptr;
    } else {
      // Allocate memory block using allocator
      void* ptr = this->AllocateRaw(n_element * sizeof(T), alignment);
      return reinterpret_cast<T*>(ptr);
    }
  }
  
  /**
   *  @name   Deallocate
   *  @fn     void Deallocate(const size_t& n_element, T* ptr)
   *  @brief  Release a previously allocated memory block
   *  @param[in]  n_element Number of element in the memory block
   *  @param[in]  ptr       Pointer to memory block
   *  @tparam T Data type, must be a basic type (float, int, ...)
   */
  template<typename T>
  void Deallocate(const size_t& n_element, T* ptr) {
    if (ptr) {
      this->DeallocateRaw(n_element * sizeof(T), ptr);
    }
  }
  
  /**
   *  @name   GatherStatistics
   *  @fn     virtual void GatherStatistics(AllocatorStatistic* stats)
   *  @brief  Provide statistics for this allocator
   *  @param[out] stats   Object filled with statistics from this allocator
   */
  virtual void GatherStatistics(AllocatorStatistic* stats) {
    stats->Clear();
  }
  
  /**
   *  @name   ClearStatistics
   *  @fn     virtual void ClearStatistics(void)
   *  @brief  Clear allocator's statistics
   */
  virtual void ClearStatistics(void) {}
};
  
/**
 *  @name   EnableAllocatorStatistics
 *  @fn     void EnableAllocatorStatistics(const bool& enable)
 *  @brief  Toggle allocation statistics
 *  @param[in]  enable  Indicate if statistics are filled or not
 */
void EnableAllocatorStatistics(const bool& enable);
  
/**
 *  @name   DefaultCpuAllocator
 *  @fn     Allocator* DefaultCpuAllocator(void)
 *  @brief  Provide default allocator for CPU usage
 *  @return CPU Allocator
 */
Allocator* DefaultCpuAllocator(void);
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_ALLOCATOR__ */
