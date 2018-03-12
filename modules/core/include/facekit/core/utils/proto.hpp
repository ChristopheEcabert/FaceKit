/**
 *  @file   proto.hpp
 *  @brief Utility function for dealing with protocol buffer object
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   01.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_PROTO__
#define __FACEKIT_PROTO__

#include <string>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @name   AddVarInt32
 *  @fn     void AddVarInt32(const uint32_t& value, std::string* str)
 *  @brief  Convert a given value into its varint representation and add it to
 *          a given string
 *  @param[in] value  Value to convert to add
 *  @param[out] str   Buffer in which to add representation
 */
void FK_EXPORTS AddVarInt32(const uint32_t& value, std::string* str);
  
/**
 *  @name   RetrieveVarInt32
 *  @fn     bool RetrieveVarInt32(std::string* str, uint32_t* value)
 *  @brief  Extract a value from its varint representation stored in a given 
 *          array
 *  @param[in,out] str  Array storing the varints representation
 *  @param[out] value   Extracted value
 *  @return True if conversion was successfull, false otherwise
 */
bool FK_EXPORTS RetrieveVarInt32(std::string* str, uint32_t* value);
  
/**
 *  @name   EncodeStringList
 *  @fn     void EncodeStringList(const std::string* list, const size_t& n,
                                  std::string* str)
 *  @brief  Convert array of strings into a single string that can be saved
 *          into a protobuf object
 *  @param[in] list Array of string to convert
 *  @param[in] n    Array dimension
 *  @param[out] str Encoded string
 *  @ingroup core
 */
void FK_EXPORTS EncodeStringList(const std::string* list,
                                 const size_t& n,
                                 std::string* str);
  
/**
 *  @name   DecodeStringList
 *  @fn     bool DecodeStringList(const std::string& in, const size_t& n,
                                  std::string* str)
 *  @brief  Split a given string `str` into the corresponding piece stored 
 *          as array. Concatenated comes from protobuf object
 *  @param[in]  in  Concatenated string
 *  @param[in]  n   Number of string to store in the array
 *  @param[in,out] str  Array of strings
 *  @return True if successfully decoded, false otherwise
 */
bool FK_EXPORTS DecodeStringList(const std::string& in,
                                 const size_t& n,
                                 std::string* str);
  
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_PROTO__ */
