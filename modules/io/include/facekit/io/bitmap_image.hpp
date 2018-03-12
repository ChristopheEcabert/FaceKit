/**
 *  @file   bitmap_image.hpp
 *  @brief Bitmap Image Object
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   07.03.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_BITMAP_IMAGE__
#define __FACEKIT_BITMAP_IMAGE__

#include "facekit/core/library_export.hpp"
#include "facekit/io/image.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/** Forward BMPHeader */
class BMPHeader;
  
/**
 *  @class  BMPImage
 *  @brief  Bitmap Image object
 *  @author Christophe Ecabert
 *  @date   07.03.18
 *  @ingroup io
 */
class BMPImage : public Image {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name BMPImage
   *  @fn BMPImage(void)
   *  @brief  Constructor
   */
  BMPImage(void);
  
  /**
   *  @name BMPImage
   *  @fn BMPImage(const BMPImage& other) = delete
   *  @brief  Copy Constructor
   *  @param[in]  other Object to copy
   */
  BMPImage(const BMPImage& other) = delete;
  
  /**
   *  @name operator=
   *  @fn BMPImage& operator=(const BMPImage& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign form
   *  @return Newly assign object
   */
  BMPImage& operator=(const BMPImage& rhs) = delete;
  
  /**
   *  @name ~BMPImage
   *  @fn ~BMPImage(void)
   *  @brief  Destructor
   */
  ~BMPImage(void) override;
  
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
  
#pragma mark -
#pragma mark Private
 private:
  /** Header */
  BMPHeader* header_;
  
};
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_BITMAP_IMAGE__ */
