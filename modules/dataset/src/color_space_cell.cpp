/**
 *  @file   color_space_cell.cpp
 *  @brief Scale image inside a given color space
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   27.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <chrono>
#include <random>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "facekit/dataset/color_space_cell.hpp"
#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name   HSVScalingCell
 *  @fn     HSVScalingCell(const double range, const size_t n_sample);
 *  @brief  Constructor
 *  @param[in] range  Scaling factor's range defined as 1 +/- range
 *  @param[in] n_sample  Patch height
 */
HSVScalingCell::HSVScalingCell(const double range,
                               const size_t n_sample) : range_(range),
                                                        n_sample_(n_sample) {
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
int HSVScalingCell::Process(const std::vector<std::string>& input,
                            const std::string& output,
                            std::vector<std::string>* generated) const {
  int err = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    // Get filename
    std::string dir, file, ext;
    Path::SplitComponent(input[i], &dir, &file, &ext);
    // Load image + convert to hsv
    cv::Mat img = cv::imread(input[i], cv::ImreadModes::IMREAD_COLOR);
    cv::Mat imgf, hsv_imgf;
    img.convertTo(imgf, CV_64F);
    cv::cvtColor(imgf, hsv_imgf, CV_BGR2HSV);
    
    if (!imgf.empty()) {
      // Create distribution
      std::uniform_real_distribution<float> s_dist(-range_, range_);
      // Create generator
      auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
      std::mt19937 gen(static_cast<std::mt19937::result_type>(seed));
      // Generate new samples
      for (size_t i = 0; i < n_sample_; ++i) {
        // Pick random scale
        double scale =  1.0 + s_dist(gen);
        
        // Apply scaling
        cv::Mat hsv_scaled, sample;
        
        
        
        cv::cvtColor(hsv_scaled, sample, CV_HSV2BGR);
        // Save
        std::string dest = output.back() == '/' ? output : output + "/";
        dest += file + "_hsv" + String::LeadingZero(i, 3) +"." + ext;
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
