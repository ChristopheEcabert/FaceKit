/**
 *  @file   ut_allocator.cpp
 *  @brief Unit test for allocator
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   13.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <vector>

#include "gtest/gtest.h"

#include "facekit/core/mem/allocator.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @name   CheckStatistics
 *  @brief  Check allocator's statistics
 *  @param[in] a  Allocator
 *  @param[in] n_alloc  Number of allocation done
 *  @param[in] n_bytes  Number of bytes alloated
 *  @param[in] max_bytes  Largest bytes usage
 *  @param[in] max_size   Largest allocation at once
 */
static void CheckStatistics(FaceKit::Allocator* a,
                            const size_t& n_alloc,
                            const size_t& n_bytes,
                            const size_t& max_bytes,
                            const size_t& max_size) {
  namespace FK = FaceKit;
  // Get statistis
  FK::AllocatorStatistic stats;
  a->GatherStatistics(&stats);
  // Check
  EXPECT_EQ(stats.n_alloc, n_alloc);
  EXPECT_EQ(stats.used_bytes, n_bytes);
  EXPECT_EQ(stats.max_used_bytes, max_bytes);
  EXPECT_EQ(stats.max_alloc_size, max_size);
}


TEST(Allocator, CpuAllocatorRaw) {
  namespace FK = FaceKit;
  // Enable statistics
  FK::EnableAllocatorStatistics(true);
  auto* a = FK::DefaultCpuAllocator();
  // Do a bunch of allocation
  std::vector<void*> ptrs;
  for (size_t i = 1; i < 512; ++i) {
    void* p = a->AllocateRaw(i, 32);  // Use default alignment
    ptrs.push_back(p);
  }
  // Check statistics
  CheckStatistics(a, 511, 130816, 130816, 511);
  // Release
  for (size_t i = 0; i < ptrs.size(); ++i) {
    a->DeallocateRaw(i + 1, ptrs[i]);
  }
  // Check statistics
  CheckStatistics(a, 511, 0, 130816, 511);
  // Check clear as well
  a->ClearStatistics();
  CheckStatistics(a, 0, 0, 0, 0);
  // Disable statistics
  FK::EnableAllocatorStatistics(false);
}

TEST(Allocator, CpuAllocatorTyped) {
  namespace FK = FaceKit;
  // Enable statistics
  FK::EnableAllocatorStatistics(true);
  auto* a = FK::DefaultCpuAllocator();
  // Do a bunch of typed allocation
  int16_t* p1 = a->Allocate<int16_t>(2048);
  size_t* p2 = a->Allocate<size_t>(65536);
  // Check statistics
  CheckStatistics(a,
                  2,
                  2048 * sizeof(int16_t) + 65536 * sizeof(size_t),
                  2048 * sizeof(int16_t) + 65536 * sizeof(size_t),
                  65536 * sizeof(size_t));
  // Deallocate
  a->Deallocate<int16_t>(2048, p1);
  a->Deallocate<size_t>(65536, p2);
  // Check statistics
  CheckStatistics(a,
                  2,
                  0,
                  2048 * sizeof(int16_t) + 65536 * sizeof(size_t),
                  65536 * sizeof(size_t));
  // Done
  a->ClearStatistics();
  FK::EnableAllocatorStatistics(false);
}

TEST(Allocator, AllocateString) {
  namespace FK = FaceKit;
  // Enable statistics
  FK::EnableAllocatorStatistics(true);
  auto* a = FK::DefaultCpuAllocator();
  // Do a bunch of typed allocation
  std::string* p1 = a->Allocate<std::string>(10);
  std::string* p2 = a->Allocate<std::string>(5);
  // Check statistics
  CheckStatistics(a,
                  2,
                  15 * sizeof(std::string),
                  15 * sizeof(std::string),
                  10 * sizeof(std::string));
  // Deallocate
  a->Deallocate<std::string>(10, p1);
  a->Deallocate<std::string>(5, p2);
  // Check statistics
  CheckStatistics(a,
                  2,
                  0,
                  15 * sizeof(std::string),
                  10 * sizeof(std::string));
  // Done
  a->ClearStatistics();
  FK::EnableAllocatorStatistics(false);
}

TEST(Allocator, CpuAllocatorMaxChunk) {
  namespace FK = FaceKit;
  // Enable statistics
  auto* a = FK::DefaultCpuAllocator();
  
  // Allocate large chunk of memory, which will lead to error
  const size_t size = std::numeric_limits<size_t>::max();
  double* ptr = a->Allocate<double>(size);
  EXPECT_EQ(ptr, nullptr);
}


int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Disable logger
  FaceKit::Logger::Instance().Disable();
  // Run unit test
  return RUN_ALL_TESTS();
}
