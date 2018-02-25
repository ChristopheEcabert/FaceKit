/**
 *  @file   ut_types.cpp
 *  @brief Unit test for NDArray
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   23.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "gtest/gtest.h"

#include "facekit/core/nd_array.hpp"


TEST(NDArray, CTor) {
  namespace FK = FaceKit;
  FK::NDArray array;
  int a = 0;
}

int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
