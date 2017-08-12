/**
 *  @file   vector.hpp
 *  @brief  Vector of size 2/3/4
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   30/07/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_VECTOR__
#define __FACEKIT_VECTOR__

#include <cmath>
#include <limits>
#include <iostream>

#include "facekit/core/library_export.hpp"
#include "facekit/core/math/type_comparator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

#pragma mark -
#pragma mark Vector2

/**
 *  @struct Vector2
 *  @brief  Vector of dimension 2
 *  @author Chrisotphe Ecabert
 *  @date   30/07/16
 *  @ingroup core
 */
template<typename T>
struct FACEKIT_EXPORTS Vector2 {

#pragma mark -
#pragma mark Initialization

  /**
   *  @name Vector2
   *  @fn Vector2(void)
   *  @brief  Constructor
   */
  Vector2(void) : x_(0), y_(0) {}

  /**
   *  @name Vector2
   *  @fn Vector2(const T x, const T y)
   *  @brief  Constructor
   *  @param[in]  x   X component
   *  @param[in]  y   Y component
   */
  Vector2(const T x, const T y) : x_(x), y_(y) {}

  /**
   *  @name Vector2
   *  @fn Vector2(const Vector2& other)
   *  @brief  Copy constructor
   */
  Vector2(const Vector2& other) {
    x_ = other.x_;
    y_ = other.y_;
  }

  /**
   *  @name operator=
   *  @fn Vector2& operator=(const Vector2& rhs)
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  Vector2& operator=(const Vector2& rhs) {
    if (this != &rhs) {
      x_ = rhs.x_;
      y_ = rhs.y_;
    }
    return *this;
  }

  /**
   *  @name Vector2
   *  @fn ~Vector2(void)
   *  @brief  Destructor
   */
  ~Vector2(void) {}

#pragma mark -
#pragma mark Usage

  /**
   *  @name Norm
   *  @fn T Norm(void) const
   *  @brief  Compute the norm
   *  @return Norm of the vector
   */
  T Norm(void) const {
    return std::sqrt(x_ * x_ + y_ * y_);
  }

  /**
   *  @name Normalize
   *  @fn void Normalize(void)
   *  @brief  Normalize to unit length
   */
  void Normalize(void) {
    const T length = this->Norm();
    if (length != T(0.0)) {
      x_ /= length;
      y_ /= length;
    } else {
      x_ = std::numeric_limits<T>::quiet_NaN();
      y_ = std::numeric_limits<T>::quiet_NaN();
    }
  }

#pragma mark -
#pragma mark Operator

  /**
   *  @name operator+=
   *  @fn Vector2& operator+=(const Vector2& rhs)
   *  @brief  Addition operator
   *  @param[in]  rhs Vector to add
   *  @return Updated vector
   */
  Vector2& operator+=(const Vector2& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    return *this;
  }

  /**
   *  @name operator+=
   *  @fn Vector2& operator+=(const T value)
   *  @brief  Addition operator
   *  @param[in]  value Value to add
   *  @return Updated vector
   */
  Vector2& operator+=(const T value) {
    x_ += value;
    y_ += value;
    return *this;
  }

  /**
   *  @name operator-=
   *  @fn Vector2& operator-=(const Vector2& rhs)
   *  @brief  Substraction operator
   *  @param[in]  rhs Vector substract
   *  @return Updated vector
   */
  Vector2& operator-=(const Vector2& rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    return *this;
  }

  /**
   *  @name operator-=
   *  @fn Vector2& operator-=(const T value)
   *  @brief  Substraction operator
   *  @param[in]  value Value to substract
   *  @return Updated vector
   */
  Vector2& operator-=(const T value) {
    x_ -= value;
    y_ -= value;
    return *this;
  }

  /**
   *  @name operator*=
   *  @fn Vector2& operator*=(const T value)
   *  @brief  Multiplaction operator
   *  @param[in]  value Value to multiply by
   *  @return Updated vector
   */
  Vector2& operator*=(const T value) {
    x_ *= value;
    y_ *= value;
    return *this;
  }

  /**
   *  @name operator/=
   *  @fn Vector2& operator/=(const T value)
   *  @brief  Division operator
   *  @param[in]  value Value to divide by
   *  @return Updated vector
   */
  Vector2& operator/=(const T value) {
    if (value != T(0)) {
      x_ /= value;
      y_ /= value;
    } else {
      x_ = std::numeric_limits<T>::quiet_NaN();
      y_ = std::numeric_limits<T>::quiet_NaN();
    }
    return *this;
  }

  /**
   *  @name operator==
   *  @fn bool operator==(const Vector2& rhs)
   *  @brief  Equality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if equal, false otherwise
   */
  bool operator==(const Vector2& rhs) {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 2; ++i) {
      if (TComparator<T>(ptr[i]) != TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator==
   *  @fn bool operator==(const Vector2& rhs) const
   *  @brief  Equality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if equal, false otherwise
   */
  bool operator==(const Vector2& rhs) const {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 2; ++i) {
      if (TComparator<T>(ptr[i]) != TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator!=
   *  @fn bool operator!=(const Vector2& rhs)
   *  @brief  Inequality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if not equal, false otherwise
   */
  bool operator!=(const Vector2& rhs) {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 2; ++i) {
      if (TComparator<T>(ptr[i]) == TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator!=
   *  @fn bool operator!=(const Vector2& rhs) const
   *  @brief  Inequality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if not equal, false otherwise
   */
  bool operator!=(const Vector2& rhs) const {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 2; ++i) {
      if (TComparator<T>(ptr[i]) == TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator>>
   *  @fn friend std::istream& operator>>(std::istream& in, Vector2<T>& v)
   *  @param[in]  in Input stream
   *  @param[in]  v   Vector to fill
   *  @return input stream
   */
  friend std::istream& operator>>(std::istream& in, Vector2& v) {
    in >> v.x_ >> v.y_;
    return in;
  }

  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out, const Vector2<T>& v)
   *  @param[in]  out Output strream
   *  @param[in]  v   Vector to write
   *  @return output stream
   */
  friend std::ostream& operator<<(std::ostream& out, const Vector2<T>& v) {
    return out << v.x_ << " " << v.y_;
  }


#pragma mark -
#pragma mark Members

  /** X component */
  T x_;
  /** Y component */
  T y_;
};

#pragma mark -
#pragma mark Vector3

/**
 *  @struct Vector3
 *  @brief  Vector of dimension 3
 *  @author Chrisotphe Ecabert
 *  @date   30/07/16
 *  @ingroup core
 */
template<typename T>
struct FACEKIT_EXPORTS Vector3 {

#pragma mark -
#pragma mark Initialization

  /**
   *  @name Vector3
   *  @fn Vector3(void)
   *  @brief  Constructor
   */
  Vector3(void) : x_(0), y_(0), z_(0) {}

  /**
   *  @name Vector3
   *  @fn Vector3(const T x, const T y, const T z)
   *  @brief  Constructor
   *  @param[in]  x   X component
   *  @param[in]  y   Y component
   *  @param[in]  z   Z component
   */
  Vector3(const T x, const T y, const T z) : x_(x), y_(y), z_(z) {}

  /**
   *  @name Vector3
   *  @fn Vector3(const Vector3& other)
   *  @brief  Copy constructor
   */
  Vector3(const Vector3& other) {
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
  }

  /**
   *  @name operator=
   *  @fn Vector3& operator=(const Vector3& rhs)
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  Vector3& operator=(const Vector3& rhs) {
    if (this != &rhs) {
      x_ = rhs.x_;
      y_ = rhs.y_;
      z_ = rhs.z_;
    }
    return *this;
  }

  /**
   *  @name ~Vector3
   *  @fn ~Vector3(void)
   *  @brief  Destructor
   */
  ~Vector3(void) {}

#pragma mark -
#pragma mark Usage

  /**
   *  @name Norm
   *  @fn T Norm(void) const
   *  @brief  Compute the norm
   *  @return Norm of the vector
   */
  T Norm(void) const {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
  }

  /**
   *  @name Normalize
   *  @fn void Normalize(void)
   *  @brief  Normalize to unit length
   */
  void Normalize(void) {
    const T length = this->Norm();
    if (length != T(0.0)) {
      x_ /= length;
      y_ /= length;
      z_ /= length;
    } else {
      x_ = std::numeric_limits<T>::quiet_NaN();
      y_ = std::numeric_limits<T>::quiet_NaN();
      z_ = std::numeric_limits<T>::quiet_NaN();
    }
  }

#pragma mark -
#pragma mark Operator

  /**
   *  @name operator+=
   *  @fn Vector3& operator+=(const Vector3& rhs)
   *  @brief  Addition operator
   *  @param[in]  rhs Vector to add
   *  @return Updated vector
   */
  Vector3& operator+=(const Vector3& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
  }

  /**
   *  @name operator+=
   *  @fn Vector3& operator+=(const T value)
   *  @brief  Addition operator
   *  @param[in]  value Value to add
   *  @return Updated vector
   */
  Vector3& operator+=(const T value) {
    x_ += value;
    y_ += value;
    z_ += value;
    return *this;
  }

  /**
   *  @name operator-=
   *  @fn Vector3& operator-=(const Vector3& rhs)
   *  @brief  Substraction operator
   *  @param[in]  rhs Vector substract
   *  @return Updated vector
   */
  Vector3& operator-=(const Vector3& rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
  }

  /**
   *  @name operator-=
   *  @fn Vector3& operator-=(const T value)
   *  @brief  Substraction operator
   *  @param[in]  value Value to substract
   *  @return Updated vector
   */
  Vector3& operator-=(const T value) {
    x_ -= value;
    y_ -= value;
    z_ -= value;
    return *this;
  }

  /**
   *  @name operator*=
   *  @fn Vector3& operator*=(const T value)
   *  @brief  Multiplaction operator
   *  @param[in]  value Value to multiply by
   *  @return Updated vector
   */
  Vector3& operator*=(const T value) {
    x_ *= value;
    y_ *= value;
    z_ *= value;
    return *this;
  }

  /**
   *  @name operator/=
   *  @fn Vector3& operator/=(const T value)
   *  @brief  Division operator
   *  @param[in]  value Value to divide by
   *  @return Updated vector
   */
  Vector3& operator/=(const T value) {
    if (value != T(0)) {
      x_ /= value;
      y_ /= value;
      z_ /= value;
    } else {
      x_ = std::numeric_limits<T>::quiet_NaN();
      y_ = std::numeric_limits<T>::quiet_NaN();
      z_ = std::numeric_limits<T>::quiet_NaN();
    }
    return *this;
  }

  /**
   *  @name operator==
   *  @fn bool operator==(const Vector3& rhs)
   *  @brief  Equality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if equal, false otherwise
   */
  bool operator==(const Vector3& rhs) {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 3; ++i) {
      if (TComparator<T>(ptr[i]) != TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator==
   *  @fn bool operator==(const Vector3& rhs) const
   *  @brief  Equality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if equal, false otherwise
   */
  bool operator==(const Vector3& rhs) const {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 3; ++i) {
      if (TComparator<T>(ptr[i]) != TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator!=
   *  @fn bool operator!=(const Vector3& rhs)
   *  @brief  Inequality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if not equal, false otherwise
   */
  bool operator!=(const Vector3& rhs) {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 3; ++i) {
      if (TComparator<T>(ptr[i]) == TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator!=
   *  @fn bool operator!=(const Vector3& rhs) const
   *  @brief  Inequality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if not equal, false otherwise
   */
  bool operator!=(const Vector3& rhs) const {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 3; ++i) {
      if (TComparator<T>(ptr[i]) == TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator>>
   *  @fn friend std::istream& operator>>(std::istream& in, Vector3<T>& v)
   *  @param[in]  in Input stream
   *  @param[in]  v   Vector to fill
   *  @return input stream
   */
  friend std::istream& operator>>(std::istream& in, Vector3& v) {
    in >> v.x_ >> v.y_ >> v.z_;
    return in;
  }
  
  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out, const Vector3<T>& v)
   *  @param[in]  out Output strream
   *  @param[in]  v   Vector to write
   *  @return output stream
   */
  friend std::ostream& operator<<(std::ostream& out, const Vector3<T>& v) {
    return out << v.x_ << " " << v.y_ << " " << v.z_;
  }
  
  
#pragma mark -
#pragma mark Members
  
  /** X component */
  T x_;
  /** Y component */
  T y_;
  /** Z component */
  T z_;
};

#pragma mark -
#pragma mark Vector4

/**
 *  @struct Vector4
 *  @brief  Vector of dimension 4
 *  @author Chrisotphe Ecabert
 *  @date   30/07/16
 *  @ingroup core
 */
template<typename T>
struct FACEKIT_EXPORTS Vector4 {

#pragma mark -
#pragma mark Initialization

  /**
   *  @name Vector4
   *  @fn Vector4(void)
   *  @brief  Constructor
   */
  Vector4(void) : x_(0), y_(0), z_(0), w_(0) {}

  /**
   *  @name Vector4
   *  @fn Vector4(const T x, const T y, const T z, const T w)
   *  @brief  Constructor
   *  @param[in]  x   X component
   *  @param[in]  y   Y component
   *  @param[in]  z   Z component
   *  @param[in]  w   W component
   */
  Vector4(const T x, const T y, const T z, const T w) : x_(x),
  y_(y),
  z_(z),
  w_(w) {}

  /**
   *  @name Vector4
   *  @fn Vector4(const Vector4& other)
   *  @brief  Copy constructor
   */
  Vector4(const Vector4& other) {
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
    w_ = other.w_;
  }

  /**
   *  @name operator=
   *  @fn Vector4& operator=(const Vector4& rhs)
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  Vector4& operator=(const Vector4& rhs) {
    if (this != &rhs) {
      x_ = rhs.x_;
      y_ = rhs.y_;
      z_ = rhs.z_;
      w_ = rhs.w_;
    }
    return *this;
  }

  /**
   *  @name ~Vector4
   *  @fn ~Vector4(void)
   *  @brief  Destructor
   */
  ~Vector4(void) {}

#pragma mark -
#pragma mark Usage

  /**
   *  @name Norm
   *  @fn T Norm(void) const
   *  @brief  Compute the norm
   *  @return Norm of the vector
   */
  T Norm(void) const {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
  }

  /**
   *  @name Normalize
   *  @fn void Normalize(void)
   *  @brief  Normalize to unit length
   */
  void Normalize(void) {
    const T length = this->Norm();
    if (length != T(0.0)) {
      x_ /= length;
      y_ /= length;
      z_ /= length;
      w_ /= length;
    } else {
      x_ = std::numeric_limits<T>::quiet_NaN();
      y_ = std::numeric_limits<T>::quiet_NaN();
      z_ = std::numeric_limits<T>::quiet_NaN();
      w_ = std::numeric_limits<T>::quiet_NaN();
    }
  }

#pragma mark -
#pragma mark Operator

  /**
   *  @name operator+=
   *  @fn Vector4& operator+=(const Vector4& rhs)
   *  @brief  Addition operator
   *  @param[in]  rhs Vector to add
   *  @return Updated vector
   */
  Vector4& operator+=(const Vector4& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    w_ += rhs.w_;
    return *this;
  }

  /**
   *  @name operator+=
   *  @fn Vector4& operator+=(const T value)
   *  @brief  Addition operator
   *  @param[in]  value Value to add
   *  @return Updated vector
   */
  Vector4& operator+=(const T value) {
    x_ += value;
    y_ += value;
    z_ += value;
    w_ += value;
    return *this;
  }

  /**
   *  @name operator-=
   *  @fn Vector4& operator-=(const Vector4& rhs)
   *  @brief  Substraction operator
   *  @param[in]  rhs Vector substract
   *  @return Updated vector
   */
  Vector4& operator-=(const Vector4& rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    w_ -= rhs.w_;
    return *this;
  }

  /**
   *  @name operator-=
   *  @fn Vector4& operator-=(const T value)
   *  @brief  Substraction operator
   *  @param[in]  value Value to substract
   *  @return Updated vector
   */
  Vector4& operator-=(const T value) {
    x_ -= value;
    y_ -= value;
    z_ -= value;
    w_ -= value;
    return *this;
  }

  /**
   *  @name operator*=
   *  @fn Vector4& operator*=(const T value)
   *  @brief  Multiplaction operator
   *  @param[in]  value Value to multiply by
   *  @return Updated vector
   */
  Vector4& operator*=(const T value) {
    x_ *= value;
    y_ *= value;
    z_ *= value;
    w_ *= value;
    return *this;
  }

  /**
   *  @name operator/=
   *  @fn Vector4& operator/=(const T value)
   *  @brief  Division operator
   *  @param[in]  value Value to divide by
   *  @return Updated vector
   */
  Vector4& operator/=(const T value) {
    if (value != T(0)) {
      x_ /= value;
      y_ /= value;
      z_ /= value;
      w_ /= value;
    } else {
      x_ = std::numeric_limits<T>::quiet_NaN();
      y_ = std::numeric_limits<T>::quiet_NaN();
      z_ = std::numeric_limits<T>::quiet_NaN();
      w_ = std::numeric_limits<T>::quiet_NaN();
    }
    return *this;
  }

  /**
   *  @name operator==
   *  @fn bool operator==(const Vector4& rhs)
   *  @brief  Equality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if equal, false otherwise
   */
  bool operator==(const Vector4& rhs) {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 4; ++i) {
      if (TComparator<T>(ptr[i]) != TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator==
   *  @fn bool operator==(const Vector4& rhs) const
   *  @brief  Equality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if equal, false otherwise
   */
  bool operator==(const Vector4& rhs) const {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 4; ++i) {
      if (TComparator<T>(ptr[i]) != TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator!=
   *  @fn bool operator!=(const Vector4& rhs)
   *  @brief  Inequality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if not equal, false otherwise
   */
  bool operator!=(const Vector4& rhs) {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 4; ++i) {
      if (TComparator<T>(ptr[i]) == TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator!=
   *  @fn bool operator!=(const Vector4& rhs) const
   *  @brief  Inequality operator
   *  @param[in]  rhs Right hand sign
   *  @return True if not equal, false otherwise
   */
  bool operator!=(const Vector4& rhs) const {
    const T* ptr = &x_;
    const T* rhs_ptr = &rhs.x_;
    for(int i = 0; i < 4; ++i) {
      if (TComparator<T>(ptr[i]) == TComparator<T>(rhs_ptr[i])) {
        return false;
      }
    }
    return true;
  }

  /**
   *  @name operator>>
   *  @fn friend std::istream& operator>>(std::istream& in, Vector4<T>& v)
   *  @param[in]  in Input stream
   *  @param[in]  v   Vector to fill
   *  @return input stream
   */
  friend std::istream& operator>>(std::istream& in, Vector4& v) {
    in >> v.x_ >> v.y_ >> v.z_ >> v.w_;
    return in;
  }
  
  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out, const Vector4<T>& v)
   *  @param[in]  out Output strream
   *  @param[in]  v   Vector to write
   *  @return output stream
   */
  friend std::ostream& operator<<(std::ostream& out, const Vector4<T>& v) {
    return out << v.x_ << " " << v.y_ << " " << v.z_ << " " << v.w_;
  }
  
  
#pragma mark -
#pragma mark Members
  
  /** X component */
  T x_;
  /** Y component */
  T y_;
  /** Z component */
  T z_;
  /** W component */
  T w_;
};

#pragma mark -
#pragma mark Vector2 operator


/** Addition */
template<typename T>
FACEKIT_EXPORTS Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  return Vector2<T>(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_);
}
template<typename T>
FACEKIT_EXPORTS Vector2<T> operator+(const Vector2<T>& lhs, const T v) {
  return Vector2<T>(lhs.x_ + v, lhs.y_ + v);
}

/** Substraction */
template<typename T>
FACEKIT_EXPORTS Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  return Vector2<T>(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_);
}
template<typename T>
FACEKIT_EXPORTS Vector2<T> operator-(const Vector2<T>& lhs, const T v) {
  return Vector2<T>(lhs.x_ - v, lhs.y_ - v);
}

/** Scalar product */
template<typename T>
FACEKIT_EXPORTS Vector2<T> operator*(const Vector2<T>& lhs, const T scalar) {
  return Vector2<T>(lhs.x_ * scalar, lhs.y_ * scalar);
}

/** Division product */
template<typename T>
FACEKIT_EXPORTS Vector2<T> operator/(const Vector2<T>& lhs, const T scalar) {
  return Vector2<T>(lhs.x_ / scalar, lhs.y_ / scalar);
}

/** Dot product */
template<typename T>
FACEKIT_EXPORTS T operator*(const Vector2<T>& lhs, const Vector2<T>& rhs) {
  return (lhs.x_ * rhs.x_) + (lhs.y_ * rhs.y_);
}

#pragma mark -
#pragma mark Vector3 operator

/** Addition */
template<typename T>
FACEKIT_EXPORTS Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs) {
  return Vector3<T>(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_);
}
template<typename T>
FACEKIT_EXPORTS Vector3<T> operator+(const Vector3<T>& lhs, const T v) {
  return Vector3<T>(lhs.x_ + v, lhs.y_ + v, lhs.z_ + v);
}

/** Substraction */
template<typename T>
FACEKIT_EXPORTS Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs) {
  return Vector3<T>(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_);
}
template<typename T>
FACEKIT_EXPORTS Vector3<T> operator-(const Vector3<T>& lhs, const T v) {
  return Vector3<T>(lhs.x_ - v, lhs.y_ - v, lhs.z_ - v);
}

/** Scalar product */
template<typename T>
FACEKIT_EXPORTS Vector3<T> operator*(const Vector3<T>& lhs, const T scalar) {
  return Vector3<T>(lhs.x_ * scalar, lhs.y_ * scalar, lhs.z_ * scalar);
}

/** Division product */
template<typename T>
FACEKIT_EXPORTS Vector3<T> operator/(const Vector3<T>& lhs, const T scalar) {
  return Vector3<T>(lhs.x_ / scalar, lhs.y_ / scalar, lhs.z_ / scalar);
}

/** Dot product */
template<typename T>
FACEKIT_EXPORTS T operator*(const Vector3<T>& lhs, const Vector3<T>& rhs) {
  return (lhs.x_ * rhs.x_) + (lhs.y_ * rhs.y_) + (lhs.z_ * rhs.z_);
}

/** Cross product */
template<typename T>
FACEKIT_EXPORTS Vector3<T> operator^(const Vector3<T>& lhs, const Vector3<T>& rhs) {
  return Vector3<T>(lhs.y_ * rhs.z_ - rhs.y_ * lhs.z_,
                    lhs.z_ * rhs.x_ - rhs.z_ * lhs.x_,
                    lhs.x_ * rhs.y_ - rhs.x_ * lhs.y_);
}

#pragma mark -
#pragma mark Vector3 operator

/** Addition */
template<typename T>
FACEKIT_EXPORTS Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs) {
  return Vector4<T>(lhs.x_ + rhs.x_,
                    lhs.y_ + rhs.y_,
                    lhs.z_ + rhs.z_,
                    lhs.w_ + rhs.w_);
}
template<typename T>
FACEKIT_EXPORTS Vector4<T> operator+(const Vector4<T>& lhs, const T v) {
  return Vector4<T>(lhs.x_ + v, lhs.y_ + v, lhs.z_ + v, lhs.w_ + v);
}

/** Substraction */
template<typename T>
FACEKIT_EXPORTS Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs) {
  return Vector4<T>(lhs.x_ - rhs.x_,
                    lhs.y_ - rhs.y_,
                    lhs.z_ - rhs.z_,
                    lhs.w_ - rhs.w_);
}
template<typename T>
FACEKIT_EXPORTS Vector4<T> operator-(const Vector4<T>& lhs, const T v) {
  return Vector4<T>(lhs.x_ - v, lhs.y_ - v, lhs.z_ - v, lhs.w_ - v);
}

/** Scalar product */
template<typename T>
FACEKIT_EXPORTS Vector4<T> operator*(const Vector4<T>& lhs, const T scalar) {
  return Vector4<T>(lhs.x_ * scalar,
                    lhs.y_ * scalar,
                    lhs.z_ * scalar,
                    lhs.w_ * scalar);
}

/** Division product */
template<typename T>
FACEKIT_EXPORTS Vector4<T> operator/(const Vector4<T>& lhs, const T scalar) {
  return Vector4<T>(lhs.x_ / scalar,
                    lhs.y_ / scalar,
                    lhs.z_ / scalar,
                    lhs.w_ / scalar);
}

/** Dot product */
template<typename T>
FACEKIT_EXPORTS T operator*(const Vector4<T>& lhs, const Vector4<T>& rhs) {
  return ((lhs.x_ * rhs.x_) +
          (lhs.y_ * rhs.y_) +
          (lhs.z_ * rhs.z_) +
          (lhs.w_ * rhs.w_));
}


}  // namespace FaceKit
#endif /* _FACEKIT_vector__ */
