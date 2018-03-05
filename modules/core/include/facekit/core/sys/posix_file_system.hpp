/**
 *  @file   posix_file_system.hpp
 *  @brief Interface to access POSIX file system (check if file/dir exist, ...).
 *         Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   02.03.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_POSIX_FILE_SYSTEM__
#define __FACEKIT_POSIX_FILE_SYSTEM__

#include "facekit/core/library_export.hpp"
#include "facekit/core/sys/file_system.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  PosixFileSystem
 *  @brief  A POSIX interface for accessing a file system
 *  @author Christophe Ecabert
 *  @date   02.03.18
 *  @ingroup core
 */
class FK_EXPORTS PosixFileSystem : public FileSystem {
 public:
  
  /**
   *  @name   ~PosixFileSystem
   *  @fn     ~PosixFileSystem(void) = default
   *  @brief  Destructor
   */
  ~PosixFileSystem(void) override = default;
  
  /**
   *  @name   FileExists
   *  @fn     Status FileExist(const std::string& filename) override
   *  @brief  Check if a given path exists
   *  @param[in] filename Path to check existence
   *  @return kGood of kNotFound
   */
  Status FileExist(const std::string& filename) override;
    
  /**
   *  @name   ListDir
   *  @fn     Status ListDir(const std::string& dir,
   *                                 std::vector<std::string>* files) override
   *  @brief  List the content of a given directory `dir`.
   *  @param[in] dir  Directory to scan
   *  @param[out] files List of files/dirs in `dir`
   *  @return kGood or Error code
   */
  Status ListDir(const std::string& dir,
                 std::vector<std::string>* files) override;
  
  /**
   *  @name   FileProp
   *  @fn     Status FileProp(const std::string& filename,
   *                                  FileProperty* prop) override
   *  @brief  Gather file properties for a given file
   *  @param[in] filename Path to the file
   *  @param[out] prop    File's properties
   *  @return kGood, kNotFound or error code.
   */
  Status FileProp(const std::string& filename, FileProperty* prop) override;
  
  /**
   *  @name   DeleteFile
   *  @fn     Status DeleteFile(const std::string& filename) override
   *  @brief  Delete a give file
   *  @param[in] filename File to delete
   *  @return Status of the operation
   */
  Status DeleteFile(const std::string& filename) override;
  
  /**
   *  @name   CreateDir
   *  @fn     Status CreateDir(const std::string& dir) override
   *  @brief  Create a directory
   *  @param[in] dir  path to the directory to create
   *  @return Status of the operation
   */
  Status CreateDir(const std::string& dir) override;
  
  /**
   *  @name   DeleteDir
   *  @fn     Status DeleteDir(const std::string& dir) override
   *  @brief  Delete a directory
   *  @param[in] dir  path to the directory to delete
   *  @return Status of the operation
   */
  Status DeleteDir(const std::string& dir) override;
  
  /**
   *  @name   RenameFile
   *  @fn     Status RenameFile(const std::string& src,
   *                                    const std::string& dst) override;
   *  @brief  Change the name of file given in `src` into `dst`
   *  @param[in] src  Old file's name
   *  @param[in] dst  New file's name
   *  @return Status of the operation
   */
  Status RenameFile(const std::string& src, const std::string& dst) override;
  
  /**
   *  @name   QueryFileSize
   *  @fn     Status QueryFileSize(const std::string& filename, size_t* size) override
   *  @brief  Gather file size in bytes
   *  @param[in] filename File to query size
   *  @param[out] size    File's size
   *  @return Status of the operation
   */
  Status QueryFileSize(const std::string& filename, size_t* size) override;
};
}  // namespace FaceKit
#endif /* __FACEKIT_POSIX_FILE_SYSTEM__ */
