/**
 *  @file   thread_pool.hpp
 *  @brief Lightweight thread pool implementation using C++11
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   17.12.17
 *    Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_THREAD_POOL__
#define __FACEKIT_THREAD_POOL__

#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <future>
#include <queue>
#include <deque>
#include <memory>
#include <utility>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/**
 *  @class  ThreaedPool
 *  @brief  Lightweight thread pool
 *  @author Christophe Ecabert
 *  @date   17.12.17
 *  @ingroup core
 */
class FK_EXPORTS ThreadPool {
 public:

#pragma mark -
#pragma mark Type Definition

  /**
   *  @enum   TaskPriority
   *  @brief  Possible task's priority
   */
  enum class TaskPriority : char {
    /** Low priority */
    kLow = 0,
    /** Normal Priority */
    kNormal,
    /** High Priority */
    kHigh
  };

#pragma mark -
#pragma mark Initialization

  /**
   *  @name   Get
   *  @fn     static ThreadPool& Get(const std::size_t& size = 4)
   *  @brief  Singleton accessor
   */
  static ThreadPool& Get(const std::size_t& size = 4);

  /**
   *  @name   ThreadPool
   *  @fn     ThreadPool(const ThreadPool& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  ThreadPool(const ThreadPool& other) = delete;

  /**
   *  @name   ThreadPool
   *  @fn     ThreadPool(ThreadPool&& other) = delete
   *  @brief  Move constructor
   *  @param[in] other  Object to move from
   */
  ThreadPool(ThreadPool&& other) = delete;

  /**
   *  @name   operator=
   *  @fn     ThreadPool& operator=(const ThreadPool& rhs) = delete;
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  ThreadPool& operator=(const ThreadPool& rhs) = delete;

  /**
   *  @name   operator=
   *  @fn     ThreadPool& operator=(ThreadPool&& rhs) = delete;
   *  @brief  Move Assignment operator
   *  @param[in]  rhs Object to move-assign from
   *  @return Newly moved-assign object
   */
  ThreadPool& operator=(ThreadPool&& rhs) = delete;

  /**
   *  @name   ~ThreadPool
   *  @fn     ~ThreadPool(void) = default
   *  @brief  Destructor
   */
  ~ThreadPool(void);

#pragma mark -
#pragma mark Usage

  /**
   *  @name
   *  @brief  Add job to the processing queue
   *  @param[in] priority Task's priority
   *  @param[in] f        Function to call
   *  @param[in] args     Function's argument
   *  @return std::future object holding the return value of the task's
   *          function
   */
  template<typename F, typename... Args>
  auto Enqueue(const TaskPriority& priority, F&& f, Args&&... args)
  -> std::future<typename std::result_of<F(Args...)>::type>;

#pragma mark -
#pragma mark Private
 private:

  /** Task type */
  using Task = std::pair<TaskPriority, std::function<void()>>;

  /**
   *  @struct  TaskComparator
   *  @brief  Functor sorting task by priority
   *  @author Christophe Ecabert
   *  @date   17.12.17
   *  @ingroup core
   */
  struct TaskComparator {

    /**
     *  @name   operator()
     *  @fn     bool operator()(const TaskPriority& lhs,
     const TaskPriority& rhs) const
     *  @brief  Compare two task prioty
     */
    bool operator()(const Task& lhs, const Task& rhs) const {
      return lhs.first > rhs.first;
    }
  };

  /** Priority queue type */
  using PriorityQueue = std::priority_queue<Task,
                                            std::deque<Task>,
                                            ThreadPool::TaskComparator>;

  /**
   *  @name   ThreadPool
   *  @fn     ThreadPool(const std::size_t& size)
   *  @brief  Constructor
   *  @param[in]  size  Pool size
   */
  ThreadPool(const std::size_t& size);

  /** Workers */
  std::vector<std::thread> workers_;
  /** Task queue */
  PriorityQueue queue_;
  /** Synchronization - Enqueue task */
  std::mutex queue_lock_;
  /** Synchronization - Conditional signal */
  std::mutex cond_lock_;
  /** Conditional variable */
  std::condition_variable cond_;
  /** Stop flag */
  bool stop_;
};

#pragma mark -
#pragma mark Implementation

/*
 *  @name   Get
 *  @fn     static ThreadPool& Get(const std::size_t& size = 4)
 *  @brief  Singleton accessor
 */
ThreadPool& ThreadPool::Get(const std::size_t& size /*= 4*/) {
  static ThreadPool pool(size);
  return pool;
}

/*
 *  @name   ThreadPool
 *  @fn     ThreadPool(const std::size_t& size)
 *  @brief  Constructor
 *  @param[in]  size  Pool size
 */
ThreadPool::ThreadPool(const std::size_t& size) : stop_(false) {
  // Initialize workers
  for (std::size_t i = 0; i < size; ++i) {
    workers_.emplace_back([this](){
      // Loop forever
      while(true) {
        // Get task
        std::function<void()> task;
        // Wait till some tasks are pending or if we stop
        {
          std::unique_lock<std::mutex> lock(this->cond_lock_);
          this->cond_.wait(lock, [this](void) {
            return this->stop_ || !this->queue_.empty();
          });
          // Stopping ?
          if (this->stop_ && this->queue_.empty()) {
            return;
          }
          // Queue not empty, pick pending job
          task = std::move(this->queue_.top().second);
          this->queue_.pop();
        }
        // Execute task
        task();
      }
    });
  }
}

/*
 *  @name
 *  @brief  Add job to the processing queue
 *  @param[in] priority Task's priority
 *  @param[in] f        Function to call
 *  @param[in] args     Function's argument
 *  @return std::future object holding the return value of the task's
 *          function
 */
template<typename F, typename... Args>
auto ThreadPool::Enqueue(const TaskPriority& priority, F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type> {
  // Fcn return type
  using rtype = typename std::result_of<F(Args...)>::type;
  // Create task
  auto task = std::make_shared<std::packaged_task<rtype()>>(std::bind(std::forward<F>(f),
                                                                      std::forward<Args>(args)...));
  // Define return object
  auto res = task->get_future();
  // Add to queue
  {
    std::unique_lock<std::mutex> lock(this->queue_lock_);
    if (this->stop_) {
      throw std::runtime_error("Error, try to add task on stopped pool");
    }
    this->queue_.emplace(priority, [task](void){ (*task)();});
  }
  // Signal new job
  this->cond_.notify_one();
  // Return future object
  return res;
}

/*
 *  @name   ~ThreadPool
 *  @fn     ~ThreadPool(void)
 *  @brief  Destructor
 */
ThreadPool::~ThreadPool(void) {
  // Stop queue
  {
    std::unique_lock<std::mutex> lock(this->queue_lock_);
    this->stop_ = true;
  }
  // Signal all waiting thread to run
  this->cond_.notify_all();
  // Wait till all threads are done
  for (std::thread& thr : this->workers_) {
    thr.join();
  }
}

}  // namespace FaceKit
#endif /* __FACEKIT_THREAD_POOL__ */
