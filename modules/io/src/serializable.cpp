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
#include "facekit/io/object_manager.hpp"
#include "facekit/io/file_io.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   StreamHelper
 *  @fn     static int StreamHelper(std::istream& stream,
 const std::string& classname)
 *  @brief  Helper method to search within a stream a given object name.
 *  @param[in,out]  stream  Binary stream to search in.
 *  @param[in]  classname   Object's name to search for
 *  @retunr -1 if no object match the name, 0 otherwise. When returning 0 the
 stream is already at the correct position, therefore object can
 be loaded directly.
 */
int Serializable::StreamHelper(std::istream& stream,
                               const std::string& classname) {
  int err = -1;
  if (stream.good()) {
    size_t id = ObjectManager::Get().GetId(classname);
    err = IO::ScanStream(stream, id);
  }
  return err;
}
  
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

/*
 * @name  Save
 * @fn    int Save(const std::string& filename) const
 * @brief Save to a given \p filename
 * @param[in] filename    Path to the model file
 * @return    -1 if error, 0 otherwise
 */
int Serializable::Save(const std::string& filename) const {
  int err = -1;
  std::ofstream stream(filename.c_str(), std::ios_base::binary);
  if (stream.is_open()) {
    err = this->Save(stream);
  }
  return err;
}
}  // namespace FaceKit
