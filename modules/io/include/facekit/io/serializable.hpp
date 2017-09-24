/**
 *  @file   serializable.hpp
 *  @brief Interface definition for object that can be saved/loaded into file
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_SERIALIZABLE__
#define __FACEKIT_SERIALIZABLE__

#include <iostream>
#include <string>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  Serializable
 *  @brief  Interface definition for object that can be saved/loaded into file
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  @ingroup io
 */
class FK_EXPORTS Serializable {
 public:
  
  /**
   * @name  Load
   * @fn    int Load(const std::string& filename)
   * @brief Load from a given \p filename
   * @param[in] filename    Path to the model file
   * @return    -1 if error, 0 otherwise
   */
  int Load(const std::string& filename);
  
  /**
   * @name  Load
   * @fn    virtual int Load(std::istream& stream) = 0
   * @brief Load from a given binary \p stream
   * @param[in] stream Binary stream to load model from
   * @return    -1 if error, 0 otherwise
   */
  virtual int Load(std::istream& stream) = 0;
  
  /**
   * @name  Save
   * @fn    int Save(const std::string& filename) const
   * @brief Save to a given \p filename
   * @param[in] filename    Path to the model file
   * @return    -1 if error, 0 otherwise
   */
  int Save(const std::string& filename) const;
  
  /**
   * @name  Save
   * @fn    virtual int Save(std::ostream& stream) const = 0;
   * @brief Save to a given binary \p stream
   * @param[in] stream Binary stream to save model from
   * @return    -1 if error, 0 otherwise
   */
  virtual int Save(std::ostream& stream) const = 0;
  
  /**
   * @name  ComputeObjectSize
   * @fn    virtual size_t ComputeObjectSize(void) const
   * @brief Compute object size in byte
   * @return    Object's size
   */
  virtual size_t ComputeObjectSize(void) const = 0;
};

}  // namespace FaceKit
#endif /* __FACEKIT_SERIALIZABLE__ */
