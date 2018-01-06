/**
 *  @file   refcounter.hpp
 *  @brief Reference counting mechanism
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   06.01.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_REFCOUNTER__
#define __FACEKIT_REFCOUNTER__

#include <atomic>
#include <cassert>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  RefCounter
 *  @brief  Reference counting mechanism
 *  @author Christophe Ecabert
 *  @date   06.01.18
 *  @ingroup core
 */
class FK_EXPORTS RefCounter {
 public:
  /**
   *  @name   RefCounter
   *  @fn     RefCounter(void)
   *  @brief  Constructor - Init with reference counter of 1.
   */
  RefCounter(void);
  
  /**
   *  @name   RefCounter
   *  @fn     RefCounter(const RefCounter& other) = delete
   *  @brief  Copy Constructor 
   *  @param[in] other  Object to copy from
   */
  RefCounter(const RefCounter& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     RefCounter& operator=(const RefCounter& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  RefCounter& operator=(const RefCounter& rhs) = delete;
  
  /**
   *  @name   Inc
   *  @fn     void Inc(void)
   *  @brief  Increment reference counter by 1
   */
  void Inc(void);
  
  /**
   *  @name   Dec
   *  @fn     bool Dec(void)
   *  @brief  Decrement reference counter by 1
   *  @return True if reached 0 (call destructor), False otherwise
   */
  bool Dec(void);
  
  /**
   *  @name   IsOne
   *  @fn     bool IsOne(void) const
   *  @brief  Indicate if reference counter is equal to 1
   *  @return True if counter is 1, False otherwise.
   */
  bool IsOne(void) const;
  
 protected:
  
  /**
   *  @name   ~RefCounter
   *  @fn     virtual ~RefCounter(void) = default
   *  @brief  Destructor, make it protected in order to avoid this class being
   *          instantiated (only subclass can)
   */
  virtual ~RefCounter(void);
  
 private:
  /** Counter */
  std::atomic<std::int_fast32_t> cnt_;
};

// Implementation goes here since inlined function can not be put into split
// files
// See: http://www.cplusplus.com/forum/general/1393/#msg4802

/*
 *  @name   RefCounter
 *  @fn     RefCounter(void)
 *  @brief  Constructor - Init with reference counter of 1.
 */
inline RefCounter::RefCounter(void) : cnt_(1) {}

/*
 *  @name   ~RefCounter
 *  @fn     virtual ~RefCounter(void) = default
 *  @brief  Destructor, make it protected in order to avoid this class being
 *          instantiated (only subclass can)
 */
inline RefCounter::~RefCounter(void) {
  assert(cnt_.load() == 0); 
}

/*
 *  @name   Inc
 *  @fn     void Inc(void)
 *  @brief  Increment reference counter by 1
 */
inline void RefCounter::Inc(void) {
  assert(cnt_.load() >= 1);
  cnt_.fetch_add(1, std::memory_order_relaxed);
}

/*
 *  @name   Dec
 *  @fn     bool Dec(void)
 *  @brief  Decrement reference counter by 1
 *  @return True if reached 0 (call destructor), False otherwise
 */
inline bool RefCounter::Dec(void) {
  assert(cnt_.load() > 0);
  if (this->IsOne() || cnt_.fetch_sub(1) == 1) {
    cnt_.store(0);
    delete this;
    return true;
  } else {
    return false;
  }
}

/*
 *  @name   IsOne
 *  @fn     bool IsOne(void) const
 *  @brief  Indicate if reference counter is equal to 1
 *  @return True if counter is 1, False otherwise.
 */
inline bool RefCounter::IsOne(void) const {
  // Thread safefty
  return cnt_.load(std::memory_order_acquire) == 1;
}

}  // namespace FaceKit
#endif /* __FACEKIT_REFCOUNTER__ */
