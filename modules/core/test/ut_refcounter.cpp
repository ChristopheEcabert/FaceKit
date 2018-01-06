/**
 *  @file   ut_refcounter.cpp
 *  @brief Unit test for reference counter
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   06.01.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "gtest/gtest.h"

#include "facekit/core/refcounter.hpp"

// Global counter
static int construction = 0;
static int destruction = 0;

// Reference counter class
class Ref : public FaceKit::RefCounter {
 public:
  
  Ref(void) {
    construction++;
  }
  
  ~Ref(void) override {
    destruction++;
  }
};

// Test case
class RefCounterTest : public ::testing::Test {
 public:
  
  RefCounterTest(void) {
    construction = 0;
    destruction = 0;
  }
};

TEST_F(RefCounterTest, Init) {
  Ref* ref = new Ref();
  EXPECT_EQ(construction, 1);
  EXPECT_EQ(destruction, 0);
  ref->Dec();
  EXPECT_EQ(construction, 1);
  EXPECT_EQ(destruction, 1);
}

TEST_F(RefCounterTest, IncDec) {
  Ref* ref = new Ref();
  EXPECT_EQ(construction, 1);
  EXPECT_EQ(destruction, 0);
  ref->Inc();
  EXPECT_EQ(destruction, 0);
  ref->Dec();
  EXPECT_EQ(destruction, 0);
  ref->Dec();
  EXPECT_EQ(destruction, 1);
}

TEST_F(RefCounterTest, IsOne) {
  Ref* ref = new Ref();
  EXPECT_TRUE(ref->IsOne());
  ref->Dec();
}

TEST_F(RefCounterTest, IsNotOne) {
  Ref* ref = new Ref();
  ref->Inc();
  EXPECT_FALSE(ref->IsOne());
  ref->Dec();
  ref->Dec();
}

TEST_F(RefCounterTest, RetValueDec) {
  Ref* ref = new Ref();
  ref->Inc();
  EXPECT_FALSE(ref->Dec());
  EXPECT_TRUE(ref->Dec());
}

int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
