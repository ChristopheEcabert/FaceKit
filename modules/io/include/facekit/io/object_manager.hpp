/**
 *  @file   object_manager.hpp
 *  @brief Gather all object's suporting read/write to file with registration 
 *         mechanism. The purpose is to be able to automatically generate new 
 *         unique object IDs.
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_OBJECT_MANAGER__
#define __FACEKIT_OBJECT_MANAGER__

#include <vector>

#include "facekit/core/library_export.hpp"
#include "facekit/io/object_proxy.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  

/**
 *  @class  ObjectManager
 *  @brief  Gather all object's suporting read/write to file with registration
 *          mechanism. The purpose is to be able to automatically generate new
 *          unique object IDs.
 *  @author Christophe Ecabert
 *  @date   24.09.17
 *  @ingroup io
 */
class FK_EXPORTS ObjectManager {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name Get
   *  @fn static ObjectManager& Get(void)
   *  @brief  Provide single a single instance of the manager.
   *  @return Object manager singleton
   */
  static ObjectManager& Get(void);
  
  /**
   *  @name ObjectManager
   *  @fn ObjectManager(const ObjectManager& other) = delete
   *  @brief  Copy constructor
   */
  ObjectManager(const ObjectManager& other) = delete;
  
  /**
   *  @name ObjectManager
   *  @fn ObjectManager(ObjectManager&& other) = delete
   *  @brief  Move constructor
   */
  ObjectManager(ObjectManager&& other) = delete;
  
  /**
   *  @name operator=
   *  @fn ObjectManager& operator=(const ObjectManager& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  ObjectManager& operator=(const ObjectManager& rhs) = delete;
  
  /**
   *  @name operator=
   *  @fn ObjectManager& operator=(ObjectManager&& rhs) = delete
   *  @brief  Move assignment operator
   *  @param[in]  rhs Object to move assign from
   *  @return Newly moved-assigned object
   */
  ObjectManager& operator=(ObjectManager&& rhs) = delete;
  
  /**
   *  @name ~ObjectManager
   *  @fn ~ObjectManager(void) = default
   *  @brief  Destructor
   */
  ~ObjectManager(void) = default;
  
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name Register
   *  @fn void Register(const ObjectProxy* proxy)
   *  @brief  Add new entry in the manager
   *  @param[in]  proxy New proxy to register with this manager
   *  @throw  ::FKError if ID already registered.
   */
  void Register(const ObjectProxy* proxy);
  
  /**
   *  @name GetId
   *  @fn void GetId(const char* classname, size_t* id) const
   *  @brief  Retrive the Id of a given class name
   *  @param[in]  classname Class name to quary
   *  @param[out] id  Object's ID, if none matching class if founded 
   *                  ID=MAX(size_t)
   */
  void GetId(const char* classname, size_t* id) const;
  
 private:
  
  /**
   *  @name ObjectManager
   *  @fn ObjectManager(void)
   *  @brief  Constructor
   */
  ObjectManager(void) = default;
  
  /** Registered proxies */
  std::vector<const ObjectProxy*> proxies_;
};

}  // namespace FaceKit
#endif /* __FACEKIT_OBJECT_MANAGER__ */
