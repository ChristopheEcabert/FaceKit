/**
 *  @file   tga_image.hpp
 *  @brief TGA Image object
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   06.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_TGA_IMAGE__
#define __FACEKIT_TGA_IMAGE__

#include "facekit/core/library_export.hpp"
#include "facekit/io/image.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/** Forward declare header */
struct TGAHeader;
  
/**
 *  @class  TGAImage
 *  @brief  TGA Image object
 *  @author Christophe Ecabert
 *  @date   06.03.18
 *  @ingroup io
 */
class FK_EXPORTS TGAImage : public Image {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name TGAImage
   *  @fn TGAImage(void)
   *  @brief  Constructor
   */
  TGAImage(void);
  
  /**
   *  @name TGAImage
   *  @fn TGAImage(const TGAImage& other) = delete
   *  @brief  Copy Constructor
   *  @param[in]  other Object to copy
   */
  TGAImage(const TGAImage& other) = delete;
  
  /**
   *  @name operator=
   *  @fn TGAImage& operator=(const TGAImage& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign form
   *  @return Newly assign object
   */
  TGAImage& operator=(const TGAImage& rhs) = delete;
  
  /**
   *  @name ~TGAImage
   *  @fn ~TGAImage(void) override
   *  @brief  Destructor
   */
  ~TGAImage(void) override;
  
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
#pragma mark Initialization
 private:
  /** Image header */
  TGAHeader* header_;
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_TGA_IMAGE__ */
