/**
 *  @file   chromosome.hpp
 *  @brief Chromosome abstraction for Genetic Algorithm solver
 *  @ingroup optimisation
 *
 *  @author Christophe Ecabert
 *  @date   09.04.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_CHROMOSOME__
#define __FACEKIT_CHROMOSOME__

#include <cstdio>
#include <vector>
#include <iostream>

#include "facekit/core/library_export.hpp"
#include "facekit/core/status.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  Chromosome
 *  @brief  Chromosome abstraction for Genetic Algorithm solver
 *  @author Christophe Ecabert
 *  @date   09/04/18
 *  @ingroup optimisation
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS Chromosome {
 public:
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   Chromosome
   *  @fn     Chromosome(const size_t& size)
   *  @brief  Constructor
   *  @param[in] size Size of the chromosome
   */
  Chromosome(const size_t& size) : state_(size) {}
  
  /**
   *  @name   Chromosome
   *  @fn     Chromosome(const Chromosome& other) = delete
   *  @brief  Copy constructor
   *  @param[in]  other Object to copy from
   */
  Chromosome(const Chromosome& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     Chromosome& operator=(const Chromosome& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assign operator
   */
  Chromosome& operator=(const Chromosome& rhs) = delete;
  
  /**
   *  @name   ~Chromosome
   *  @fn     virtual ~Chromosome(void) = default
   *  @brief  Destructor
   */
  virtual ~Chromosome(void) = default;
  
  /**
   *  @name   Init
   *  @fn     virtual void Init(void) = 0
   *  @brief  Initialise the chromosome, need to be overloaded by the user
   */
  virtual void Init(void) = 0;
  
  /**
   *  @name   Save
   *  @fn     virtual Status Save(std::ostream& stream) = 0
   *  @brief  Save the chromosome into a given stream
   *  @param[in] stream Stream where to dump the chromosome
   *  @return Status of the operation
   */
  virtual Status Save(std::ostream& stream) = 0;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Fitness
   *  @fn     virtual T Fitness(void) const = 0
   *  @brief  Compute the fitness of this chromosome
   */
  virtual T Fitness(void) const = 0;
  
  /**
   *  @name   Mutate
   *  @fn     virtual void Mutate(const size_t& i) = 0
   *  @brief  Mutate the ith gene.
   */
  virtual void Mutate(const size_t& i) = 0;
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   size
   *  @fn     size_t size(void) const
   *  @brief  Provide chromosome size
   *  @return Size
   */
  size_t size(void) const {
    return state_.size();
  }
  
  /**
   *  @name   at
   *  @fn     const T& at(const size_t& i) const
   *  @brief  Access the ith gene
   *  @param[in] i  ith indexes
   *  @return Gene value
   */
  const T& at(const size_t& i) const {
    return state_[i];
  }
  
  /**
   *  @name   at
   *  @fn     T& at(const size_t& i)
   *  @brief  Access the ith gene
   *  @param[in] i  ith indexes
   *  @return Gene value
   */
  T& at(const size_t& i) {
    return state_[i];
  }
  
  
#pragma mark -
#pragma mark Protected
  
 protected:
  /** Chromosome state */
  std::vector<T> state_;
};

}  // namespace FaceKit
#endif /* __FACEKIT_CHROMOSOME__ */
