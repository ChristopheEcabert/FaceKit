/**
 *  @file   object_header.cpp
 *  @brief Object Descriptor stored in file to handle "navigation" between
 *         consecutive stored object
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include "facekit/io/object_header.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name ObjectHeader
 *  @fn ObjectHeader(void) = default
 *  @brief  Constructor
 */
ObjectHeader::ObjectHeader(void) : id_(0), size_(0) {
}


/*
 *  @name ObjectHeader
 *  @fn ObjectHeader(void) = default
 *  @brief  Constructor
 *  @param[in] id   Object's ID
 *  @param[in] size Object's size in bytes
 */
ObjectHeader::ObjectHeader(const size_t id,
                           const size_t size) : id_(id), size_(size) {
}
  
/*
 *  @name operator<<
 *  @fn friend std::ostream& operator<<(std::ostream& os,
 const ObjectHeader& header);
 *  @brief  Write a given header into a stream
 *  @param[in]  os  Output stream
 *  @param[in]  header  Header to dump into the stream
 *  @return Output stream to support chaining
 */
std::ostream& operator<<(std::ostream& os, const ObjectHeader& header) {
  os.write(reinterpret_cast<const char*>(header.id_), sizeof(header.id_));
  os.write(reinterpret_cast<const char*>(header.size_), sizeof(header.size_));
  return os;
}

/*
 *  @name operator>>
 *  @fn friend std::istream& operator>>(std::istream& is,
 ObjectHeader& header)
 *  @brief  Read a header from a stream
 *  @param[in]  is  Output stream
 *  @param[out]  header  Readed header from the stream
 *  @return Output stream to support chaining
 */
std::istream& operator>>(std::istream& is, ObjectHeader& header) {
  is.read(reinterpret_cast<char*>(header.id_), sizeof(header.id_));
  is.read(reinterpret_cast<char*>(header.size_), sizeof(header.size_));
  return is;
}
  
}  // namespace FaceKit
