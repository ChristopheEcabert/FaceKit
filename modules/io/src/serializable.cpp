/**
 *  @file   serializable.cpp
 *  @brief Interface definition for object that can be saved/loaded into file
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <fstream>

#include "facekit/io/serializable.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 * @name  Load
 * @fn    int Load(const std::string& filename)
 * @brief Load from a given \p filename
 * @param[in] filename    Path to the model file
 * @return    -1 if error, 0 otherwise
 */
int Serializable::Load(const std::string& filename) {
  int err = -1;
  std::ifstream stream(filename.c_str(), std::ios_base::binary);
  if (stream.is_open()) {
    err = this->Load(stream);
  }
  return err;
}
  
  
}  // namespace FaceKit
