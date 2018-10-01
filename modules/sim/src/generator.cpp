/**
 *  @file   generator.cpp
 *  @brief Particle generator interface definition
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/sim/generator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialisation
  
/*
 *  @name   Emitter
 *  @fn     explicit Emitter(const T& rate)
 *  @brief  Constructor
 *  @param[in]  rate  Emission rate, speed at which the particles are emitted
 */
template<typename T>
Emitter<T>::Emitter(const T& rate) : gen_(0, nullptr), rate_(rate) {}
  
/*
 *  @name   ~Emitter
 *  @fn     ~Emitter()
 *  @brief  Destructor
 */
template<typename T>
Emitter<T>::~Emitter() {
  for (auto* g : gen_) {
    g->Dec();
  }
}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Emit
 *  @fn     void Emit(const T& dt, Particles<T>* particles) const
 *  @brief  Emit particles using all assiciated generators.
 *  @param[in]  dt  Time differences
 *  @param[in,out]  particles Container where generated particles will be
 *                  stored
 */
template<typename T>
void Emitter<T>::Emit(const T& dt, Particles<T>* particles) const {
  const auto max_new_p = static_cast<size_t>(dt * rate_);
  const auto start = particles->get_n_alive();
  const auto end = std::min(start + max_new_p, particles->get_n_particle() - 1);
  // Call generators
  for (auto* g : gen_) {
    g->Generate(dt, start, end, particles);
  }
  // Wake new particles
  for (size_t i = start; i < end; ++i) {
    particles->Wake(i);
  }
}
  
/*
 *  @name   AddGenerator
 *  @fn     void AddGenerator(IGenerator<T>* generator)
 *  @brief  Add a given `generator` to this emitter.
 *  @param[in] generator  Generator to add to this emitter.
 */
template<typename T>
void Emitter<T>::AddGenerator(IGenerator<T>* generator) {
  generator->Inc();
  gen_.push_back(generator);
}
  
#pragma mark -
#pragma mark Explicit instantiation
  
/** Float */
template class Emitter<float>;
/** Double */
template class Emitter<double>;
  
}  // namespace FaceKit
