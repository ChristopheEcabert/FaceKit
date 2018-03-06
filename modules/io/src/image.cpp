/**
 *  @file   image.cpp
 *  @brief  Image object registration mechanism
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   05.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/io/image.hpp"
#include "facekit/io/image_factory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
  
/*
 *  @name Load
 *  @fn Status Load(const std::string& filename)
 *  @brief  Load image from dist
 *  @param[in]  filename  Path to ressource on the disk
 *  @return Operation status
 */
Status Image::Load(const std::string& filename) {
  Status status;
  std::ifstream stream(filename.c_str(),
                       std::ios_base::in | std::ios_base::binary);
  if (stream.is_open()) {
    status = this->Load(stream);
  } else {
    status = Status(Status::Type::kInvalidArgument,
                    "Can not open: " + filename);
  }
  return status;
}
  
/*
 *  @name Save
 *  @fn virtual Status Save(const std::string& filename) const
 *  @brief  Save image to dist
 *  @param[in]  filename  Path to ressource on the disk
 *  @return Operation status
 */
Status Image::Save(const std::string& filename) const {
  Status status;
  std::ofstream stream(filename.c_str(),
                       std::ios_base::out | std::ios_base::binary);
  if (stream.is_open()) {
    status = this->Save(stream);
  } else {
    status = Status(Status::Type::kInvalidArgument,
                    "Can not open: " + filename);
  }
  return status;
}
  
/*
 *  @name   ImageProxy
 *  @fn     ImageProxy(void)
 *  @brief  Constructor
 */
ImageProxy::ImageProxy(void) {
  ImageFactory::Get().Register(this);
}
  
}  // namespace FaceKit
  

