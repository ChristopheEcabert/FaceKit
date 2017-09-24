/**
 *  @file   object_manager.cpp
 *  @brief Gather all object's suporting read/write to file with registration
 *         mechanism
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <algorithm>
#include <limits>
#include <cstring>

#include "facekit/io/object_manager.hpp"
#include "facekit/core/error.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name Get
 *  @fn static ObjectManager& Get(void)
 *  @brief  Provide single a single instance of the manager.
 *  @return Object manager singleton
 */
ObjectManager& ObjectManager::Get(void) {
  static ObjectManager manager;
  return manager;
}
  
#pragma mark -
#pragma mark Usage
  
  
/*
 *  @name Register
 *  @fn void Register(const ObjectProxy* proxy)
 *  @brief  Add new entry in the manager
 *  @param[in]  proxy New proxy to register with this manager
 *  @throw  ::FKError if ID already registered.
 */
void ObjectManager::Register(const ObjectProxy* proxy) {
  // Trying to add an existing ID ?
  const size_t& id = proxy->get_id();
  auto it = std::find_if(proxies_.begin(),
                         proxies_.end(),
                         [&](const ObjectProxy* p) {
                           return p->get_id() == id ? true : false;
                         });
  if (it == proxies_.end()) {
    // Not registered -> add it
    proxies_.push_back(proxy);
  } else {
    // Already in, throw execption
    std::string msg = "Object with ID: " + std::to_string(id);
    msg += " has already been registered, please choose a different ID";
    throw FKError(FKError::Type::kGeneric,
                  msg,
                  FUNC_NAME);
  }
}
  
/*
 *  @name GetId
 *  @fn void GetId(const char* classname, size_t* id) const
 *  @brief  Retrive the Id of a given class name
 *  @param[in]  classname Class name to quary
 *  @param[out] id  Object's ID, if none matching class if founded
 *                  ID=MAX(size_t)
 */
void ObjectManager::GetId(const char* classname, size_t* id) const {
  *id = std::numeric_limits<size_t>::max();
  for (const auto* p : proxies_) {
    if (strcmp(classname, p->get_classname().c_str()) == 0) {
      *id = p->get_id();
      break;
    }
  }
}
  
}  // namespace FaceKit
