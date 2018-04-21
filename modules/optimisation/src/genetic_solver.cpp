/**
 *  @file   genetic_solver.cpp
 *  @brief Genetic Solver implementation of "Genetic algorithms for modelling 
 *         and optimisation"
 *  @ingroup optimisation
 *
 *  @author Christophe Ecabert
 *  @date   10.04.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <fstream>

#include "facekit/optimisation/genetic_solver.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialisation
  
/*
 *  @name   GeneticSolver
 *  @fn     GeneticSolver(const size_t& pop_size, const size_t& chromo_size,
 *                        const ChromosomeCtor& ctor)
 *  @brief  Constructor
 *  @param[in] pop_size Population size (Number of chromosomes)
 *  @param[in] chromo_size  Size of one chromosome
 *  @param[in] ctor Callback creating a derived chromosome
 */
template<typename T>
GeneticSolver<T>::GeneticSolver(const size_t& pop_size,
                                const size_t& chromo_size,
                                const ChromosomeCtor& ctor) :
  curr_population_(new Population<T>(pop_size)),
  next_population_(new Population<T>(pop_size)),
  chromo_length_(chromo_size) {
  // Init population
    curr_population_->Create(chromo_size, ctor);
    next_population_->Create(chromo_size, ctor);
}
  
/*
 *  @name   ~GeneticSolver
 *  @fn     ~GeneticSolver(void)
 *  @brief  Destructor
 */
template<typename T>
GeneticSolver<T>::~GeneticSolver(void) {
  if (curr_population_) {
    delete curr_population_;
  }
  if (next_population_) {
    delete next_population_;
  }
}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Solve
 *  @fn     ConvergenceType Solve(const Parameters& params)
 *  @brief  Solve the given optimisation problem
 *  @param[in] params Configuration
 */
template<typename T>
typename GeneticSolver<T>::ConvergenceType GeneticSolver<T>::Solve(const Parameters& params) {
  
  std::ofstream stream("log.txt");
  
  // Compute fitness
  curr_population_->Fitness();
  size_t n_gen = 0;
  T prev_max_fit = 0.0;
  T max_max_fit = 0.0;
  size_t hist_max_fit_cnt = 0;  // Counter of how many generation have the same max_fit
  // Iterate
  while(n_gen < params.max_generation && prev_max_fit < params.fitness_target && hist_max_fit_cnt < params.n_max_fitness_generation) {
    // Perform CrossOver / Mutation / Fitness
    this->CrossOver(params.p_crossover);
    this->Mutate(params.p_mutation);
    T avg_fit_next = next_population_->Fitness();
    T next_max_fit = next_population_->maximum_fitness();
    // Alternate current <-> next generation
    std::swap(curr_population_, next_population_);
    n_gen++;
    prev_max_fit = next_max_fit;
    if (prev_max_fit > max_max_fit) {
      max_max_fit = prev_max_fit;
      hist_max_fit_cnt = 0;
    } else {
      T fit_inc_percent = std::abs(prev_max_fit - max_max_fit) / max_max_fit;
      if (fit_inc_percent != 0.0 && fit_inc_percent <= params.percentage_fitness) {
        hist_max_fit_cnt += 1;
      } else {
        hist_max_fit_cnt = 0;
      }
    }
  }
  return n_gen == params.max_generation ? ConvergenceType::kReachMaxGeneration : ConvergenceType::kConverged;
}
  
/*
 *  @name   BestFitness
 *  @fn     ChromosomeType* BestFitness(void) const
 *  @brief  Give the chromosome with the best fitness
 *  @return Chromosome with solution
 */
template<typename T>
typename GeneticSolver<T>::ChromosomeType* GeneticSolver<T>::BestFitness(void) const {
  // Get best fitness index
  size_t k = curr_population_->maximum_fitness_index();
  return curr_population_->at(k);
}
  
#pragma mark -
#pragma mark Private
  
/*
 *  @name   CrossOver
 *  @fn     void CrossOver(const T& rate)
 *  @brief  Perform crossover
 *  @param[in] rate Crossover rate
 */
template<typename T>
void GeneticSolver<T>::CrossOver(const T& rate) {
  // Loop overall
  size_t k = 0;
  /*for (; k < curr_population_->size() - 2; k += 2) {
    auto* s1 = next_population_->at(k);
    auto* s2 = next_population_->at(k+1);
    curr_population_->CrossOver(rate, s1, s2);
  }*/
  for (;k < curr_population_->size(); ++k) {
    auto* s1 = next_population_->at(k);
    curr_population_->CrossOver(rate, s1, nullptr);
  }
}
  
/*
 *  @name   Mutate
 *  @fn     void Mutate(const T& rate)
 *  @brief  Perform mutation
 *  @param[in] rate Mutation rate
 */
template<typename T>
void GeneticSolver<T>::Mutate(const T& rate) {
  next_population_->Mutate(rate);
}
  
  
#pragma mark -
#pragma mark Explicit instantiation
  
/** Float */
template class GeneticSolver<float>;
/** Double */
template class GeneticSolver<double>;
  
}  // namespace FaceKit
