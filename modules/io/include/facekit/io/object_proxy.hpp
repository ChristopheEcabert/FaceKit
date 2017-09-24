/**
 *  @file   object_proxy.hpp
 *  @brief Registration mechanism for object that need to be readable / writable
 *         into file
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <string>

#ifndef __FACEKIT_OBJECT_PROXY__
#define __FACEKIT_OBJECT_PROXY__

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  ObjectProxy
 *  @brief  Interface for adding class to the object manager via registration 
 *          mechanism. New entry can be add by inherriting this class.
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  @ingroup io
 */
class ObjectProxy {
 public:
  
  /**
   *  @name ObjectProxy
   *  @fn ObjectProxy(const std::string& classname, const size_t id)
   *  @brief  Constructor
   *  @param[in]  classname  Class name to be represented by this proxy
   *  @param[in]  id          Object's unique ID
   */
  ObjectProxy(const std::string& classname, const size_t id);
  
  /**
   *  @name ~ObjectProxy
   *  @fn virtual ~ObjectProxy(void) = default
   *  @brief  Destructor
   */
  virtual ~ObjectProxy(void) = default;
  
  /**
   *  @name get_class_name
   *  @fn const std::string& get_classname(void) const 
   *  @brief  Provide the name of the class represented by this proxy
   *  @return Class name
   */
  const std::string& get_classname(void) const {
    return classname_;
  }
  
  /**
   *  @name get_id
   *  @fn const size_t& get_id(void) const
   *  @brief  Provide the ID of the class represended by this proxy
   *  @return Object's ID
   */
  const size_t& get_id(void) const {
    return id_;
  }
  
 private:
  /** Class represented by this proxy */
  std::string classname_;
  /** Object ID */
  size_t id_;
};
}  // namespace FaceKit
#endif /* __FACEKIT_OBJECT_PROXY__ */
