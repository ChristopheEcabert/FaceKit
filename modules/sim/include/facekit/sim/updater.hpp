/**
 *  @file   updater.hpp
 *  @brief Particle updater interface
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_UPDATER__
#define __FACEKIT_UPDATER__

#include "facekit/core/refcounter.hpp"
#include "facekit/sim/particles.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  

/**
 *  @class  IUpdater
 *  @brief  Particle updater interface
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class IUpdater : public RefCounter {
public:
  
  /**
   *  @name   ~IUpdater
   *  @fn     virtual ~IUpdater() = default
   *  @brief  Destructor
   */
  virtual ~IUpdater() = default;
  
  /**
   *  @name   Update
   *  @fn     virtual void Update(const T& dt, Particles<T>* particles) = 0
   *  @brief  Update particles hold by a given container.
   *  @param[in] dt Time variation
   *  @param[in,out] particles  Particles to be updated.
   */
  virtual void Update(const T& dt, Particles<T>* particles) = 0;
};
}  // namespace FaceKit
#endif /* __FACEKIT_UPDATER__ */
