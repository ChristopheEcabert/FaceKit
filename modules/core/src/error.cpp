/**
 *  @file   error.cpp
 *  @brief  Error class
 *
 *  @author Christophe Ecabert
 *  @date   02/08/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/error.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

#pragma mark -
#pragma mark Initialization

/*
 *  @name OGLError
 *  @fn OGLError(void)
 *  @brief  Constructor
 */
OGLError::OGLError(void) : msg_("") {
}

/*
 *  @name OGLError
 *  @fn OGLError(const Type err_code,
                const std::string& message,
                const std::string& function_name)
 *  @brief  Constructor
 *  @param[in]  err_code      Error code
 *  @param[in]  message       Error message
 *  @param[in]  function_name Name of the function that trigger the exception
 */
OGLError::OGLError(const Type err_code,
                 const std::string& message,
                 const std::string& function_name) {
  msg_ = function_name + " gives error : " + std::to_string(err_code) +
         " with the following message : " + message;
}

/*
 *  @name ~OGLError
 *  @fn virtual ~OGLError(void) throw()
 *  @brief  Destructor
 */
OGLError::~OGLError(void) throw() {
}

#pragma mark -
#pragma mark Usage

/*
*  @name   what
*  @fn virtual const char *what() const throw()
*  @brief  Return description and context of the error
*/
const char* OGLError::what() const throw() {
  return msg_.c_str();
}

}  // namespace FaceKit
