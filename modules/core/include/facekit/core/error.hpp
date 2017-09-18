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

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/**
 *  @class  FKError
 *  @brief  Error class
 *  @author Christophe Ecabert
 *  @date   02/08/16
 *  @ingroup core
 */
class FK_EXPORTS FKError : public std::exception {

 public:

#pragma mark -
#pragma mark Type definition

  /**
   *  @enum Type
   *  @brief  Type of error
   */
  enum Type {
    /** Generic */
    kGeneric = -1,
    /** Opening file */
    kFileOpening = -2,
    /** Reading data */
    kReadingData = -3
  };

#pragma mark -
#pragma mark Initialization

  /**
   *  @name FKError
   *  @fn FKError(void)
   *  @brief  Constructor
   */
  FKError(void);

  /**
   *  @name FKError
   *  @fn FKError(const Type err_code, 
                  const std::string& message,
                  const std::string& function_name)
   *  @brief  Constructor
   *  @param[in]  err_code      Error code
   *  @param[in]  message       Error message
   *  @param[in]  function_name Name of the function that trigger the exception
   */
  FKError(const Type err_code,
          const std::string& message,
          const std::string& function_name);

  /**
   *  @name ~FKError
   *  @fn virtual ~FKError(void) throw()
   *  @brief  Destructor
   */
  virtual ~FKError(void) throw();

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
