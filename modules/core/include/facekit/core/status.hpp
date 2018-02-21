/**
 *  @file   status.hpp
 *  @brief Indicator of success or failure of functions. Strip and adapted from
 *         Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   20.02.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_STATUS__
#define __FACEKIT_STATUS__

#include <string>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  Status
 *  @brief  Indicator of success or failure of functions. Strip and adapted from
 *          Tensorflow
 *  @author Christophe Ecabert
 *  @date   20.02.18
 *  @ingroup core
 */
class FK_EXPORTS Status {
 public:
  
#pragma mark -
#pragma mark Type definition
  
  /**
   *  @enum Type
   *  @brief  List possible error types
   */
  enum class Type : int {
    /** Everything's good */
    kOk = 0,
    /** Something unknown happens */
    kUnknown = -1,
    /** Passed argument are not correct */
    kInvalidArgument = -2,
    /** File/Directory is not found */
    kNotFound = -3,
    /** File/Directory already exists */
    kAlreadyExists = -4,
    /** Out of range */
    kOutOfRange = -5,
    /** Unimplemented, program reached code unavailable */
    kUnimplemented = -6,
    /** Internal, low level system failed */
    kInternalError = -7
  };
    
#pragma mark -
#pragma mark Initialization
    
  /**
   *  @name   Status
   *  @fn     Status(void)
   *  @brief  Constructor
   */
  Status(void);
    
  /**
   *  @name   Status
   *  @fn     Status(const Status& other)
   *  @brief  Copy Constructor
   *  @param[in] other  Object to copy from
   */
  Status(const Status& other);
  
  /**
   *  @name   Status
   *  @fn     Status(const Type& type, const std::string& msg)
   *  @brief  Constructor
   *  @param[in] type Type of error/status
   *  @param[in] msg  Message explaining what went wrong (error description)
   */
  Status(const Type& type, const std::string& msg);
  
  /**
   *  @name   operator=
   *  @fn Status& operator=(const Status& other)
   *  @brief  Assignment operator
   *  @param[in] other  Object to assign from
   *  @return Newly assigned object
   */
  Status& operator=(const Status& other);
    
  /**
   *  @name   ~Status
   *  @fn     ~Status(void) = default
   *  @brief  Destructor
   */
  ~Status(void) = default;
    
#pragma mark -
#pragma mark Usage
    
  /**
   *  @name   operator==
   *  @fn     bool operator==(const Status& other) const
   *  @brief  Equality comparator
   *  @param[in] rhs  Object to compare against
   *  @return True if both object are equal, false otherwise
   */
  bool operator==(const Status& rhs) const;
    
  /**
   *  @name   operator!=
   *  @fn     bool operator!=(const Status& other) const
   *  @brief  Inequality comparator
   *  @param[in] rhs  Object to compare against
   *  @return True if both object are different, false otherwise
   */
  bool operator!=(const Status& rhs) const;
    
  /**
   *  @name   ToString
   *  @fn     std::string ToString(void) const
   *  @brief  Convert status error code into human readable string. Usefull for
   *          debugging
   */
  std::string ToString(void) const;
    
  /**
   *  @name   Code
   *  @fn     const Type& Code(void) const
   *  @brief  Provide error code
   *  @return Error code for this status
   */
  const Type& Code(void) const {
    return err_;
  }
    
  /**
   *  @name   get_message
   *  @fn     const std::string& get_message(void) const
   *  @brief  Give messge associated with this Status.
   *  @return Error description
   */
  const std::string& get_message(void) const {
    return msg_;
  }
  
#pragma mark -
#pragma mark Private
  private:
    /** Error code */
    Type err_;
    /** Message description */
    std::string msg_;
};
  
#pragma mark -
#pragma mark Implementation
  
/*
 *  @name   Status
 *  @fn     Status(void)
 *  @brief  Constructor
 */
inline Status::Status(void) : err_(Type::kOk), msg_("") {}

/*
 *  @name   Status
 *  @fn     Status(const Status& other)
 *  @brief  Copy Constructor
 *  @param[in] other  Object to copy from
 */
inline Status::Status(const Status& other) : err_(other.err_),
                                             msg_(other.msg_) {}
  
/*
 *  @name   operator=
 *  @fn Status& operator=(const Status& other)
 *  @brief  Assignment operator
 *  @param[in] other  Object to assign from
 *  @return Newly assigned object
 */
inline Status& Status::operator=(const Status& other) {
  if (this != &other && err_ != other.err_ && err_ == Type::kOk) {
    err_ = other.err_;
    msg_ = other.msg_;
  }
  return *this;
}
  
/*
 *  @name   operator==
 *  @fn     bool operator==(const Status& other) const
 *  @brief  Equality comparator
 *  @param[in] rhs  Object to compare against
 *  @return True if both object are equal, false otherwise
 */
inline bool Status::operator==(const Status& rhs) const {
  return err_ == rhs.err_ && msg_ == rhs.msg_;
}

/*
 *  @name   operator!=
 *  @fn     bool operator!=(const Status& other) const
 *  @brief  Inequality comparator
 *  @param[in] rhs  Object to compare against
 *  @return True if both object are different, false otherwise
 */
inline bool Status::operator!=(const Status& rhs) const {
  return !(*this == rhs);
}

}  // namespace FaceKit
#endif /* __FACEKIT_STATUS__ */
