/**
 *  @file   allocator_factory.hpp
 *  @brief Mechanism to register various type of allocator usabe for container
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_ALLOCATOR_FACTORY__
#define __FACEKIT_ALLOCATOR_FACTORY__

#include <unordered_map>

#include "facekit/core/library_export.hpp"
#include "facekit/core/mem/allocator.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  AllocatorFactory
 *  @brief  Factory where available allocator are stored. The factory is a 
 *          process singleton
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  @ingroup core
 */
class FK_EXPORTS AllocatorFactory {
 public:
  
#pragma mark -
#pragma mark Initialization
  
  /**
   *  @name   Get
   *  @fn     static AllocatorFactory& Get(void)
   *  @brief  Get single instance of the factory
   *  @return Allocator factory
   */
  static AllocatorFactory& Get(void);
  
  /**
   *  @name   AllocatorFactory
   *  @fn     AllocatorFactory(const AllocatorFactory& other) = delete
   *  @brief  Copy constructor
   *  @param[in]  other Object to copy from
   */
  AllocatorFactory(const AllocatorFactory& other) = delete;
  
  /**
   *  @name   AllocatorFactory
   *  @fn     AllocatorFactory(AllocatorFactory&& other) = delete
   *  @brief  Move constructor
   *  @param[in]  other Object to move
   */
  AllocatorFactory(AllocatorFactory&& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     AllocatorFactory& operator=(const AllocatorFactory& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in]  rhs Object to assign from
   *  @return Newly assigned object
   */
  AllocatorFactory& operator=(const AllocatorFactory& rhs) = delete;
  
  /**
   *  @name   operator=
   *  @fn     AllocatorFactory& operator=(AllocatorFactory&& rhs) = delete
   *  @brief  Move-Assignment operator
   *  @param[in]  rhs Object to move-assign from
   *  @return Newly moved-assign object
   */
  AllocatorFactory& operator=(AllocatorFactory&& rhs) = delete;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Register
   *  @fn     void Register(const std::string& name, Allocator* allocator)
   *  @brief  Add a new allocator in the factory
   *  @param[in]  name  Allocator's name
   *  @param[in]  allocator Allocator to register, factory does not take the 
   *              ownership
   *  @return -1 if name already taken, 0 otherwise
   */
  int Register(const std::string& name, Allocator* allocator);
  
  /**
   *  @name   GetAllocator
   *  @fn     Allocator* GetAllocator(const std::string& name) const
   *  @brief  Search for a given allocator
   *  @param[in]  name  Allocator's name to search for
   *  @return Allocator instance or nullptr if `name` is not in the factory
   */
  Allocator* GetAllocator(const std::string& name) const;
  
#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   AllocatorFactory
   *  @fn     AllocatorFactory(void)
   *  @brief  Constructor
   */
  AllocatorFactory(void) = default;
  
  /** Registered allocators */
  std::unordered_map<std::string, Allocator*> allocators_;
};
  
/**
 *  @class  AllocatorProxy
 *  @brief  Proxy for registering allocator with factory
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  @ingroup core
 */
class FK_EXPORTS AllocatorProxy {
 public:
 
  /**
   *  @name   AllocatorProxy
   *  @fn     AllocatorProxy(const std::string& name, Allocator* allocator)
   *  @brief  Constructor
   *  @param[in]  name      Allocator's name
   *  @param[in]  allocator Allocator to register, no ownership taken
   */
  AllocatorProxy(const std::string& name, Allocator* allocator) {
    if(AllocatorFactory::Get().Register(name, allocator)) {
      FACEKIT_LOG_ERROR("Allocator with name '" << name <<
                        "' is already registered");
    }
  }
};

/**
 *  @macro  REGISTER_ALLOCATOR
 *  @brief  Register a given allocator
 */
#define REGISTER_ALLOCATOR(name, allocator) \
  REGISTER_ALLOCATOR_UNIQ_HELPER(__COUNTER__, name, allocator)
  
/**
 *  @macro REGISTER_ALLOCATOR_UNIQ_HELPER
 *  @brief  Helper function to create unique allocator proxy
 */
#define REGISTER_ALLOCATOR_UNIQ_HELPER(cnt, name, allocator) \
  REGISTER_ALLOCATOR_UNIQ(cnt, name, allocator)
  
/**
 *  @macro REGISTER_ALLOCATOR_UNIQ
 *  @brief  Create unique allocator proxy
 */
#define REGISTER_ALLOCATOR_UNIQ(cnt, name, allocator) \
  static AllocatorProxy register_allocator_##cnt(name, new allocator)
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_ALLOCATOR_FACTORY__ */
