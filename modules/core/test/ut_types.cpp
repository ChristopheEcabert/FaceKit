/**
 *  @file   ut_types.cpp
 *  @brief Unit test for Data types
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   13.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "gtest/gtest.h"

#include "facekit/core/types.hpp"
#include "facekit/core/logger.hpp"



TEST(DataTypeToString, Types) {
  namespace FK = FaceKit;
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kUnknown), "unknown");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kInt8), "int8");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kUInt8), "uint8");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kInt16), "int16");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kUInt16), "uint16");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kInt32), "int32");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kUInt32), "uint32");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kFloat), "float");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kDouble), "double");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kSize_t), "size_t");
  EXPECT_EQ(FK::DataTypeToString(FK::DataType::kBool), "bool");
  EXPECT_EQ(FK::DataTypeToString((FK::DataType)32), "unsupported");
}

TEST(DataTypeFromString, Types) {
  namespace FK = FaceKit;
  
  FK::DataType type;
  EXPECT_TRUE(FK::DataTypeFromString("int8", &type));
  EXPECT_EQ(type, FK::DataType::kInt8);
  EXPECT_TRUE(FK::DataTypeFromString("uint8", &type));
  EXPECT_EQ(type, FK::DataType::kUInt8);
  EXPECT_TRUE(FK::DataTypeFromString("int16", &type));
  EXPECT_EQ(type, FK::DataType::kInt16);
  EXPECT_TRUE(FK::DataTypeFromString("uint16", &type));
  EXPECT_EQ(type, FK::DataType::kUInt16);
  EXPECT_TRUE(FK::DataTypeFromString("int32", &type));
  EXPECT_EQ(type, FK::DataType::kInt32);
  EXPECT_TRUE(FK::DataTypeFromString("uint32", &type));
  EXPECT_EQ(type, FK::DataType::kUInt32);
  EXPECT_TRUE(FK::DataTypeFromString("float", &type));
  EXPECT_EQ(type, FK::DataType::kFloat);
  EXPECT_TRUE(FK::DataTypeFromString("double", &type));
  EXPECT_EQ(type, FK::DataType::kDouble);
  EXPECT_TRUE(FK::DataTypeFromString("size_t", &type));
  EXPECT_EQ(type, FK::DataType::kSize_t);
  EXPECT_TRUE(FK::DataTypeFromString("bool", &type));
  EXPECT_EQ(type, FK::DataType::kBool);
  // Wrong entry
  type = FK::DataType::kUnknown;
  EXPECT_FALSE(FK::DataTypeFromString("char", &type));
  EXPECT_EQ(type, FK::DataType::kUnknown);
}

TEST(IsDataTypeValid, Types) {
  namespace FK = FaceKit;
  // Valid
  EXPECT_TRUE((bool)FK::IsDataTypeValid<int8_t>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<uint8_t>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<int16_t>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<uint16_t>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<int32_t>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<uint32_t>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<float>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<double>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<size_t>::value);
  EXPECT_TRUE((bool)FK::IsDataTypeValid<bool>::value);
  // Invalid
  EXPECT_FALSE((bool)FK::IsDataTypeValid<long>::value);
  EXPECT_FALSE((bool)FK::IsDataTypeValid<long long>::value);
}

TEST(DataTypeToEnum, Types) {
  namespace FK = FaceKit;
  using DType = FaceKit::DataType;
  // Valid
  EXPECT_EQ((DType)FK::DataTypeToEnum<int8_t>::value, DType::kInt8);
  EXPECT_EQ((DType)FK::DataTypeToEnum<uint8_t>::value, DType::kUInt8);
  EXPECT_EQ((DType)FK::DataTypeToEnum<int16_t>::value, DType::kInt16);
  EXPECT_EQ((DType)FK::DataTypeToEnum<uint16_t>::value, DType::kUInt16);
  EXPECT_EQ((DType)FK::DataTypeToEnum<int32_t>::value, DType::kInt32);
  EXPECT_EQ((DType)FK::DataTypeToEnum<uint32_t>::value, DType::kUInt32);
  EXPECT_EQ((DType)FK::DataTypeToEnum<float>::value, DType::kFloat);
  EXPECT_EQ((DType)FK::DataTypeToEnum<double>::value, DType::kDouble);
  EXPECT_EQ((DType)FK::DataTypeToEnum<size_t>::value, DType::kSize_t);
  EXPECT_EQ((DType)FK::DataTypeToEnum<bool>::value, DType::kBool);
}

TEST(DataTypeSize, Types) {
  namespace FK = FaceKit;
  using DType = FaceKit::DataType;
  // Valid
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kInt8>::value, sizeof(int8_t));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kUInt8>::value, sizeof(uint8_t));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kInt16>::value, sizeof(int16_t));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kUInt16>::value, sizeof(uint16_t));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kInt32>::value, sizeof(int32_t));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kUInt32>::value, sizeof(uint32_t));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kFloat>::value, sizeof(float));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kDouble>::value, sizeof(double));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kSize_t>::value, sizeof(size_t));
  EXPECT_EQ((size_t)FK::DataTypeSize<DType::kBool>::value, sizeof(bool));
  // Invalid
  EXPECT_EQ((size_t)FK::DataTypeSize<(DType)20>::value, 0);
  EXPECT_EQ((size_t)FK::DataTypeSize<(DType)42>::value, 0);
}


int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Disable logger
  FaceKit::Logger::Instance().Disable();
  // Run unit test
  return RUN_ALL_TESTS();
}
