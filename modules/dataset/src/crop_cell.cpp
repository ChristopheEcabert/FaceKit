/**
 *  @file   crop_cell.cpp
 *  @brief Extract region from the corner/middle of the images
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   27.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

#include "facekit/dataset/crop_cell.hpp"
#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   ImageCropCell
 *  @fn     ImageCropCell(const int width, const int height)
 *  @brief  Constructor
 *  @param[in] width  Patch width
 *  @param[in] height  Patch height
 */
ImageCropCell::ImageCropCell(const int width,
                             const int height) :  width_(width),
                                                  height_(height) {
}
  
/*
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
int ImageCropCell::Process(const std::vector<std::string>& input,
                           const std::string& output,
                           std::vector<std::string>* generated) const {
  int err = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    // Get filename
    std::string dir, file, ext;
    String::ExtractDirectory(input[i], &dir, &file, &ext);
    // Load image
    cv::Mat img = cv::imread(input[i], cv::ImreadModes::IMREAD_COLOR);
    if (!img.empty() && img.cols > width_ && img.rows > height_) {
      cv::Rect roi;
      roi.width = width_;
      roi.height = height_;
      for (int i = 0; i < 5; ++i) {
        // Compute ROI
        switch (i) {
          // Top left
          case 0: roi.x = 0;
                  roi.y = 0;
            break;
            
          // Top right
          case 1: roi.x = img.cols - width_ - 1;
                  roi.y = 0;
            break;
            
          // Bottom left
          case 2: roi.x = 0;
                  roi.y = img.rows - height_ - 1;
            break;
          // Bottom Right
          case 3: roi.x = img.cols - width_ - 1;
                  roi.y = img.rows - height_ - 1;
            break;
          // Center
          case 4: roi.x = (img.cols - width_) / 2;
                  roi.y = (img.rows - height_) / 2;
            break;
        }
        // Extract region
        cv::Mat sample = img(roi).clone();
        // Save
        std::string dest = output.back() == '/' ? output : output + "/";
        dest += file + "_crop" + String::LeadingZero(i, 3) +"." + ext;
        cv::imwrite(dest, sample);
        generated->push_back(dest);
      }
    } else {
      err |= -1;
    }
  }
  
  return err;
}
  
  
  
}  // namespace FaceKit
