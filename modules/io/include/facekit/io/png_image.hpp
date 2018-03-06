/**
 *  @file   png_image.hpp
 *  @brief PNG Image object
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   06.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_PNG_IMAGE__
#define __FACEKIT_PNG_IMAGE__

#include <iostream>

#include "facekit/core/library_export.hpp"
#include "facekit/io/image.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  PNGImage
 *  @brief  PNG Image object
 *  @author Christophe Ecabert
 *  @date   05.03.18
 *  @ingroup io
 */
class FK_EXPORTS PNGImage : public Image {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name PNGImage
   *  @fn PNGImage(void)
   *  @brief  Constructor
   */
  PNGImage(void);
  
  /**
   *  @name PNGImage
   *  @fn PNGImage(const PNGImage& other) = delete
   *  @brief  Copy Constructor
   *  @param[in]  other Object to copy
   */
  PNGImage(const PNGImage& other) = delete;
  
  /**
   *  @name operator=
   *  @fn PNGImage& operator=(const PNGImage& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign form
   *  @return Newly assign object
   */
  PNGImage& operator=(const PNGImage& rhs) = delete;
  
  /**
   *  @name ~PNGImage
   *  @fn ~PNGImage(void) override
   *  @brief  Destructor
   */
  ~PNGImage(void) override;
  
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
#endif /* __FACEKIT_PNG_IMAGE__ */
