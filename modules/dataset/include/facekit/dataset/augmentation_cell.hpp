/**
 *  @file   augmentation_cell.hpp
 *  @brief Interface for various augementation cells
 *  @ingroup datasets
 *
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_AUGMENTATION_CELL__
#define __FACEKIT_AUGMENTATION_CELL__

#include <vector>
#include <string>

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  AugmentationCell
 *  @brief  Interface for various augementation cells
 *  @author Christophe Ecabert
 *  @date   24.10.2017
 *  @ingroup dataset
 */
class AugmentationCell {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   ~AugmentationCell
   *  @fn     virtual ~AugmentationCell(void)
   *  @brief  Destructor
   */
  virtual ~AugmentationCell(void) {}
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Process
   *  @fn     virtual int Process(const std::vector<std::string>& input, 
                                  const std::string& output,
                                  std::vector<std::string>* generated) const = 0
   *  @brief  Run augmentation step on provided data
   *  @param[in]  input     List of file to augment
   *  @param[in]  output    Location where to place the generated samples
   *  @param[out] generated List of path to the generated samples
   *  @return -1 if error, 0 otherwise
   */
  virtual int Process(const std::vector<std::string>& input,
                      const std::string& output,
                      std::vector<std::string>* generated) const = 0;
  
#pragma mark -
#pragma mark Accessors
  
  /**
   *  @name   name
   *  @fn     virtual const char* name(void) const = 0
   *  @brief  Provide name of the operation
   */
  virtual const char* name(void) const = 0;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_AUGMENTATION_CELL__ */
