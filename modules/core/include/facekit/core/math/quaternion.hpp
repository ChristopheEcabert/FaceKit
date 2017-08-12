/**
 *  @file   quaternion.hpp
 *  @brief  Quaternion abstraction
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   16.12.16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_quaternion__
#define __FACEKIT_quaternion__

#include "facekit/core/library_export.hpp"
#include "facekit/core/math/vector.hpp"
#include "facekit/core/math/matrix.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  Quaternion
 *  @brief  Quaternion abstraction
 *  @author Christophe Ecabert
 *  @date   16.12.16
 *  @ingroup core
 */
template <typename T>
class FACEKIT_EXPORTS Quaternion {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name Quaternion
   *  @fn Quaternion(void)
   *  @brief  Constructor
   */
  Quaternion(void) : q_(T(0.0)), v_{T(0.0)} {}
  
  /**
   *  @name Quaternion
   *  @fn Quaternion(const T q, const FaceKit::Vector3<T>& v)
   *  @brief  Constructor
   *  @param[in]  q   Real value to initialize with
   *  @param[in]  v   Imaginary value to initialize with
   */
  Quaternion(const T q, const FaceKit::Vector3<T>& v) : q_(q), v_(v) {
  }
  
  /**
   *  @name Quaternion
   *  @fn Quaternion(const Quaternion& other)
   *  @brief  Copy constructor
   *  @param[in]  other Object to copy from
   */
  Quaternion(const Quaternion& other) : q_(other.q_), v_(other.v_) {
  }
  
  /**
   *  @name operator=
   *  @fn Quaternion& operator=(const Quaternion& rhs)
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  Quaternion& operator=(const Quaternion& rhs) {
    if (this != &rhs) {
      q_ = rhs.q_;
      v_ = rhs.v_;
    }
    return *this;
  }
  
  /**
   *  @name Quaternion
   *  @fn Quaternion(const FaceKit::Vector3<T>& axis, const T angle)
   *  @brief  Constructor
   *  @param[in]  axis   Rotation axis
   *  @param[in]  angle  Amount of rotation around a given \p axis.
   */
  Quaternion(const FaceKit::Vector3<T>& axis, const T angle) {
    // Define real part
    const T h_angle = angle / T(2.0);
    q_ = std::cos(h_angle);
    // Define imaginary
    const T axis_n = axis.Norm();
    const T s = std::sin(h_angle) / axis_n;
    v_ = axis * s;
  }
  
  /**
   *  @name ~Quaternion
   *  @fn ~Quaternion(void)
   *  @brief  Destructor
   */
  ~Quaternion(void) {}
  
#pragma mark -
#pragma mark Operator
  
  /**
   *  @name operator+
   *  @fn Quaternion& operator+(const Quaternion& rhs)
   *  @brief  Addition operator
   *  @param[in]  rhs Quaternion to add
   *  @return Addition result
   */
  Quaternion& operator+(const Quaternion& rhs) {
    return Quaternion(q_ + rhs.q_, v_ + rhs.v_);
  }
  
  /**
   *  @name operator-
   *  @fn Quaternion& operator-(const Quaternion& rhs)
   *  @brief  Addition operator
   *  @param[in]  rhs Quaternion to add
   *  @return Addition result
   */
  Quaternion& operator-(const Quaternion& rhs) {
    return Quaternion(q_ - rhs.q_, v_ - rhs.v_);
  }
  
  /**
   *  @name operator*
   *  @fn Quaternion& operator*(const Quaternion& rhs)
   *  @brief  Multiplication operator
   *  @param[in]  rhs Quaternion to multiply
   *  @return Multiply result
   */
  Quaternion& operator*(const Quaternion& rhs) {
    return Quaternion(q_ * rhs.q_ + v_ * rhs.v_,
                      v_ ^ rhs.v_ + q_ * rhs.v_ + rhs.q_ * v_);
  }
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name InPlaceConjugate
   *  @fn void InPlaceConjugate(void)
   *  @brief  In place conjugate (i.e. q.v = -q.v)
   */
  void InPlaceConjugate(void) {
    v_ = -v_;
  }
  
  /**
   *  @name Conjugate
   *  @fn void Conjugate(void)
   *  @brief  Conjugate (i.e. q.v = -q.v)
   *  @return Return conjugate quaternion
   */
  Quaternion& Conjugate(void) {
    return Quaternion(q_, -v_);
  }
  
  /**
   *  @name SquaredNorm
   *  @fn T SquaredNorm(void) const
   *  @brief  Compute squared norm of the quaternion
   *  @return Squared norm
   */
  T SquaredNorm(void) const {
    return (q_ * q_) + (v_.x_ * v_.x_) + (v_.y_ * v_.y_) + (v_.z_ * v_.z_);
  }
  
  /**
   *  @name Norm
   *  @fn T Norm(void) const
   *  @brief  Compute norm of the quaternion
   *  @return Norm
   */
  T Norm(void) const {
    return std::sqrt((q_ * q_) +
                     (v_.x_ * v_.x_) +
                     (v_.y_ * v_.y_) +
                     (v_.z_ * v_.z_));
  }
  
  /**
   *  @name Normalize
   *  @fn void Normalize(void)
   *  @brief  Normalize quaternion
   */
  void Normalize(void) {
    const T qn = this->SquaredNorm();
    if (qn != T(1.0)) {
      const T s = T(1.0) / std::sqrt(qn);
      q_ *= s;
      v_ *= s;
    }
  }
  
  /**
   *  @name ToRotationMatrix
   *  @fn void ToRotationMatrix(Matrix3<T>* m)
   *  @brief  Transform quaternion to rotation matrix
   *  @param[out] m Rotation matrix
   */
  void ToRotationMatrix(Matrix3<T>* m) {
    const T qq0 = q_ * q_;
    const T qq1 = v_.x_ * v_.x_;
    const T qq2 = v_.y_ * v_.y_;
    const T qq3 = v_.z_ * v_.z_;
    Matrix3<T>& mm = *m;
    mm[0] = qq0 + qq1 - qq2 - qq3;
    mm[1] = T(2.0) * (v_.x_ * v_.y_ + q_ * v_.z_);
    mm[2] = T(2.0) * (v_.x_ * v_.z_ - q_ * v_.y_);
    mm[3] = T(2.0) * (v_.x_ * v_.y_ - q_ * v_.z_);
    mm[4] = qq0 - qq1 + qq2 - qq3;
    mm[5] = T(2.0) * (v_.y_ * v_.z_ + q_ * v_.x_);
    mm[6] = T(2.0) * (v_.x_ * v_.z_ + q_ * v_.y_);
    mm[7] = T(2.0) * (v_.y_ * v_.z_ - q_ * v_.x_);
    mm[8] = qq0 - qq1 - qq2 + qq3;
  }
  
  /**
   *  @name ToRotationMatrix
   *  @fn void ToRotationMatrix(Matrix4<T>* m)
   *  @brief  Transform quaternion to rotation matrix
   *  @param[out] m Rotation matrix
   */
  void ToRotationMatrix(Matrix4<T>* m) {
    const T qq0 = q_ * q_;
    const T qq1 = v_.x_ * v_.x_;
    const T qq2 = v_.y_ * v_.y_;
    const T qq3 = v_.z_ * v_.z_;
    Matrix4<T>& mm = *m;
    mm[0] = qq0 + qq1 - qq2 - qq3;
    mm[1] = T(2.0) * (v_.x_ * v_.y_ + q_ * v_.z_);
    mm[2] = T(2.0) * (v_.x_ * v_.z_ - q_ * v_.y_);
    mm[3] = T(0.0);
    mm[4] = T(2.0) * (v_.x_ * v_.y_ - q_ * v_.z_);
    mm[5] = qq0 - qq1 + qq2 - qq3;
    mm[6] = T(2.0) * (v_.y_ * v_.z_ + q_ * v_.x_);
    mm[7] = T(0.0);
    mm[8] = T(2.0) * (v_.x_ * v_.z_ + q_ * v_.y_);
    mm[9] = T(2.0) * (v_.y_ * v_.z_ - q_ * v_.x_);
    mm[10] = qq0 - qq1 - qq2 + qq3;
    mm[11] = T(0.0);
    mm[12] = T(0.0);
    mm[13] = T(0.0);
    mm[14] = T(0.0);
    mm[15] = T(1.0);
  }
  
#pragma mark -
#pragma mark Private
  
 private:
  
  /** Real part */
  T q_;
  /** Imaginary part */
  FaceKit::Vector3<T> v_;
};
  
}  // namespace FaceKit
#endif /* _FACEKIT_quaternion__ */
