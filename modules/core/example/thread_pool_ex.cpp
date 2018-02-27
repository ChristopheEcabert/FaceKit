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
  auto worker = [](const int& i) -> void {
    std::cout << i << " Thread ID " << std::this_thread::get_id() << std::endl;
  };

  // Get thread pool
  FK::ThreadPool& pool = FK::ThreadPool::Get();
  // Stacks of results
  std::vector<std::future<void>> tasks;
  // Launch job
  for (int i = 0; i < 4; ++i) {
    auto res = pool.Enqueue(TaskPriority::kNormal, worker, i);
    tasks.push_back(std::move(res));
  }
  
  // Wait till all jobs are done
  for (auto& f : tasks) {
    f.wait();
  }
  
  

  
  
  
}
