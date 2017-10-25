/**
 *  @file   in_plane_rotation_cell.hpp
 *  @brief Generate random in-plane rotation around a random point
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_IN_PLANE_ROTATION_CELL__
#define __FACEKIT_IN_PLANE_ROTATION_CELL__

#include "facekit/core/library_export.hpp"
#include "facekit/dataset/augmentation_cell.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  ImgInPlaneRotationCell
 *  @brief  Generate image with random in-plane rotation around a random point
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  @ingroup dataset
 */
class FK_EXPORTS ImgInPlaneRotationCell : public AugmentationCell {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   ImgInPlaneRotationCell
   *  @fn     ImgInPlaneRotationCell(const double range, 
                                       const size_t n_sample)
   *  @brief  Constructor
   *  @param[in] range    Angle upper bound. Range is defined within
   *                      [-range, range] and angles are uniformly sampled in it
   *  @param[in] n_sample Number of sample to generate for each image
   */
  ImgInPlaneRotationCell(const double range, const size_t n_sample);
  
  /**
   *  @name   ~ImgInPlaneRotationCell
   *  @fn     ~ImgInPlaneRotationCell(void) = default
   *  @brief  Destructor
   */
  ~ImgInPlaneRotationCell(void) = default;
  
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
    return "ImgInPlaneRotationCell";
  }
  
#pragma mark -
#pragma mark Private
private:
  /** Angle's range */
  double range_;
  /** Number of sample to generate */
  size_t n_sample_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_IN_PLANE_ROTATION_CELL__ */
