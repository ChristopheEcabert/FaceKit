/**
 *  @file   ut_nd_array_map.cpp
 *  @brief Unit test for NDArrayMap
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   18.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <numeric>
#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "facekit/core/nd_array_map.hpp"

template<typename T>
class NDArrayMapTest : public ::testing::Test {
 public:
};

// List all types to test + register test
typedef ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double> TypeToTest;
TYPED_TEST_CASE(NDArrayMapTest, TypeToTest);

/** CTOR + properties */
TYPED_TEST(NDArrayMapTest, CTor) {
  namespace FK = FaceKit;
  typedef TypeParam T;
  // Simulate raw buffer + Map it
  std::vector<T> buff(48, 0);
  std::iota(buff.begin(), buff.end(), 1);
  
  // Scalar
  {
    FK::NDArrayMap<const T, 0> map(buff.data());
    EXPECT_EQ(map.rank(), 0);
    EXPECT_EQ(map.size(), 1);
  }
  // Vector
  {
    FK::NDArrayMap<const T, 1> map(buff.data(), 48);
    EXPECT_EQ(map.rank(), 1);
    EXPECT_EQ(map.size(), 48);
    EXPECT_EQ(map.dim_size(0), 48);
  }
  // Matrix
  {
    FK::NDArrayMap<const T, 2> map(buff.data(), 3, 16);
    EXPECT_EQ(map.rank(), 2);
    EXPECT_EQ(map.size(), 48);
    EXPECT_EQ(map.dim_size(0), 3);
    EXPECT_EQ(map.dim_size(1), 16);
  }
  // NDArray - 3
  {
    FK::NDArrayMap<const T, 3> map(buff.data(), 2, 8, 3);
    EXPECT_EQ(map.rank(), 3);
    EXPECT_EQ(map.size(), 48);
    EXPECT_EQ(map.dim_size(0), 2);
    EXPECT_EQ(map.dim_size(1), 8);
    EXPECT_EQ(map.dim_size(2), 3);
  }
  // NDArray - 4
  {
    FK::NDArrayMap<const T, 4> map(buff.data(), 2, 2, 4, 3);
    EXPECT_EQ(map.rank(), 4);
    EXPECT_EQ(map.size(), 48);
    EXPECT_EQ(map.dim_size(0), 2);
    EXPECT_EQ(map.dim_size(1), 2);
    EXPECT_EQ(map.dim_size(2), 4);
    EXPECT_EQ(map.dim_size(3), 3);
  }
}

/** Getter */
TYPED_TEST(NDArrayMapTest, Getter) {
  namespace FK = FaceKit;
  typedef TypeParam T;
  // Simulate raw buffer + Map it
  std::vector<T> buff(48, 0);
  std::iota(buff.begin(), buff.end(), 1);
  
  // Scalar
  {
    FK::NDArrayMap<const T, 0> map(buff.data());
    auto& v0 = map();
    EXPECT_EQ(v0, buff[0]);
  }
  // Vector
  {
    FK::NDArrayMap<const T, 1> map(buff.data(), 48);
    auto& v0 = map(16);
    EXPECT_EQ(v0, buff[16]);
    auto& v1 = map(36);
    EXPECT_EQ(v1, buff[36]);
  }
  // Matrix
  {
    FK::NDArrayMap<const T, 2> map(buff.data(), 3, 16);
    auto& v0 = map(0, 15);
    EXPECT_EQ(v0, buff[15]);
    auto& v1 = map(2, 8);
    EXPECT_EQ(v1, buff[40]);
  }
  // NDArray - 3
  {
    FK::NDArrayMap<const T, 3> map(buff.data(), 2, 8, 3);
    auto& v0 = map(0, 7, 1);
    EXPECT_EQ(v0, buff[22]);
    auto& v1 = map(1, 3, 2);
    EXPECT_EQ(v1, buff[35]);
    auto& v2 = map(1, 7, 2);
    EXPECT_EQ(v2, buff[47]);
  }
  // NDArray - 4
  {
    FK::NDArrayMap<const T, 4> map(buff.data(), 2, 2, 4, 3);
    auto& v0 = map(0, 0, 3, 1);
    EXPECT_EQ(v0, buff[10]);
    auto& v1 = map(1, 1, 0, 2);
    EXPECT_EQ(v1, buff[38]);
    auto& v2 = map(1, 1, 3, 2);
    EXPECT_EQ(v2, buff[47]);
  }
}

/** Setter */
TYPED_TEST(NDArrayMapTest, Setter) {
  namespace FK = FaceKit;
  typedef TypeParam T;
  // Simulate raw buffer + Map it
  std::vector<T> buff(48, 0);
  std::iota(buff.begin(), buff.end(), 1);
  
  // Scalar
  {
    FK::NDArrayMap<T, 0> map(buff.data());
    map() = T(10);
    EXPECT_EQ(buff[0], T(10));
  }
  // Vector
  {
    FK::NDArrayMap<T, 1> map(buff.data(), 48);
    map(16) = T(32);
    EXPECT_EQ(buff[16], T(32));
    map(36) = T(3);
    EXPECT_EQ(buff[36], T(3));
  }
  // Matrix
  {
    FK::NDArrayMap<T, 2> map(buff.data(), 3, 16);
    map(0, 15) = T(4);
    EXPECT_EQ(buff[15], T(4));
    map(2, 8) = T(32);
    EXPECT_EQ(buff[40], T(32));
  }
  // NDArray - 3
  {
    FK::NDArrayMap<T, 3> map(buff.data(), 2, 8, 3);
    map(0, 7, 1) = T(45);
    EXPECT_EQ(buff[22], T(45));
    map(1, 3, 2) = T(3);
    EXPECT_EQ(buff[35], T(3));
    map(1, 7, 2) = T(58);
    EXPECT_EQ(buff[47], T(58));
  }
  // NDArray - 4
  {
    FK::NDArrayMap<T, 4> map(buff.data(), 2, 2, 4, 3);
    map(0, 0, 3, 1) = T(68);
    EXPECT_EQ(buff[10], T(68));
    map(1, 1, 0, 2) = T(14);
    EXPECT_EQ(buff[38], T(14));
    map(1, 1, 3, 2) = T(27);
    EXPECT_EQ(buff[47], T(27));
  }
}


int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
