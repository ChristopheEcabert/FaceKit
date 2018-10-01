/**
 *  @file   time_updater.cpp
 *  @brief Update lifetime of particles
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/sim/time_updater.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   Update
 *  @fn     void Update(const T& dt, Particles<T>* particles)
 *  @brief  Update particles hold by a given container.
 *  @param[in] dt Time variation
 *  @param[in,out] particles  Particles to be updated.
 */
template<typename T>
void TimeUpdater<T>::Update(const T& dt, Particles<T>* particles) {
  auto end = particles->get_n_particle();
  auto& time = particles->get_time();
  for (size_t i = 0; i < end; ++i) {
    // Update time for particle `i`
    time[i] -= dt;
    // Does particle reach end of lifetime ?
    if (time[i] < T(0.0)) {
      // Kill particles
      particles->Kill(i);
      end = (particles->get_n_alive() < particles->get_n_particle() ?
             particles->get_n_alive() :
             particles->get_n_particle());
    }
  }
}
  
#pragma mark -
#pragma mark Explicit instantiation
  
template class TimeUpdater<float>;
template class TimeUpdater<double>;
  
  
  
}  // namespace FaceKit
