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
 *  @name FKError
 *  @fn FKError(void)
 *  @brief  Constructor
 */
FKError::FKError(void) : msg_("") {
}

/*
 *  @name FKError
 *  @fn FKError(const Type err_code,
                const std::string& message,
                const std::string& function_name)
 *  @brief  Constructor
 *  @param[in]  err_code      Error code
 *  @param[in]  message       Error message
 *  @param[in]  function_name Name of the function that trigger the exception
 */
FKError::FKError(const Type err_code,
                 const std::string& message,
                 const std::string& function_name) {
  msg_ = function_name + " gives error : " + std::to_string(err_code) +
         " with the following message : " + message;
}

/*
 *  @name ~FKError
 *  @fn virtual ~FKError(void) throw()
 *  @brief  Destructor
 */
FKError::~FKError(void) throw() {
}

#pragma mark -
#pragma mark Usage

/*
*  @name   what
*  @fn virtual const char *what() const throw()
*  @brief  Return description and context of the error
*/
const char* FKError::what() const throw() {
  return msg_.c_str();
}

}  // namespace FaceKit
