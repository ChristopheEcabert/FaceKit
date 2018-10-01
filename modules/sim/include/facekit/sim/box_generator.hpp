/**
 *  @file   box_generator.hpp
 *  @brief Randomly generate particles inside a given bounding box
 *  @ingroup sim
 *
 *  @author Christophe Ecabert
 *  @date   01.10.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_BOX_GENERATOR__
#define __FACEKIT_BOX_GENERATOR__

#include "facekit/core/library_export.hpp"
#include "facekit/sim/generator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
  /**
   *  @class  PositionBoxGenerator
   *  @brief  Randomly generate particles inside a given bounding box
   *  @author Christophe Ecabert
   *  @date   01.10.18
   *  @ingroup sim
   *  @tparam T Data type
   */
template<typename T>
class FK_EXPORTS PositionBoxGenerator : public IGenerator<T> {
 public:
  /** Position type */
  using Pos = Vector3<T>;
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   PositionBoxGenerator
   *  @fn     PositionBoxGenerator(const Pos& origin, const Pos& side)
   *  @brief  Constructor
   *  @param[in] origin Top left corner position
   *  @param[in] side   Bounding box width (length of the sides)
   */
  PositionBoxGenerator(const Pos& origin, const Pos& side);
  
  /**
   *  @name   PositionBoxGenerator
   *  @fn     PositionBoxGenerator(const PositionBoxGenerator& other) = default
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  PositionBoxGenerator(const PositionBoxGenerator& other) = default;
  
  /**
   *  @name   operator=
   *  @fn     PositionBoxGenerator& operator=(const PositionBoxGenerator& rhs) = default
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  PositionBoxGenerator& operator=(const PositionBoxGenerator& rhs) = default;
  
  /**
   *  @name   ~PositionBoxGenerator
   *  @fn     ~PositionBoxGenerator() override = default
   *  @brief  Destructor
   */
  ~PositionBoxGenerator() override = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Generate
   *  @fn     void Generate(const T& dt, const size_t start_id,
                        const size_t& end_id, Particles<T>* particles) override
   *  @brief  Generate particles inside a given bounding box
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
  /** Top left corner */
  Pos min_;
  /** Sides dimensions */
  Pos side_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_BOX_GENERATOR__ */
