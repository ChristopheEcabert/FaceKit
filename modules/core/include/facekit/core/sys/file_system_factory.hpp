/**
 *  @file   file_system_factory.hpp
 *  @brief File system factory
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   05.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_FILE_SYSTEM_FACTORY__
#define __FACEKIT_FILE_SYSTEM_FACTORY__

#include <unordered_set>

#include "facekit/core/library_export.hpp"
#include "facekit/core/sys/file_system.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  FileSystemFactory
 *  @brief  Mechanism to store file systems implementations
 *  @author Christophe Ecabert
 *  @date   05.03.02
 *  @ingroup core
 */
class FK_EXPORTS FileSystemFactory {
 public:
  
#pragma mark - 
#pragma mark Initialization
  
  /**
   *  @name Get
   *  @fn static FileSystemFactory& Get(void)
   *  @brief  Provide access to single instance of FileSystemFactory
   */
  static FileSystemFactory& Get(void);
  
  /**
   *  @name   ~FileSystemFactory
   *  @fn     ~FileSystemFactory(void)
   *  @brief  Destructor
   */
  ~FileSystemFactory(void);
  
  /**
   *  @name   FileSystemFactory
   *  @fn     FileSystemFactory(const FileSystemFactory& other) = delete
   *  @brief  Copy constructor
   */
  FileSystemFactory(const FileSystemFactory& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     FileSystemFactory& operator=(const FileSystemFactory& rhs) = delete
   *  @brief  Copy assignment
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  FileSystemFactory& operator=(const FileSystemFactory& rhs) = delete;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   Register
   *  @fn     void Register(const FileSystemProxy* proxy)
   *  @brief  Register a new file system into the factory
   *  @param[in]  proxy File system's proxy (from macro REGISTER_FILE_SYSTEM)
   */
  void Register(const FileSystemProxy* proxy);
  
  /**
   *  @name   Retrieve
   *  @fn     FileSystem* Retrieve(const std::string& name) const
   *  @brief  Retrieve file system corresponding to a given name or nullptr
   *  @return File system instance or nullptr.
   */
  FileSystem* Retrieve(const std::string& name) const;
  
#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   FileSystemFactory
   *  @fn     FileSystemFactory(void) = default
   *  @brief  Constructor
   */
  FileSystemFactory(void) = default;
  
  
  /** Registered FileSystem */
  std::unordered_set<const FileSystemProxy*> proxies_;
};
  
/**
 *  @def  REGISTER_FILE_SYSTEM
 *  @brief  Register a given file_system
 */
#define REGISTER_FILE_SYSTEM(name, file_system)                               \
  static file_system file_system##Instance;                                   \
  class file_system##Proxy : public FileSystemProxy {                         \
  public:                                                                     \
  file_system##Proxy(void) : FileSystemProxy() {}                             \
  ~file_system##Proxy(void) override = default;                               \
  FileSystem* Create(void) const override { return &file_system##Instance; }  \
  const char* Name(void) const override { return name; }                      \
  };                                                                          \
  static file_system##Proxy file_system##Registrator;

}  // namespace FaceKit
#endif /* __FACEKIT_FILE_SYSTEM_FACTORY__ */
