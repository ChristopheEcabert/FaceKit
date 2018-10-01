/**
 *  @file   particles.hpp
 *  @brief Particles container
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   19.09.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_PARTICLES__
#define __FACEKIT_PARTICLES__

#include <vector>

#include "facekit/core/library_export.hpp"
#include "facekit/core/math/vector.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  Particles
 *  @brief  Container holding various particles.
 *  @author Christophe Ecabert
 *  @date   19.09.18
 *  @ingroup sim
 *  @tparam T Data type
 */
template<typename T>
class FK_EXPORTS Particles {
 public:
  
  /** Properties type */
  using PropType = Vector3<T>;
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   Particles()
   *  @fn     Particles() = default
   *  @brief  Constructor, no particle will be generated
   */
  Particles() = default;
  
  /**
   *  @name   Particles
   *  @fn     explicit Particles(const size_t& n_particle)
   *  @brief  Constructor, generate a given number of particles
   *  @param[in] n_particle Number of particle to generate
   */
  explicit Particles(const size_t& n_particle);
  
  /**
   *  @name   Particles
   *  @fn     Particles(const Particles& other) = default
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  Particles(const Particles& other) = default;
  
  /**
   *  @name   ~Particles
   *  @fn     ~Particles() = default
   *  @brief  Destructor
   */
  ~Particles() = default;
  
  /**
   *  @name   operator=
   *  @fn     Particles& operator=(const Particles& rhs) = default
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  Particles& operator=(const Particles& rhs) = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Generate
   *  @fn     void Generate(const size_t& n_particle)
   *  @brief  Initialize a given amount of particles
   *  @param[in] n_particle  Number of particles to generate
   */
  void Generate(const size_t& n_particle);
  
  /**
   *  @name   Wake
   *  @fn     void Wake(const size_t& idx)
   *  @brief  Turn alive a given particle
   *  @param[in] idx  Index of the particle to change state
   */
  void Wake(const size_t& idx);
  
  /**
   *  @name   Kill
   *  @fn     void Kill(const size_t& idx)
   *  @brief  Kill a given particle
   *  @param[in] idx  Index of the particle to change state
   */
  void Kill(const size_t& idx);
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   get_position
   *  @fn     std::vector<PropType>& get_position() const
   *  @brief  Provide mutable access to position
   *  @return Vector of particle's position
   */
  std::vector<PropType>& get_position() {
    return position_;
  }
  
  /**
   *  @name   get_position
   *  @fn     std::vector<PropType>& get_position() const
   *  @brief  Provide mutable access to position
   *  @return Vector of particle's position
   */
  const std::vector<PropType>& get_position() const {
    return position_;
  }
  
  /**
   *  @name   get_velocity
   *  @fn     std::vector<PropType>& get_velocity() const
   *  @brief  Provide mutable access to velocity
   *  @return Vector of particle's velocity
   */
  std::vector<PropType>& get_velocity() {
    return velocity_;
  }
  
  /**
   *  @name   get_velocity
   *  @fn     std::vector<PropType>& get_velocity() const
   *  @brief  Provide immutable access to velocity
   *  @return Vector of particle's velocity
   */
  const std::vector<PropType>& get_velocity() const {
    return velocity_;
  }
  
  /**
   *  @name   get_acceleration
   *  @fn     std::vector<PropType>& get_acceleration() const
   *  @brief  Provide mutable access to acceleration
   *  @return Vector of particle's acceleration
   */
  std::vector<PropType>& get_acceleration() {
    return acceleration_;
  }
  
  /**
   *  @name   get_acceleration
   *  @fn     std::vector<PropType>& get_acceleration() const
   *  @brief  Provide immutable access to acceleration
   *  @return Vector of particle's acceleration
   */
  const std::vector<PropType>& get_acceleration() const {
    return acceleration_;
  }
  
  /**
   *  @name   get_time
   *  @fn     std::vector<T>& get_time() const
   *  @brief  Provide mutable access to time
   *  @return Vector of particle's time
   */
  std::vector<T>& get_time() {
    return time_;
  }
  
  /**
   *  @name   get_time
   *  @fn     std::vector<T>& get_time() const
   *  @brief  Provide immutable access to time
   *  @return Vector of particle's time
   */
  const std::vector<T>& get_time() const {
    return time_;
  }
  
  /**
   *  @name   get_n_alive
   *  @fn     size_t get_n_alive()
   *  @brief  Indicate how many particles are alive
   *  @return Number of particles alive
   */
  size_t get_n_alive() {
    return n_alive_;
  }
  
  /**
   *  @name   get_n_alive
   *  @fn     size_t get_n_alive() const
   *  @brief  Indicate how many particles are alive
   *  @return Number of particles alive
   */
  size_t get_n_alive() const {
    return n_alive_;
  }
  
  /**
   *  @name   get_n_particle
   *  @fn     size_t get_n_particle()
   *  @brief  Indicate how many particles are present
   *  @return Total number of particles
   */
  size_t get_n_particle() {
    return n_particle_;
  }
  
  /**
   *  @name   get_n_particle
   *  @fn     size_t get_n_particle() const
   *  @brief  Indicate how many particles are present
   *  @return Total number of particles
   */
  size_t get_n_particle() const {
    return n_particle_;
  }

#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   Swap
   *  @fn     void Swap(const size_t& a, const size_t& b)
   *  @brief  Swap properties between two particles `a` and `b`
   *  @param[in] a  First particle
   *  @param[in] b  Second particle
   */
  void Swap(const size_t& a, const size_t& b);
  
  /** Acceleration */
  std::vector<PropType> acceleration_;
  /** Velocity */
  std::vector<PropType> velocity_;
  /** Position */
  std::vector<PropType> position_;
  /** Time */
  std::vector<T> time_;
  /** Alive flags */
  std::vector<bool> alive_;
  /** Number of alive particle */
  size_t n_alive_;
  /** Total number of particle */
  size_t n_particle_;
};
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_PARTICLES__ */
