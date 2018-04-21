/**
 *  @file   ex_genetic_solver.cpp
 *  @brief Genetic Algorithm usage example
 *  @ingroup optimisation
 *
 *  @author Christophe Ecabert
 *  @date   09.04.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <chrono>
#include <random>
#include <iostream>
#include <fstream>

#include "facekit/core/cmd_parser.hpp"
#include "facekit/core/logger.hpp"
#include "facekit/core/utils/string.hpp"

#include "facekit/optimisation/genetic_solver.hpp"

namespace FK = FaceKit;

#pragma mark -
#pragma mark BinaryString Problem

template<typename T>
class BinaryString : public FK::Chromosome<T> {
 public:
  
  /**
   *  @name   BinaryString
   *  @fn     BinaryString(const size_t& size)
   *  @brief  Constructor
   *  @param[in] size Size of the chromosome
   */
  BinaryString(const size_t& size) : FK::Chromosome<T>::Chromosome(size) {}
  
  /**
   *  @name   ~BinaryString
   *  @fn     ~BinaryString(void) override
   *  @brief  Destructor
   */
  ~BinaryString(void) override {}
  
  /**
   *  @name   Init
   *  @fn     void Init(void) override
   *  @brief  Initialise the chromosome, need to be overloaded by the user
   */
  void Init(void) override {
    static std::mt19937_64 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<T> dist;
    for (size_t k = 0; k < this->size(); ++k) {
      T v = dist(gen);
      this->state_[k] = v < T(0.5) ? T(0.0) : T(1.0);
    }
  }
  
  /**
   *  @name   Save
   *  @fn     FK::Status Save(std::ostream& stream) override
   *  @brief  Save the chromosome into a given stream
   *  @param[in] stream Stream where to dump the chromosome
   *  @return Status of the operation
   */
  FK::Status Save(std::ostream& stream) override {
    stream << "Solution found:" << std::endl;
    for (size_t k = 0; k < this->size(); ++k) {
      stream << this->state_[k] << " ";
    }
    stream << std::endl;
    return FK::Status();
  }
  
  /**
   *  @name   Fitness
   *  @fn     T Fitness(void) const override
   *  @brief  Compute the fitness of this chromosome
   */
  T Fitness(void) const override {
    T fitness = T(0.0);
    for (size_t k = 0; k < this->size(); ++k) {
      if (k < 4) {
        fitness += this->state_[k];
      } else if (k < 8) {
        fitness += (T(1.0) - this->state_[k]);
      } else if (k < 12) {
        fitness += this->state_[k];
      } else {
        fitness += (T(1.0) - this->state_[k]);
      }
    }
    return fitness;
  }
  
  /**
   *  @name   Mutate
   *  @fn     virtual void Mutate(const size_t& i) = 0
   *  @brief  Mutate the ith gene.
   */
  void Mutate(const size_t& i) override {
    this->state_[i] = 1.0 - this->state_[i];
  }
};

#pragma mark -
#pragma mark Knapsack Problem

template<typename T>
class KnapsackData {
 public:
  
  /**
   *  @struct Item
   *  @brief  Knapsack's item
   */
  struct Item {
    /** Item weight */
    T weight;
    /** Item value */
    T value;
    
    friend std::istream& operator>>(std::istream& is, Item& item) {
      is >> item.value >> item.weight;
      return is;
    }
  };
  
  /**
   *  @name   KnapsackData
   *  @fn     KnapsackData(void) = default
   *  @brief  Constructor
   */
  KnapsackData(void) = default;
  
  /**
   *  @name   KnapsackData
   *  @fn     KnapsackData(const std::string& path)
   *  @brief  Constructor
   *  @param[in] path Path to file storing data
   */
  KnapsackData(const std::string& path) {
    this->Load(path);
  }
  
  /**
   *  @name   ~KnapsackData
   *  @fn     ~KnapsackData(void) = default
   *  @brief  Destructor
   */
  ~KnapsackData(void) = default;
  
  /**
   *  @name   Load
   *  @fn     FK::Status Load(const std::string& path)
   *  @brief  Load data from file
   *  @param[in] path Path to data file
   *  @return Status
   */
  FK::Status Load(const std::string& path) {
    std::ifstream is(path.c_str());
    if (is.is_open()) {
      int N = 0;
      items_.clear();
      // Get number of item in the list + capacity of the knapsack
      is >> N >> capacity_;
      // Load item
      for (int i = 0; i < N; ++i) {
        Item item;
        is >> item;
        items_.push_back(item);
      }
      return FK::Status();
    } else {
      return FK::Status(FK::Status::Type::kInvalidArgument,
                        "Can not open file: " + path);
    }
  }
  
  /**
   *  @name   size
   *  @fn     size_t size(void) const
   *  @brief  Number of element in the knapsack
   *  @return Size
   */
  size_t size(void) const {
    return items_.size();
  }
  
  /**
   *  @name   capacity
   *  @fn     T capacity(void) const
   *  @brief  Knapsack capacity
   */
  T capacity(void) const {
    return capacity_;
  }
  
  /**
   *  @name   data
   *  @fn     const std::vector<Item>& data() const
   *  @brief  List of items in the the knapsack
   *  @return Item list
   */
  const std::vector<Item>& data() const {
    return items_;
  }
  
 private:
  /** Collection of item */
  std::vector<Item> items_;
  /** Capacity */
  T capacity_;
};

template<typename T>
class Knapsack : public FK::Chromosome<T> {
 public:
  /** Data */
  using KnapsackDataType = KnapsackData<T>;
  
  /**
   *  @name   Knapsack
   *  @fn     Knapsack(const KnapsackDataType* data)
   *  @brief  Constructor
   *  @param[in] data Knapsack items
   */
  Knapsack(const KnapsackDataType* data) : FK::Chromosome<T>::Chromosome(data->size()), data_(data) {}
  
  /**
   *  @name   ~Knapsack
   *  @fn     ~Knapsack(void) override
   *  @brief  Destructor
   */
  ~Knapsack(void) override {}
  
  /**
   *  @name   Init
   *  @fn     void Init(void) override
   *  @brief  Initialise the chromosome, need to be overloaded by the user
   */
  void Init(void) override {
    static std::mt19937_64 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<T> dist;
    for (size_t k = 0; k < this->size(); ++k) {
      T v = dist(gen);
      // Take 1% of objects
      this->state_[k] = v < T(0.01) ? T(1.0) : T(0.0);
    }
  }
  
  /**
   *  @name   Save
   *  @fn     FK::Status Save(std::ostream& stream) override
   *  @brief  Save the chromosome into a given stream
   *  @param[in] stream Stream where to dump the chromosome
   *  @return Status of the operation
   */
  FK::Status Save(std::ostream& stream) override {
    stream << "*** Solution found ***" << std::endl;
    stream << "Selection:" << std::endl;
    for (size_t k = 0; k < this->size(); ++k) {
      stream << this->state_[k] << " ";
    }
    stream << std::endl;
    return FK::Status();
  }
  
  /**
   *  @name   Fitness
   *  @fn     T Fitness(void) const override
   *  @brief  Compute the fitness of this chromosome
   */
  T Fitness(void) const override {
    T fitness = 0.0;
    T weights = 0.0;
    // Check cumulated does not overflow capacity, count value in the knapsack
    const auto& data = data_->data();
    T capacity = data_->capacity();
    for (size_t k = 0; k < this->size(); ++k) {
      const auto& item = data[k];
      if (this->state_[k] != 0.0) {
        weights += item.weight;
        fitness += item.value;
      }
    }
    fitness /= static_cast<T>(data.size() * 100);
    fitness = std::pow(fitness, 3.0);
    if (weights > capacity) { // Loose 90% of the fitness if the weights overflow the capacity
      fitness = 0.01;
    }
    
    
    return fitness;
  }
  
  /**
   *  @name   Mutate
   *  @fn     virtual void Mutate(const size_t& i) = 0
   *  @brief  Mutate the ith gene.
   */
  void Mutate(const size_t& i) override {
    this->state_[i] = 1.0 - this->state_[i];
  }
  
 private:
  /** Data */
  const KnapsackDataType* data_;
};


#pragma mark -

int main(const int argc, const char** argv) {
  using T = float;
  // Define argument needed
  FK::CmdLineParser parser;
  parser.AddArgument("-t",
                     FK::CmdLineParser::ArgState::kNeeded,
                     "Problem type {BinaryString, Knapsack}");
  parser.AddArgument("-f",
                     FK::CmdLineParser::ArgState::kOptional,
                     "Fodler storing data, if needed");
  int err = parser.ParseCmdLine(argc, argv);
  if (!err) {
    std::string prob_type;
    std::string folder;
    parser.HasArgument("-t", &prob_type);
    parser.HasArgument("-f", &folder);
    
    if (prob_type == "BinaryString") {
      
      // Callback for chromosome creation
      auto fcn = [](const size_t& sz) -> FK::Chromosome<T>* {
        return new BinaryString<T>(sz);
      };
      // Create solver
      FK::GeneticSolver<T>::Parameters params;
      params.p_mutation = 0.01;
      params.p_crossover = 0.75;
      params.fitness_target = 16.0;
      params.n_max_fitness_generation = 10;
      params.percentage_fitness = 0.1;
      FK::GeneticSolver<T> solver(100, 16, fcn);
      solver.Solve(params);
      
      // Print solution
      auto solution = solver.BestFitness();
      solution->Save(std::cout);
      // Ground truth
      std::cout << "Ground truth" << std::endl;
      std::cout << "1 1 1 1 0 0 0 0 1 1 1 1 0 0 0 0" << std::endl;

    } else if (prob_type == "Knapsack") {
      // Load knapscak data
      KnapsackData<T> data;
      //data.Load(FK::Path::Join(folder, "ks_10000_0.txt"));
      data.Load(FK::Path::Join(folder, "ks_100_0.txt"));
      //data.Load(FK::Path::Join(folder, "ks_4_0.txt"));
      
      // Callback for chromosome creation
      auto fcn = [&](const size_t& sz) -> FK::Chromosome<T>* {
        return new Knapsack<T>(&data);
      };
      
      // Create solver
      FK::GeneticSolver<T>::Parameters params;
      params.p_mutation = 0.02;
      params.p_crossover = 0.8;
      params.max_generation = 100;
      params.fitness_target = 1e6;
      params.n_max_fitness_generation = 5;
      params.percentage_fitness = 0.01;
      FK::GeneticSolver<T> solver(50, data.size(), fcn);
      solver.Solve(params);

      // Print solution
      auto solution = solver.BestFitness();
      solution->Save(std::cout);
      
      // Check capacity
      size_t sz = solution->size();
      const auto& items = data.data();
      T capacity = 0.0, value = 0.0;
      for (size_t k = 0; k < sz; ++k) {
        const auto& v = solution->at(k);
        if (v != 0.0) {
          capacity += items[k].weight;
          value += items[k].value;
        }
      }
      std::cout << "Knapsack weight: " << capacity << " / " << data.capacity() << std::endl;
      std::cout << "Knapsack value: " << value << std::endl;
    } else {
      err = -1;
      FACEKIT_LOG_ERROR("Unknown type of problem");
    }
  } else {
    FACEKIT_LOG_ERROR("Unable to parse command line!");
  }
  return err;
}
