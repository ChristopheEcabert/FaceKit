/**
 *  @file   in_plane_rotation_cell.cpp
 *  @brief Generate random in-plane rotation around a random point
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   25.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <chrono>
#include <random>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "facekit/dataset/in_plane_rotation_cell.hpp"
#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   ImgInPlaneRotationCell
 *  @fn     ImgInPlaneRotationCell(const double range, const size_t n_sample)
 *  @brief  Constructor
 *  @param[in] range    Angle upper bound. Range is defined within
 *                      [-range, range] and angles are uniformly sampled in it
 *  @param[in] n_sample Number of sample to generate for each image
 */
ImgInPlaneRotationCell::ImgInPlaneRotationCell(const double range,
                                                   const size_t n_sample) :
range_(range), n_sample_(n_sample) {
}
  
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
int ImgInPlaneRotationCell::Process(const std::vector<std::string>& input,
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
      // Create distribution
      const float cx = float(img.cols) / 2.f;
      const float cy = float(img.rows) / 2.f;
      const float r = float(std::min(img.cols, img.rows)) / 2.f;
      std::uniform_real_distribution<float> p_dist(0, r/2.f);
      std::uniform_real_distribution<double> ang_dist(-range_, range_);
      // Create generator
      auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
      std::mt19937 gen(static_cast<std::mt19937::result_type>(seed));
      // Generate new samples
      for (size_t i = 0; i < n_sample_; ++i) {
        // Pick rotation enter
        cv::Point2f p;
        p.x = cx + p_dist(gen);
        p.y = cy + p_dist(gen);
        // Select rotation angle
        double ang = ang_dist(gen);
        // Compute rotation matrix
        cv::Mat rot = cv::getRotationMatrix2D(p, ang, 1.0);
        // Warp
        cv::Mat sample;
        cv::warpAffine(img,
                       sample,
                       rot,
                       cv::Size(img.cols, img.rows),
                       cv::INTER_LINEAR);
        // Save
        std::string dest = output.back() == '/' ? output : output + "/";
        dest += file + "_rot" + String::LeadingZero(i, 3) +"." + ext;
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
