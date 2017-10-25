/**
 *  @file   enum_bitmask_operator.hpp
 *  @brief Allow `enum class` to be used with bitmask operation
 *  @see http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ENUM_BIT_MASK_OPERATOR__
#define __FACEKIT_ENUM_BIT_MASK_OPERATOR__

#include <type_traits>

#include "facekit/core/library_export.hpp" 

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @struct  EnableBitMaskOperators
 *  @brief  Indicate if bitmask operator are enabled for a specific enum type.
 *          By default they are not.
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  @ingroup core
 *  @tparam Enum  Type of enum to enable bitmasking operator
 */
template<typename Enum>
struct FK_EXPORTS EnableBitMaskOperators {
  /** Indicate if operators are enable for this specific type */
  static const bool enable = false;
};

/**
 *  @def ENABLE_BITMASK_OPERATORS(x)
 *  @brief  Helper macro for enabling bitmask operator for a specific enum type
 */
#define ENABLE_BITMASK_OPERATORS(x)           \
template<>                                    \
struct FK_EXPORTS EnableBitMaskOperators<x> { \
  static const bool enable = true;            \
}

/**
 *  @name   operator|
 *  @brief  OR-operator
 *  @fn Enum operator|(const Enum& lhs, const Enum& rhs)
 *  @param[in] lhs  Left hand sign
 *  @param[in] rhs  Right hand sign
 *  @return \p lhs OR \p rhs.
 *  @tparam Enum  Enumerate type
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
FK_EXPORTS operator|(const Enum& lhs, const Enum& rhs) {
  // Ensure template parameter is an enum
  static_assert(std::is_enum<Enum>::value,
                "Template parameter is not an enum type");
  using UnderlyingT = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (static_cast<UnderlyingT>(lhs) |
                            static_cast<UnderlyingT>(rhs));
}
  
/**
 *  @name   operator&
 *  @fn Enum operator&(const Enum& lhs, const Enum& rhs)
 *  @brief  AND-operator
 *  @param[in] lhs  Left hand sign
 *  @param[in] rhs  Right hand sign
 *  @return \p lhs AND \p rhs.
 *  @tparam Enum  Enumerate type
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
FK_EXPORTS operator&(const Enum& lhs, const Enum& rhs) {
  // Ensure template parameter is an enum
  static_assert(std::is_enum<Enum>::value,
                "Template parameter is not an enum type");
  using UnderlyingT = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (static_cast<UnderlyingT>(lhs) &
                            static_cast<UnderlyingT>(rhs));
}
  
/**
 *  @name   operator^
 *  @fn Enum operator^(const Enum& lhs, const Enum& rhs)
 *  @brief  XOR-operator
 *  @param[in] lhs  Left hand sign
 *  @param[in] rhs  Right hand sign
 *  @return \p lhs XOR \p rhs.
 *  @tparam Enum  Enumerate type
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
FK_EXPORTS operator^(const Enum& lhs, const Enum& rhs) {
  // Ensure template parameter is an enum
  static_assert(std::is_enum<Enum>::value,
                "Template parameter is not an enum type");
  using UnderlyingT = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (static_cast<UnderlyingT>(lhs) ^
                            static_cast<UnderlyingT>(rhs));
}
  
/**
 *  @name   operator~
 *  @fn Enum operator~(const Enum& rhs)
 *  @brief  Inversion-operator
 *  @param[in] rhs  Right hand sign
 *  @return Inversion \p rhs.
 *  @tparam Enum  Enumerate type
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
FK_EXPORTS operator~(const Enum& rhs) {
  // Ensure template parameter is an enum
  static_assert(std::is_enum<Enum>::value,
                "Template parameter is not an enum type");
  using UnderlyingT = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (~static_cast<UnderlyingT>(rhs));
}
  
/**
 *  @name   operator|=
 *  @fn Enum operator|=(const Enum& lhs, const Enum& rhs)
 *  @brief  OR-assignment-operator
 *  @param[in] lhs  Right hand sign
 *  @param[in] rhs  Right hand sign
 *  @return  \p lhs = \p lhs OR \p rhs.
 *  @tparam Enum  Enumerate type
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
FK_EXPORTS operator|=(Enum& lhs, Enum& rhs) {
  // Ensure template parameter is an enum
  static_assert(std::is_enum<Enum>::value,
                "Template parameter is not an enum type");
  using UnderlyingT = typename std::underlying_type<Enum>::type;
  lhs = static_cast<Enum> (static_cast<UnderlyingT>(rhs) |
                           static_cast<UnderlyingT>(rhs));
  return lhs;
}
  
/**
 *  @name   operator&=
 *  @fn Enum operator&=(const Enum& lhs, const Enum& rhs)
 *  @brief  AND-assignment-operator
 *  @param[in] lhs  Right hand sign
 *  @param[in] rhs  Right hand sign
 *  @return  \p lhs = \p lhs AND \p rhs.
 *  @tparam Enum  Enumerate type
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
FK_EXPORTS operator&=(Enum& lhs, Enum& rhs) {
  // Ensure template parameter is an enum
  static_assert(std::is_enum<Enum>::value,
                "Template parameter is not an enum type");
  using UnderlyingT = typename std::underlying_type<Enum>::type;
  lhs = static_cast<Enum> (static_cast<UnderlyingT>(rhs) &
                           static_cast<UnderlyingT>(rhs));
  return lhs;
}

/**
 *  @name   operator^=
 *  @fn Enum operator^=(const Enum& lhs, const Enum& rhs)
 *  @brief  AND-assignment-operator
 *  @param[in] lhs  Right hand sign
 *  @param[in] rhs  Right hand sign
 *  @return  \p lhs = \p lhs AND \p rhs.
 *  @tparam Enum  Enumerate type
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
FK_EXPORTS operator^=(Enum& lhs, Enum& rhs) {
  // Ensure template parameter is an enum
  static_assert(std::is_enum<Enum>::value,
                "Template parameter is not an enum type");
  using UnderlyingT = typename std::underlying_type<Enum>::type;
  lhs = static_cast<Enum> (static_cast<UnderlyingT>(rhs) ^
                           static_cast<UnderlyingT>(rhs));
  return lhs;
}
}  // namespace FaceKit
#endif /* __FACEKIT_ENUM_BIT_MASK_OPERATOR__ */
