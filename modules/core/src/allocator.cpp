/**
 *  @file   allocator.cpp
 *  @brief Memory allocator interface. Inspired from Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <mutex>

#include "facekit/core/mem/allocator.hpp"
#include "facekit/core/mem/allocator_factory.hpp"
#include "facekit/core/mem/memory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Statistics
  
/*
 *  @name   AllocatorStatistic
 *  @fn     AllocatorStatistic(void)
 *  @brief  Constructor
 */
AllocatorStatistic::AllocatorStatistic(void) {
  this->Clear();
}
  
/*
 *  @name   Clear
 *  @fn     void Clear(void)
 *  @brief  Reset statistics
 */
void AllocatorStatistic::Clear(void) {
  n_alloc = 0;
  used_bytes = 0;
  max_used_bytes = 0;
  max_alloc_size = 0;
}
  
/*
 *  @name   ToString
 *  @fn     std::string ToString(void) const
 *  @brief  Convert statistics to a readable string
 *  @return String formatted statistics
 */
std::string AllocatorStatistic::ToString(void) const {
  std::string str;
  str += "Used:          "  + std::to_string(used_bytes) + "\n";
  str += "Max used:      "  + std::to_string(max_used_bytes) + "\n";
  str += "#Allocs:       "  + std::to_string(n_alloc) + "\n";
  str += "Max allocated: "  + std::to_string(max_alloc_size) + "\n";
  return str;
}
  
#pragma mark -
#pragma mark Allocator
  
// Default alignment
constexpr size_t Allocator::kDefaultAlignment;
  
/*
 *  @name   ~Allocator
 *  @fn     virtual ~Allocator(void)
 *  @brief  Destructor
 */
Allocator::~Allocator(void) = default;
  
// Flag indicating if statistics are filled or not
static bool allocator_gather_stats = false;
  
/*
 *  @name   EnableAllocatorStatistics
 *  @fn     void EnableAllocatorStatistics(const bool& enable)
 *  @brief  Toggle allocation statistics
 *  @param[in]  enable  Indicate if statistics are filled or not
 */
void EnableAllocatorStatistics(const bool& enable) {
  allocator_gather_stats = enable;
}
  
#pragma mark -
#pragma mark CPU Allocator
  
/**
 *  @class  CpuAllocator
 *  @brief  Basic Memory allocator for CPU
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  @ingroup core
 */
class CpuAllocator : public Allocator {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   CpuAllocator
   *  @fn     CpuAllocator(void) = default
   *  @brief  Constructor
   */
  CpuAllocator(void) = default;
  
  /**
   *  @name   CpuAllocator
   *  @fn     CpuAllocator(const CpuAllocator& other) = delete
   *  @brief  Copy constructor
   *  @param[in]  other Object to copy from
   */
  CpuAllocator(const CpuAllocator& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     CpuAllocator& operator=(const CpuAllocator& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  CpuAllocator& operator=(const CpuAllocator& rhs) = delete;
  
  /**
   *  @name   ~CpuAllocator
   *  @fn     ~CpuAllocator(void) override = default
   *  @brief  Destructor
   */
  ~CpuAllocator(void) override = default;
  
  /**
   *  @name   Name
   *  @fn     std::string Name(void) const override
   *  @brief  Give allocator's name
   */
  std::string Name(void) const override {
    return "default_cpu_allocator";
  }
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   AllocateRaw
   *  @fn     void* AllocateRaw(const size_t& size,
                                const size_t& alignment) override
   *  @brief  Allocate a block of memory of a given size with specific
   *          alignment. Alignment must be a power of 2 and minimum of
   *          sizeof(void*)
   *  @param[in]  size      Size of the memory block in bytes
   *  @param[in]  alignment Desired memory alignment
   *  @return Pointer to memory block or nullptr if failed.
   */
  void* AllocateRaw(const size_t& size, const size_t& alignment) override {
    void* ptr = Mem::MallocAligned(size, alignment);
    if (allocator_gather_stats ) {
      std::lock_guard<std::mutex> l(lock_);
      ++stats_.n_alloc;
      stats_.used_bytes += size;
      stats_.max_used_bytes = std::max(stats_.max_used_bytes,
                                       stats_.used_bytes);
      stats_.max_alloc_size = std::max(stats_.max_alloc_size, size);
    }
    return ptr;
  };
  
  /**
   *  @name   DeallocateRaw
   *  @fn     void DeallocateRaw(void* ptr) override
   *  @brief  Release a block of memory pointed by `ptr`
   *  @param[in]  size  Size of the memory block in bytes
   *  @param[in]  ptr   Pointer to memory block
   */
  void DeallocateRaw(const size_t& size, void* ptr) override {
    if (allocator_gather_stats) {
      std::lock_guard<std::mutex> l(lock_);
      stats_.used_bytes -= size;
    }
    Mem::FreeAligned(ptr);
  }
  
  /**
   *  @name   GatherStatistics
   *  @fn     virtual void GatherStatistics(AllocatorStatistic* stats)
   *  @brief  Provide statistics for this allocator
   *  @param[out] stats   Object filled with statistics from this allocator
   */
  void GatherStatistics(AllocatorStatistic* stats) override {
    std::lock_guard<std::mutex> l(lock_);
    *stats = stats_;
  }
  
  /**
   *  @name   ClearStatistics
   *  @fn     void ClearStatistics(void) override
   *  @brief  Clear allocator's statistics
   */
  void ClearStatistics(void) override {
    std::lock_guard<std::mutex> l(lock_);
    stats_.Clear();
  }
  
 private:
  /** Mutex */
  std::mutex lock_;
  /** Statistics */
  AllocatorStatistic stats_;
};
  
/*
 *  @name   DefaultCpuAllocator
 *  @fn     Allocator* DefaultCpuAllocator(void)
 *  @brief  Provide default allocator for CPU usage
 *  @return CPU Allocator
 */
Allocator* DefaultCpuAllocator(void) {
  return AllocatorFactory::Get().GetAllocator("default_cpu_allocator");
}
  
/*
 *  @name   GetAllocator
 *  @fn     Allocator* GetAllocator(const std::string& name)
 *  @brief  Search for an allocator of a given name
 *  @param[in] name Allocator's name
 *  @return corresponding allocator or nullptr of it does not excist
 */
Allocator* GetAllocator(const std::string& name) {
  return AllocatorFactory::Get().GetAllocator(name);
}
  
// Register CPU
REGISTER_ALLOCATOR("default_cpu_allocator", CpuAllocator);
  
}  // namespace FaceKit
