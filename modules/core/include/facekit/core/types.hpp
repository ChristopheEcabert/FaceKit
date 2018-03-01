/**
 *  @file   types.hpp
 *  @brief Data type definitions. Mostly inspired by Tensforflow framework
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   13.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_TYPES__
#define __FACEKIT_TYPES__

#include <string>

#include "types.pb.h"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @name DataTypeToString
 *  @fn std::string DataTypeToString(const DataType& type)
 *  @brief  Convert a given \p type into string.
 *  @param[in] type Data type to convert
 *  @return Data type converted to string
 */
std::string DataTypeToString(const DataType& type);

/**
 *  @name DataTypeFromString
 *  @fn bool DataTypeFromString(const std::string& str, DataType* type)
 *  @brief  Convert a given \p str into the corresponding DataType.
 *  @param[in]  str String to convert to data type
 *  @param[out] type Data type corresponding
 *  @return true if conversion is successful, false otherwise
 */
bool DataTypeFromString(const std::string& str, DataType* type);
  
/**
 *  @name   DataTypeDynamicSize
 *  @fn     size_t DataTypeDynamicSize(const DataType& dtype)
 *  @brief  Provide dynamically the size of a given type
 *  @param[in] dtype  Data type to query its size
 */
size_t DataTypeDynamicSize(const DataType& dtype);
  
/**
 *  @struct  IsDataTypeValid
 *  @brief  Indicate if T is a valid DataType or not
 *  @author Christophe Ecabert
 *  @date   13/02/2018
 *  @tparam T Data type (float, int32, ...)
 *  @ingroup core
 */
template<class T>
struct IsDataTypeValid {
  static constexpr bool value = false;
};
  
/**
 *  @struct  DataTypeFromEnum
 *  @brief  Convert a DataType to the underlying type 
 *          (i.e. DataTypeFromEnum<T>::Type)
 *  @author Christophe Ecabert
 *  @date   13/02/2018
 *  @tparam ENUM Enum representing a data type
 *  @ingroup core
 */
template<DataType ENUM>
struct DataTypeFromEnum {};
  
/**
 *  @struct  DataTypeSize
 *  @brief  Provide the size in bytes for a given DataType
 *          (i.e. DataTypeSize<T>::value)
 *  @author Christophe Ecabert
 *  @date   13/02/2018
 *  @tparam ENUM Enum representing a data type
 *  @ingroup core
 */
template<DataType ENUM>
struct DataTypeSize {
  static constexpr size_t value = 0;
};
  
/**
 *  @struct  DataTypeToEnum
 *  @brief  Deduce a DataType from the underlying type
 *          (i.e. DataTypeToEnum<T>::value)
 *  @author Christophe Ecabert
 *  @date   13/02/2018
 *  @tparam T Data type
 *  @ingroup core
 */
template<class T>
struct DataTypeToEnum {
  static_assert(IsDataTypeValid<T>::value, "Required Data Type not supported");
};
  
/**
 *  @def  SPECIALIZE_TYPE_AND_ENUM
 *  @brief  Define template specialization for the supported data type
 */
#define SPECIALIZE_TYPE_AND_ENUM(TYPE, ENUM, SIZE)  \
  template<>                                        \
  struct IsDataTypeValid<TYPE> {                    \
    static constexpr bool value = true;             \
  };                                                \
  template<>                                        \
  struct DataTypeFromEnum<ENUM> {                   \
    typedef TYPE Type;                              \
  };                                                \
  template<>                                        \
  struct DataTypeToEnum<TYPE> {                     \
    static DataType v() { return ENUM; }            \
    static constexpr DataType value = ENUM;         \
  };                                                \
  template<>                                        \
  struct DataTypeSize<ENUM> {                       \
    static constexpr size_t value= SIZE;            \
  };
  
/** Int8 Specialization */
SPECIALIZE_TYPE_AND_ENUM(int8_t, DataType::kInt8, sizeof(int8_t));
/** UInt8 Specialization */
SPECIALIZE_TYPE_AND_ENUM(uint8_t, DataType::kUInt8, sizeof(uint8_t));
/** Int16 Specialization */
SPECIALIZE_TYPE_AND_ENUM(int16_t, DataType::kInt16, sizeof(int16_t));
/** UInt16 Specialization */
SPECIALIZE_TYPE_AND_ENUM(uint16_t, DataType::kUInt16, sizeof(uint16_t));
/** Int32 Specialization */
SPECIALIZE_TYPE_AND_ENUM(int32_t, DataType::kInt32, sizeof(int32_t));
/** UInt32 Specialization */
SPECIALIZE_TYPE_AND_ENUM(uint32_t, DataType::kUInt32, sizeof(uint32_t));
/** Int64 Specialization */
SPECIALIZE_TYPE_AND_ENUM(int64_t, DataType::kInt64, sizeof(int64_t));
/** UInt64 Specialization */
SPECIALIZE_TYPE_AND_ENUM(uint64_t, DataType::kUInt64, sizeof(uint64_t));
/** Float Specialization */
SPECIALIZE_TYPE_AND_ENUM(float, DataType::kFloat, sizeof(float));
/** Double Specialization */
SPECIALIZE_TYPE_AND_ENUM(double, DataType::kDouble, sizeof(double));
/** bool Specialization */
SPECIALIZE_TYPE_AND_ENUM(bool, DataType::kBool, sizeof(bool));
/** string Specialization */
SPECIALIZE_TYPE_AND_ENUM(std::string, DataType::kString, sizeof(std::string));
  
#undef SPECIALIZE_TYPE_AND_ENUM
  
}  // namespace FaceKit
#endif /* __FACEKIT_TYPES__ */
