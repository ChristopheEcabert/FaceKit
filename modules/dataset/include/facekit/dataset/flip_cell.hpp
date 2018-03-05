/**
 *  @file   flip_cell.hpp
 *  @brief Perform flip into horizontal/vertical direction
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_FLIP_CELL__
#define __FACEKIT_FLIP_CELL__

#include "facekit/core/library_export.hpp"
#include "facekit/dataset/augmentation_cell.hpp"
#include "facekit/core/utils/enum_bitmask_operator.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  ImgFlipCell
 *  @brief  Perform flip into horizontal/vertical direction
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  @ingroup dataset
 */
class FK_EXPORTS ImgFlipCell : public AugmentationCell {
 public:
  
#pragma mark -
#pragma mark Type Definition
  
  /**
   *  @enum Direction
   *  @brief  List of possible flip direction
   */
  enum class Direction : uint8_t {
    /** Vertical */
    kVertical = 0x01,
    /** Horizontal */
    kHorizontal = 0x02,
    /** Both */
    kBoth = 0x03
  };
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   ImgFlipCell
   *  @fn     explicit ImgFlipCell(const Direction direction)
   *  @brief  Constructor
   *  @param[in] direction Direction in which to perform the flip
   */
  explicit ImgFlipCell(const Direction direction);
  
  /**
   *  @name   ~ImgFlipCell
   *  @fn     ~ImgFlipCell(void) = default
   *  @brief  Destructor
   */
  ~ImgFlipCell(void) = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Process
   *  @fn     int Process(const std::vector<std::string>& input,
                          const std::string& output,
                          std::vector<std::string>* generated) const
   *  @brief  Run augmentation step on provided data
   *  @param[in]  input     List of file to augment
   *  @param[in]  output    Location where to place the generated samples
   *  @param[out] generated List of path to the generated samples
   *  @return -1 if error, 0 otherwise
   */
  int Process(const std::vector<std::string>& input,
              const std::string& output,
              std::vector<std::string>* generated) const;
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   name
   *  @fn     const char* name(void) const
   *  @brief  Provide name of the operation
   */
  const char* name(void) const {
    return "ImgFlipCell";
  }
  
#pragma mark -
#pragma mark Private
 private:
  /** Flip direction */
  Direction dir_;
};
  
// Enable bitmask operator for nested enum
ENABLE_BITMASK_OPERATORS(ImgFlipCell::Direction);
  
}  // namespace FaceKit
#endif /* __FACEKIT_FLIP_CELL__ */
