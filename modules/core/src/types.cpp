/**
 *  @file   types.cpp
 *  @brief Data type definitions
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   13.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <limits>

#include "facekit/core/types.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name DataTypeToString
 *  @fn std::string DataTypeToString(const DataType& type)
 *  @brief  Convert a given \p type into string.
 *  @param[in] type Data type to convert
 *  @return Data type converted to string
 */
std::string DataTypeToString(const DataType& type) {
  switch (type) {
    case DataType::kUnknown:
      return "unknown";
    case DataType::kInt8:
      return "int8";
    case DataType::kUInt8:
      return "uint8";
    case DataType::kInt16:
      return "int16";
    case DataType::kUInt16:
      return "uint16";
    case DataType::kInt32:
      return "int32";
    case DataType::kUInt32:
      return "uint32";
    case DataType::kInt64:
      return "int64";
    case DataType::kUInt64:
      return "uint64";
    case DataType::kFloat:
      return "float";
    case DataType::kDouble:
      return "double";
    case DataType::kBool:
      return "bool";
    case DataType::kString:
      return "string";
    default:
      FACEKIT_LOG_ERROR("unsupported data type");
      return "unsupported";
  }
}

/*
 *  @name DataTypeFromString
 *  @fn bool DataTypeFromString(const std::string& str, DataType* type)
 *  @brief  Convert a given \p str into the corresponding DataType.
 *  @param[in]  str String to convert to data type
 *  @param[out] type Data type corresponding
 *  @return true if conversion is successful, false otherwise
 */
bool DataTypeFromString(const std::string& str, DataType* type) {
  if (str == "int8") {
    *type = DataType::kInt8;
    return true;
  } else if (str == "uint8") {
    *type = DataType::kUInt8;
    return true;
  } else if (str == "int16") {
    *type = DataType::kInt16;
    return true;
  } else if (str == "uint16") {
    *type = DataType::kUInt16;
    return true;
  } else if (str == "int32") {
    *type = DataType::kInt32;
    return true;
  } else if (str == "uint32") {
    *type = DataType::kUInt32;
    return true;
  } else if (str == "int64") {
    *type = DataType::kInt64;
    return true;
  } else if (str == "uint64") {
    *type = DataType::kUInt64;
    return true;
  } else if (str == "float") {
    *type = DataType::kFloat;
    return true;
  } else if (str == "double") {
    *type = DataType::kDouble;
    return true;
  } else if (str == "bool") {
    *type = DataType::kBool;
    return true;
  } else if (str == "string") {
    *type = DataType::kString;
    return true;
  }
  return false;
}
  
/*
 *  @name   DataTypeDynamicSize
 *  @fn     size_t DataTypeDynamicSize(const DataType& dtype)
 *  @brief  Provide dynamically the size of a given type
 */
size_t DataTypeDynamicSize(const DataType& dtype) {
  switch (dtype) {
    case DataType::kUnknown:
      return 0;
    case DataType::kInt8:
      return sizeof(int8_t);
    case DataType::kUInt8:
      return sizeof(uint8_t);
    case DataType::kInt16:
      return sizeof(int16_t);
    case DataType::kUInt16:
      return sizeof(uint16_t);
    case DataType::kInt32:
      return sizeof(int32_t);
    case DataType::kUInt32:
      return sizeof(uint32_t);
    case DataType::kInt64:
      return sizeof(int64_t);
    case DataType::kUInt64:
      return sizeof(uint64_t);
    case DataType::kFloat:
      return sizeof(float);
    case DataType::kDouble:
      return sizeof(double);
    case DataType::kBool:
      return sizeof(bool);
    case DataType::kString:
      return sizeof(std::string);
    default:
      FACEKIT_LOG_ERROR("unsupported data type");
      return std::numeric_limits<size_t>::max();
  }
}
  
  
  
}  // namespace FaceKit
