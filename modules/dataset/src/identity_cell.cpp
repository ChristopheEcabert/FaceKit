/**
 *  @file   identity_cell.cpp
 *  @brief Basic augmentation bloc, where input is copied to output locations
 *  @ingroup dataset
 *
 *  @author Christophe Ecabert
 *  @date   24.10.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <fstream>

#include "facekit/dataset/identity_cell.hpp"
#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
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
int IdentityCell::Process(const std::vector<std::string>& input,
                          const std::string& output,
                          std::vector<std::string>* generated) const {
  int err = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    // Get filename
    std::string dir, file, ext;
    String::ExtractDirectory(input[i], &dir, &file, &ext);
    // Copy
    std::string dest = output.back() == '/' ? output : output + "/";
    dest += file + "_id." + ext;
    std::ifstream in_stream(input[i].c_str(), std::ios::binary);
    std::ofstream out_stream(dest.c_str(), std::ios::binary);
    if (in_stream.is_open() && out_stream.is_open()) {
      // copy file
      out_stream << in_stream.rdbuf();
      // update generated list
      generated->push_back(dest);
    } else {
      err |= -1;
    }
  }
  return err;
}
  
}  // namespace FaceKit
