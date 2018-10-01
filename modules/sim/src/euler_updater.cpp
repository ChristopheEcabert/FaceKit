/**
 *  @file   euler_updater.cpp
 *  @brief Simple newton mechanic updater
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/sim/euler_updater.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   EulerUpdater
 *  @fn     explicit EulerUpdater(const Acc& acceleration)
 *  @brief  Constructor
 *  @param[in] acceleration Global acceleration (i.e. gravity)
 */
template<typename T>
EulerUpdater<T>::EulerUpdater(const Acc& acceleration) : acc_(acceleration) {}
  
/*
 *  @name   Update
 *  @fn     void Update(const T& dt, Particles<T>* particles)
 *  @brief  Update particles hold by a given container.
 *  @param[in] dt Time variation
 *  @param[in,out] particles  Particles to be updated.
 */
template<typename T>
void EulerUpdater<T>::Update(const T& dt, Particles<T>* particles) {
  auto end = particles->get_n_particle();
  auto& acc = particles->get_acceleration();
  auto& vel = particles->get_velocity();
  auto& pos = particles->get_position();
  for (size_t i = 0; i < end; ++i) {
    // Update acc, i.e. constant
    acc[i] = acc_;
    // Update velocity: v(t) = a*t + v0
    vel[i] += acc_ * dt * T(0.5);
    // Update position: x(t) = 0.5 * a * t^2 + v0*t + x0
    pos[i] += vel[i] * dt;
  }
}
  
#pragma mark -
#pragma mark Explicit instantiation
  
template class EulerUpdater<float>;
template class EulerUpdater<double>;
  
}  // namespace FaceKit
