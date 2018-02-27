/**
 *  @file   ut_types.cpp
 *  @brief Unit test for NDArray
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   23.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <string>

#include "gtest/gtest.h"

#include "facekit/core/nd_array.hpp"

#pragma mark -
#pragma mark Type definition

template<typename T>
class NDArrayTest : public ::testing::Test {
public:
};

// List all types to test + register test
typedef ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, float, double, size_t, bool, std::string> TypeToTest;
TYPED_TEST_CASE(NDArrayTest, TypeToTest);

#pragma mark -
#pragma mark Utility function

/**
 *  @name   FillIota
 *  @brief  Fill an initaliazed tensor with value from [0, ..., N]
 */
template<typename T>
void FillIota(FaceKit::NDArray* array);

/**
 *  @name   CheckIota
 *  @brief  Check if tensor is filled with iota [0, ..., N]
 */
template<typename T>
void CheckIota(const FaceKit::NDArray& array);


#pragma mark -
#pragma mark Test Cases

// Test various ctor
TYPED_TEST(NDArrayTest, CTor) {
  namespace FK = FaceKit;
  using T = TypeParam;
  
  // Check for leaks
  FK::EnableAllocatorStatistics(true);
  
  { // Default
    FK::NDArray array;
    EXPECT_EQ(array.dims(), 0);
    EXPECT_EQ(array.n_elems(), 1);
    EXPECT_FALSE(array.IsInitialized());
  }
  { // With allocator
    FK::NDArray array(FK::DefaultCpuAllocator());
    EXPECT_EQ(array.dims(), 0);
    EXPECT_EQ(array.n_elems(), 1);
    EXPECT_FALSE(array.IsInitialized());
  }
  { // With Data type + dimensions
    FK::DataType dt = FK::DataTypeToEnum<T>::value;
    FK::NDArray array(dt, {10, 5});
    EXPECT_TRUE(array.IsInitialized());
    EXPECT_EQ(array.dims(), 2);
    EXPECT_EQ(array.n_elems(), 50);
    EXPECT_EQ(array.dim_size(0), 10);
    EXPECT_EQ(array.dim_size(1), 5);
  }
  { // With Data type + dimensions + allocator
    FK::DataType dt = FK::DataTypeToEnum<T>::value;
    FK::NDArray array(dt, {2, 15, 3, 1}, FK::DefaultCpuAllocator());
    EXPECT_TRUE(array.IsInitialized());
    EXPECT_EQ(array.dims(), 4);
    EXPECT_EQ(array.n_elems(), 90);
    EXPECT_EQ(array.dim_size(0), 2);
    EXPECT_EQ(array.dim_size(1), 15);
    EXPECT_EQ(array.dim_size(2), 3);
    EXPECT_EQ(array.dim_size(3), 1);
  }
  { // Copy constructor
    FK::DataType dt = FK::DataTypeToEnum<T>::value;
    FK::NDArray array(dt, {4, 20, 3, 3});
    FK::NDArray copy_array(array);
    EXPECT_TRUE(array.IsInitialized());
    EXPECT_TRUE(copy_array.IsInitialized());
    EXPECT_EQ(copy_array.dims(), 4);
    EXPECT_EQ(copy_array.n_elems(), 720);
    EXPECT_EQ(copy_array.dim_size(0), 4);
    EXPECT_EQ(copy_array.dim_size(1), 20);
    EXPECT_EQ(copy_array.dim_size(2), 3);
    EXPECT_EQ(copy_array.dim_size(3), 3);
  }
  { // Move constructor
    FK::DataType dt = FK::DataTypeToEnum<T>::value;
    FK::NDArray array(dt, {4, 10, 1, 3});
    FK::NDArray copy_array(std::move(array));
    EXPECT_FALSE(array.IsInitialized());
    EXPECT_TRUE(copy_array.IsInitialized());
    EXPECT_EQ(copy_array.dims(), 4);
    EXPECT_EQ(copy_array.n_elems(), 120);
    EXPECT_EQ(copy_array.dim_size(0), 4);
    EXPECT_EQ(copy_array.dim_size(1), 10);
    EXPECT_EQ(copy_array.dim_size(2), 1);
    EXPECT_EQ(copy_array.dim_size(3), 3);
  }
  { // Assignment operator
    FK::DataType dt = FK::DataTypeToEnum<T>::value;
    FK::NDArray array(dt, {4, 10, 1});
    FK::NDArray array2 = array;
    EXPECT_TRUE(array2.IsInitialized());
    EXPECT_EQ(array2.dims(), 3);
    EXPECT_EQ(array2.n_elems(), 40);
    EXPECT_EQ(array2.dim_size(0), 4);
    EXPECT_EQ(array2.dim_size(1), 10);
    EXPECT_EQ(array2.dim_size(2), 1);
  }
  { // Assignment operator
    FK::DataType dt = FK::DataTypeToEnum<T>::value;
    FK::NDArray array(dt, {4, 10, 1});
    FK::NDArray array2 = std::move(array);
    EXPECT_TRUE(array2.IsInitialized());
    EXPECT_FALSE(array.IsInitialized());
    EXPECT_EQ(array2.dims(), 3);
    EXPECT_EQ(array2.n_elems(), 40);
    EXPECT_EQ(array2.dim_size(0), 4);
    EXPECT_EQ(array2.dim_size(1), 10);
    EXPECT_EQ(array2.dim_size(2), 1);
  }
  
  // Check allocator statistics
  FK::AllocatorStatistic stats;
  auto* a = FK::DefaultCpuAllocator();
  a->GatherStatistics(&stats);
  EXPECT_EQ(stats.used_bytes, 0);
  FK::EnableAllocatorStatistics(false);
}

TYPED_TEST(NDArrayTest, FillIota) {
  namespace FK = FaceKit;
  using T = TypeParam;

  // Create matrix
  FK::DataType dt = FK::DataTypeToEnum<T>::value;
  FK::NDArray array(dt, {2, 3});
  // Fill
  FillIota<T>(&array);
  // Check filled value
  CheckIota<T>(array);
}

int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}

/*
 *  @name   FillIota
 *  @brief  Fill an initaliazed tensor with value from [0, ..., N]
 */
template<typename T>
void FillIota(FaceKit::NDArray* array) {
  EXPECT_GT(array->dims(), 0);
  auto map = array->AsFlat<T>();
  for (size_t k = 0; k < map.size(); ++k) {
    map(k) = T(k);
  }
}

template<>
void FillIota<std::string>(FaceKit::NDArray* array) {
  EXPECT_GT(array->dims(), 0);
  auto map = array->AsFlat<std::string>();
  for (size_t k = 0; k < map.size(); ++k) {
    map(k) = "str_" + std::to_string(k);
  }
}

/*
 *  @name   CheckIota
 *  @brief  Check if tensor is filled with iota [0, ..., N]
 */
template<typename T>
void CheckIota(const FaceKit::NDArray& array) {
  auto map = array.AsFlat<T>();
  for (size_t k = 0; k < map.size(); ++k) {
    EXPECT_EQ(map(k), T(k));
  }
}
template<>
void CheckIota<std::string>(const FaceKit::NDArray& array) {
  auto map = array.AsFlat<std::string>();
  for (size_t k = 0; k < map.size(); ++k) {
    EXPECT_EQ(map(k), "str_" + std::to_string(k));
  }
}
