/**
 *  @file   velocity_generator.hpp
 *  @brief Randomly generate particle's velocity vector in a given range
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_VELOCITY_GENERATOR__
#define __FACEKIT_VELOCITY_GENERATOR__

#include "facekit/core/library_export.hpp"
#include "facekit/sim/generator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  VelocityGenerator
 *  @brief  Randomly generate particle's velocity vector in a given range
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS VelocityGenerator : public IGenerator<T> {
 public:
  /** Velocity type */
  using Vel = Vector3<T>;
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   VelocityGenerator
   *  @fn     VelocityGenerator(const Vel& min, const Vel& delta)
   *  @brief  Constructor
   *  @param[in] min  Minimum velocity
   *  @param[in] delta Range of velocity variation allowed for this generator
   */
  VelocityGenerator(const Vel& min, const Vel& delta);
  
  /**
   *  @name   VelocityGenerator
   *  @fn     VelocityGenerator(const VelocityGenerator& other) = default
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  VelocityGenerator(const VelocityGenerator& other) = default;
  
  /**
   *  @name   operator=
   *  @fn     VelocityGenerator& operator=(const VelocityGenerator& rhs) = default
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  VelocityGenerator& operator=(const VelocityGenerator& rhs) = default;
  
  /**
   *  @name   ~VelocityGenerator
   *  @fn     ~VelocityGenerator() override = default
   *  @brief  Destructor
   */
  ~VelocityGenerator() override = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Generate
   *  @fn     void Generate(const T& dt, const size_t start_id,
                        const size_t& end_id, Particles<T>* particles) override
   *  @brief  Generate particle's velocity in a given range
   *  @param[in] dt             Delta time
   *  @param[in] start_id       Index where to start the generation
   *  @param[in] end_id         Index where to stop the generation
   *  @param[in,out]  particles Container where generated particles will be
   *                            placed
   */
  void Generate(const T& dt,
                const size_t start_id,
                const size_t& end_id,
                Particles<T>* particles) override;
  
#pragma mark -
#pragma mark Private
 private:
  /** Minimum velocity */
  Vel min_;
  /** Velocity variation */
  Vel delta_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_VELOCITY_GENERATOR__ */
