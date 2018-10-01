/**
 *  @file   particles.cpp
 *  @brief Particles container
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   19.09.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <cassert>

#include "facekit/sim/particles.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name   Particles
 *  @fn     explicit Particles(const size_t& n_particle)
 *  @brief  Constructor, generate a given number of particles
 *  @param[in] n_particle Number of particle to generate
 */
template<typename T>
Particles<T>::Particles(const size_t& n_particles) : Particles<T>::Particles() {
  this->Generate(n_particles);
}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Generate
 *  @fn     void Generate(const size_t& n_particle)
 *  @brief  Initialize a given amount of particles
 *  @param[in] n_particle  Number of particles to generate
 */
template<typename T>
void Particles<T>::Generate(const size_t& n_particle) {
  n_particle_ = n_particle;
  n_alive_ = 0;
  acceleration_.resize(n_particle_);  // Default ctor set to {0, 0, 0, 0};
  velocity_.resize(n_particle_);
  position_.resize(n_particle_);
  alive_.resize(n_particle_, false);  // Particle are not alive at begining
}

/*
 *  @name   Wake
 *  @fn     void Wake(const size_t& idx)
 *  @brief  Turn alive a given particle
 *  @param[in] idx  Index of the particle to change state
 */
template<typename T>
void Particles<T>::Wake(const size_t& idx) {
  assert(n_alive_ < n_particle_);
  this->alive_[idx] = true;
  this->Swap(idx, n_alive_);
  ++n_alive_;
}

/*
 *  @name   Kill
 *  @fn     void Kill(const size_t& idx)
 *  @brief  Kill a given particle
 *  @param[in] idx  Index of the particle to change state
 */
template<typename T>
void Particles<T>::Kill(const size_t& idx) {
  assert(n_alive_ > 0);
  this->alive_[idx] = false;
  this->Swap(idx, n_alive_ - 1);
  --n_alive_;
}
  
  
#pragma mark -
#pragma mark Private
  
/*
 *  @name   Swap
 *  @fn     void Swap(const size_t& a, const size_t& b)
 *  @brief  Swap properties between two particles `a` and `b`
 *  @param[in] a  First particle
 *  @param[in] b  Second particle
 */
template<typename T>
void Particles<T>::Swap(const size_t& a, const size_t& b) {
  // Swap properties
  std::swap(acceleration_[a], acceleration_[b]);
  std::swap(velocity_[a], velocity_[b]);
  std::swap(position_[a], position_[b]);
  std::swap(alive_[a], alive_[b]);
}
  
  
#pragma mark -
#pragma mark Explicit declaration
  
/** Float */
template class Particles<float>;
/** Double */
template class Particles<double>;
  
}  // namespace FaceKit
