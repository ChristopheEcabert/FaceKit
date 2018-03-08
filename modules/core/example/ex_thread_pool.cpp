/**
 *  @file   thread_pool_ex.cpp
 *  @brief  Thread pool sample
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   06.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <iostream>
#include <vector>

#include "facekit/core/thread_pool.hpp"

int main(const int argc, const char** argv) {
  namespace FK = FaceKit;
  using TaskPriority = FK::ThreadPool::TaskPriority;

  // Define worker
  auto worker = [](const int& i) -> int {
    return i * i;
  };

  // Get thread pool
  FK::ThreadPool& pool = FK::ThreadPool::Get();
  // Stacks of results
  std::vector<std::future<int>> tasks;
  // Launch job
  for (int i = 0; i < 4; ++i) {
    tasks.push_back(pool.Enqueue(TaskPriority::kNormal, worker, i));
  }
  std::cout << "There is: " << tasks.size() << " tasks started" << std::endl;

  // Wait till all jobs are done
  size_t i = 0;
  for (auto& f : tasks) {
    f.wait();
    std::cout << "Output for i=" << i++ << " f(i)=" << f.get() << std::endl;
  }
}
