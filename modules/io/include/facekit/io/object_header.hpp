/**
 *  @file   object_header.hpp
 *  @brief Object Descriptor stored in file to handle "navigation" between 
 *         consecutive stored object
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_OBJECT_HEADER__
#define __FACEKIT_OBJECT_HEADER__

#include <iostream>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class    ObjectHeader
 *  @brief    Object Descriptor stored in file to handle "navigation" between
 *            consecutive stored object
 *  @author   Christophe Ecabert
 *  @date     24.09.17
 *  @ingroup  io
 */
class FK_EXPORTS ObjectHeader {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name ObjectHeader
   *  @fn ObjectHeader(void)
   *  @brief  Constructor
   */
  ObjectHeader(void);
  
  /**
   *  @name ObjectHeader
   *  @fn ObjectHeader(const size_t id, const size_t size)
   *  @brief  Constructor
   *  @param[in] id   Object's ID
   *  @param[in] size Object's size in bytes
   */
  ObjectHeader(const size_t id, const size_t size);
  
  /**
   *  @name ~ObjectHeader
   *  @fn ~ObjectHeader(void) =  default
   *  @brief  Destructor
   */
  ~ObjectHeader(void) = default;
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name get_id
   *  @fn size_t get_id(void) const
   *  @brief  Provider object ID
   *  @return Object ID
   */
  size_t get_id(void) const {
    return id_;
  }
  
  /**
   *  @name get_size
   *  @fn size_t get_size(void) const
   *  @brief  Provider object size
   *  @return Object size
   */
  size_t get_size(void) const {
    return size_;
  }
  
  
#pragma mark -
#pragma mark Operator
  
  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& os, 
                                          const ObjectHeader& header);
   *  @brief  Write a given header into a stream
   *  @param[in]  os  Output stream
   *  @param[in]  header  Header to dump into the stream
   *  @return Output stream to support chaining
   */
  friend std::ostream& operator<<(std::ostream& os, const ObjectHeader& header);
  
  /**
   *  @name operator>>
   *  @fn friend std::istream& operator>>(std::istream& is,
                                          ObjectHeader& header)
   *  @brief  Read a header from a stream
   *  @param[in]  is  Output stream
   *  @param[out]  header  Readed header from the stream
   *  @return Output stream to support chaining
   */
  friend std::istream& operator>>(std::istream& is, ObjectHeader& header);
  
 private:
  /** Object ID - Unique */
  size_t id_;
  /** Object size in bytes */
  size_t size_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_OBJECT_HEADER__ */
