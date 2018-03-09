/**
 *  @file   image_factory.hpp
 *  @brief  Helper class to load images into memory
 *  @ingroup io
 
 *  @author Christophe Ecabert
 *  @date   05.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_IMAGE_FACOTRY__
#define __FACEKIT_IMAGE_FACOTRY__

#include <unordered_set>

#include "facekit/core/library_export.hpp"
#include "facekit/io/image.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  ImageFactory
 *  @brief  Singleton helper for image create. New image type can be added with 
 *          registration mechanism
 *  @author Christophe Ecabert
 *  @date   11.03.17
 *  @ingroup io
 *  @see http://www.drdobbs.com/cpp/self-registering-objects-in-c/184410633
 */
class FK_EXPORTS ImageFactory {
 
public:
  
  /**
   *  @name Get
   *  @fn static ImageFactory& Get(void)
   *  @brief  Provide access to single instance of ImageFactory
   */
  static ImageFactory& Get(void);
  
  /**
   *  @name   ~ImageFactory
   *  @fn     ~ImageFactory(void) = default
   *  @brief  Destructor
   */
  ~ImageFactory(void) = default;
  
  /**
   *  @name   ImageFactory
   *  @fn     ImageFactory(const ImageFactory& other) = delete;
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  ImageFactory(const ImageFactory& other) = delete;
  
  /**
   *  @name operator=
   *  @fn ImageFactory& operator=(const ImageFactory& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned operator
   */
  ImageFactory& operator=(const ImageFactory& rhs) = delete;
  
  /** 
   *  @name CreateByExtension
   *  @fn Image* CreateByExtension(const std::string& extension)
   *  @brief  Create an image based on the extension type
   *  @param[in]  extension  Image extension (type)
   *  @return Pointer to an instance of the given type, nullptr if type is 
   *  unknown
   */
  Image* CreateByExtension(const std::string& extension);
  
  /**
   *  @name Register
   *  @fn void Register(const ImageProxy* object)
   *  @brief  Register a type of image with a given proxy.
   *  @param[in]  object  Object to register
   */
  void Register(const ImageProxy* object);
  
 private:
  
  /**
   *  @name   ImageFactory
   *  @fn     ImageFactory(void)
   *  @brief  Constructor
   */
  ImageFactory(void) = default;
  
  /** List of all unique registered image's proxies */
  std::unordered_set<const ImageProxy*> proxies_;
};
  
/**
 *  @def  REGISTER_IMAGE_IMPL
 *  @brief  Create proxy for a given image implementation type
 */
#define REGISTER_IMAGE_IMPL(img, ext)                             \
  class img##Proxy : public ImageProxy {                          \
   public:                                                        \
    img##Proxy() : ImageProxy() {}                                \
    ~img##Proxy() override = default;                             \
    Image* Create(void) const override { return new img(); }      \
    const char* Extension(void) const override { return ext; }    \
  };                                                              \
  static img##Proxy img##Registry;

}  // namespace FaceKit
#endif /* __FACEKIT_IMAGE_FACOTRY__ */
