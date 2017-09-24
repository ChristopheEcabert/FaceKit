/**
 *  @file   object_proxy.cpp
 *  @brief Registration mechanism for object that need to be readable / writable
 *         into file
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include "facekit/io/object_proxy.hpp"
#include "facekit/io/object_manager.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name ObjectProxy
 *  @fn ObjectProxy(const std::string& classname, const size_t id)
 *  @brief  Constructor
 *  @param[in]  classname  Class name to be represented by this proxy
 *  @param[in]  id          Object's unique ID
 */
ObjectProxy::ObjectProxy(const std::string& classname,
                         const size_t id) : classname_(classname), id_(id) {
  ObjectManager::Get().Register(this);
}
  
}  // namespace FaceKit
