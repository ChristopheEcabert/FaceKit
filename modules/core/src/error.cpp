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
 *  @name Error
 *  @fn Error(const Status& status, const std::string& fcn_name)
 *  @brief  Constructor
 *  @param[in]  status  Status code that generate the exception
 *  @param[in]  fcn_name Name of the function that trigger the exception
 */
Error::Error(const Status& status, const std::string& fcn_name) {
  msg_ = fcn_name + ";Error with " + status.ToString();
}

/*
 *  @name ~Error
 *  @fn virtual ~Error(void) throw()
 *  @brief  Destructor
 */
Error::~Error(void) throw() {
}

#pragma mark -
#pragma mark Usage

/*
*  @name   what
*  @fn virtual const char *what() const throw()
*  @brief  Return description and context of the error
*/
const char* Error::what() const throw() {
  return msg_.c_str();
}

}  // namespace FaceKit
