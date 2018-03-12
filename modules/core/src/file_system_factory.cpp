/**
 *  @file   file_system_factory.cpp
 *  @brief File system factory
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   05.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/sys/file_system_factory.hpp"
#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
#pragma mark -
#pragma mark Initialization
  
/*
 *  @name Get
 *  @fn static FileSystemFactory& Get(void)
 *  @brief  Provide access to single instance of FileSystemFactory
 */
FileSystemFactory& FileSystemFactory::Get(void) {
  static FileSystemFactory factory;
  return factory;
}
  
/*
 *  @name   ~FileSystemFactory
 *  @fn     ~FileSystemFactory(void)
 *  @brief  Destructor
 */
FileSystemFactory::~FileSystemFactory(void) {}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   Register
 *  @fn     void Register(const FileSystemProxy* proxy)
 *  @brief  Register a new file system into the factory
 *  @param[in]  proxy File system's proxy (from macro REGISTER_FILE_SYSTEM)
 */
void FileSystemFactory::Register(const FileSystemProxy* proxy) {
  
  // Already registered ?
  auto it = proxies_.find(proxy);
  if (it == proxies_.end()) {
    // No register
    proxies_.insert(proxy);
  } else {
    FACEKIT_LOG_ERROR("FileSystem already registered");
  }
}
  
/*
 *  @name   Retrieve
 *  @fn     FileSystem* Retrieve(const std::string& name) const
 *  @brief  Retrieve file system corresponding to a given name or nullptr
 *  @return File system instance or nullptr.
 */
FileSystem* FileSystemFactory::Retrieve(const std::string& name) const {
  FileSystem* fs = nullptr;
  for (const auto& p : proxies_) {
    if (std::string(p->Name()) == name) {
      fs = p->Create();
      break;
    }
  }
  return fs;
}
  
}  // namespace FaceKit


