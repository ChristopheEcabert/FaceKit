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
typedef __CLPK_integer  lapack_int;
typedef __CLPK_real lapack_flt;
typedef __CLPK_doublereal lapack_dbl;
#else
// Include proper file for Linux/Win32 -> OpenBLAS
#include "cblas.h"
#include "lapacke.h"
typedef float lapack_flt;
typedef double lapack_dbl;
#endif

#include "facekit/core/math/linear_algebra.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark BLAS
  
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
#pragma mark Sbmv
  
/*
 * @name  Sbmv
 * @fn    static void Sbmv(const cv::Mat& A, const T alpha, const cv::Mat& x,
                           const T beta, cv::Mat* y)
 * @brief Perform matrix-vector operation :  y := alpha*A*x + beta*y
 *        where alpha and beta are scalars, x and y are n element vectors and
 *        A is an n by n symmetric band matrix, with k super-diagonals.
 *        At the moment support only diagonal matrix
 *        i.e. element-wise vector multiplication
 * @param[in] A       Vector of element on the matrix's diagonal
 * @param[in] alpha   Scaling factor
 * @param[in] x       Vector
 * @param[in] beta    Scaling factor
 * @param[in, out] y  Output
 */
template<typename T>
void LinearAlgebra<T>::Sbmv(const cv::Mat& A,
                            const T alpha,
                            const cv::Mat& x,
                            const T beta, cv::Mat* y) {
  // STub
}

template<>
void LinearAlgebra<float>::Sbmv(const cv::Mat& A,
                                const float alpha,
                                const cv::Mat& x,
                                const float beta,
                                cv::Mat* y) {
  // Sanity check
  assert((A.cols == 1 && A.rows > 1) || (A.cols > 1 && A.rows == 1));
  assert((x.cols == 1 && x.rows > 1) || (x.cols > 1 && x.rows == 1));
  assert(A.type() == CV_32FC1 && x.type() == CV_32FC1);
  // Define output
  y->create(x.rows, x.cols, x.type());
  // Compute
  const int k = 0;  // only diagonal
  const int N = std::max(A.cols, A.rows);
  const int lda = 1;
  const int inc = 1;
  cblas_ssbmv(CBLAS_ORDER::CblasRowMajor,
              CBLAS_UPLO::CblasUpper,
              N,
              k,
              alpha,
              reinterpret_cast<const float*>(A.data),
              lda,
              reinterpret_cast<const float*>(x.data),
              inc,
              beta,
              reinterpret_cast<float*>(y->data),
              inc);
}

template<>
void LinearAlgebra<double>::Sbmv(const cv::Mat& A,
                                 const double alpha,
                                 const cv::Mat& x,
                                 const double beta,
                                 cv::Mat* y) {
  // Sanity check
  assert((A.cols == 1 && A.rows > 1) || (A.cols > 1 && A.rows == 1));
  assert((x.cols == 1 && x.rows > 1) || (x.cols > 1 && x.rows == 1));
  assert(A.type() == CV_64FC1 && x.type() == CV_64FC1);
  // Define output
  y->create(x.rows, x.cols, x.type());
  // Compute
  const int k = 0;  // only diagonal
  const int N = std::max(A.cols, A.rows);
  const int lda = 1;
  const int inc = 1;
  cblas_dsbmv(CBLAS_ORDER::CblasRowMajor,
              CBLAS_UPLO::CblasUpper,
              N,
              k,
              alpha,
              reinterpret_cast<const double*>(A.data),
              lda,
              reinterpret_cast<const double*>(x.data),
              inc,
              beta,
              reinterpret_cast<double*>(y->data),
              inc);
}
  
#pragma mark -
#pragma mark LAPACK
  
#pragma mark Linear Solver
  
/*
 *  @name   LinearSolverCall
 *  @fn inline void LinearSolverCall(const lin_solver_params& p)
 *  @brief  Interface for lapack function call - Undefined type
 *  @throw  std::runtime_error()
 */
template<typename T>
void LinearAlgebra<T>::Lapack::LinearSolverCall(lin_solver_params& p) {
  throw std::runtime_error("Error Unsupported Type");
}

template<>
void LinearAlgebra<float>::Lapack::LinearSolverCall(lin_solver_params& p) {
  sgels_(&p.k_trans,
         &p.k_m,
         &p.k_n,
         &p.k_nrhs,
         (lapack_flt*)p.k_a,
         &p.k_lda,
         (lapack_flt*)p.k_b,
         &p.k_ldb,
         (lapack_flt*)p.k_work,
         &p.k_lwork,
         &p.k_info);
}

template<>
void LinearAlgebra<double>::Lapack::LinearSolverCall(lin_solver_params& p) {
  dgels_(&p.k_trans,
         &p.k_m,
         &p.k_n,
         &p.k_nrhs,
         (lapack_dbl*)p.k_a,
         &p.k_lda,
         (lapack_dbl*)p.k_b,
         &p.k_ldb,
         (lapack_dbl*)p.k_work,
         &p.k_lwork,
         &p.k_info);
}
  
/*
 *  @name   LinearSolver
 *  @fn LinearSolver(void)
 *  @brief  Constructor
 */
template<typename T>
LinearAlgebra<T>::LinearSolver::LinearSolver(void) {
  p_.k_a = nullptr;
  p_.k_b = nullptr;
  p_.k_work = nullptr;
}

/*
 *  @name   ~LinearSolver
 *  @fn ~LinearSolver(void)
 *  @brief  Destructor
 */
template<typename T>
LinearAlgebra<T>::LinearSolver::~LinearSolver(void) {
  if (p_.k_a) {
    delete[] p_.k_a;
  }
  if (p_.k_b) {
    delete[] p_.k_b;
  }
  if (p_.k_work) {
    delete[] p_.k_work;
  }
}

/*
 *  @name Solve
 *  @fn void Solve(const cv::Mat& A,
 const cv::Mat& b,
 cv::Mat* x)
 *  @brief  Solve AX = B
 *  @param[in]  A  Matrix A
 *  @param[in]  b  Matrix B, stored in column
 *  @param[out] x  Solution
 */
template<typename T>
void LinearAlgebra<T>::LinearSolver::Solve(const cv::Mat& A,
                                           const cv::Mat& b,
                                           cv::Mat* x) {
  //Init
  p_.k_trans = 'N';
  if (p_.k_n != A.cols ||
      p_.k_m != A.rows ||
      p_.k_lda != A.rows ||
      p_.k_nrhs != b.cols) {
    // Release ressources if necessary
    if (p_.k_a) { delete[] p_.k_a; }
    if (p_.k_b) { delete[] p_.k_b; }
    if (p_.k_work) {delete [] p_.k_work; }
    // Define dimensions
    p_.k_m = A.rows; //Column-major system
    p_.k_n = A.cols;
    p_.k_nrhs = b.cols;
    p_.k_lda = p_.k_m;
    p_.k_nrhs = b.cols;
    p_.k_ldb = std::max(p_.k_m, p_.k_n);
    // Init arrays
    p_.k_a = new T[p_.k_m * p_.k_n];
    p_.k_b = new T[p_.k_ldb * p_.k_nrhs];
    //Workspace
    p_.k_work = new T[1];
    p_.k_lwork = -1;
    //Query workspace size
    Lapack::LinearSolverCall(p_);
    //Setup workspace with correct size
    p_.k_lwork = static_cast<int>(p_.k_work[0]);
    delete[] p_.k_work;
    p_.k_work = new T[p_.k_lwork];
  }
  //Convert to column major memory layout for A
  cv::Mat tmp_mat_a = cv::Mat(A.cols,
                              A.rows,
                              cv::DataType<T>::type,
                              p_.k_a);
  tmp_mat_a = A.t();
  //Convert to column major memory layout for B
  cv::Mat tmp_mat_b = cv::Mat(b.cols,
                              b.rows,
                              cv::DataType<T>::type,
                              p_.k_b);
  tmp_mat_b = b.t();
  //Done, Solve the problem
  p_.k_info = 0;
  Lapack::LinearSolverCall(p_);
  //Retrive solution
  if (p_.k_info == 0) {
    x->create(p_.k_nrhs, A.cols, cv::DataType<T>::type);
    T* src_ptr = p_.k_b;
    T* dest_ptr = x->ptr<T>(0);
    for(int i = 0 ; i < p_.k_nrhs; ++i) {
      std::memcpy((void*)dest_ptr, (const void*)src_ptr, p_.k_n * sizeof(T));
      src_ptr += p_.k_ldb;
      dest_ptr += p_.k_n;
    }
    //Transpose back to columnwise structure
    *x = x->t();
  } else {
    FACEKIT_LOG_INFO("Solver can not find a solution to the provided system");
    *x = cv::Mat();
  }
}
  
#pragma mark Square Linear Solver
  
/*
 *  @name   SquareLinearSolverCall
 *  @fn inline void SquareLinearSolverCall(lin_solver_params<T>& p)
 *  @brief  Interface for lapack function call - Undefined type
 *  @throw  std::runtime_error()
 */
template<typename T>
void LinearAlgebra<T>::Lapack::
SquareLinearSolverCall(square_lin_solver_params& p) {
  throw std::runtime_error("Error Unsupported Type");
}

template<>
void LinearAlgebra<float>::Lapack::
SquareLinearSolverCall(square_lin_solver_params& p) {
  sgesv_(&p.k_n,
         &p.k_nrhs,
         (lapack_flt*)p.k_a,
         &p.k_lda,
         (lapack_int*)p.k_ipiv,
         (lapack_flt*)p.k_b,
         &p.k_ldb,
         &p.k_info);
}

template<>
void LinearAlgebra<double>::Lapack::
SquareLinearSolverCall(square_lin_solver_params& p) {
  dgesv_(&p.k_n,
         &p.k_nrhs,
         (lapack_dbl*)p.k_a,
         &p.k_lda,
         (lapack_int*)p.k_ipiv,
         (lapack_dbl*)p.k_b,
         &p.k_ldb,
         &p.k_info);
}
  
/*
 *  @name   SquareLinearSolver
 *  @fn SquareLinearSolver(void)
 *  @brief  Constructor
 */
template<typename T>
LinearAlgebra<T>::SquareLinearSolver::SquareLinearSolver(void) {
  p_.k_a = nullptr;
  p_.k_ipiv = nullptr;
  p_.k_b = nullptr;
}

/*
 *  @name   ~SquareLinearSolver
 *  @fn ~SquareLinearSolver(void)
 *  @brief  Destructor
 */
template<typename T>
LinearAlgebra<T>::SquareLinearSolver::~SquareLinearSolver(void) {
  if (p_.k_a) {
    delete[] p_.k_a;
    p_.k_a = nullptr;
  }
  if (p_.k_b) {
    delete[] p_.k_b;
    p_.k_b = nullptr;
  }
  if (p_.k_ipiv) {
    delete[] p_.k_ipiv;
    p_.k_ipiv = nullptr;
  }
}

/*
 *  @name Solve
 *  @fn void Solve(const cv::Mat& A, const cv::Mat& b, cv::Mat* x)
 *  @brief  Solve AX = B
 *  @param[in]  A  Matrix A
 *  @param[in]  b  Matrix B, stored in column
 *  @param[out] x  Solution
 */
template<typename T>
void LinearAlgebra<T>::SquareLinearSolver::Solve(const cv::Mat& A,
                                                 const cv::Mat& b,
                                                 cv::Mat* x) {
  assert(A.rows == A.cols);
  // Init
  if (p_.k_n != A.rows ||
      p_.k_lda != A.rows ||
      p_.k_nrhs != b.cols) {
    // Release ressources if necessary
    if (p_.k_a) { delete[] p_.k_a; }
    if (p_.k_b) { delete[] p_.k_b; }
    if (p_.k_ipiv) { delete[] p_.k_ipiv; }
    // Define dimensions
    p_.k_n = A.rows; //Column-major system
    p_.k_lda = A.rows;
    p_.k_nrhs = b.cols;
    p_.k_ldb = p_.k_n;
    // Init arrays
    p_.k_a = new T[p_.k_n * p_.k_n];
    p_.k_b = new T[p_.k_ldb * p_.k_nrhs];
    p_.k_ipiv = new int[p_.k_n];
  }
  
  //Convert to column major memory layout for A
  cv::Mat tmp_a = cv::Mat(A.cols,
                          A.rows,
                          cv::DataType<T>::type,
                          p_.k_a );
  tmp_a = A.t();
  //Convert to column major memory layout for B
  cv::Mat tmp_b = cv::Mat(b.cols,
                          b.rows,
                          cv::DataType<T>::type,
                          p_.k_b);
  tmp_b = b.t();
  //Done, Solve the problem
  p_.k_info = 0;
  Lapack::SquareLinearSolverCall(p_);
  // Retrieve results
  if (p_.k_info == 0) {
    cv::Mat buff(p_.k_nrhs, p_.k_ldb, cv::DataType<T>::type, p_.k_b);
    *x = buff.t();
  } else {
    FACEKIT_LOG_INFO("Solver can not find a solution to the provided system");
    *x = cv::Mat();
  }
}
  
  
#pragma mark -
#pragma mark Explicit Instantiation
  
/** Float - LinearAlgebra */
template class LinearAlgebra<float>;
/** Double - LinearAlgebra */
template class LinearAlgebra<double>;
  
}  // namespace FaceKit
