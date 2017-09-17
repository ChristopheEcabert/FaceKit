/**
 *  @file   linear_algebra.hpp
 *  @brief  Blas/Lapack wrapper for common linear algebra operation
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   13.08.17
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_LINEAR_ALGEBRA__
#define __FACEKIT_LINEAR_ALGEBRA__

#include "opencv2/core/core.hpp"

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  LinearAlgebra
 *  @brief  Blas/Lapack wrapper for common linear algebra operation
 *  @author Christophe Ecabert
 *  @date   13.08.17
 *  @ingroup core
 *  @tparam T Data type on which operation will be performed
 */
template<typename T>
class FK_EXPORTS LinearAlgebra {
 public:
  
  /**
   * @enum  TransposeType
   * @brief List of operation on matrix
   */
  enum class TransposeType : int {
    /** No transpose */
    kNoTranspose = 111,
    /** Transpose */
    kTranspose = 112,
    /** Conjugate transpose */
    kConjTranspose = 113,
    /** Conjugate transpose */
    kConjNoTranspose = 114
  };
    
  /**
   *  @name L2Norm
   *  @fn T L2Norm(const cv::Mat& vector)
   *  @brief  Compute the L2 norm of a vector
   *  @param[in]  vector  Vector to compute the norm
   *  @return L2 Norm
   */
  static T L2Norm(const cv::Mat& vector);
    
  /**
   *  @name Mean
   *  @fn static void Mean(const cv::Mat& A,
                            const int axis, cv::Mat* mean)
   *  @brief  Compute average value of a matrix \p A in a given direction
   *  @param[in]  A       Data matrix
   *  @param[in]  axis    Direction along which the mean need to be computed
   *                      0 = row, 1 = column
   *  @param[out] mean    Average values
   */
  static void Mean(const cv::Mat& A, const int axis, cv::Mat* mean);
    
  /**
   *  @name Axpy
   *  @fn void Axpy(const cv::Mat& A, const T alpha, cv::Mat* B)
   *  @brief  Compute : B := alpha * A + B
   *  @param[in]      A         Vector/Matrix A
   *  @param[in]      alpha     Alpha scaling factor
   *  @param[in,out]  B         Vector/Matrix B
   */
  static void Axpy(const cv::Mat& A, const T alpha, cv::Mat* B);
    
  /**
   *  @name Axpby
   *  @fn void Axpby(const cv::Mat& A, const T alpha,
   *                 const T beta, cv::Mat* B)
   *  @brief  Compute : B := alpha * A + beta * B
   *  @param[in]      A         Vector/Matrix A
   *  @param[in]      alpha     Alpha scaling factor
   *  @param[in]      beta      Beta scaling factor
   *  @param[in,out]  B  Vector/Matrix B
   */
  static void Axpby(const cv::Mat& A, const T alpha, const T beta, cv::Mat* B);
    
  /**
   *  @name   Dot
   *  @fn T Dot(const cv::Mat& a, const cv::Mat& b)
   *  @brief  Compute the dot product between two vectors \p a and \p b
   *  @param[in]  a  Vector A
   *  @param[in]  b  Vector B
   *  @return Dot product
   */
  static T Dot(const cv::Mat& a, const cv::Mat& b);
  
  /**
   *  @name   Dot
   *  @fn T Dot(const cv::Mat& a,const int inc_a,
                const cv::Mat& b, const int inc_b)
   *  @brief  Compute the dot product between two vectors
   *  @param[in]  a         Vector A
   *  @param[in]  inc_a     Stride within \p a
   *  @param[in]  b         Vector B
   *  @param[in]  inc_b     Stride within \p b
   *  @return Dot product or -1.0 if error(s)
   */
  static T Dot(const cv::Mat& a,const int inc_a,
               const cv::Mat& b, const int inc_b);
  
  /**
   *  @name   Gemv
   *  @fn void Gemv(const cv::Mat& A,
                    const TransposeType trans_a,
                    const T alpha,
                    const cv::Mat& x,
                    const T beta,
                    cv::Mat* y)
   *  @brief  Multiplies a matrix by a vector
   *          Y = a * Ax + b Y
   *  @param[in] A        Matrix A
   *  @param[in] trans_a  Indicate if A is transpose or not
   *  @param[in] alpha    Scaling factor alpha
   *  @param[in] x        Vector X
   *  @param[in] beta     Scaling factor beta
   *  @param[in,out] y    Output vector
   */
  static void Gemv(const cv::Mat& A,
                   const TransposeType trans_a,
                   const T alpha,
                   const cv::Mat& x,
                   const T beta,
                   cv::Mat* y);
  
  /**
   *  @name   Gemm
   *  @fn void Gemm(const cv::Mat& A,
                     const TransposeType trans_a,
                     const T alpha,
                     const cv::Mat& B,
                     const TransposeType trans_b,
                     const T beta,
                     cv::Mat* C)
   *  @brief  Compute the product between two matrices
   *          C = a * AB + b * C
   *  @param[in] A         Matrix A
   *  @param[in] trans_a   Transpose flag indicator for A
   *  @param[in] alpha     Alpha coefficient
   *  @param[in] B         Matrix B
   *  @param[in] trans_b   Transpose flag indicator for B
   *  @param[in] beta      Beta coefficient
   *  @param[in,out] C     Resulting matrix
   */
  static void Gemm(const cv::Mat& A,
                   const TransposeType trans_a,
                   const T alpha,
                   const cv::Mat& B,
                   const TransposeType trans_b,
                   const T beta,
                   cv::Mat* C);
  
};
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_LINEAR_ALGEBRA__ */
