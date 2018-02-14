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

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @enum DataType
 *  @brief  List all supported types
 *  @ingroup core
 */
enum DataType {
  /** Unknown */
  kUnknown = 0,
  /** Signed Int8 */
  kInt8,
  /** Signed UInt8 */
  kUInt8,
  /** Signed Int16 */
  kInt16,
  /** Signed UInt16 */
  kUInt16,
  /** Signed Int32 */
  kInt32,
  /** Signed UInt32 */
  kUInt32,
  /** Float - 32bits */
  kFloat,
  /** Double - 64bits */
  kDouble,
  /** Size_t */
  kSize_t,
  /** Bool */
  kBool
};
  
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
 *  @macro  SPECIALIZE_TYPE_AND_ENUM
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
/** UInt8 Specialization */
SPECIALIZE_TYPE_AND_ENUM(uint32_t, DataType::kUInt32, sizeof(uint32_t));
/** Float Specialization */
SPECIALIZE_TYPE_AND_ENUM(float, DataType::kFloat, sizeof(float));
/** Double Specialization */
SPECIALIZE_TYPE_AND_ENUM(double, DataType::kDouble, sizeof(double));
/** size_t Specialization */
SPECIALIZE_TYPE_AND_ENUM(size_t, DataType::kSize_t, sizeof(size_t));
/** bool Specialization */
SPECIALIZE_TYPE_AND_ENUM(bool, DataType::kBool, sizeof(bool));

#undef SPECIALIZE_TYPE_AND_ENUM
  
}  // namespace FaceKit
#endif /* __FACEKIT_TYPES__ */
