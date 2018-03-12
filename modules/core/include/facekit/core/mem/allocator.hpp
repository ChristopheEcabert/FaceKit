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
   *  @name   Allocator
   *  @fn     Allocator(void)
   *  @brief  Constructor
   */
  Allocator(void) : call_cdtor_(true) {}
  
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
      T* t_ptr = reinterpret_cast<T*>(ptr);
      if (t_ptr && this->call_cdtor_) {
        CallCTor(n_element, t_ptr);
      }
      return t_ptr;
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
      if (this->call_cdtor_) {
        CallDTor<T>(n_element, ptr);
      }
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
  
#pragma mark -
#pragma mark Protected
 protected:
  /** Indicate if constructor/destructor needs to be runned */
  bool call_cdtor_;
  
#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   CallCTor
   *  @tparam T Object type being allocated
   *  @fn     void CallCTor(const size_t& n_element, T* ptr)
   *  @brief  Call constructor for specific type, this is mandatory with placed 
   *          instantiation
   *  @param[in] n_element  Number of element to constructor
   *  @param[in,out]  ptr   Buffer where elements need to be constructed
   */
  template<typename T>
  void CallCTor(const size_t& n_element, T* ptr) {
    static_assert(std::is_trivial<T>::value,
                  "Type T is not trivial, spcial constructor is needed");
  }
  
  /**
   *  @name   CallDTor
   *  @tparam T Object type being allocated
   *  @fn     void CallDTor(const size_t& n_element, T* ptr)
   *  @brief  Call destructor for specific type, this is mandatory with placed
   *          instantiation
   *  @param[in] n_element  Number of element to destruct
   *  @param[in,out]  ptr   Buffer where elements need to be destructed
   */
  template<typename T>
  void CallDTor(const size_t& n_element, T* ptr) {}
};
  
// String specialization
template<>
inline void Allocator::CallCTor(const size_t& n_element, std::string* t_ptr) {
  for (size_t i = 0; i < n_element; ++i, ++t_ptr) {
    new (t_ptr) std::string();
  }
}
template<>
inline void Allocator::CallDTor(const size_t& n_element, std::string* t_ptr) {
  for (size_t i = 0; i < n_element; ++i, ++t_ptr) {
    t_ptr->~basic_string();
  }
}
  
  
  
  
  
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
  
/**
 *  @name   GetAllocator
 *  @fn     Allocator* GetAllocator(const std::string& name)
 *  @brief  Search for an allocator of a given name
 *  @param[in] name Allocator's name
 *  @return corresponding allocator or nullptr of it does not excist
 */
Allocator* GetAllocator(const std::string& name);
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_ALLOCATOR__ */
