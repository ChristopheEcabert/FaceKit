/**
 *  @file   proto.cpp
 *  @brief Utility function for dealing with protocol buffer object
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   01.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <vector>

#include "facekit/core/utils/proto.hpp"


/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @name   EncodeVarInt32
 *  @fn     char* EncodeVarInt32(uint32_t value, char* dst)
 *  @brief  Convert a given value into proper formatting for protobuf object
 *  @see https://developers.google.com/protocol-buffers/docs/encoding#varints
 *  @param[in] value  Value to convert to varints
 *  @param[out] dst   Buffer where to write the results
 *  @return Final buffer position
 */
char* EncodeVarInt32(uint32_t value, char* dst) {
  // Operate on characters as unsigneds
  unsigned char* ptr = reinterpret_cast<unsigned char*>(dst);
  static const int B = 128; // Mask 8th bit to indicate more bytes are needed
  if (value < (1 << 7)) { // [0-127]
    *(ptr++) = value;
  } else if (value < (1 << 14)) { // [0-16383]
    *(ptr++) = value | B;
    *(ptr++) = value >> 7;
  } else if (value < (1 << 21)) { // [0-2'097'151]
    *(ptr++) = value | B;
    *(ptr++) = (value >> 7) | B;
    *(ptr++) = value >> 14;
  } else if (value < (1 << 28)) { // [0-268'435'455]
    *(ptr++) = value | B;
    *(ptr++) = (value >> 7) | B;
    *(ptr++) = (value >> 14) | B;
    *(ptr++) = value >> 21;
  } else { // larger than ]268'435'455, 2^32]
    *(ptr++) = value | B;
    *(ptr++) = (value >> 7) | B;
    *(ptr++) = (value >> 14) | B;
    *(ptr++) = (value >> 21) | B;
    *(ptr++) = value >> 28;
  }
  return reinterpret_cast<char*>(ptr);
}
  
  /**
   *  @name   DecodeVarInt32
   *  @fn     const char* DecodeVarInt32(const char* p, const char* limit, uint32_t* value)
   *  @brief  Extract value from protobuf code varints
   *  @param[in] p      Array storing the encoded value
   *  @param[in] limit  Search space limit
   *  @param[out] value Extracted value
   *  @return Pointer to the next coded value
   */
const char* DecodeVarInt32(const char* p, const char* limit, uint32_t* value) {
  if (p < limit) {
    const uint32_t result = *(reinterpret_cast<const unsigned char*>(p));
    if ((result & 128) == 0) {
      *value = result;
      return p + 1;
    }
  }
  // There is more than one value
  uint32_t res = 0;
  // Loop at most 4 times (== 32bits)
  for (uint32_t shift = 0; shift <= 28 && p < limit; shift += 7) {
    uint32_t byte = *(reinterpret_cast<const unsigned char*>(p));
    p++;
    if (byte & 128) {
      // More bytes need to go further
      res |= ((byte & 127) << shift);
    } else {
      // Reach MSB
      res |= (byte << shift);
      *value = res;
      return reinterpret_cast<const char*>(p);
    }
  }
  // Not suppost to reach this position
  return nullptr;
}
  
/*
 *  @name   AddVarInt32
 *  @fn     void AddVarInt32(const uint32_t& value, std::string* str)
 *  @brief  Convert a given value into its varint representation and add it to
 *          a given string
 *  @param[in] value  Value to convert to add
 *  @param[out] str   Buffer in which to add representation
 */
void AddVarInt32(const uint32_t& value, std::string* str) {
  char buff[5];
  char* ptr = EncodeVarInt32(value, buff);
  str->append(buff, ptr - buff);
}

/*
 *  @name   RetrieveVarInt32
 *  @fn     bool RetrieveVarInt32(const std::string& str, uint32_t* value)
 *  @brief  Extract a value from its varint representation stored in a given
 *          array
 *  @param[in] str    Array storing the varints representation
 *  @param[out] value Extracted value
 *  @return True if conversion was successfull, false otherwise
 */
bool RetrieveVarInt32(std::string* str, uint32_t* value) {
  const char* ptr = str->data();
  const char* limit = ptr + str->size(); // Limit scope to the whole array
  const char* q = DecodeVarInt32(ptr, limit, value);
  if (q != nullptr) {
    *str = std::string(q, limit - q);
    return true;
  }
  return false;
}
  
/*
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
void EncodeStringList(const std::string* list,
                      const size_t& n,
                      std::string* str) {
  str->clear();
  // Add dimension in front of data
  for (size_t k = 0; k < n; ++k) {
    AddVarInt32(static_cast<uint32_t>(list[k].size()), str);
  }
  // Add strings value
  for (size_t k = 0; k < n; ++k) {
    str->append(list[k]);
  }
}
  
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
bool DecodeStringList(const std::string& in,
                      const size_t& n,
                      std::string* str) {
  // Get payload dimensions stored at the begin of the array
  std::vector<uint32_t> dims(n, 0);
  std::string src(in); // Copy data
  int64_t length = 0;
  for (auto& d : dims) {
    if(!RetrieveVarInt32(&src, &d)) {
      return false;
    } else {
      length += d;
    }
  }
  // Check if we reach the end of the buffer
  if (length != static_cast<int64_t>(src.size())) {
    return false;
  }
  
  std::string* data = str;
  size_t shift = 0;
  for (size_t k = 0; k < n; ++k, ++data) {
    auto& d = dims[k];
    if (d > src.size()) {
      return false; // Buffer too shord, ask for too large chunk of memory
    }
    data->assign(src.data() + shift, d);
    shift += d;
  }
  return true;
}
  
  
  
}  // namespace FaceKit
