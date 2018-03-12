/**
 *  @file   error.hpp
 *  @brief  Error class
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   02/08/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ERROR__
#define __FACEKIT_ERROR__

#include <exception>
#include <string>

#include "facekit/core/library_export.hpp"
#include "facekit/core/status.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/**
 *  @class  Error
 *  @brief  Error class
 *  @author Christophe Ecabert
 *  @date   02/08/16
 *  @ingroup core
 */
class FK_EXPORTS Error : public std::exception {
 public:

#pragma mark -
#pragma mark Initialization


  /**
   *  @name Error
   *  @fn Error(const Status& status, const std::string& fcn_name)
   *  @brief  Constructor
   *  @param[in]  status  Status code that generate the exception
   *  @param[in]  fcn_name Name of the function that trigger the exception
   */
  Error(const Status& status, const std::string& fcn_name);

  /**
   *  @name ~Error
   *  @fn virtual ~Error(void) throw()
   *  @brief  Destructor
   */
  virtual ~Error(void) throw();

#pragma mark -
#pragma mark Usage

  /**
   *  @name   what
   *  @fn virtual const char *what() const throw()
   *  @brief  Return description and context of the error
   */
  virtual const char* what() const throw();

#pragma mark -
#pragma mark Protected

 protected:
  /** Error message */
  std::string msg_;
};

}  // namespace FaceKit
#endif /* __FACEKIT_ERROR__ */
