/**
 *  @file   euler_updater.hpp
 *  @brief Simple newton mechanic updater
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_EULER_UPDATER__
#define __FACEKIT_EULER_UPDATER__

#include "facekit/core/library_export.hpp"
#include "facekit/sim/updater.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  EulerUpdater
 *  @brief  Simple newton mechanic updater
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class EulerUpdater : public IUpdater<T> {
public:
#pragma mark -
#pragma mark Initialisation
  
  /** Acceleration tpye */
  using Acc = Vector3<T>;
  
  /**
   *  @name   EulerUpdater
   *  @fn     explicit EulerUpdater(const Acc& acceleration)
   *  @brief  Constructor
   *  @param[in] acceleration Global acceleration (i.e. gravity)
   */
  explicit EulerUpdater(const Acc& acceleration);
  
  /**
   *  @name   EulerUpdater
   *  @fn     EulerUpdater(const EulerUpdater& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  EulerUpdater(const EulerUpdater& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     EulerUpdater& operator=(const EulerUpdater& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  EulerUpdater& operator=(const EulerUpdater& rhs) = delete;
  
  /**
   *  @name   ~EulerUpdater
   *  @fn     ~EulerUpdater() = default
   *  @brief  Destructor
   */
  ~EulerUpdater() = default;
  
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
  
 private:
  /** Acceleration */
  Acc acc_;
};

}  // namespace FaceKit
#endif /* __FACEKIT_EULER_UPDATER__ */
