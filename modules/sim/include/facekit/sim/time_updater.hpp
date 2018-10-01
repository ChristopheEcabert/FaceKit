/**
 *  @file   time_updater.hpp
 *  @brief Update lifetime of particles
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_TIME_UPDATER__
#define __FACEKIT_TIME_UPDATER__

#include "facekit/core/library_export.hpp"
#include "facekit/sim/updater.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  TimeUpdater
 *  @brief  Update lifetime of particles
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class TimeUpdater : public IUpdater<T> {
 public:
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   TimeUpdater
   *  @fn     TimeUpdater() = default
   *  @brief  Constructor
   */
  TimeUpdater() = default;
  
  /**
   *  @name   TimeUpdater
   *  @fn     TimeUpdater(const TimeUpdater& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  TimeUpdater(const TimeUpdater& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     TimeUpdater& operator=(const TimeUpdater& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  TimeUpdater& operator=(const TimeUpdater& rhs) = delete;
  
  /**
   *  @name   ~TimeUpdater
   *  @fn     ~TimeUpdater() = default
   *  @brief  Destructor
   */
  ~TimeUpdater() = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Update
   *  @fn     void Update(const T& dt, Particles<T>* particles)
   *  @brief  Update particles hold by a given container.
   *  @param[in] dt Time variation
   *  @param[in,out] particles  Particles to be updated.
   */
  void Update(const T& dt, Particles<T>* particles);
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_TIME_UPDATER__ */
