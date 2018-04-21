/**
 *  @file   population.hpp
 *  @brief Representation of a population of chromosomes
 *  @ingroup optimisation
 *
 *  @author Christophe Ecabert
 *  @date   09.04.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_POPULATION__
#define __FACEKIT_POPULATION__

#include <vector>
#include <functional>
#include <random>

#include "facekit/core/library_export.hpp"
#include "facekit/optimisation/chromosome.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  Population
 *  @brief  Representation of a population of chromosomes
 *  @author Christophe Ecabert
 *  @date   10/03/18
 *  @ingroup optimisation
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS Population {
 public:
  
#pragma mark -
#pragma mark Type definition
  
  /** Chromosome */
  using Chromosome = Chromosome<T>;
  /** Chromosome constructor */
  using ChromosomeCtor = std::function<Chromosome*(const size_t&)>;
 
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   Population
   *  @fn     Population(const size_t& size)
   *  @brief  Constructor
   *  @param[in]  size  Population's size (i.e. number of chromosomes)
   */
  Population(const size_t& size);
  
  /**
   *  @name   Population
   *  @fn     Population(const Population& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  Population(const Population& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     Population& operator=(const Population& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned operator
   */
  Population& operator=(const Population& rhs) = delete;
  
  /**
   *  @name   Create
   *  @fn     void Create(const size_t& size, const ChromosomeCtor& ctor)
   *  @brief  Create a popultation of a given size
   *  @param[in] size Chromosome size
   *  @param[in] ctor Function creating a chromosome
   */
  void Create(const size_t& size, const ChromosomeCtor& ctor);
  
  /**
   *  @name   ~Population
   *  @fn     ~Population(void)
   *  @brief  Destructor
   */
  ~Population(void);
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Fitness
   *  @fn     T Fitness(void)
   *  @brief  Compute the fitness for each chromosomes in the populutation and 
   *          return the average fitness
   *  @return Average fitness for the population
   */
  T Fitness(void);
  
  /**
   *  @name   CrossOver
   *  @fn     void CrossOver(const T& rate, Chromosome* f_sibling, Chromosome* s_sibling)
   *  @brief  Perform crossover on this population based on each chromosomes 
   *          fitness. The selection of the parents are done with a "Roulette
   *          Wheel".
   *  @param[in]  rate  Cross over rate
   *  @param[out] f_sibling First sibling to be generrate by the crossover
   *  @param[out] s_sibling Second sibling generated, if nullptr not used.
   */
  void CrossOver(const T& rate, Chromosome* f_sibling, Chromosome* s_sibling);
  
  /**
   *  @name   Mutate
   *  @fn     void Mutate(const T& rate)
   *  @brief  Call mutation function on the population
   *  @param[in]  rate  Rate of mutation
   */
  void Mutate(const T& rate);

#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   size
   *  @fn     size_t size(void) const
   *  @brief  Give populiation's size (i.e. number of chromosomes)
   *  @return Size
   */
  size_t size(void) const {
    return popultation_.size();
  }
  
  /**
   *  @name   maximum_fitness
   *  @fn     T maximum_fitness(void) const
   *  @brief  Give the maximum fitness for this population
   *  @return Largest fitness
   */
  T maximum_fitness(void) const {
    return max_fitness_;
  }
  
  /**
   *  @name   maximum_fitness_index
   *  @fn     size_t maximum_fitness_index(void) const
   *  @brief  Chromosome index with the maximum fitness
   *  @return index
   */
  size_t maximum_fitness_index(void) const {
    return max_fitness_idx_;
  }
  
  /**
   *  @name   at
   *  @fn     const Chromosome* at(const size_t& i) const
   *  @brief  Access chromosome at the ith position
   *  @param[in] i  Position to select
   *  @return Chromosome instance
   */
  const Chromosome* at(const size_t& i) const {
    return popultation_[i];
  }
  
  /**
   *  @name   at
   *  @fn     Chromosome* at(const size_t& i)105
   *  @brief  Access chromosome at the ith position
   *  @param[in] i  Position to select
   *  @return Chromosome instance
   */
  Chromosome* at(const size_t& i) {
    return popultation_[i];
  }
  
  /**
   *  @name   operator[]
   *  @fn     Chromosome* operator[](size_t i)
   *  @brief  Access chromosome at the ith position
   *  @param[in] i  Position to select
   *  @return Chromosome instance
   */
  Chromosome* operator[](size_t i) {
    return popultation_[i];
  }
  
  /**
   *  @name   operator[]
   *  @fn     const Chromosome* operator[](size_t i) const
   *  @brief  Access chromosome at the ith position
   *  @param[in] i  Position to select
   *  @return Chromosome instance
   */
  const Chromosome* operator[](size_t i) const {
    return popultation_[i];
  }
  
  
#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   RouletteWheel
   *  @fn     void RouletteWheel(size_t* p1, size_t* p2)
   *  @brief  Select two parents using the "Roulette Wheel" concept. 
   *          Fitness needs to be up to date.
   *  @param[out] p1  First parent index
   *  @param[out] p2  Second parent index
   */
  void RouletteWheel(size_t* p1, size_t* p2);
  
  /** List of chromosomes */
  std::vector<Chromosome*> popultation_;
  /** Chromosome's fitness */
  std::vector<T> fitness_;
  /** Maximum fitness for the current population */
  T max_fitness_;
  /** Chromosome index with the maximum fitness */
  size_t max_fitness_idx_;
  /** Random generator, crossover + mutation */
  std::mt19937_64 generator_;
  /** Distribution */
  std::uniform_real_distribution<T> dist_;
};
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_POPULATION__ */
