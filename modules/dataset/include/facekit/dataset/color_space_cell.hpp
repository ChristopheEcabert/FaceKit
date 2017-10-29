/**
 *  @file   color_space_cell.hpp
 *  @brief Scale image inside a given color space
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   27.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/library_export.hpp"
#include "facekit/dataset/augmentation_cell.hpp"


#ifndef __FACEKIT_COLOR_SPACE_CELL__
#define __FACEKIT_COLOR_SPACE_CELL__

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
class FK_EXPORTS HSVScalingCell : public AugmentationCell {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   HSVScalingCell
   *  @fn     HSVScalingCell(const double range, const size_t n_sample);
   *  @brief  Constructor
   *  @param[in] range  Scaling factor's range defined as 1 +/- range
   *  @param[in] n_sample  Patch height
   */
  HSVScalingCell(const double range, const size_t n_sample);
  
  /**
   *  @name   ~HSVScalingCell
   *  @fn     ~HSVScalingCell(void) = default
   *  @brief  Destructor
   */
  ~HSVScalingCell(void) = default;
  
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
    return "HSVScalingCell";
  }
  
#pragma mark -
#pragma mark Private
private:
  /** Range */
  double range_;
  /** Number of sample to generate for each image */
  size_t n_sample_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_COLOR_SPACE_CELL__ */
