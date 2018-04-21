/**
 *  @file   genetic_solver.hpp
 *  @brief Genetic Solver implementation of "Genetic algorithms for modelling
 *         and optimisation"
 *  @ingroup optimisation
 *
 *  @author Christophe Ecabert
 *  @date   10.04.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_GENETIC_SOLVER__
#define __FACEKIT_GENETIC_SOLVER__

#include "facekit/core/library_export.hpp"
#include "facekit/optimisation/population.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
  
  
/**
 *  @class  GeneticSolver
 *  @brief  Implementation of "Genetic algorithms for modelling
 *         and optimisation"
 *  @author Christophe Ecabert
 *  @date   10/04/18
 *  @ingroup optimisation
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS GeneticSolver {
 public:
#pragma mark -
#pragma mark Type Definition
  
  /** Chromosome */
  using ChromosomeType = typename Population<T>::ChromosomeType;
  /** Chromosome constructor callback */
  using ChromosomeCtor = typename Population<T>::ChromosomeCtor;
  
  /**
   *  @struct Parameters
   *  @brief  Solver configuration
   *  @author Christophe Ecabert
   *  @date   10/04/18
   *  @ingroup optimisation
   */
  struct Parameters {
    /** CrossOver probability */
    T p_crossover;
    /** Mutation probability */
    T p_mutation;
    /** Fitness target */
    T fitness_target;
    /** Number max of generation/iteration */
    size_t max_generation;
    /** Fitness increase minimum in percentage */
    T percentage_fitness;
    /** Number of generation at maximum fitness before stopping */
    size_t n_max_fitness_generation;
    
    /**
     *  @name   Parameters
     *  @fn     Parameters(void)
     *  @brief  Cnstructor
     */
    Parameters(void) : p_crossover(0.8), p_mutation(0.02), max_generation(50), percentage_fitness(5.0), n_max_fitness_generation(5) {}
  };
  
  /**
   *  @enum ConvergenceType
   *  @brief  List of convergence
   */
  enum class ConvergenceType : int8_t {
    /** Maximum number of generation reached */
    kReachMaxGeneration = 0,
    /** Reach convergence criteria */
    kConverged
  };
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   GeneticSolver
   *  @fn     GeneticSolver(const size_t& pop_size, const size_t& chromo_size,
   *                        const ChromosomeCtor& ctor)
   *  @brief  Constructor
   *  @param[in] pop_size Population size (Number of chromosomes)
   *  @param[in] chromo_size  Size of one chromosome
   *  @param[in] ctor Callback creating a derived chromosome
   */
  GeneticSolver(const size_t& pop_size,
                const size_t& chromo_size,
                const ChromosomeCtor& ctor);
  
  /**
   *  @name   GeneticSolver
   *  @fn     GeneticSolver(const GeneticSolver& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  GeneticSolver(const GeneticSolver& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     GeneticSolver& operator=(const GeneticSolver& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned operator
   */
  GeneticSolver& operator=(const GeneticSolver& rhs) = delete;
  
  /**
   *  @name   ~GeneticSolver
   *  @fn     ~GeneticSolver(void)
   *  @brief  Destructor
   */
  ~GeneticSolver(void);
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Solve
   *  @fn     ConvergenceType Solve(const Parameters& params)
   *  @brief  Solve the given optimisation problem 
   *  @param[in] params Configuration
   */
  ConvergenceType Solve(const Parameters& params);
  
  /**
   *  @name   BestFitness
   *  @fn     ChromosomeType* BestFitness(void) const
   *  @brief  Give the chromosome with the best fitness
   *  @return Chromosome with solution
   */
  ChromosomeType* BestFitness(void) const;
  
#pragma mark -
#pragma mark Accessors
  
#pragma mark -
#pragma mark Private
private:
  
  /**
   *  @name   CrossOver
   *  @fn     void CrossOver(const T& rate)
   *  @brief  Perform crossover
   *  @param[in] rate Crossover rate
   */
  void CrossOver(const T& rate);
  
  /**
   *  @name   Mutate
   *  @fn     void Mutate(const T& rate)
   *  @brief  Perform mutation
   *  @param[in] rate Mutation rate
   */
  void Mutate(const T& rate);
  
  
  
  
  /** Current population */
  Population<T>* curr_population_;
  /** Next population */
  Population<T>* next_population_;
  /** Chromosome length */
  size_t chromo_length_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_GENETIC_SOLVER__ */
