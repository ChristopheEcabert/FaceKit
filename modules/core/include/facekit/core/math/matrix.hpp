/**
 *  @file   matrix.hpp
 *  @brief  Matrix class 
 *          Colmun-major data layout
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   31/07/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_MATRIX__
#define __FACEKIT_MATRIX__

#include <cstring>

#include "facekit/core/library_export.hpp"
#include "facekit/core/math/vector.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/**
 *  @class  Matrix3
 *  @brief  3x3 matrix class
 *  @author Christophe Ecabert
 *  @date   31/07/16
 *  @ingroup core
 */
template<typename T>
class FACEKIT_EXPORTS Matrix3 {

 public:

#pragma mark -
#pragma mark Initialization

  /** 
   *  @name Matrix3
   *  @fn Matrix3(void)
   *  @brief  Construcotr
   */
  Matrix3(void) : m_{T(0)} {
    m_[0] = T(1);
    m_[4] = T(1);
    m_[8] = T(1);
  }

  /**
   *  @name Matrix3
   *  @fn Matrix3(const T* data)
   *  @brief  Construcotr
   *  @param[in]  data  Value to use to fill Matrix
   */
  Matrix3(const T* data) {
    memcpy(reinterpret_cast<void*>(&m_[0]),
           reinterpret_cast<const void*>(data),
           9 * sizeof(T));
  }

  /**
   *  @name Matrix3
   *  @fn Matrix3(const Matrix3& other)
   *  @brief  Copy Construcotr
   *  @param[in]  other  Object to copy from
   */
  Matrix3(const Matrix3& other) {
    memcpy(reinterpret_cast<void*>(&m_[0]),
           reinterpret_cast<const void*>(&other.m_[0]),
           9 * sizeof(T));
  }

  /**
   *  @name operator=
   *  @fn Matrix3& operator=(const Matrix3& rhs)
   *  @brief  Assignment operator
   *  @param[in]  rhs  Object to assign from
   *  @return Newly assigned object
   */
  Matrix3& operator=(const Matrix3& rhs) {
    if ( this != &rhs) {
      memcpy(reinterpret_cast<void*>(&m_[0]),
             reinterpret_cast<const void*>(&rhs.m_[0]),
             9 * sizeof(T));
    }
    return *this;
  }

  /**
   *  @name ~Matrix3
   *  @fn ~Matrix3(void)
   *  @brief  Destrucotr
   */
  ~Matrix3(void) {}

#pragma mark -
#pragma mark Usage

  /**
   *  @name Determinent
   *  @fn T Determinent(void) const
   *  brief Compute matrix determinent
   *  @return Matrix determinent
   */
  T Determinent(void) const {
    T det = m_[0] * (m_[4] * m_[8] - m_[5] * m_[7]);
    det -=  m_[3] * (m_[1] * m_[8] - m_[2] * m_[7]);
    det += m_[6] * (m_[1] * m_[5] - m_[2] * m_[4]);
    return det;
  }

  /**
   *  @name Trace
   *  @fn T Trace(void) const
   *  brief Compute matrix trace
   *  @return Matrix trace
   */
  T Trace(void) const {
    return m_[0] + m_[4] + m_[8];
  }

  /**
   *  @name Transpose
   *  @fn Matrix3& Transpose(void) const
   *  @brief  Compute the matrix transpose
   *  @return Transposed matrix
   */
  Matrix3 Transpose(void) const {
    Matrix3 M;
    M[0] = m_[0];
    M[1] = m_[3];
    M[2] = m_[6];
    M[3] = m_[1];
    M[4] = m_[4];
    M[5] = m_[7];
    M[6] = m_[2];
    M[7] = m_[5];
    M[8] = m_[8];
    return M;
  }

  /**
   *  @name Inverse
   *  @fn Matrix3 Inverse(void) const
   *  @brief  Compute the inverse of this matrix
   *  @return Inverse matrix
   */
  Matrix3 Inverse(void) const {
    Matrix3 iM;
    const T det = this->Determinent();
    if (det != T(0)) {
      const T idet = T(1) / det;
      iM[0] = (m_[4]*m_[8] - m_[5]*m_[7]) * idet;
      iM[1] = (m_[7]*m_[2] - m_[8]*m_[1]) * idet;
      iM[2] = (m_[1]*m_[5] - m_[2]*m_[4]) * idet;
      iM[3] = (m_[6]*m_[5] - m_[8]*m_[3]) * idet;
      iM[4] = (m_[0]*m_[8] - m_[2]*m_[6]) * idet;
      iM[5] = (m_[3]*m_[2] - m_[5]*m_[0]) * idet;
      iM[6] = (m_[3]*m_[7] - m_[4]*m_[6]) * idet;
      iM[7] = (m_[6]*m_[1] - m_[7]*m_[0]) * idet;
      iM[8] = (m_[0]*m_[4] - m_[1]*m_[3]) * idet;
    }
    return iM;
  }


#pragma mark -
#pragma mark Accessors

  /**
   *  @name data
   *  @fn T* data(void)
   *  @brief  Provide reference to data
   *  @return Pointer to data array
   */
  T* data(void) {
    return &m_[0];
  }

  /**
   *  @name data
   *  @fn const T* data(void) const
   *  @brief  Provide constant reference to data
   *  @return Constant pointer to data array
   */
  const T* data(void) const {
    return &m_[0];
  }

#pragma mark -
#pragma mark Operator

  /**
   *  @name operator()(
   *  @fn T& operator()(const int row, const int col)
   *  @brief  Row/Col matrix accessor
   *  @param[in]  row Index of the row
   *  @param[in]  col Index of the column
   *  @return Reference to the selected location
   */
  T& operator()(const int row, const int col) {
    return m_[col * 3 + row];
  }

  /**
   *  @name operator()(
   *  @fn const T& operator()(const int row, const int col) const
   *  @brief  Row/Col matrix accessor
   *  @param[in]  row Index of the row
   *  @param[in]  col Index of the column
   *  @return Reference to the selected location
   */
  const T& operator()(const int row, const int col) const {
    return m_[col * 3 + row];
  }

  /**
   *  @name operator*
   *  @fn Matrix3 operator*(const Matrix3& rhs) const
   *  @brief  Matrix multiplication
   *  @param[in]  rhs Right hand sign matrix
   *  @return Matrix product
   */
  Matrix3 operator*(const Matrix3& rhs) const {
    Matrix3 M;
    M[0] = m_[0] * rhs[0] + m_[3] * rhs[1] + m_[6] * rhs[2];
    M[1] = m_[1] * rhs[0] + m_[4] * rhs[1] + m_[7] * rhs[2];
    M[2] = m_[2] * rhs[0] + m_[5] * rhs[1] + m_[8] * rhs[2];
    M[3] = m_[0] * rhs[3] + m_[3] * rhs[4] + m_[6] * rhs[5];
    M[4] = m_[1] * rhs[3] + m_[4] * rhs[4] + m_[7] * rhs[5];
    M[5] = m_[2] * rhs[3] + m_[5] * rhs[4] + m_[8] * rhs[5];
    M[6] = m_[0] * rhs[6] + m_[3] * rhs[7] + m_[6] * rhs[8];
    M[7] = m_[1] * rhs[6] + m_[4] * rhs[7] + m_[7] * rhs[8];
    M[8] = m_[2] * rhs[6] + m_[5] * rhs[7] + m_[8] * rhs[8];
    return M;
  }

  /**
   *  @name operator*
   *  @fn Vector3<T> operator*(const Vector3<T>& rhs) const
   *  @brief  Multiply by a Vector3
   *  @param[in]  rhs Right hand sign vector
   *  @return Matrix-Vector product
   */
  Vector3<T> operator*(const Vector3<T>& rhs) const {
    Vector3<T> v;
    v.x_ = m_[0] * rhs.x_ + m_[3] * rhs.y_ + m_[6] * rhs.z_;
    v.y_ = m_[1] * rhs.x_ + m_[4] * rhs.y_ + m_[7] * rhs.z_;
    v.z_ = m_[2] * rhs.x_ + m_[5] * rhs.y_ + m_[8] * rhs.z_;
    return v;
  }

  /**
   *  @name operator*
   *  @fn Matrix3 operator*(const T s) const
   *  @brief  Multiply by a scalar
   *  @param[in]  s Scalar to multiply by
   *  @return Scaled Matrix
   */
  Matrix3<T> operator*(const T s) const {
    Matrix3 M;
    #pragma unroll
    for (int i = 0; i < 9; ++i) {
      M[i] = m_[i] * s;
    }
    return M;
  }

  /**
   *  @name operator+
   *  @fn Matrix3 operator+(const Matrix3& rhs) const
   *  @brief  Add another Matrix3
   *  @param[in]  rhs Right hand sign matrix
   *  @return Matrix addition
   */
  Matrix3 operator+(const Matrix3& rhs) const {
    Matrix3 M;
    #pragma unroll
    for (int i = 0; i < 9; ++i) {
      M[i] = m_[i] + rhs[i];
    }
    return M;
  }

  /**
   *  @name operator-
   *  @fn Matrix3 operator-(const Matrix3& rhs) const
   *  @brief  Subtract another Matrix3
   *  @param[in]  rhs Right hand sign matrix
   *  @return Matrix subtraction
   */
  Matrix3 operator-(const Matrix3& rhs) const {
    Matrix3 M;
    #pragma unroll
    for (int i = 0; i < 9; ++i) {
      M[i] = m_[i] - rhs[i];
    }
    return M;
  }

  /**
   *  @name operator=
   *  @fn Matrix3& operator=(const T s)
   *  @brief  Assignment operator
   *  @param[in]  s Scalar to assign to this matrix
   *  @return
   */
  Matrix3& operator=(const T s) {
    #pragma unroll
    for (int i = 0 ; i < 9 ; ++i) {
      m_[i] = s;
    }
    return *this;
  }

  /**
   *  @name operator[]
   *  @fn T& operator[](const int idx)
   *  @brief  Operator to linearly access element stored in column major flavor
   *  @param[in]  idx Index to reach
   *  @return Reference to the selected element
   */
  T& operator[](const int idx) {
    return m_[idx];
  }

  /**
   *  @name operator[]
   *  @fn const T& operator[](const int idx) const
   *  @brief  Operator to linearly access element stored in row major flavor
   *  @param[in]  idx Index to reach
   *  @return Reference to the selected element
   */
  const T& operator[](const int idx) const {
    return m_[idx];
  }


  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out, const Matrix3& m)
   *  @brief  Print matrix into a given steram
   *  @param[in]  out   Output stream
   *  @param[in]  m     Matrix to print
   *  @return Output stream
   */
  friend std::ostream& operator<<(std::ostream& out, const Matrix3& m) {
    out << m[0] << " " << m[3] << " " << m[6] << std::endl;
    out << m[1] << " " << m[4] << " " << m[7] << std::endl;
    out << m[2] << " " << m[5] << " " << m[8] << std::endl;
    return out;
  }


#pragma mark -
#pragma mark Private

 private:
  /** Data array - ColumnMajor layout */
  T m_[9];
};

/**
 *  @class  Matrix4
 *  @brief  4x4 matrix class
 *  @author Christophe Ecabert
 *  @date   31/07/16
 *  @ingroup core
 */
template<typename T>
class FACEKIT_EXPORTS Matrix4 {

public:

#pragma mark -
#pragma mark Initialization

  /**
   *  @name Matrix4
   *  @fn Matrix4(void)
   *  @brief  Construcotr
   */
  Matrix4(void) : m_{T(0)} {
    m_[0] = T(1);
    m_[5] = T(1);
    m_[10] = T(1);
    m_[15] = T(1);
  }

  /**
   *  @name Matrix4
   *  @fn Matrix4(const T* data)
   *  @brief  Construcotr
   *  @param[in]  data  Value to use to fill Matrix
   */
  Matrix4(const T* data) {
    memcpy(reinterpret_cast<void*>(&m_[0]),
           reinterpret_cast<const void*>(data),
           16 * sizeof(T));
  }

  /**
   *  @name Matrix4
   *  @fn Matrix4(const Matrix4& other)
   *  @brief  Copy Construcotr
   *  @param[in]  other  Object to copy from
   */
  Matrix4(const Matrix4& other) {
    memcpy(reinterpret_cast<void*>(&m_[0]),
           reinterpret_cast<const void*>(&other.m_[0]),
           16 * sizeof(T));
  }

  /**
   *  @name operator=
   *  @fn Matrix4& operator=(const Matrix4& rhs)
   *  @brief  Assignment operator
   *  @param[in]  rhs  Object to assign from
   *  @return Newly assigned object
   */
  Matrix4& operator=(const Matrix4& rhs) {
    if ( this != &rhs) {
      memcpy(reinterpret_cast<void*>(&m_[0]),
             reinterpret_cast<const void*>(&rhs.m_[0]),
             16 * sizeof(T));
    }
    return *this;
  }

  /**
   *  @name ~Matrix4
   *  @fn ~Matrix4(void)
   *  @brief  Destrucotr
   */
  ~Matrix4(void) {}

#pragma mark -
#pragma mark Usage

  /**
   *  @name Determinent
   *  @fn T Determinent(void) const
   *  brief Compute matrix determinent
   *  @return Matrix determinent
   */
  T Determinent(void) const {
    T det = ((m_[0]*m_[5]*m_[10]*m_[15]) +
             (m_[0]*m_[9]*m_[14]*m_[7]) +
             (m_[0]*m_[13]*m_[6]*m_[11]) +
             (m_[4]*m_[1]*m_[14]*m_[11]) +
             (m_[4]*m_[9]*m_[2]*m_[15]) +
             (m_[4]*m_[13]*m_[10]*m_[3]) +
             (m_[8]*m_[1]*m_[6]*m_[15]) +
             (m_[8]*m_[5]*m_[14]*m_[3]) +
             (m_[8]*m_[13]*m_[2]*m_[7]) +
             (m_[12]*m_[1]*m_[10]*m_[7]) +
             (m_[12]*m_[5]*m_[2]*m_[11]) +
             (m_[12]*m_[9]*m_[6]*m_[3]) -
             (m_[0]*m_[5]*m_[14]*m_[11]) -
             (m_[0]*m_[9]*m_[6]*m_[15]) -
             (m_[0]*m_[13]*m_[10]*m_[7]) -
             (m_[4]*m_[1]*m_[10]*m_[15]) -
             (m_[4]*m_[9]*m_[14]*m_[3]) -
             (m_[4]*m_[13]*m_[2]*m_[11]) -
             (m_[8]*m_[1]*m_[14]*m_[7]) -
             (m_[8]*m_[5]*m_[2]*m_[15]) -
             (m_[8]*m_[13]*m_[6]*m_[3]) -
             (m_[12]*m_[1]*m_[6]*m_[11]) -
             (m_[12]*m_[5]*m_[10]*m_[3]) -
             (m_[12]*m_[9]*m_[2]*m_[7]));
    return det;
  }

  /**
   *  @name Trace
   *  @fn T Trace(void) const
   *  brief Compute matrix trace
   *  @return Matrix trace
   */
  T Trace(void) const {
    return m_[0] + m_[5] + m_[10] + m_[15];
  }

  /**
   *  @name Transpose
   *  @fn Matrix4& Transpose(void) const
   *  @brief  Compute the matrix transpose
   *  @return Transposed matrix
   */
  Matrix4 Transpose(void) const {
    Matrix4 M;
    M[0] = m_[0];
    M[1] = m_[4];
    M[2] = m_[8];
    M[3] = m_[12];
    M[4] = m_[1];
    M[5] = m_[5];
    M[6] = m_[9];
    M[7] = m_[13];
    M[8] = m_[2];
    M[9] = m_[6];
    M[10] = m_[10];
    M[11] = m_[14];
    M[12] = m_[3];
    M[13] = m_[7];
    M[14] = m_[11];
    M[15] = m_[15];
    return M;
  }

  /**
   *  @name Inverse
   *  @fn Matrix4 Inverse(void) const
   *  @brief  Compute the inverse of this matrix
   *  @return Inverse matrix
   */
  Matrix4 Inverse(void) const {
    Matrix4 iM;
    const T det = this->Determinent();
    if (det != T(0)) {
      const T idet = T(1) / det;
      iM[0] = (m_[5]*m_[10]*m_[15] + m_[9]*m_[14]*m_[7] + m_[13]*m_[6]*m_[11] -
               m_[5]*m_[14]*m_[11] - m_[9]*m_[6]*m_[15] - m_[13]*m_[10]*m_[7]) * idet;
      iM[1] = (m_[1]*m_[14]*m_[11] + m_[9]*m_[2]*m_[15] + m_[13]*m_[10]*m_[3] -
               m_[1]*m_[10]*m_[15] - m_[9]*m_[14]*m_[3] - m_[13]*m_[2]*m_[11]) * idet;
      iM[2] = (m_[1]*m_[6]*m_[15] + m_[5]*m_[14]*m_[3] + m_[13]*m_[2]*m_[7] -
               m_[1]*m_[14]*m_[7] - m_[5]*m_[2]*m_[15] - m_[13]*m_[6]*m_[3]) * idet;
      iM[3] = (m_[1]*m_[10]*m_[7] + m_[5]*m_[2]*m_[11] + m_[9]*m_[6]*m_[3] -
               m_[1]*m_[6]*m_[11] - m_[5]*m_[10]*m_[3] - m_[9]*m_[2]*m_[7]) * idet;
      iM[4] = (m_[4]*m_[14]*m_[11] + m_[8]*m_[6]*m_[15] + m_[12]*m_[10]*m_[7] -
               m_[4]*m_[10]*m_[15] - m_[8]*m_[14]*m_[7] - m_[12]*m_[6]*m_[11]) * idet;
      iM[5] = (m_[0]*m_[10]*m_[15] + m_[8]*m_[14]*m_[3] + m_[12]*m_[2]*m_[11] -
               m_[0]*m_[14]*m_[11] - m_[8]*m_[2]*m_[15] - m_[12]*m_[10]*m_[3]) * idet;
      iM[6] = (m_[0]*m_[14]*m_[7] + m_[4]*m_[2]*m_[15] + m_[12]*m_[6]*m_[3] -
               m_[0]*m_[6]*m_[15] - m_[4]*m_[14]*m_[3] - m_[12]*m_[2]*m_[7]) * idet;
      iM[7] = (m_[0]*m_[6]*m_[11] + m_[4]*m_[10]*m_[3] + m_[8]*m_[2]*m_[7] -
               m_[0]*m_[10]*m_[7] - m_[4]*m_[2]*m_[11] - m_[8]*m_[6]*m_[3]) * idet;
      iM[8] = (m_[4]*m_[9]*m_[15] + m_[8]*m_[13]*m_[7] + m_[12]*m_[5]*m_[11] -
               m_[4]*m_[13]*m_[11] - m_[8]*m_[5]*m_[15] - m_[12]*m_[9]*m_[7]) * idet;
      iM[9] = (m_[0]*m_[13]*m_[11] + m_[8]*m_[1]*m_[15] + m_[12]*m_[9]*m_[3] -
               m_[0]*m_[9]*m_[15] - m_[8]*m_[13]*m_[3] - m_[12]*m_[1]*m_[11]) * idet;
      iM[10] = (m_[0]*m_[5]*m_[15] + m_[4]*m_[13]*m_[3] + m_[12]*m_[1]*m_[7] -
                m_[0]*m_[13]*m_[7] - m_[4]*m_[1]*m_[15] - m_[12]*m_[5]*m_[3]) * idet;
      iM[11] = (m_[0]*m_[9]*m_[7] + m_[4]*m_[1]*m_[11] + m_[8]*m_[5]*m_[3] -
                m_[0]*m_[5]*m_[11] - m_[4]*m_[9]*m_[3] - m_[8]*m_[1]*m_[7]) * idet;
      iM[12] = (m_[4]*m_[13]*m_[10] + m_[8]*m_[5]*m_[14] + m_[12]*m_[9]*m_[6] -
                m_[4]*m_[9]*m_[14] - m_[8]*m_[13]*m_[6] - m_[12]*m_[5]*m_[10]) * idet;
      iM[13] = (m_[0]*m_[9]*m_[14] + m_[8]*m_[13]*m_[2] + m_[12]*m_[1]*m_[10] -
                m_[0]*m_[13]*m_[10] - m_[8]*m_[1]*m_[14] - m_[12]*m_[9]*m_[2]) * idet;
      iM[14] = (m_[0]*m_[13]*m_[6] + m_[4]*m_[1]*m_[14] + m_[12]*m_[5]*m_[2] -
                m_[0]*m_[5]*m_[14] - m_[4]*m_[13]*m_[2] - m_[12]*m_[1]*m_[6]) * idet;
      iM[15] = (m_[0]*m_[5]*m_[10] + m_[4]*m_[9]*m_[2] + m_[8]*m_[1]*m_[6] -
                m_[0]*m_[9]*m_[6] - m_[4]*m_[1]*m_[10] - m_[8]*m_[5]*m_[2]) * idet;
    }
    return iM;
  }


#pragma mark -
#pragma mark Accessors

  /**
   *  @name data
   *  @fn T* data(void)
   *  @brief  Provide reference to data
   *  @return Pointer to data array
   */
  T* data(void) {
    return &m_[0];
  }

  /**
   *  @name data
   *  @fn const T* data(void) const
   *  @brief  Provide constant reference to data
   *  @return Constant pointer to data array
   */
  const T* data(void) const {
    return &m_[0];
  }

#pragma mark -
#pragma mark Operator

  /**
   *  @name operator()(
   *  @fn T& operator()(const int row, const int col)
   *  @brief  Row/Col matrix accessor
   *  @param[in]  row Index of the row
   *  @param[in]  col Index of the column
   *  @return Reference to the selected location
   */
  T& operator()(const int row, const int col) {
    return m_[col * 4 + row];
  }

  /**
   *  @name operator()(
   *  @fn const T& operator()(const int row, const int col) const
   *  @brief  Row/Col matrix accessor
   *  @param[in]  row Index of the row
   *  @param[in]  col Index of the column
   *  @return Reference to the selected location
   */
  const T& operator()(const int row, const int col) const {
    return m_[col * 4 + row];
  }

  /**
   *  @name operator*
   *  @fn Matrix4 operator*(const Matrix4& rhs) const
   *  @brief  Matrix multiplication
   *  @param[in]  rhs Right hand sign matrix
   *  @return Matrix product
   */
  Matrix4 operator*(const Matrix4& rhs) const {
    Matrix4 M;
    M[0] = m_[0] * rhs[0] + m_[4] * rhs[1] + m_[8] * rhs[2] + m_[12] * rhs[3];
    M[1] = m_[1] * rhs[0] + m_[5] * rhs[1] + m_[9] * rhs[2] + m_[13] * rhs[3];
    M[2] = m_[2] * rhs[0] + m_[6] * rhs[1] + m_[10] * rhs[2] + m_[14] * rhs[3];
    M[3] = m_[3] * rhs[0] + m_[7] * rhs[1] + m_[11] * rhs[2] + m_[15] * rhs[3];
    M[4] = m_[0] * rhs[4] + m_[4] * rhs[5] + m_[8] * rhs[6] + m_[12] * rhs[7];
    M[5] = m_[1] * rhs[4] + m_[5] * rhs[5] + m_[9] * rhs[6] + m_[13] * rhs[7];
    M[6] = m_[2] * rhs[4] + m_[6] * rhs[5] + m_[10] * rhs[6] + m_[14] * rhs[7];
    M[7] = m_[3] * rhs[4] + m_[7] * rhs[5] + m_[11] * rhs[6] + m_[15] * rhs[7];
    M[8] = m_[0] * rhs[8] + m_[4] * rhs[9] + m_[8] * rhs[10] + m_[12] * rhs[11];
    M[9] = m_[1] * rhs[8] + m_[5] * rhs[9] + m_[9] * rhs[10] + m_[13] * rhs[11];
    M[10] = m_[2] * rhs[8] + m_[6] * rhs[9] + m_[10] * rhs[10] + m_[14] * rhs[11];
    M[11] = m_[3] * rhs[8] + m_[7] * rhs[9] + m_[11] * rhs[10] + m_[15] * rhs[11];
    M[12] = m_[0] * rhs[12] + m_[4] * rhs[13] + m_[8] * rhs[14] + m_[12] * rhs[15];
    M[13] = m_[1] * rhs[12] + m_[5] * rhs[13] + m_[9] * rhs[14] + m_[13] * rhs[15];
    M[14] = m_[2] * rhs[12] + m_[6] * rhs[13] + m_[10] * rhs[14] + m_[14] * rhs[15];
    M[15] = m_[3] * rhs[12] + m_[7] * rhs[13] + m_[11] * rhs[14] + m_[15] * rhs[15];
    return M;
  }

  /**
   *  @name operator*
   *  @fn Vector4<T> operator*(const Vector4<T>& rhs) const
   *  @brief  Multiply by a Vector3
   *  @param[in]  rhs Right hand sign vector
   *  @return Matrix-Vector product
   */
  Vector4<T> operator*(const Vector4<T>& rhs) const {
    Vector4<T> v;
    v.x_ = m_[0] * rhs.x_ + m_[4] * rhs.y_ + m_[8] * rhs.z_ + m_[12] * rhs.w_;
    v.y_ = m_[1] * rhs.x_ + m_[5] * rhs.y_ + m_[9] * rhs.z_ + m_[13] * rhs.w_;
    v.z_ = m_[2] * rhs.x_ + m_[6] * rhs.y_ + m_[10] * rhs.z_ + m_[14] * rhs.w_;
    v.w_ = m_[3] * rhs.x_ + m_[7] * rhs.y_ + m_[11] * rhs.z_ + m_[15] * rhs.w_;
    return v;
  }

  /**
   *  @name operator*
   *  @fn Matrix4 operator*(const T s) const
   *  @brief  Multiply by a scalar
   *  @param[in]  s Scalar to multiply by
   *  @return Scaled Matrix
   */
  Matrix4<T> operator*(const T s) const {
    Matrix4 M;
    #pragma unroll
    for (int i = 0; i < 16; ++i) {
      M[i] = m_[i] * s;
    }
    return M;
  }

  /**
   *  @name operator+
   *  @fn Matrix4 operator+(const Matrix4& rhs) const
   *  @brief  Add another Matrix4
   *  @param[in]  rhs Right hand sign matrix
   *  @return Matrix addition
   */
  Matrix4 operator+(const Matrix4& rhs) const {
    Matrix4 M;
    #pragma unroll
    for (int i = 0; i < 16; ++i) {
      M[i] = m_[i] + rhs[i];
    }
    return M;
  }

  /**
   *  @name operator-
   *  @fn Matrix4 operator-(const Matrix4& rhs) const
   *  @brief  Subtract another Matrix4
   *  @param[in]  rhs Right hand sign matrix
   *  @return Matrix subtraction
   */
  Matrix4 operator-(const Matrix4& rhs) const {
    Matrix4 M;
    #pragma unroll
    for (int i = 0; i < 16; ++i) {
      M[i] = m_[i] - rhs[i];
    }
    return M;
  }

  /**
   *  @name operator=
   *  @fn Matrix4& operator=(const T s)
   *  @brief  Assignment operator
   *  @param[in]  s Scalar to assign to this matrix
   *  @return
   */
  Matrix4& operator=(const T s) {
    #pragma unroll
    for (int i = 0 ; i < 16 ; ++i) {
      m_[i] = s;
    }
    return *this;
  }

  /**
   *  @name operator[]
   *  @fn T& operator[](const int idx)
   *  @brief  Operator to linearly access element stored in column major flavor
   *  @param[in]  idx Index to reach
   *  @return Reference to the selected element
   */
  T& operator[](const int idx) {
    return m_[idx];
  }

  /**
   *  @name operator[]
   *  @fn const T& operator[](const int idx) const
   *  @brief  Operator to linearly access element stored in row major flavor
   *  @param[in]  idx Index to reach
   *  @return Reference to the selected element
   */
  const T& operator[](const int idx) const {
    return m_[idx];
  }
  
  
  /**
   *  @name operator<<
   *  @fn friend std::ostream& operator<<(std::ostream& out, const Matrix4& m)
   *  @brief  Print matrix into a given steram
   *  @param[in]  out   Output stream
   *  @param[in]  m     Matrix to print
   *  @return Output stream
   */
  friend std::ostream& operator<<(std::ostream& out, const Matrix4& m) {
    out << m[0] << " " << m[4] << " " << m[8] << " " << m[12] << std::endl;
    out << m[1] << " " << m[5] << " " << m[9] << " " << m[13] << std::endl;
    out << m[2] << " " << m[6] << " " << m[10] << " " << m[14] << std::endl;
    out << m[3] << " " << m[7] << " " << m[11] << " " << m[15] << std::endl;
    return out;
  }
  
  
#pragma mark -
#pragma mark Private
  
private:
  /** Data array - ColumnMajor layout */
  T m_[16];
};
}  // namespace FaceKit
#endif /* __FACEKIT_MATRIX__ */
