/**
 *  @file   population.cpp
 *  @brief Representation of a population of chromosomes
 *  @ingroup optimisation
 *
 *  @author Christophe Ecabert
 *  @date   10.04.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <chrono>
#include <limits>

#include "facekit/optimisation/population.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
using clock = std::chrono::high_resolution_clock;
  
#pragma mark -
#pragma mark Initialisation
  
  
/*
 *  @name   Population
 *  @fn     Population(const size_t& size)
 *  @brief  Constructor
 *  @param[in]  size  Population's size (i.e. number of chromosomes)
 */
template<typename T>
Population<T>::Population(const size_t& size) :
  popultation_(size),
  fitness_(size),
  generator_(clock::now().time_since_epoch().count()) {
}
  
/*
 *  @name   Create
 *  @fn     void Create(const size_t& size, const ChromosomeCtor& ctor)
 *  @brief  Create a popultation of a given size
 *  @param[in] size Chromosome size
 *  @param[in] ctor Function creating a chromosome
 */
template<typename T>
void Population<T>::Create(const size_t& size, const ChromosomeCtor& ctor) {
  // Iterate over all chromosomes
  for (size_t k = 0; k < popultation_.size(); ++k) {
    // Check if element already init
    if (popultation_[k] != nullptr) {
      delete popultation_[k];
    }
    // Init
    auto* obj = ctor(size);
    obj->Init();
    popultation_[k] = obj;
  }
}
  
/*
 *  @name   ~Population
 *  @fn     ~Population(void)
 *  @brief  Destructor
 */
template<typename T>
Population<T>::~Population(void) {
  for (size_t k = 0; k < popultation_.size(); ++k) {
    if (popultation_[k] != nullptr) {
      delete popultation_[k];
    }
  }
}
  
#pragma mark -
#pragma mark Usage

/*
 *  @name   Fitness
 *  @fn     T Fitness(void)
 *  @brief  Compute the fitness for each chromosomes in the populutation and
 *          return the average fitness
 *  @return Average fitness for the population
 */
template<typename T>
T Population<T>::Fitness(void) {
  T avg = T(0.0);
  max_fitness_ = 0.0;
  max_fitness_idx_ = 0;
  for (size_t k = 0; k < popultation_.size(); ++k) {
    T fitness = popultation_[k]->Fitness();
    fitness_[k] = fitness;
    avg += fitness;
    if (fitness > max_fitness_) {
      max_fitness_ = fitness;
      max_fitness_idx_ = k;
    }
  }
  return avg / static_cast<T>(popultation_.size());
}
  
/*
 *  @name   CrossOver
 *  @fn     void CrossOver(const T& rate, ChromosomeType* f_signling,
 *                         ChromosomeType* s_sibling) const
 *  @brief  Perform crossover on this population based on each chromosomes
 *          fitness. The selection of the parents are done with a "Roulette
 *          Wheel".
 *  @param[in]  rate  Cross over rate
 *  @param[out] f_sibling First sibling to be generrate by the crossover
 *  @param[out] s_sibling Second sibling generated, if nullptr not used.
 */
template<typename T>
void Population<T>::CrossOver(const T& rate,
                              ChromosomeType* f_sibling,
                              ChromosomeType* s_sibling) {
  // Select parents - Roulette wheel
  size_t p1 = 0, p2 = 0;
  this->RouletteWheel(&p1, &p2);
  // Perform crossover if needed or transfer the parents to the siblings
  auto* f_parent = popultation_[p1];
  auto* s_parent = popultation_[p2];
  T p_co = dist_(generator_);
  if (p_co <= rate) {
    // Do crossover
    T length = static_cast<T>(f_sibling->size());
    size_t t1 = static_cast<size_t>(dist_(generator_) * (length - 1.0));
    size_t t2 = static_cast<size_t>(dist_(generator_) * (length - 1.0));
    size_t t_min = std::min(t1, t2);
    size_t t_max = std::max(t1, t2);
    for (size_t k = 0; k < f_parent->size(); ++k) {
      if (k < t_min || k > t_max) {
        f_sibling->at(k) = f_parent->at(k);
        if (s_sibling) {
          s_sibling->at(k) = s_parent->at(k);
        }
      } else {
        f_sibling->at(k) = s_parent->at(k);
        if (s_sibling) {
          s_sibling->at(k) = f_parent->at(k);
        }
      }
    }
  } else {
    // Copy chromosome without doing crossover
    for (size_t k = 0; k < f_parent->size(); ++k) {
      f_sibling->at(k) = f_parent->at(k);
      if (s_sibling) {
        s_sibling->at(k) = s_parent->at(k);
      }
    }
  }
}
  
/*
 *  @name   Mutate
 *  @fn     void Mutate(const T& rate)
 *  @brief  Call mutation function on the population
 *  @param[in]  rate  Rate of mutation
 */
template<typename T>
void Population<T>::Mutate(const T& rate) {
  // Loop over all chromosome in the population
  for (size_t k = 0; k < popultation_.size(); ++k) {
    auto* c = popultation_[k];
    // Loop over all gene
    for (size_t i = 0; i < c->size(); ++i) {
      T p_m = dist_(generator_);
      if (p_m <= rate) {
        // Mutate the ith gene
        c->Mutate(i);
      }
    }
  }
}
  
  
#pragma mark -
#pragma mark Private
  
/*
 *  @name   RouletteWheel
 *  @fn     void RouletteWheel(size_t* p1, size_t* p2)
 *  @brief  Select two parents using the "Roulette Wheel" concept.
 *          Fitness needs to be up to date.
 *  @param[out] p1  First parent index
 *  @param[out] p2  Second parent index
 */
template<typename T>
void Population<T>::RouletteWheel(size_t* p1, size_t* p2) {
  // Compute fitness proportion
  static std::vector<T> fit_prop(fitness_.size());
  T sum = std::accumulate(fitness_.begin(), fitness_.end(), T(0.0));
  sum += std::numeric_limits<T>::epsilon();
  fit_prop[0] = fitness_[0] / sum;
  for (size_t k = 1; k < fitness_.size(); ++k) {
    fit_prop[k] = (fitness_[k] / sum) + fit_prop[k-1];
  }
  // Select parents
  int cnt = -1;
  do {
    T first = dist_(generator_);
    T second = dist_(generator_);
    bool p1_set = false, p2_set = false;
    // Find matching index
    for (size_t k = 0; k < fitness_.size(); ++k) {
      if (!p1_set && first <= fit_prop[k]) {
        *p1 = k;
        p1_set = true;
      }
      if (!p2_set && second <= fit_prop[k]) {
        *p2 = k;
        p2_set = true;
      }
    }
    cnt++;
  } while (*p1 == *p2 && cnt < 5);
}
  
#pragma mark -
#pragma mark Explicit instantiation
  
/** Population - float */
template class Population<float>;
/** Population - double */
template class Population<double>;
  
}  // namespace FaceKit
