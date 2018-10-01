/**
 *  @file   velocity_generator.cpp
 *  @brief Randomly generate particle's velocity vector in a given range
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <random>


#include "facekit/sim/velocity_generator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialisation
  
/*
 *  @name   VelocityGenerator
 *  @fn     VelocityGenerator(const Pos& orgin, const Pos& side)
 *  @brief  Constructor
 *  @param[in] min  Minimum velocity
 *  @param[in] delta Range of velocity variation allowed for this generator
 */
template<typename T>
VelocityGenerator<T>::VelocityGenerator(const Vel& min,
                                        const Vel& delta) : min_(min),
                                                            delta_(delta) {}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Generate
 *  @fn     void Generate(const T& dt, const size_t start_id,
                          const size_t& end_id, Particles<T>* particles)
 *  @brief  Generate particles inside a given bounding box
 *  @param[in] dt             Delta time
 *  @param[in] start_id       Index where to start the generation
 *  @param[in] end_id         Index where to stop the generation
 *  @param[in,out]  particles Container where generated particles will be
 *                            placed
 */
template<typename T>
void VelocityGenerator<T>::Generate(const T& dt,
                                       const size_t start_id,
                                       const size_t& end_id,
                                       Particles<T>* particles) {
  // Create distribution
  std::uniform_real_distribution<T> dist;
  std::mt19937 gen;
  // Generate particles
  auto& vel = particles->get_velocity();
  for (size_t i = start_id; i < end_id; ++i) {
    vel[i].x_ = min_.x_ + dist(gen) * delta_.x_;
    vel[i].y_ = min_.y_ + dist(gen) * delta_.y_;
    vel[i].z_ = min_.z_ + dist(gen) * delta_.z_;
  }
}
  
#pragma mark -
#pragma mark Explicit Instantiation
  
/** Float */
template class VelocityGenerator<float>;
/** Double */
template class VelocityGenerator<double>;
  
}  // namespace FaceKit
