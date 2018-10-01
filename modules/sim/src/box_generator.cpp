/**
 *  @file   box_generator.cpp
 *  @brief Randomly generate particles inside a given bounding box
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <random>


#include "facekit/sim/box_generator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialisation
  
/*
 *  @name   PositionBoxGenerator
 *  @fn     PositionBoxGenerator(const Pos& orgin, const Pos& side)
 *  @brief  Constructor
 *  @param[in] origin Top left corner position
 *  @param[in] side   Bounding box width (length of the sides)
 */
template<typename T>
PositionBoxGenerator<T>::PositionBoxGenerator(const Pos& origin,
                                              const Pos& side) : min_(origin),
                                                                 side_(side) {}
  
  
  
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
void PositionBoxGenerator<T>::Generate(const T& dt,
                                       const size_t start_id,
                                       const size_t& end_id,
                                       Particles<T>* particles) {
  // Create distribution
  std::uniform_real_distribution<T> dist;
  std::mt19937 gen;
  // Generate particles
  auto& pos = particles->get_position();
  for (size_t i = start_id; i < end_id; ++i) {
    pos[i].x_ = min_.x_ + dist(gen) * side_.x_;
    pos[i].y_ = min_.y_ + dist(gen) * side_.y_;
    pos[i].z_ = min_.z_ + dist(gen) * side_.z_;
  }
}
  
#pragma mark -
#pragma mark Explicit Instantiation
  
/** Float */
template class PositionBoxGenerator<float>;
/** Double */
template class PositionBoxGenerator<double>;
  
}  // namespace FaceKit
