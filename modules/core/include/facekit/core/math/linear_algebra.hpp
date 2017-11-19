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
  
#pragma mark -
#pragma mark BLAS
  
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
    
  /**
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
  static void Sbmv(const cv::Mat& A,
                   const T alpha,
                   const cv::Mat& x,
                   const T beta, cv::Mat* y);
    
#pragma mark -
#pragma mark LAPACK
  
  /**
   *  @class  Lapack
   *  @brief  Subspace for lapack call
   */
  class Lapack {
   public:
    
    /**
     *  @struct lin_solver_params
     *  @brief  Structure including all parameters for linear solver Ax = B
     */
    struct lin_solver_params {
      /** Indicate if matrix A is transposed */
      char k_trans;
      /** Number of rows in matrix A */
      int k_m;
      /** Number of cols in matrix A */
      int k_n;
      /** Number of columns of the matrices B and X */
      int k_nrhs;
      /** Matrix A data */
      T* k_a;
      /** Leading direction of A */
      int k_lda;
      /** Matrix B data */
      T* k_b;
      /** Leading direction of B */
      int k_ldb;
      /** Workspace */
      T* k_work;
      /** Workspace size */
      int k_lwork;
      /** Info */
      int k_info;
    };
    
    /**
     *  @name   LinearSolverCall
     *  @fn inline void LinearSolverCall(lin_solver_params& p)
     *  @brief  Interface for lapack function call - Undefined type
     *  @throw  std::runtime_error()
     */
    static void LinearSolverCall(lin_solver_params& p);
    
    /**
     * @struct  square_lin_solver_params
     * @brief   Structure including all parameters for linear solver Ax = B.
     *          A matrix must be square
     */
    struct square_lin_solver_params {
      /** Number of linear equation */
      int k_n;
      /** Number of right hand side, i.e. number of column in matrix B */
      int k_nrhs;
      /** Matrix A [N x N] */
      T* k_a;
      /** Leading direction in A */
      int k_lda;
      /** List of pivot indexes (i.e. permutation) [N x 1] */
      int* k_ipiv;
      /** Right hand side - Matrix B */
      T* k_b;
      /** Leading direction in B */
      int k_ldb;
      /** Info - output */
      int k_info;
    };
    
    /**
     *  @name   SquareLinearSolverCall
     *  @fn inline void SquareLinearSolverCall(square_lin_solver_params& p)
     *  @brief  Interface for lapack function call - Undefined type
     *  @throw  std::runtime_error()
     */
    static void SquareLinearSolverCall(square_lin_solver_params& p);
  };
    
#pragma mark Linear Solver
    
  /**
   *  @class LinearSolver
   *  @brief  Linear solver class
   */
  class LinearSolver {
   public:
    
    /**
     *  @name   LinearSolver
     *  @fn LinearSolver(void)
     *  @brief  Constructor
     */
    LinearSolver(void);
    
    /**
     *  @name   ~LinearSolver
     *  @fn ~LinearSolver(void)
     *  @brief  Destructor
     */
    ~LinearSolver(void);
    
    /**
     *  @name Solve
     *  @fn void Solve(const cv::Mat& A, const cv::Mat& b, cv::Mat* x)
     *  @brief  Solve Ax = B
     *  @param[in]  A  Matrix A
     *  @param[in]  b  Matrix B, stored in column
     *  @param[out] x  Solution
     */
    void Solve(const cv::Mat& A,
               const cv::Mat& b,
               cv::Mat* x);
    
   private:
    /** Solver parameters */
    typename Lapack::lin_solver_params p_;
  };
    
  /**
   *  @class SquareLinearSolver
   *  @brief  Linear solver class for square system. Solve Ax = B using LU
   *          decomposition on A
   */
  class SquareLinearSolver {
   public:
    
    /**
     *  @name   SquareLinearSolver
     *  @fn SquareLinearSolver(void)
     *  @brief  Constructor
     */
    SquareLinearSolver(void);
    
    /**
     *  @name   ~SquareLinearSolver
     *  @fn ~SquareLinearSolver(void)
     *  @brief  Destructor
     */
    ~SquareLinearSolver(void);
    
    /**
     *  @name Solve
     *  @fn void Solve(const cv::Mat& matrix_a,
     const cv::Mat& matrix_b,
     cv::Mat* matrix_x)
     *  @brief  Solve Ax = B
     *  @param[in]  A  Matrix A
     *  @param[in]  b  Matrix B, stored in column
     *  @param[out] x  Solution
     */
    void Solve(const cv::Mat& A,
               const cv::Mat& b,
               cv::Mat* x);
   private:
    /** Solver configuration */
    typename Lapack::square_lin_solver_params p_;
  };
};
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_LINEAR_ALGEBRA__ */
