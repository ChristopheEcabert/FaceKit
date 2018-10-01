/**
 *  @file   time_generator.cpp
 *  @brief Randomly generate particle's life time in a given range
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <random>

#include "facekit/sim/time_generator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialisation
  
/*
 *  @name   TimeGenerator
 *  @fn     TimeGenerator(const Time& min, const Time& delta)
 *  @brief  Constructor
 *  @param[in] min  Minimum velocity
 *  @param[in] delta Range of velocity variation allowed for this generator
 */
template<typename T>
TimeGenerator<T>::TimeGenerator(const Time& min,
                                const Time& delta) : min_(min),
                                                     delta_(delta) {}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Generate
 *  @fn     void Generate(const T& dt, const size_t start_id,
 const size_t& end_id, Particles<T>* particles) override
 *  @brief  Generate particle's lifetime in a given range
 *  @param[in] dt             Delta time
 *  @param[in] start_id       Index where to start the generation
 *  @param[in] end_id         Index where to stop the generation
 *  @param[in,out]  particles Container where generated particles will be
 *                            placed
 */
template<typename T>
void TimeGenerator<T>::Generate(const T& dt,
                                const size_t start_id,
                                const size_t& end_id,
                                Particles<T>* particles) {
  // Create distribution
  std::uniform_real_distribution<T> dist;
  std::mt19937 gen;
  // Generate particles
  auto& time = particles->get_time();
  for (size_t i = start_id; i < end_id; ++i) {
    time[i] = min_ + dist(gen) * delta_;
  }
}
  
#pragma mark -
#pragma mark Explicit Instantiation
  
/** Float */
template class TimeGenerator<float>;
/** Double */
template class TimeGenerator<double>;
  
}  // namespace FaceKit
