/**
 *  @file   map_allocator.hpp
 *  @brief Allocator acting as a ghost allocator in order to map existing raw
 *          buffer for NDArray container.
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   24.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_MAP_ALLOCATOR__
#define __FACEKIT_MAP_ALLOCATOR__

#include "facekit/core/library_export.hpp"
#include "facekit/core/mem/allocator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  MapAllocator
 *  @brief  Allocator acting as a ghost allocator in order to map existing raw
 *          buffer for NDArray container.
 *  @author Christophe Ecabert
 *  @date   24.02.18
 *  @ingroup core
 */
class FK_EXPORTS MapAllocator : public Allocator {
public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   MapAllocator
   *  @fn     explicit MapAllocator(void* ptr)
   *  @brief  Constructor
   *  @param[in] ptr  Buffer to map
   */
  explicit MapAllocator(void* ptr);
  
  /**
   *  @name   MapAllocator
   *  @fn     MapAllocator(const MapAllocator& other) = delete
   *  @brief  Copy constructor
   *  @param[in]  other Object to copy from
   */
  MapAllocator(const MapAllocator& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     MapAllocator& operator=(const MapAllocator& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  MapAllocator& operator=(const MapAllocator& rhs) = delete;
  
  /**
   *  @name   ~MapAllocator
   *  @fn     ~MapAllocator(void) override = default
   *  @brief  Destructor
   */
  ~MapAllocator(void) override = default;
  
  /**
   *  @name   Name
   *  @fn     std::string Name(void) const override
   *  @brief  Give allocator's name
   */
  std::string Name(void) const override {
    return "map_alloctor";
  }
  
#pragma mark -
#pragma mark Usage
  
  /**
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
  void* AllocateRaw(const size_t& size, const size_t& alignment) override;
  
  /**
   *  @name   DeallocateRaw
   *  @fn     void DeallocateRaw(void* ptr) override
   *  @brief  Release a block of memory pointed by `ptr`
   *  @param[in]  size  Size of the memory block in bytes
   *  @param[in]  ptr   Pointer to memory block
   */
  void DeallocateRaw(const size_t& size, void* ptr) override;
  
private:
  /** Mapped buffer, should last longer than the lifetime of this allocator */
  void* buffer_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_MAP_ALLOCATOR__ */
