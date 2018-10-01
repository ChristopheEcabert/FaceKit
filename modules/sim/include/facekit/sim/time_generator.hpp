/**
 *  @file   time_generator.hpp
 *  @brief Randomly generate particle's life time in a given range
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_TIME_GENERATOR__
#define __FACEKIT_TIME_GENERATOR__

#include "facekit/core/library_export.hpp"
#include "facekit/sim/generator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  TimeGenerator
 *  @brief  Randomly generate particle's velocity vector in a given range
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS TimeGenerator : public IGenerator<T> {
 public:
  /** Time type */
  using Time = T;
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   TimeGenerator
   *  @fn     TimeGenerator(const Time& min, const Time& delta)
   *  @brief  Constructor
   *  @param[in] min  Minimum lifetime
   *  @param[in] delta Range of possible lifetime variation
   */
  TimeGenerator(const Time& min, const Time& delta);
  
  /**
   *  @name   TimeGenerator
   *  @fn     TimeGenerator(const TimeGenerator& other) = default
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  TimeGenerator(const TimeGenerator& other) = default;
  
  /**
   *  @name   operator=
   *  @fn     TimeGenerator& operator=(const TimeGenerator& rhs) = default
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  TimeGenerator& operator=(const TimeGenerator& rhs) = default;
  
  /**
   *  @name   ~TimeGenerator
   *  @fn     ~TimeGenerator() override = default
   *  @brief  Destructor
   */
  ~TimeGenerator() override = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Generate
   *  @fn     void Generate(const T& dt, const size_t start_id,
                        const size_t& end_id, Particles<T>* particles) override
   *  @brief  Generate particle's lifetime in a given range
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
  /** Mininum lifetime */
  Time min_;
  /** Possible lifetime variation */
  Time delta_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_TIME_GENERATOR__ */
