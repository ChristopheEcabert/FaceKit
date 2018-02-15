/**
 *  @file   ut_array_dims.cpp
 *  @brief Unit test for ArrayDims
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   15.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "gtest/gtest.h"
#include "google/protobuf/text_format.h"

#include "array_dims.pb.h"

#include "facekit/core/array_dims.hpp"

TEST(ArrayDims, CTor) {
  namespace FK = FaceKit;
  // Default
  FK::ArrayDims d;
  EXPECT_EQ(d.dims(), 0);
  EXPECT_EQ(d.n_elems(), 1);
  // Assignment
  FK::ArrayDims d1({4, 20});
  d = d1;
  EXPECT_EQ(d.dims(), 2);
  EXPECT_EQ(d.n_elems(), 80);
  // Copy
  FK::ArrayDims d2(d1);
  EXPECT_EQ(d2.dims(), 2);
  EXPECT_EQ(d2.n_elems(), 80);
  // Move assignment
  FK::ArrayDims d3;
  d3 = std::move(d);
  EXPECT_EQ(d3.dims(), 2);
  EXPECT_EQ(d3.n_elems(), 80);
  // Move constructor
  FK::ArrayDims d4(std::move(d2));
  EXPECT_EQ(d3.dims(), 2);
  EXPECT_EQ(d3.n_elems(), 80);
}

TEST(ArrayDims, AddDimension) {
  namespace FK = FaceKit;
  FK::ArrayDims d;
  
  // Add dim - 1
  d.AddDim(100);
  EXPECT_EQ(d.dims(), 1);
  EXPECT_EQ(d.dim_size(0), 100);
  EXPECT_EQ(d.n_elems(), 100);
  // Add dim - 2
  d.AddDim(25);
  EXPECT_EQ(d.dims(), 2);
  EXPECT_EQ(d.dim_size(1), 25);
  EXPECT_EQ(d.n_elems(), 2500);
  // Add dim - 3
  d.AddDim(4);
  EXPECT_EQ(d.dims(), 3);
  EXPECT_EQ(d.dim_size(2), 4);
  EXPECT_EQ(d.n_elems(), 10000);
  // Add dim - 4
  d.AddDim(2);
  EXPECT_EQ(d.dims(), 4);
  EXPECT_EQ(d.dim_size(3), 2);
  EXPECT_EQ(d.n_elems(), 20000);
}

TEST(ArrayDims, RemoveDimension) {
  namespace FK = FaceKit;
  FK::ArrayDims d({5, 20, 30});
  
  // Remove 2nd dimension -> {5, 30}
  d.RemoveDim(1);
  EXPECT_EQ(d.dims(), 2);
  EXPECT_EQ(d.dim_size(0), 5);
  EXPECT_EQ(d.dim_size(1), 30);
  EXPECT_EQ(d.n_elems(), 150);
  // Remove 1st dimension -> {30}
  d.RemoveDim(0);
  EXPECT_EQ(d.dims(), 1);
  EXPECT_EQ(d.dim_size(0), 30);
  EXPECT_EQ(d.n_elems(), 30);
}

TEST(ArrayDims, AddRemoveDimension) {
  namespace FK = FaceKit;
  FK::ArrayDims d({5, 20});
  
  // Add dimension -> {5, 20, 30}
  d.AddDim(30);
  EXPECT_EQ(d.dims(), 3);
  EXPECT_EQ(d.dim_size(0), 5);
  EXPECT_EQ(d.dim_size(1), 20);
  EXPECT_EQ(d.dim_size(2), 30);
  EXPECT_EQ(d.n_elems(), 3000);
  // Remove 1st dimension -> {20, 30}
  d.RemoveDim(0);
  EXPECT_EQ(d.dims(), 2);
  EXPECT_EQ(d.dim_size(0), 20);
  EXPECT_EQ(d.dim_size(1), 30);
  EXPECT_EQ(d.n_elems(), 600);
}

TEST(ArrayDims, SetDimension) {
  namespace FK = FaceKit;
  FK::ArrayDims d({25, 4});
  // Change first dimension
  d.set_dim(0, 100);
  EXPECT_EQ(d.dims(), 2);
  EXPECT_EQ(d.dim_size(0), 100);
  EXPECT_EQ(d.n_elems(), 400);
  // Set to zero
  d.set_dim(1, 0);
  EXPECT_EQ(d.dims(), 2);
  EXPECT_EQ(d.dim_size(1), 0);
  EXPECT_EQ(d.n_elems(), 0);
}

TEST(ArrayDims, Proto) {
  namespace FK = FaceKit;
  namespace pb = google::protobuf;
  
  
  // Create protobuf message from string
  FK::ArrayDimsProto proto;
  std::string in = "dims {\n size: 20\n}\n dims{\n size: 250\n}\n";
  EXPECT_TRUE(pb::TextFormat::ParseFromString(in, &proto));
  // Create ArrayDims obj
  FK::ArrayDims d(proto);
  EXPECT_EQ(d.dims(), 2);
  EXPECT_EQ(d.dim_size(0), 20);
  EXPECT_EQ(d.dim_size(1), 250);
  EXPECT_EQ(d.n_elems(), 5000);
  
  
  // Conversion to protobuf message
  FK::ArrayDims d1({10, 5, 15});
  d1.ToProto(&proto);
  EXPECT_EQ(proto.dims_size(), 3);
  EXPECT_EQ(proto.dims(0).size(), 10);
  EXPECT_EQ(proto.dims(1).size(), 5);
  EXPECT_EQ(proto.dims(2).size(), 15);
  
  // Conversion from protobuf message
  d1.Clear();
  d1.FromProto(proto);
  EXPECT_EQ(d1.dims(), 3);
  EXPECT_EQ(d1.dim_size(0), 10);
  EXPECT_EQ(d1.dim_size(1), 5);
  EXPECT_EQ(d1.dim_size(2), 15);
  EXPECT_EQ(d1.n_elems(), 750);
}


int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
