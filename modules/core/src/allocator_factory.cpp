/**
 *  @file   allocator_factory.cpp
 *  @brief Mechanism to register various type of allocator usabe for container
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   14.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/mem/allocator_factory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name   Get
 *  @fn     static AllocatorFactory& Get(void)
 *  @brief  Get single instance of the factory
 *  @return Allocator factory
 */
AllocatorFactory& AllocatorFactory::Get() {
  static AllocatorFactory factory;
  return factory;
}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Register
 *  @fn     void Register(const std::string& name, Allocator* allocator)
 *  @brief  Add a new allocator in the factory
 *  @param[in]  name  Allocator's name
 *  @param[in]  allocator Allocator to register, factory does not take the
 *              ownership
 *  @return -1 if name already taken, 0 otherwise
 */
int AllocatorFactory::Register(const std::string& name,
                               Allocator* allocator) {
  // Name already used ?
  auto it = allocators_.emplace(name, allocator);
  return it.second ? 0 : -1;
}

/*
 *  @name   GetAllocator
 *  @fn     Allocator* GetAllocator(const std::string& name) const
 *  @brief  Search for a given allocator
 *  @param[in]  name  Allocator's name to search for
 *  @return Allocator instance or nullptr if `name` is not in the factory
 */
Allocator* AllocatorFactory::GetAllocator(const std::string& name) const {
  auto it = allocators_.find(name);
  return it == allocators_.end() ? nullptr : it->second;
}
  
  
  
}  // namespace FaceKit
