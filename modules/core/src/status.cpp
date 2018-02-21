/**
 *  @file   status.cpp
 *  @brief <#description≤#>
 *  @ingroup <#group#>
 *
 *  @author Christophe Ecabert
 *  @date   20.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <cassert>

#include "facekit/core/status.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name   Status
 *  @fn     Status(const Type& type, const std::string& msg)
 *  @brief  Constructor
 *  @param[in] type Type of error/status
 *  @param[in] msg  Message explaining what went wrong (error description)
 */
Status::Status(const Type& type,
               const std::string& msg) : err_(type), msg_(msg) {
  assert(type != Type::kOk);
}
  
/*
 *  @name   ToString
 *  @fn     std::string ToString(void) const
 *  @brief  Convert status error code into human readable string. Usefull for
 *          debugging
 */
std::string Status::ToString(void) const {
  std::string str;
  if (err_ == Type::kOk) {
    str = "Ok";
  } else {
    switch (err_) {
        case Type::kUnknown: str = "Unknown: ";
        break;
        case Type::kInvalidArgument: str = "Invalid argument: ";
        break;
        case Type::kNotFound: str = "Not found: ";
        break;
        case Type::kAlreadyExists: str = "Already exists: ";
        break;
        case Type::kOutOfRange: str = "Out of range: ";
        break;
        case Type::kUnimplemented: str = "Unimplemented: ";
        break;
        case Type::kInternalError: str = "Internal error: ";
        break;
      default:  str="Unknown code (" + std::to_string((int)err_) + "): ";
        break;
    }
    str += msg_;
  }
  return str;
}
  
  
}  // namespace FaceKit
