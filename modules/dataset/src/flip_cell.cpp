/**
 *  @file   flip_cell.cpp
 *  @brief Perform flip into horizontal/vertical direction
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

#include "facekit/dataset/flip_cell.hpp"
#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name   ImgFlipCell
 *  @fn     explicit ImgFlipCell(const Direction direction)
 *  @brief  Constructor
 *  @param[in] direction Direction in which to perform the flip
 */
ImgFlipCell::ImgFlipCell(const Direction direction) : dir_(direction) {
}
  
#pragma mark -
#pragma mark Usage
  
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
int ImgFlipCell::Process(const std::vector<std::string>& input,
                         const std::string& output,
                         std::vector<std::string>* generated) const {
  int err = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    // Get filename
    std::string dir, file, ext;
    String::ExtractDirectory(input[i], &dir, &file, &ext);
    // Load image
    cv::Mat img = cv::imread(input[i], cv::ImreadModes::IMREAD_COLOR);
    if (!img.empty()) {
      if ((dir_ & Direction::kHorizontal) == Direction::kHorizontal) {
        // Horizontal flip
        cv::Mat himg;
        cv::flip(img, himg, 1);
        // Save it
        std::string dest = output.back() == '/' ? output : output + "/";
        dest += file + "_fh." + ext;
        cv::imwrite(dest, himg);
        generated->push_back(dest);
      }
      if ((dir_ & Direction::kVertical) == Direction::kVertical) {
        // Vertical flip
        cv::Mat vimg;
        cv::flip(img, vimg, 0);
        std::string dest = output.back() == '/' ? output : output + "/";
        dest += file + "_fv." + ext;
        cv::imwrite(dest, vimg);
        generated->push_back(dest);
      }
    } else {
      err |= -1;
    }
  }
  return err;
}
  
}  // namespace FaceKit
