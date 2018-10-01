/**
 *  @file   generator.hpp
 *  @brief Particle generator interface definition
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_GENERATOR__
#define __FACEKIT_GENERATOR__

#include <vector>

#include "facekit/core/library_export.hpp"
#include "facekit/core/refcounter.hpp"
#include "facekit/sim/particles.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  IGenerator
 *  @brief  Particle generator interface
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class IGenerator : public RefCounter {
 public:
  
  /**
   *  @name   ~IGenerator
   *  @fn     ~IGenerator()
   *  @brief  Destructor
   */
  virtual ~IGenerator() = default;
  
  /**
   *  @name   Generate
   *  @fn     virtual void Generate(const T& dt, const size_t start_id,
   const size_t& end_id, Particles<T>* particles) = 0
   *  @brief Generate particles for a given range given delta time.
   *  @param[in] dt         Time variation
   *  @param[in] start_id   Index where to start the generation of particles
   *  @param[in] end_id     Index where to stop the generation of particles
   *  @param[out] particles Container where generated particles will be placed.
   */
  virtual void Generate(const T& dt,
                        const size_t start_id,
                        const size_t& end_id,
                        Particles<T>* particles) = 0;
};

/**
 *  @class  Emitter
 *  @brief  Use various type of generator in order to emit particles at a given
 *          rate.
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS Emitter {
 public:
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   Emitter
   *  @fn     explicit Emitter(const T& rate)
   *  @brief  Constructor
   *  @param[in]  rate  Emission rate, speed at which the particles are emitted
   */
  explicit Emitter(const T& rate);
  
  /**
   *  @name   Emitter
   *  @fn     Emitter(const Emitter& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  Emitter(const Emitter& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     Emitter& operator=(const Emitter& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned operator
   */
  Emitter& operator=(const Emitter& rhs) = delete;
  
  /**
   *  @name   ~Emitter
   *  @fn     ~Emitter()
   *  @brief  Destructor
   */
  ~Emitter();
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Emit
   *  @fn     void Emit(const T& dt, Particles<T>* particles) const
   *  @brief  Emit particles using all assiciated generators.
   *  @param[in]  dt  Time differences
   *  @param[in,out]  particles Container where generated particles will be
   *                  stored
   */
  void Emit(const T& dt, Particles<T>* particles) const;
  
  /**
   *  @name   AddGenerator
   *  @fn     void AddGenerator(IGenerator<T>* generator)
   *  @brief  Add a given `generator` to this emitter.
   *  @param[in] generator  Generator to add to this emitter.
   */
  void AddGenerator(IGenerator<T>* generator);
  
#pragma mark -
#pragma mark Private
 private:
  /** Generators */
  std::vector<IGenerator<T>*> gen_;
  /** Emission rate */
  T rate_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_GENERATOR__ */
