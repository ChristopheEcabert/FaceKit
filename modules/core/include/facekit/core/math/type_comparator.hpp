/**
 *  @file   type_comparator.hpp
 *  @brief  Helper class to compare floating number
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   30/07/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_TYPE_COMPARATOR__
#define __FACEKIT_TYPE_COMPARATOR__

#include <cstdlib>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/**
 *  @class  TComparator
 *  @brief  Class to compare floating number
 *  @author Christophe Ecabert
 *  @date   30/07/16
 *  @ingroup core
 *  @see https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 */
template<typename T>
class FACEKIT_EXPORTS TComparator {

 public:

#pragma mark -
#pragma mark Initialization

  /**
   *  @name TComparator
   *  @fn TComparator(void)
   *  @brief  Constructor
   */
  TComparator(void) : v_(0) {}

  /**
   *  @name TComparator
   *  @fn explicit TComparator(const T value)
   *  @brief  Constructor
   */
  explicit TComparator(const T value) : v_(value) {}

  /**
   * @name IsNegative
   * @fn  bool IsNegative(void) const
   * @brief Check if number is negative
   * @return  True if negative, false otherwise
   */
  bool IsNegative(void) const {
    return bits_ < T(0);
  }

#pragma mark -
#pragma mark Operator

  /**
   * @name operator==
   * @fn  bool operator==(const TComparator<T>& rhs)
   * @brief Equality operator
   * @param[in] rhs  Element to compare with
   * @return  True if equal, false otherwise
   */
  bool operator==(const TComparator<T>& rhs) {
    return this->v_ == rhs.v_;
  }

  /**
   * @name  operator==
   * @fn  bool operator==(const TComparator<T>& rhs) const
   * @brief Equality operator
   * @param[in] rhs  Element to compare with
   * @return  True if equal, false otherwise
   */
  bool operator==(const TComparator<T>& rhs) const {
    return this->v_ == rhs.v_;
  }

  /**
   * @name operator!=
   * @fn  bool operator!=(const TComparator<T>& rhs)
   * @brief Inequality operator
   * @param[in] rhs  Element to compare with
   * @return  False if equal, true otherwise
   */
  bool operator!=(const TComparator<T>& rhs) {
    return this->v_ != rhs.v_;
  }

  /**
   * @name operator!=
   * @fn  bool operator!=(const TComparator<T>& rhs)
   * @brief Inequality operator
   * @param[in] rhs  Element to compare with
   * @return  False if equal, true otherwise
   */
  bool operator!=(const TComparator<T>& rhs) const {
    return this->v_ != rhs.v_;
  }

#pragma mark -
#pragma mark Private
 private:

  /**
   * How many ULP's (Units in the Last Place) we want to tolerate when
   * comparing two numbers.  The larger the value, the more error we
   * allow.  A 0 value means that two numbers must be exactly the same
   * to be considered equal.
   *
   * The maximum error of a single floating-point operation is 0.5
   * units in the last place.  On Intel CPU's, all floating-point
   * calculations are done with 80-bit precision, while double has 64
   * bits.  Therefore, 4 should be enough for ordinary use.
   */
  static const int32_t max_ulps_ = 4;
  
  union {
    /** Value */
    T v_;
    /** Binary number */
    int32_t bits_;
  };
};

/**
 *  @class  TComparator
 *  @brief  Class to compare floating number
 *  @author Christophe Ecabert
 *  @date   30/07/16
 *  @ingroup core
 *  @see https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 */
template<>
class FACEKIT_EXPORTS TComparator<float> {

public:

#pragma mark -
#pragma mark Initialization

  /**
   *  @name TComparator
   *  @fn TComparator(void)
   *  @brief  Constructor
   */
  TComparator(void) : v_(0) {}

  /**
   *  @name TComparator
   *  @fn explicit TComparator(const float value)
   *  @brief  Constructor
   */
  explicit TComparator(const float value) : v_(value) {}

  /**
   * @name IsNegative
   * @fn  bool IsNegative(void) const
   * @brief Check if number is negative
   * @return  True if negative, false otherwise
   */
  bool IsNegative(void) const {
    return bits_ < 0.f;
  }

#pragma mark -
#pragma mark Operator

  /**
   * @name operator==
   * @fn  bool operator==(const TComparator<float>& rhs)
   * @brief Equality operator
   * @param[in] rhs  Element to compare with
   * @return  True if equal, false otherwise
   */
  bool operator==(const TComparator<float>& rhs) {
    // Different signs means they do not match.
    if (this->IsNegative() != rhs.IsNegative()) {
      // Check for equality to make sure +0==-0
      return this->v_ == rhs.v_;
    }
    // Find the difference in ULPs.
    return std::abs(this->bits_ - rhs.bits_) <= max_ulps_;
  }

  /**
   * @name  operator==
   * @fn  bool operator==(const TComparator<float>& rhs) const
   * @brief Equality operator
   * @param[in] rhs  Element to compare with
   * @return  True if equal, false otherwise
   */
  bool operator==(const TComparator<float>& rhs) const {
    // Different signs means they do not match.
    if (this->IsNegative() != rhs.IsNegative()) {
      // Check for equality to make sure +0==-0
      return this->v_ == rhs.v_;
    }
    // Find the difference in ULPs.
    return std::abs(this->bits_ - rhs.bits_) <= max_ulps_;
  }

  /**
   * @name operator!=
   * @fn  bool operator!=(const TComparator<float>& rhs)
   * @brief Inequality operator
   * @param[in] rhs  Element to compare with
   * @return  False if equal, true otherwise
   */
  bool operator!=(const TComparator<float>& rhs) {
    return this->v_ != rhs.v_;
  }

  /**
   * @name operator!=
   * @fn  bool operator!=(const TComparator<float>& rhs)
   * @brief Inequality operator
   * @param[in] rhs  Element to compare with
   * @return  False if equal, true otherwise
   */
  bool operator!=(const TComparator<float>& rhs) const {
    return this->v_ != rhs.v_;
  }

#pragma mark -
#pragma mark Private
private:

  /**
   * How many ULP's (Units in the Last Place) we want to tolerate when
   * comparing two numbers.  The larger the value, the more error we
   * allow.  A 0 value means that two numbers must be exactly the same
   * to be considered equal.
   *
   * The maximum error of a single floating-point operation is 0.5
   * units in the last place.  On Intel CPU's, all floating-point
   * calculations are done with 80-bit precision, while double has 64
   * bits.  Therefore, 4 should be enough for ordinary use.
   */
  static const int32_t max_ulps_ = 4;
  
  union {
    /** Value */
    float v_;
    /** Binary number */
    int32_t bits_;
  };
};

/**
 *  @class  TComparator
 *  @brief  Class to compare floating number
 *  @author Christophe Ecabert
 *  @date   30/07/16
 *  @ingroup core
 *  @see https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 */
template<>
class FACEKIT_EXPORTS TComparator<double> {

public:

#pragma mark -
#pragma mark Initialization

  /**
   *  @name TComparator
   *  @fn TComparator(void)
   *  @brief  Constructor
   */
  TComparator(void) : v_(0) {}

  /**
   *  @name TComparator
   *  @fn explicit TComparator(const double value)
   *  @brief  Constructor
   */
  explicit TComparator(const double value) : v_(value) {}

  /**
   * @name IsNegative
   * @fn  bool IsNegative(void) const
   * @brief Check if number is negative
   * @return  True if negative, false otherwise
   */
  bool IsNegative(void) const {
    return bits_ < 0.0;
  }

#pragma mark -
#pragma mark Operator

  /**
   * @name operator==
   * @fn  bool operator==(const TComparator<double>& rhs)
   * @brief Equality operator
   * @param[in] rhs  Element to compare with
   * @return  True if equal, false otherwise
   */
  bool operator==(const TComparator<double>& rhs) {
    // Different signs means they do not match.
    if (this->IsNegative() != rhs.IsNegative()) {
      // Check for equality to make sure +0==-0
      return this->v_ == rhs.v_;
    }
    // Find the difference in ULPs.
    return std::abs(this->bits_ - rhs.bits_) <= max_ulps_;
  }

  /**
   * @name  operator==
   * @fn  bool operator==(const TComparator<double>& rhs) const
   * @brief Equality operator
   * @param[in] rhs  Element to compare with
   * @return  True if equal, false otherwise
   */
  bool operator==(const TComparator<double>& rhs) const {
    // Different signs means they do not match.
    if (this->IsNegative() != rhs.IsNegative()) {
      // Check for equality to make sure +0==-0
      return this->v_ == rhs.v_;
    }
    // Find the difference in ULPs.
    return std::abs(this->bits_ - rhs.bits_) <= max_ulps_;
  }

  /**
   * @name operator!=
   * @fn  bool operator!=(const TComparator<double>& rhs)
   * @brief Inequality operator
   * @param[in] rhs  Element to compare with
   * @return  False if equal, true otherwise
   */
  bool operator!=(const TComparator<double>& rhs) {
    return this->v_ != rhs.v_;
  }

  /**
   * @name operator!=
   * @fn  bool operator!=(const TComparator<double>& rhs)
   * @brief Inequality operator
   * @param[in] rhs  Element to compare with
   * @return  False if equal, true otherwise
   */
  bool operator!=(const TComparator<double>& rhs) const {
    return this->v_ != rhs.v_;
  }

#pragma mark -
#pragma mark Private
private:

  /**
   * How many ULP's (Units in the Last Place) we want to tolerate when
   * comparing two numbers.  The larger the value, the more error we
   * allow.  A 0 value means that two numbers must be exactly the same
   * to be considered equal.
   *
   * The maximum error of a single floating-point operation is 0.5
   * units in the last place.  On Intel CPU's, all floating-point
   * calculations are done with 80-bit precision, while double has 64
   * bits.  Therefore, 4 should be enough for ordinary use.
   */
  static const int64_t max_ulps_ = 4;
  
  union {
    /** Value */
    double v_;
    /** Binary number */
    int64_t bits_;
  };
};


}  // namespace FaceKit


#endif /* __FACEKIT_TYPE_COMPARATOR__ */
