/**
 *  @file   crop_cell.hpp
 *  @brief Extract region from the corner/middle of the images
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   27.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_CROP_CELL__
#define __FACEKIT_CROP_CELL__

#include "facekit/core/library_export.hpp"
#include "facekit/dataset/augmentation_cell.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  ImageCropCell
 *  @brief  Extract region from the corner/middle of the images
 *  @author Christophe Ecabert
 *  @date   27.10.17
 *  @ingroup dataset
 */
class FK_EXPORTS ImageCropCell : public AugmentationCell {
  public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   ImageCropCell
   *  @fn     ImageCropCell(const int width, const int height)
   *  @brief  Constructor
   *  @param[in] width  Patch width
   *  @param[in] height  Patch height
   */
  ImageCropCell(const int width, const int height);
  
  /**
   *  @name   ~ImageCropCell
   *  @fn     ~ImageCropCell(void) = default
   *  @brief  Destructor
   */
  ~ImageCropCell(void) = default;
  
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
    return "ImageCropCell";
  }
  
#pragma mark -
#pragma mark Private
private:
  /** Patch width */
  int width_;
  /** Patch height */
  int height_;
  
};
}  // namespace FaceKit
#endif /* __FACEKIT_CROP_CELL__ */
