/**
 *  @file   types.cpp
 *  @brief Data type definitions
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   13.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

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
    case DataType::kFloat:
      return "float";
    case DataType::kDouble:
      return "double";
    case DataType::kSize_t:
      return "size_t";
    case DataType::kBool:
      return "bool";
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
  } else if (str == "float") {
    *type = DataType::kFloat;
    return true;
  } else if (str == "double") {
    *type = DataType::kDouble;
    return true;
  } else if (str == "size_t") {
    *type = DataType::kSize_t;
    return true;
  } else if (str == "bool") {
    *type = DataType::kBool;
    return true;
  }
  return false;
}
  
  
  
}  // namespace FaceKit
