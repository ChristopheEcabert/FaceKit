/**
 *  @file   jpeg_image.hpp
 *  @brief JPEG Image object
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   06.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_JPEG_IMAGE__
#define __FACEKIT_JPEG_IMAGE__

#include "facekit/io/image.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
  
/**
 *  @class  JPEGImage
 *  @brief  JPEG Image object
 *  @author Christophe Ecabert
 *  @date   06.03.18
 *  @ingroup io
 */
class FK_EXPORTS JPEGImage : public Image {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name JPEGImage
   *  @fn JPEGImage(void) = default
   *  @brief  Constructor
   */
  JPEGImage(void) = default;
  
  /**
   *  @name JPEGImage
   *  @fn JPEGImage(const JPEGImage& other) = delete
   *  @brief  Copy Constructor
   *  @param[in]  other Object to copy
   */
  JPEGImage(const JPEGImage& other) = delete;
  
  /**
   *  @name operator=
   *  @fn JPEGImage& operator=(const JPEGImage& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign form
   *  @return Newly assign object
   */
  JPEGImage& operator=(const JPEGImage& rhs) = delete;
  
  /**
   *  @name ~JPEGImage
   *  @fn ~JPEGImage(void) override = default
   *  @brief  Destructor
   */
  ~JPEGImage(void) override = default;
  
  /**
   *  @name Load
   *  @fn Status Load(std::istream& stream) override
   *  @brief  Load image from dist
   *  @param[in]  stream  Binary stream from where to load the ressource
   *  @return Operation status
   */
  Status Load(std::istream& stream) override;
  
  /**
   *  @name Save
   *  @fn Status Save(std::ostream& stream) const override
   *  @brief  Load image to dist
   *  @param[in]  stream  Binary stream to where to save the ressource
   *  @return Operation status
   */
  Status Save(std::ostream& stream) const override;
};
}  // namespace FaceKit
#endif /* __FACEKIT_JPEG_IMAGE__ */
