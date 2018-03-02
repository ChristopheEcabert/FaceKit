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
#include <vector>

#include "gtest/gtest.h"

#include "nd_array.pb.h"

#include "facekit/core/nd_array.hpp"
#include "facekit/core/mem/map_allocator.hpp"
#include "facekit/core/utils/proto.hpp"

#pragma mark -
#pragma mark Type definition

template<typename T>
class NDArrayTest : public ::testing::Test {
public:
};

// List all types to test + register test
typedef ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, bool, std::string> TypeToTest;
TYPED_TEST_CASE(NDArrayTest, TypeToTest);

#pragma mark -
#pragma mark Utility function

/**
 *  @name   ConvertValue
 *  @brief  Convert number to proper type
 */
template<typename T>
T ConvertValue(const size_t& v);

template<typename T>
std::vector<T> ConvertValues(const std::initializer_list<size_t>& v);

/**
 *  @name   CheckIota
 *  @brief  Check if tensor is filled with iota [0, ..., N]
 */
template<typename T>
void CheckIota(const FaceKit::NDArray& array);

/**
 *  @enum CompType
 *  @brief  Type of comparision
 */
enum CompType {
  kGreaterThan,
  kLessThan,
  kEqual
};

/**
 *  @class  ArrayComparator
 *  @brief  Compare content of NDArrays
 *  @author Christophe Ecabert
 *  @date   28.02.18
 *  @ingroup core
 */
template<typename T, CompType COMP>
struct ArrayComparator {
  /**
   *  @name   Compare
   *  @brief  Compare two arrays
   *  @param[in] a  First array
   *  @param[in] b  Second array to compare against
   */
  static void Compare(const FaceKit::NDArray& a, const FaceKit::NDArray& b);
};

template<typename T>
struct ArrayComparator<T, CompType::kGreaterThan> {
  static void Compare(const FaceKit::NDArray& a, const FaceKit::NDArray& b) {
    // Check type
    EXPECT_EQ(a.type(), FaceKit::DataTypeToEnum<T>::v());
    EXPECT_EQ(a.type(), b.type());
    // dims
    EXPECT_EQ(a.dims(), b.dims());
    size_t N = a.dims();
    const auto a_map = a.AsFlat<T>();
    const auto b_map = b.AsFlat<T>();
    // Compare elements
    for (size_t k = 0; k < N; ++k) {
      EXPECT_GT(a_map(k), b_map(k));
    }
  }
};
template<typename T>
struct ArrayComparator<T, CompType::kLessThan> {
  static void Compare(const FaceKit::NDArray& a, const FaceKit::NDArray& b) {
    // Check type
    EXPECT_EQ(a.type(), FaceKit::DataTypeToEnum<T>::v());
    EXPECT_EQ(a.type(), b.type());
    // dims
    EXPECT_EQ(a.dims(), b.dims());
    size_t N = a.dims();
    const auto a_map = a.AsFlat<T>();
    const auto b_map = b.AsFlat<T>();
    // Compare elements
    for (size_t k = 0; k < N; ++k) {
      EXPECT_LT(a_map(k), b_map(k));
    }
  }
};

template<typename T>
struct ArrayComparator<T, CompType::kEqual> {
  static void Compare(const FaceKit::NDArray& a, const FaceKit::NDArray& b) {
    // Check type
    EXPECT_EQ(a.type(), FaceKit::DataTypeToEnum<T>::v());
    EXPECT_EQ(a.type(), b.type());
    // dims
    EXPECT_EQ(a.dims(), b.dims());
    size_t N = a.dims();
    auto a_map = a.AsFlat<T>();
    auto b_map = b.AsFlat<T>();
    // Compare elements
    for (size_t k = 0; k < N; ++k) {
      EXPECT_EQ(a_map(k), b_map(k));
    }
  }
};



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

TYPED_TEST(NDArrayTest, ConstructWithValues) {
  namespace FK = FaceKit;
  using T = TypeParam;
  
  { // Create scalar
    T value = ConvertValue<T>(15);
    FK::NDArray array = FK::NDArray::WithScalar(value);
    auto map = array.AsScalar<T>();
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map.rank(), 0);
    EXPECT_EQ(map(), value);
  }
  { // Vector
    auto values = ConvertValues<T>({10, 5, 3, 9, 42, 0});
    FK::NDArray array = FK::NDArray::WithValues(values);
    auto map = array.AsVector<T>();
    EXPECT_EQ(map.size(), 6);
    EXPECT_EQ(map.rank(), 1);
    EXPECT_EQ(map.dim_size(0), 6);
    for (size_t k = 0; k < map.size(); ++k) {
      EXPECT_EQ(map(k), values[k]);
    }
  }
  { // NDArry
    auto values = ConvertValues<T>({1, 9, 100, 42, 16, 68});
    FK::NDArray array = FK::NDArray::WithValues(values, {3, 2});
    auto map = array.AsMatrix<T>();
    EXPECT_EQ(map.size(), 6);
    EXPECT_EQ(map.rank(), 2);
    EXPECT_EQ(map.dim_size(0), 3);
    EXPECT_EQ(map.dim_size(1), 2);
    for (size_t r = 0; r < 3; ++r) {
      for (size_t c = 0; c < 2; ++c) {
        EXPECT_EQ(map(r, c), values[r * 2 + c]);
      }
    }
  }
}

TYPED_TEST(NDArrayTest, FillIota) {
  namespace FK = FaceKit;
  using T = TypeParam;

  // Create matrix
  auto values = ConvertValues<T>({0, 1, 2, 3, 4, 5});
  auto array = FK::NDArray::WithValues(values, {2, 3});
  // Check filled value
  CheckIota<T>(array);
}

TYPED_TEST(NDArrayTest, Assignment) {
  namespace FK = FaceKit;
  using T = TypeParam;
  
  auto values = ConvertValues<T>({1, 10, 42, 64, 38, 96});
  { // Copy assignment
    std::cout << "[   INFO   ] copy assignment" << std::endl;
    auto a = FK::NDArray::WithValues(values, {3, 2});
    auto b = a;
    ArrayComparator<T, CompType::kEqual>::Compare(a, b);
  }
  
  { // Deep copy
    std::cout << "[   INFO   ] Deep copy" << std::endl;
    auto a = FK::NDArray::WithValues(values, {3, 2});
    FK::NDArray b;
    a.DeepCopy(&b);
    ArrayComparator<T, CompType::kEqual>::Compare(a, b);
  }
  
  { // move assignment
    std::cout << "[   INFO   ] Move assignment" << std::endl;
    auto a = FK::NDArray::WithValues(values, {3, 2});
    FK::NDArray b;
    a.DeepCopy(&b);
    auto c = std::move(b);
    ArrayComparator<T, CompType::kEqual>::Compare(a, c);
  }
}


template<typename T>
void MapAllocatorTestCase(void) {
  namespace FK = FaceKit;
  // Values
  auto values = ConvertValues<T>({1, 10, 42, 64, 38, 96});
  { // Map array to this buffer
    FK::MapAllocator map_allocator(reinterpret_cast<void*>(values.data()));
    FK::NDArray a(&map_allocator);
    a.Resize(FK::DataTypeToEnum<T>::v(), {3, 2});
    auto map = a.AsMatrix<T>();
    for (size_t r = 0; r < 3; ++r) {
      for (size_t c = 0; c < 2; ++c) {
        EXPECT_EQ(map(r, c), values[r * 2 + c]);
      }
    }
  }
  { // Copy mapped array
    FK::MapAllocator map_allocator(reinterpret_cast<void*>(values.data()));
    FK::NDArray a(&map_allocator);
    a.Resize(FK::DataTypeToEnum<T>::v(), {3, 2});
    
    FK::NDArray b;
    a.DeepCopy(&b);
    ArrayComparator<T, CompType::kEqual>::Compare(a, b);
  }
}
template<>
void MapAllocatorTestCase<bool>(void) {
  // no-op
  EXPECT_TRUE(true);
}

TYPED_TEST(NDArrayTest, MapAllocator) {
  // Work around to cancel <bool> test case since vector<bool> does not have
  // ::data() method since it is implemented as a bitfield.
  MapAllocatorTestCase<TypeParam>();
}

TYPED_TEST(NDArrayTest, ShareBuffer) {
  namespace FK = FaceKit;
  using T = TypeParam;
  
  FK::NDArray a;
  FK::NDArray b;
  FK::NDArray a1(FK::DataTypeToEnum<T>::v(), {1});
  FK::NDArray b1(FK::DataTypeToEnum<T>::v(), {1});
  FK::NDArray c1 = a1;
  FK::NDArray a2;
  b1.DeepCopy(&a2);
  
  
  // Check shared buffer
  EXPECT_FALSE(a.ShareBuffer(a));
  EXPECT_FALSE(b.ShareBuffer(b));
  EXPECT_TRUE(a1.ShareBuffer(a1));
  EXPECT_TRUE(b1.ShareBuffer(b1));
  EXPECT_TRUE(a1.ShareBuffer(c1));
  EXPECT_FALSE(b1.ShareBuffer(a2));
}

TYPED_TEST(NDArrayTest, Slice) {
  namespace FK = FaceKit;
  using T = TypeParam;
  
  // Create array
  auto values = ConvertValues<T>({1, 35, 192, 37, 256, 1024});
  auto array = FK::NDArray::WithValues(values, {3, 2});
  
  // Create slice
  FK::NDArray slice = array.Slice(1, 2);
  EXPECT_EQ(slice.dims(), 2);
  EXPECT_EQ(slice.dim_size(0), 1);
  EXPECT_EQ(slice.dim_size(1), 2);
  // Check content
  auto map = slice.AsMatrix<T>();
  EXPECT_EQ(map(0, 0), values[2]);
  EXPECT_EQ(map(0, 1), values[3]);
}

TEST(NDArrayTest, ProtoUtils) {
  namespace FK = FaceKit;
  
  { // Samller than 2^7
    std::string buffer;
    // Encode
    FK::AddVarInt32(42, &buffer);
    EXPECT_EQ(buffer.size(), 1);
    // Decode
    uint32_t value;
    EXPECT_TRUE(FK::RetrieveVarInt32(&buffer, &value));
    EXPECT_EQ(value, 42);
    EXPECT_EQ(buffer.size(), 0);
  }
  { // Samller than 2^14
    std::string buffer;
    // Encode
    uint32_t v = (0x01 << 12);
    FK::AddVarInt32(v, &buffer);
    EXPECT_EQ(buffer.size(), 2);
    // Decode
    uint32_t value;
    EXPECT_TRUE(FK::RetrieveVarInt32(&buffer, &value));
    EXPECT_EQ(value, v);
    EXPECT_EQ(buffer.size(), 0);
  }
  { // Samller than 2^21
    std::string buffer;
    // Encode
    uint32_t v = (0x01 << 17);
    FK::AddVarInt32(v, &buffer);
    EXPECT_EQ(buffer.size(), 3);
    // Decode
    uint32_t value;
    EXPECT_TRUE(FK::RetrieveVarInt32(&buffer, &value));
    EXPECT_EQ(value, v);
    EXPECT_EQ(buffer.size(), 0);
  }
  { // Samller than 2^28
    std::string buffer;
    // Encode
    uint32_t v = (0x01 << 24);
    FK::AddVarInt32(v, &buffer);
    EXPECT_EQ(buffer.size(), 4);
    // Decode
    uint32_t value;
    EXPECT_TRUE(FK::RetrieveVarInt32(&buffer, &value));
    EXPECT_EQ(value, v);
    EXPECT_EQ(buffer.size(), 0);
  }
  { // Samller than 2^32
    std::string buffer;
    // Encode
    uint32_t v = (0x01 << 31);
    FK::AddVarInt32(v, &buffer);
    EXPECT_EQ(buffer.size(), 5);
    // Decode
    uint32_t value;
    EXPECT_TRUE(FK::RetrieveVarInt32(&buffer, &value));
    EXPECT_EQ(value, v);
    EXPECT_EQ(buffer.size(), 0);
  }
  { // Add multiple value
    std::string buffer;
    // Encode
    uint32_t v1 = (0x01 << 5);
    uint32_t v2 = (0x01 << 13);
    FK::AddVarInt32(v1, &buffer);
    FK::AddVarInt32(v2, &buffer);
    EXPECT_EQ(buffer.size(), 3);
    // Decode
    uint32_t value1;
    uint32_t value2;
    EXPECT_TRUE(FK::RetrieveVarInt32(&buffer, &value1));
    EXPECT_EQ(value1, v1);
    EXPECT_EQ(buffer.size(), 2);
    EXPECT_TRUE(FK::RetrieveVarInt32(&buffer, &value2));
    EXPECT_EQ(value2, v2);
    EXPECT_EQ(buffer.size(), 0);
  }
  { // Encode/Decode array of string
    std::string buffer;
    std::vector<std::string> array = {"Hello", "World", "Lausanne", "42"};
    FK::EncodeStringList(array.data(), array.size(), &buffer);
    EXPECT_GT(buffer.size(), 0);
    
    // Decode
    std::vector<std::string> decoded_array(array.size());
    EXPECT_TRUE(FK::DecodeStringList(buffer,
                                     array.size(),
                                     decoded_array.data()));
    EXPECT_EQ(decoded_array[0], "Hello");
    EXPECT_EQ(decoded_array[1], "World");
    EXPECT_EQ(decoded_array[2], "Lausanne");
    EXPECT_EQ(decoded_array[3], "42");
  }
}

TYPED_TEST(NDArrayTest, Proto) {
  namespace FK = FaceKit;
  using T = TypeParam;
  
  // Create array
  auto values = ConvertValues<T>({1, 35, 192, 37, 256, 1024});
  auto array = FK::NDArray::WithValues(values, {3, 2});
  
  // Convert array to proto
  FK::NDArrayProto proto;
  array.ToProto(&proto);
  
  // Convert back to array
  FK::NDArray p_array;
  EXPECT_TRUE(p_array.FromProto(proto).Good());
  EXPECT_EQ(p_array.type(), FK::DataTypeToEnum<T>::v());
  EXPECT_EQ(p_array.dims(), 2);
  EXPECT_EQ(p_array.dim_size(0), 3);
  EXPECT_EQ(p_array.dim_size(1), 2);
  ArrayComparator<T, CompType::kEqual>::Compare(array, p_array);
}

int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}

/*
 *  @name   ConvertValue
 *  @brief  Convert number to proper type
 */
template<typename T>
T ConvertValue(const size_t& v) {
  return T(v);
}
template<>
std::string ConvertValue<std::string>(const size_t& v) {
  return "str_" + std::to_string(v);
}

template<typename T>
std::vector<T> ConvertValues(const std::initializer_list<size_t>& v) {
  std::vector<T> values(v.size());
  auto v_it = values.begin();
  for (auto it = v.begin(); it != v.end(); ++it, ++v_it) {
    *v_it = T(*it);
  }
  return values;
}
template<>
std::vector<std::string> ConvertValues<std::string>(const std::initializer_list<size_t>& v) {
  std::vector<std::string> values(v.size());
  auto v_it = values.begin();
  for (auto it = v.begin(); it != v.end(); ++it, ++v_it) {
    *v_it = "str_" + std::to_string(*it);
  }
  return values;
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
