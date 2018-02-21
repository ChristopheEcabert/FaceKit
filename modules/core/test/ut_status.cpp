/**
 *  @file   ut_status.cpp
 *  @brief Unit test for Status/Error class
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   21.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "gtest/gtest.h"

#include "facekit/core/status.hpp"



TEST(Status, DefaultCtor) {
  namespace FK = FaceKit;
  FK::Status s;
  EXPECT_EQ(s.Code(), FK::Status::Type::kOk);
  EXPECT_EQ(s.get_message(), "");
}

TEST(Status, SpecificCtor) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s(Type::kInternalError, "Internal Error");
  EXPECT_EQ(s.Code(), Type::kInternalError);
  EXPECT_EQ(s.get_message(), "Internal Error");
}

TEST(Status, CopyCtor) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s1(Type::kUnknown, "Unknown");
  FK::Status s2(s1);
  EXPECT_EQ(s1.Code(), s2.Code());
  EXPECT_EQ(s1.get_message(), s2.get_message());
}

TEST(Status, Assignment) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s1(Type::kInvalidArgument, "Invalid Arg");
  FK::Status s2;
  s2 = s1;
  EXPECT_EQ(s1.Code(), s2.Code());
  EXPECT_EQ(s1.get_message(), s2.get_message());
}

TEST(Status, Equals) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s1(Type::kInvalidArgument, "Invalid Arg");
  FK::Status s2(Type::kInvalidArgument, "Invalid Arg");
  ASSERT_EQ(s1, s2);
}

TEST(Status, EqualsCopy) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s1(Type::kNotFound, "Not Found");
  FK::Status s2(s1);
  ASSERT_EQ(s1, s2);
}

TEST(Status, EqualsAssignment) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s1(Type::kAlreadyExists, "Exists");
  FK::Status s2 = s1;
  ASSERT_EQ(s1, s2);
}

TEST(Status, DiffCode) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s1(Type::kOutOfRange, "Out");
  FK::Status s2(Type::kInvalidArgument, "Out");
  ASSERT_NE(s1, s2);
}

TEST(Status, DiffMessage) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;
  FK::Status s1(Type::kOutOfRange, "Out");
  FK::Status s2(Type::kOutOfRange, "Out range");
  ASSERT_NE(s1, s2);
}

TEST(Status, ToString) {
  namespace FK = FaceKit;
  using Type = FK::Status::Type;

  FK::Status s1;
  EXPECT_EQ(s1.ToString(), "Ok");
  FK::Status s2(Type::kUnknown, "message");
  EXPECT_EQ(s2.ToString(), "Unknown: message");
  FK::Status s3(Type::kInvalidArgument, "message");
  EXPECT_EQ(s3.ToString(), "Invalid argument: message");
  FK::Status s4(Type::kNotFound, "message");
  EXPECT_EQ(s4.ToString(), "Not found: message");
  FK::Status s5(Type::kAlreadyExists, "message");
  EXPECT_EQ(s5.ToString(), "Already exists: message");
  FK::Status s6(Type::kOutOfRange, "message");
  EXPECT_EQ(s6.ToString(), "Out of range: message");
  FK::Status s7(Type::kUnimplemented, "message");
  EXPECT_EQ(s7.ToString(), "Unimplemented: message");
  FK::Status s8(Type::kInternalError, "message");
  EXPECT_EQ(s8.ToString(), "Internal error: message");
  FK::Status s9((Type)-10, "message");
  EXPECT_EQ(s9.ToString(), "Unknown code (-10): message");
}


int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
