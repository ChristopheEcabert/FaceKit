/**
 *  @file   linear_algebra.cpp
 *  @brief  Blas/Lapack wrapper for common linear algebra operation
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   13.08.17
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#else
// Include proper file for Linux/Win32 -> OpenBLAS
#endif

#include "facekit/core/math/linear_algebra.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark L2 Norm
  
/*
 *  @name L2Norm
 *  @fn T L2Norm(const cv::Mat& vector)
 *  @brief  Compute the L2 norm of a vector
 *  @param[in]  vector  Vector to compute the norm
 *  @return L2 Norm
 */
template<typename T>
T LinearAlgebra<T>::L2Norm(const cv::Mat& vector) {
  //  stub
  return T(-1.0);
}
  
template<>
float LinearAlgebra<float>::L2Norm(const cv::Mat& vector) {
  // Sanity check
  assert(vector.type() == cv::DataType<float>::type && vector.channels() == 1);
  const int N = vector.rows * vector.cols;
  return cblas_snrm2(N, reinterpret_cast<const float*>(vector.data), 1);
}

template<>
double LinearAlgebra<double>::L2Norm(const cv::Mat& vector) {
  // Sanity check
  assert(vector.type() == cv::DataType<double>::type && vector.channels() == 1);
  const int N = vector.rows * vector.cols;
  return cblas_dnrm2(N, reinterpret_cast<const double*>(vector.data), 1);
}
  
#pragma mark -
#pragma mark Mean
  
/*
 *  @name Mean
 *  @fn void Mean(const cv::Mat& A,
                  const int axis, cv::Mat* mean)
 *  @brief  Compute average value of a matrix \p A in a given direction
 *  @param[in]  A       Data matrix
 *  @param[in]  axis    Direction along which the mean need to be computed
 *                      0 = row, 1 = column
 *  @param[out] mean    Average values
 */
template<typename T>
void LinearAlgebra<T>::Mean(const cv::Mat& A, const int axis, cv::Mat* mean) {
  // stub
}
template<>
void LinearAlgebra<float>::Mean(const cv::Mat& A,
                                const int axis,
                                cv::Mat* mean) {
  assert(A.type() == CV_32FC1);
  if (axis == 0) {
    // by row
    mean->create(1, A.cols, A.type());
    cv::Mat one = cv::Mat(A.rows, 1, A.type(), cv::Scalar_<float>(1.f));
    const float alpha = 1.0f / static_cast<float>(A.rows);
    cblas_sgemv(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasTrans,
                A.rows,
                A.cols,
                alpha,
                reinterpret_cast<const float*>(A.data),
                A.cols,
                reinterpret_cast<const float*>(one.data),
                1,
                0.0f,
                reinterpret_cast<float*>(mean->data),
                1);
  } else {
    // by column
    mean->create(A.rows, 1, A.type());
    cv::Mat one = cv::Mat(A.cols ,1 , A.type(), cv::Scalar_<float>(1.f));
    const float alpha = 1.0f / static_cast<float>(A.cols);
    cblas_sgemv(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasNoTrans,
                A.rows,
                A.cols,
                alpha,
                reinterpret_cast<const float*>(A.data),
                A.cols,
                reinterpret_cast<const float*>(one.data),
                1,
                0.0f,
                reinterpret_cast<float*>(mean->data),
                1);
  }
}
template<>
void LinearAlgebra<double>::Mean(const cv::Mat& A,
                                 const int axis,
                                 cv::Mat* mean) {
  assert(A.type() == CV_64FC1);
  if (axis == 0) {
    // by row
    mean->create(1, A.cols, A.type());
    cv::Mat one = cv::Mat(A.rows, 1, A.type(), cv::Scalar_<double>(1.0));
    const float alpha = 1.0 / static_cast<double>(A.rows);
    cblas_dgemv(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasTrans,
                A.rows,
                A.cols,
                alpha,
                reinterpret_cast<const double*>(A.data),
                A.cols,
                reinterpret_cast<const double*>(one.data),
                1,
                0.0,
                reinterpret_cast<double*>(mean->data),
                1);
  } else {
    // by column
    mean->create(A.rows, 1, A.type());
    cv::Mat one = cv::Mat(A.cols ,1 , A.type(), cv::Scalar_<double>(1.0));
    const double alpha = 1.0 / static_cast<double>(A.cols);
    cblas_dgemv(CBLAS_ORDER::CblasRowMajor,
                CBLAS_TRANSPOSE::CblasNoTrans,
                A.rows,
                A.cols,
                alpha,
                reinterpret_cast<const double*>(A.data),
                A.cols,
                reinterpret_cast<const double*>(one.data),
                1,
                0.0,
                reinterpret_cast<double*>(mean->data),
                1);

  }
}
  
  
#pragma mark -
#pragma mark Axpy

/*
 *  @name Axpy
 *  @fn void Axpy(const cv::Mat& A, const T alpha, cv::Mat* B)
 *  @brief  Compute : B := alpha * A + B
 *  @param[in]      A         Vector/Matrix A
 *  @param[in]      alpha     Alpha scaling factor
 *  @param[in,out]  B         Vector/Matrix B
 */
template<typename T>
void LinearAlgebra<T>::Axpy(const cv::Mat& A, const T alpha, cv::Mat* B) {
  // stub
}
template<>
void LinearAlgebra<float>::Axpy(const cv::Mat& A,
                                const float alpha,
                                cv::Mat* B) {
  assert(A.type() == CV_32FC1);
  B->create(A.rows, A.cols, A.type());
  const int N = static_cast<int>(A.rows * A.cols);
  cblas_saxpy(N,
              alpha,
              reinterpret_cast<const float*>(A.data),
              1,
              reinterpret_cast<float*>(B->data),
              1);
}
template<>
void LinearAlgebra<double>::Axpy(const cv::Mat& A,
                                 const double alpha,
                                 cv::Mat* B) {
  assert(A.type() == CV_64FC1);
  B->create(A.rows, A.cols, A.type());
  const int N = static_cast<int>(A.rows * A.cols);
  cblas_daxpy(N,
              alpha,
              reinterpret_cast<const double*>(A.data),
              1,
              reinterpret_cast<double*>(B->data),
              1);
}
#pragma mark -
#pragma mark Axpby

/*
 *  @name Axpby
 *  @fn void Axpby(const cv::Mat& A, const T alpha,
 *                 const T beta, cv::Mat* B)
 *  @brief  Compute : B := alpha * A + beta * B
 *  @param[in]      A         Vector/Matrix A
 *  @param[in]      alpha     Alpha scaling factor
 *  @param[in]      beta      Beta scaling factor
 *  @param[in,out]  B  Vector/Matrix B
 */
template<typename T>
void LinearAlgebra<T>::Axpby(const cv::Mat& A,
                             const T alpha,
                             const T beta,
                             cv::Mat* B) {
  // stub
}
template<>
void LinearAlgebra<float>::Axpby(const cv::Mat& A,
                             const float alpha,
                             const float beta,
                             cv::Mat* B) {
  assert(A.type() == CV_32FC1);
  B->create(A.rows, A.cols, A.type());
  const int N = static_cast<int>(A.rows * A.cols);
#ifdef __APPLE__
  catlas_saxpby(N,
                alpha,
                reinterpret_cast<const float*>(A.data),
                1,
                beta,
                reinterpret_cast<float*>(B->data),
                1);
#else
  cblas_saxpby(N,
               alpha,
               reinterpret_cast<const float*>(A.data),
               1,
               beta,
               reinterpret_cast<float*>(B->data),
               1);
#endif
}
template<>
void LinearAlgebra<double>::Axpby(const cv::Mat& A,
                             const double alpha,
                             const double beta,
                             cv::Mat* B) {
  assert(A.type() == CV_64FC1);
  B->create(A.rows, A.cols, A.type());
  const int N = static_cast<int>(A.rows * A.cols);
#ifdef __APPLE__
  catlas_daxpby(N,
                alpha,
                reinterpret_cast<const double*>(A.data),
                1,
                beta,
                reinterpret_cast<double*>(B->data),
                1);
#else
  cblas_daxpby(N,
               alpha,
               reinterpret_cast<const double*>(A.data),
               1,
               beta,
               reinterpret_cast<double*>(B->data),
               1);
#endif
}

#pragma mark -
#pragma mark Dot

/*
 *  @name   Dot
 *  @fn T Dot(const cv::Mat& a, const cv::Mat& b)
 *  @brief  Compute the dot product between two vectors \p a and \p b
 *  @param[in]  a  Vector A
 *  @param[in]  b  Vector B
 *  @return Dot product
 */
template<typename T>
T LinearAlgebra<T>::Dot(const cv::Mat& a, const cv::Mat& b) {
  // stub
  return T(-1.0);
}
template<>
float LinearAlgebra<float>::Dot(const cv::Mat& a, const cv::Mat& b) {
  assert((a.type() == CV_32FC1) && (b.type() == CV_32FC1));
  const int N = std::max(a.rows,a.cols);
  assert(N == std::max(b.rows,b.cols));
  return cblas_sdot(N,
                    reinterpret_cast<const float*>(a.data),
                    1,
                    reinterpret_cast<const float*>(b.data),
                    1);
}
template<>
double LinearAlgebra<double>::Dot(const cv::Mat& a, const cv::Mat& b) {
  assert((a.type() == CV_64FC1) && (b.type() == CV_64FC1));
  const int N = std::max(a.rows,a.cols);
  assert(N == std::max(b.rows,b.cols));
  return cblas_ddot(N,
                    reinterpret_cast<const double*>(a.data),
                    1,
                    reinterpret_cast<const double*>(b.data),
                    1);
}
  
/*
 *  @name   Dot
 *  @fn T Dot(const cv::Mat& A,const int inc_a,
 const cv::Mat& b, const int inc_b)
 *  @brief  Compute the dot product between two vectors
 *  @param[in]  a  Vector A
 *  @param[in]  inc_a     Stride within \p a
 *  @param[in]  b         Vector B
 *  @param[in]  inc_b     Stride within \p b
 *  @return Dot product or -1.0 if error(s)
 */
template<typename T>
T LinearAlgebra<T>::Dot(const cv::Mat& a,const int inc_a,
                        const cv::Mat& b, const int inc_b) {
  // stub
  return T(-1.0);
}
template<>
float LinearAlgebra<float>::Dot(const cv::Mat& a,const int inc_a,
                                const cv::Mat& b, const int inc_b) {
  assert((a.type() == CV_32FC1) && (b.type() == CV_32FC1));
  int Na = static_cast<int>(std::ceil(std::max(a.rows, a.cols) /
                                      static_cast<float>(inc_a)));
  int Nb = static_cast<int>(std::ceil(std::max(b.rows, b.cols) /
                                      static_cast<float>(inc_b)));
  if (Na == Nb) {
    return cblas_sdot(Na,
                      reinterpret_cast<const float*>(a.data),
                      inc_a,
                      reinterpret_cast<const float*>(b.data),
                      inc_b);
  } else {
    return -2.f;
  }
}
template<>
double LinearAlgebra<double>::Dot(const cv::Mat& a,const int inc_a,
                                  const cv::Mat& b, const int inc_b) {
  assert((a.type() == CV_64FC1) && (b.type() == CV_64FC1));
  int Na = static_cast<int>(std::ceil(std::max(a.rows, a.cols) /
                                      static_cast<double>(inc_a)));
  int Nb = static_cast<int>(std::ceil(std::max(b.rows, b.cols) /
                                      static_cast<double>(inc_b)));
  if (Na == Nb) {
    return cblas_ddot(Na,
                      reinterpret_cast<const double*>(a.data),
                      inc_a,
                      reinterpret_cast<const double*>(b.data),
                      inc_b);
  } else {
    return -2.0;
  }
}
  
#pragma mark -
#pragma mark Gemv

/*
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
template<typename T>
void LinearAlgebra<T>::Gemv(const cv::Mat& A,
                            const TransposeType trans_a,
                            const T alpha,
                            const cv::Mat& x,
                            const T beta,
                            cv::Mat* y) {
  // stub
}
template<>
void LinearAlgebra<float>::Gemv(const cv::Mat& A,
                                const TransposeType trans_a,
                                const float alpha,
                                const cv::Mat& x,
                                const float beta,
                                cv::Mat* y) {
  assert((A.type() == CV_32FC1) && (x.type() == CV_32FC1));
  //Take care for output
  if(trans_a == TransposeType::kNoTranspose) {
    assert(A.cols == std::max(x.cols, x.rows));
    y->create(A.rows, 1, A.type());
  } else {
    assert(A.rows == std::max(x.cols, x.rows));
    y->create(A.cols, 1, A.type());
  }
  cblas_sgemv(CBLAS_ORDER::CblasRowMajor,
              static_cast<CBLAS_TRANSPOSE>(trans_a),
              A.rows,
              A.cols,
              alpha,
              reinterpret_cast<const float*>(A.data),
              A.cols,
              reinterpret_cast<const float*>(x.data),
              1,
              beta,
              reinterpret_cast<float*>(y->data),
              1);
}
template<>
void LinearAlgebra<double>::Gemv(const cv::Mat& A,
                                 const TransposeType trans_a,
                                 const double alpha,
                                 const cv::Mat& x,
                                 const double beta,
                                 cv::Mat* y) {
  assert((A.type() == CV_64FC1) && (x.type() == CV_64FC1));
  //Take care for output
  if(trans_a == TransposeType::kNoTranspose) {
    assert(A.cols == std::max(x.cols, x.rows));
    y->create(A.rows, 1, A.type());
  } else {
    assert(A.rows == std::max(x.cols, x.rows));
    y->create(A.cols, 1, A.type());
  }
  cblas_dgemv(CBLAS_ORDER::CblasRowMajor,
              static_cast<CBLAS_TRANSPOSE>(trans_a),
              A.rows,
              A.cols,
              alpha,
              reinterpret_cast<const double*>(A.data),
              A.cols,
              reinterpret_cast<const double*>(x.data),
              1,
              beta,
              reinterpret_cast<double*>(y->data),
              1);
}

#pragma mark -
#pragma mark Gemm

/*
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
template<typename T>
void LinearAlgebra<T>::Gemm(const cv::Mat& A,
                            const TransposeType trans_a,
                            const T alpha,
                            const cv::Mat& B,
                            const TransposeType trans_b,
                            const T beta,
                            cv::Mat* C) {
  // stub
}
template<>
void LinearAlgebra<float>::Gemm(const cv::Mat& A,
                                const TransposeType trans_a,
                                const float alpha,
                                const cv::Mat& B,
                                const TransposeType trans_b,
                                const float beta,
                                cv::Mat* C) {
  assert((A.type() == CV_32FC1) && (B.type() == CV_32FC1));
  //Define output matrix size
  const int out_row = (trans_a == TransposeType::kNoTranspose ?
                       A.rows :
                       A.cols);
  const int out_col = (trans_b == TransposeType::kNoTranspose ?
                       B.cols :
                       B.rows);
  const int K = (trans_a == TransposeType::kNoTranspose ?
                 A.cols :
                 A.rows);
  C->create(out_row, out_col, A.type());
  // multiplication
  cblas_sgemm(CBLAS_ORDER::CblasRowMajor,
              static_cast<CBLAS_TRANSPOSE>(trans_a),
              static_cast<CBLAS_TRANSPOSE>(trans_b),
              out_row,
              out_col,
              K,
              alpha,
              reinterpret_cast<const float*>(A.data),
              A.cols,
              reinterpret_cast<const float*>(B.data),
              B.cols,
              beta,
              reinterpret_cast<float*>(C->data),
              C->cols);
}
template<>
void LinearAlgebra<double>::Gemm(const cv::Mat& A,
                                 const TransposeType trans_a,
                                 const double alpha,
                                 const cv::Mat& B,
                                 const TransposeType trans_b,
                                 const double beta,
                                 cv::Mat* C) {
  assert((A.type() == CV_64FC1) && (B.type() == CV_64FC1));
  //Define output matrix size
  const int out_row = (trans_a == TransposeType::kNoTranspose ?
                       A.rows :
                       A.cols);
  const int out_col = (trans_b == TransposeType::kNoTranspose ?
                       B.cols :
                       B.rows);
  const int K = (trans_a == TransposeType::kNoTranspose ?
                 A.cols :
                 A.rows);
  C->create(out_row, out_col, A.type());
  // multiplication
  cblas_dgemm(CBLAS_ORDER::CblasRowMajor,
              static_cast<CBLAS_TRANSPOSE>(trans_a),
              static_cast<CBLAS_TRANSPOSE>(trans_b),
              out_row,
              out_col,
              K,
              alpha,
              reinterpret_cast<const double*>(A.data),
              A.cols,
              reinterpret_cast<const double*>(B.data),
              B.cols,
              beta,
              reinterpret_cast<double*>(C->data),
              C->cols);
}
  
#pragma mark -
#pragma mark Explicit Instantiation
  
/** Float - LinearAlgebra */
template class LinearAlgebra<float>;
/** Double - LinearAlgebra */
template class LinearAlgebra<double>;
  
}  // namespace FaceKit
