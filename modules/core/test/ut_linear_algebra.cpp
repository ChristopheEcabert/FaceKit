/**
 *  @file   ut_linear_algebra.cpp
 *  @brief Unit test for LinearAlgebra class
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   19.08.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <random>
#include <chrono>

#include "gtest/gtest.h"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"

#include "facekit/core/math/linear_algebra.hpp"

template<typename T>
class LinearAlgebraUnitTest : public testing::Test {
 public:
  /**
   *  @name LinearAlgebraUnitTest
   *  @brief  Contructor
   */
  LinearAlgebraUnitTest(void) = default;
};

// List of type to test against
typedef ::testing::Types<float, double> TypeToTest;
// Provide list of tested type to basic test fixture
TYPED_TEST_CASE(LinearAlgebraUnitTest, TypeToTest);

#pragma mark -
#pragma mark L2 Norm

/** L2 Norm */
TYPED_TEST(LinearAlgebraUnitTest, L2Norm) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  // Define input to vector
  cv::Mat v(57, 1, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(v, TypeParam(0.0), TypeParam(1.0));
  
  // Compute ground truth
  TypeParam gt_nrm = TypeParam(0.0);
  for (int i = 0; i < v.rows; ++i) {
    gt_nrm += v.at<TypeParam>(i) * v.at<TypeParam>(i);
  }
  gt_nrm = std::sqrt(gt_nrm);
  // Call cblas
  TypeParam nrm = LA::L2Norm(v);
  // Compare
  TypeParam diff = (gt_nrm - nrm) / TypeParam(57.0);
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

#pragma mark -
#pragma mark Mean

/** Mean - Row */
TYPED_TEST(LinearAlgebraUnitTest, MeanRow) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  // Define input to vector
  cv::Mat m(57, 13, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(m, TypeParam(0.0), TypeParam(1.0));
  
  // Compute ground truth
  cv::Mat gt_mean(1, m.cols, cv::DataType<TypeParam>::type);
  for (int c = 0; c < gt_mean.cols; ++c) {
    TypeParam sum = 0;
    for (int r = 0; r < m.rows; ++r) {
      sum += m.at<TypeParam>(r, c);
    }
    gt_mean.at<TypeParam>(c) = sum / static_cast<TypeParam>(m.rows);
  }
  // Call cblas
  cv::Mat mean;
  LA::Mean(m, 0, &mean);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_mean, mean) / gt_mean.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

/** Mean - Col */
TYPED_TEST(LinearAlgebraUnitTest, MeanCol) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  // Define input to vector
  cv::Mat m(57, 13, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(m, TypeParam(0.0), TypeParam(1.0));
  
  // Compute ground truth
  cv::Mat gt_mean(m.rows, 1, cv::DataType<TypeParam>::type);
  for (int r = 0; r < gt_mean.rows; ++r) {
    TypeParam sum = 0;
    for (int c = 0; c < m.cols; ++c) {
      sum += m.at<TypeParam>(r, c);
    }
    gt_mean.at<TypeParam>(r) = sum / static_cast<TypeParam>(m.cols);
  }
  // Call cblas
  cv::Mat mean;
  LA::Mean(m, 1, &mean);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_mean, mean) / gt_mean.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

#pragma mark -
#pragma mark Axpy

/** Axpy */
TYPED_TEST(LinearAlgebraUnitTest, Axpy) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  // Define input to vector
  cv::Mat A(57, 13, cv::DataType<TypeParam>::type);
  cv::Mat B(57, 13, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(B, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_B = (A * alpha) + B;
  // Call cblas
  LA::Axpy(A, alpha, &B);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_B, B) / gt_B.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

#pragma mark -
#pragma mark Axpby

/** Axpy */
TYPED_TEST(LinearAlgebraUnitTest, Axpby) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  // Define input to vector
  cv::Mat A(57, 13, cv::DataType<TypeParam>::type);
  cv::Mat B(57, 13, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(B, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  TypeParam beta = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_B = (A * alpha) + (B * beta);
  // Call cblas
  LA::Axpby(A, alpha, beta, &B);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_B, B) / gt_B.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

#pragma mark -
#pragma mark Dot

/** Dot product */
TYPED_TEST(LinearAlgebraUnitTest, Dot1Stride) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  // Define input to vector
  cv::Mat a(57, 1, cv::DataType<TypeParam>::type);
  cv::Mat b(57, 1, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(a, TypeParam(0.0), TypeParam(1.0));
  cv::randn(b, TypeParam(0.0), TypeParam(1.0));
  // Compute ground truth
  TypeParam gt_dot = TypeParam(0.0);
  for (int i = 0; i < a.rows; ++i) {
    gt_dot += a.at<TypeParam>(i) * b.at<TypeParam>(i);
  }
  // Call cblas
  TypeParam dot = LA::Dot(a, b);
  // Compare
  TypeParam diff = (gt_dot - dot) / TypeParam(57.0);
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

/** Dot product with stride */
TYPED_TEST(LinearAlgebraUnitTest, Dot7Stride) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  // Define input to vector
  cv::Mat a(57, 1, cv::DataType<TypeParam>::type);
  cv::Mat b(57, 1, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(a, TypeParam(0.0), TypeParam(1.0));
  cv::randn(b, TypeParam(0.0), TypeParam(1.0));
  // Compute ground truth
  TypeParam gt_dot = TypeParam(0.0);
  for (int i = 0; i < a.rows; i += 7) {
    gt_dot += a.at<TypeParam>(i) * b.at<TypeParam>(i);
  }
  // Call cblas
  TypeParam dot = LA::Dot(a, 7, b, 7);
  // Compare
  TypeParam diff = (gt_dot - dot) / TypeParam(8.0);
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

#pragma mark -
#pragma mark Gemv

/** Gemv */
TYPED_TEST(LinearAlgebraUnitTest, GemvNoTrans) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  using TType = typename FaceKit::LinearAlgebra<TypeParam>::TransposeType;
  // Define input to vector
  cv::Mat A(57, 13, cv::DataType<TypeParam>::type);
  cv::Mat x(13, 1, cv::DataType<TypeParam>::type);
  cv::Mat y(57, 1, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(x, TypeParam(0.0), TypeParam(1.0));
  cv::randn(y, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  TypeParam beta = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_y = (alpha * (A * x)) + (beta * y);
  // Call cblas
  LA::Gemv(A, TType::kNoTranspose, alpha, x, beta, &y);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_y, y) / gt_y.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

/** Gemv */
TYPED_TEST(LinearAlgebraUnitTest, GemvTrans) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  using TType = typename FaceKit::LinearAlgebra<TypeParam>::TransposeType;
  // Define input to vector
  cv::Mat A(57, 13, cv::DataType<TypeParam>::type);
  cv::Mat x(57, 1, cv::DataType<TypeParam>::type);
  cv::Mat y(13, 1, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(x, TypeParam(0.0), TypeParam(1.0));
  cv::randn(y, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  TypeParam beta = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_y = (alpha * (A.t() * x)) + (beta * y);
  // Call cblas
  LA::Gemv(A, TType::kTranspose, alpha, x, beta, &y);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_y, y) / gt_y.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

#pragma mark -
#pragma mark Gemm

/** Gemm */
TYPED_TEST(LinearAlgebraUnitTest, Gemm_AB) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  using TType = typename FaceKit::LinearAlgebra<TypeParam>::TransposeType;
  // Define input to vector
  cv::Mat A(57, 33, cv::DataType<TypeParam>::type);
  cv::Mat B(33, 17, cv::DataType<TypeParam>::type);
  cv::Mat C(57, 17, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(B, TypeParam(0.0), TypeParam(1.0));
  cv::randn(C, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  TypeParam beta = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_C = (alpha * (A * B)) + (beta * C);
  // Call cblas
  LA::Gemm(A, TType::kNoTranspose, alpha, B, TType::kNoTranspose, beta, &C);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_C, C) / gt_C.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

/** Gemm */
TYPED_TEST(LinearAlgebraUnitTest, Gemm_AtB) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  using TType = typename FaceKit::LinearAlgebra<TypeParam>::TransposeType;
  // Define input to vector
  cv::Mat A(57, 33, cv::DataType<TypeParam>::type);
  cv::Mat B(57, 17, cv::DataType<TypeParam>::type);
  cv::Mat C(33, 17, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(B, TypeParam(0.0), TypeParam(1.0));
  cv::randn(C, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  TypeParam beta = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_C = (alpha * (A.t() * B)) + (beta * C);
  // Call cblas
  LA::Gemm(A, TType::kTranspose, alpha, B, TType::kNoTranspose, beta, &C);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_C, C) / gt_C.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

/** Gemm */
TYPED_TEST(LinearAlgebraUnitTest, Gemm_ABt) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  using TType = typename FaceKit::LinearAlgebra<TypeParam>::TransposeType;
  // Define input to vector
  cv::Mat A(57, 33, cv::DataType<TypeParam>::type);
  cv::Mat B(17, 33, cv::DataType<TypeParam>::type);
  cv::Mat C(57, 17, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(B, TypeParam(0.0), TypeParam(1.0));
  cv::randn(C, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  TypeParam beta = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_C = (alpha * (A * B.t())) + (beta * C);
  // Call cblas
  LA::Gemm(A, TType::kNoTranspose, alpha, B, TType::kTranspose, beta, &C);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_C, C) / gt_C.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}

/** Gemm */
TYPED_TEST(LinearAlgebraUnitTest, Gemm_AtBt) {
  using LA = FaceKit::LinearAlgebra<TypeParam>;
  using TType = typename FaceKit::LinearAlgebra<TypeParam>::TransposeType;
  // Define input to vector
  cv::Mat A(57, 33, cv::DataType<TypeParam>::type);
  cv::Mat B(17, 57, cv::DataType<TypeParam>::type);
  cv::Mat C(33, 17, cv::DataType<TypeParam>::type);
  cv::theRNG().state = static_cast<uint64_t>(cv::getTickCount());
  cv::randn(A, TypeParam(0.0), TypeParam(1.0));
  cv::randn(B, TypeParam(0.0), TypeParam(1.0));
  cv::randn(C, TypeParam(0.0), TypeParam(1.0));
  // Define random alpha
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::normal_distribution<TypeParam> dist(TypeParam(0.0), TypeParam(1.0));
  TypeParam alpha = dist(gen);
  TypeParam beta = dist(gen);
  
  // Compute ground truth
  cv::Mat gt_C = (alpha * (A.t() * B.t())) + (beta * C);
  // Call cblas
  LA::Gemm(A, TType::kTranspose, alpha, B, TType::kTranspose, beta, &C);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(gt_C, C) / gt_C.total();
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-8;
  EXPECT_LT(diff, thr);
}


/** Sbmv */
TYPED_TEST(LinearAlgebraUnitTest, Sbmv) {
  // Input
  cv::Mat A(17, 1, cv::DataType<TypeParam>::type);
  cv::Mat x(17, 1, cv::DataType<TypeParam>::type);
  cv::randn(A, TypeParam(0.0), TypeParam(5.0));
  cv::randn(x, TypeParam(0.0), TypeParam(5.0));
  // Output
  cv::Mat ytrue, y;
  ytrue = A.mul(x);
  
  // Compute element-wise product
  FaceKit::LinearAlgebra<TypeParam>::Sbmv(A,
                                          TypeParam(1.0),
                                          x,
                                          TypeParam(0.0),
                                          &y);
  // Compare
  TypeParam diff = (TypeParam)cv::norm(ytrue - y) / TypeParam(x.total());
  TypeParam thr = sizeof(TypeParam) == 4 ? 1e-6 : 1e-14;
  EXPECT_LE(diff, thr);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
