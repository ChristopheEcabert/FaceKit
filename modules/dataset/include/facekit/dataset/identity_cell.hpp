/**
 *  @file   identity_cell.hpp
 *  @brief Basic augmentation bloc, where input is copied to output locations
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_IDENTITY_CELL__
#define __FACEKIT_IDENTITY_CELL__

#include "facekit/core/library_export.hpp"
#include "facekit/dataset/augmentation_cell.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  IdentityCell
 *  @brief  Basic augmentation bloc, where input is copied to output locations
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  @ingroup dataset
 */
class FK_EXPORTS IdentityCell : public AugmentationCell {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   IdentityCell
   *  @fn     IdentityCell(void) = default
   *  @brief  Constructor
   */
  IdentityCell(void) = default;
  
  /**
   *  @name   ~IdentityCell(void)
   *  @fn     ~IdentityCell(void) = default
   *  @brief  Destructor
   */
  ~IdentityCell(void) = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Process
   *  @fn     int Process(const std::vector<std::string>& input,
                          const std::string& output,
                          std::vector<std::string>* generated) const = 0
   *  @brief  Copy input sample to output location
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
    return "IdentityCell";
  }
  
};

}  // namespace FaceKit
#endif /* __FACEKIT_IDENTITY_CELL__ */
