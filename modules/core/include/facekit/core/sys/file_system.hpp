/**
 *  @file   file_system.hpp
 *  @brief Interface to access file system (check if file/dir exist, ...).
 *         Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   02.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_FILE_SYSTEM__
#define __FACEKIT_FILE_SYSTEM__

#include <vector>
#include <string>
#include <limits>


#include "facekit/core/status.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @struct  FileProperty
 *  @brief  Hold file properties
 *  @author Christophe Ecabert
 *  @date   02.03.18
 *  @ingroup core
 */
struct FK_EXPORTS FileProperty {
  /** File size, or size_t::max() if not supported */
  size_t size = std::numeric_limits<size_t>::max();
  /** Directory flags */
  bool is_dir = false;
  
  /**
   *  @name   FileProperty
   *  @fn     FileProperty(void) = default
   *  @brief  Constructor
   */
  FileProperty(void) = default;
  
  /**
   *  @name   FileProperty
   *  @fn     FileProperty(void) = default
   *  @brief  Constructor
   */
  FileProperty(const size_t& sz, const bool is_dir) : size(sz), is_dir(is_dir) {}
  
  /**
   *  @name   ~FileProperty
   *  @fn     ~FileProperty(void) = default
   *  @brief  Destructor
   */
  ~FileProperty(void) = default;
};
  

/**
 *  @class  FileSystem
 *  @brief  A generic interface for accessing a file system
 *  @author Christophe Ecabert
 *  @date   02.03.18
 *  @ingroup core
 */
class FK_EXPORTS FileSystem {
 public:
  
  /** Default buffer size for copy, 128k */
  static constexpr size_t kBufferSize = 128 * (1 << 10);
  
  /**
   *  @name   ~FileSystem
   *  @fn     virtual ~FileSystem(void) = default
   *  @brief  Destructor
   */
  virtual ~FileSystem(void) = default;
  
  /**
   *  @name   NormalizePath
   *  @fn     virtual std::string NormalizePath(const std::string& path)
   *  @brief  Convert a given path into a standardized representation
   *  @param[in] path Path to normalize
   *  @return Normalized path
   */
  virtual std::string NormalizePath(const std::string& path);
  
  /**
   *  @name   FileExists
   *  @fn     virtual Status FileExist(const std::string& filename) = 0
   *  @brief  Check if a given path exists
   *  @param[in] filename Path to check existence
   *  @return kGood of kNotFound
   */
  virtual Status FileExist(const std::string& filename) = 0;
  
  /**
   *  @name   FilesExists
   *  @fn     virtual bool FilesExists(const std::vector<std::string>& filenames,
                                       std::vector<Status>* status)
   *  @brief  Check if a list of files exists. 
   *  @param[in] filenames  List of files to check the existence
   *  @param[out] status    List of status for each files. Optionnal can be 
   *                        nullptr
   *  @return True if all files exists, false otherwise
   */
  virtual bool FilesExists(const std::vector<std::string>& filenames,
                           std::vector<Status>* status);
  
  /**
   *  @name   ListDir
   *  @fn     virtual Status ListDir(const std::string& dir, 
   *                                 std::vector<std::string>* files) = 0
   *  @brief  List the content of a given directory `dir`.
   *  @param[in] dir  Directory to scan
   *  @param[out] files List of files/dirs in `dir`
   *  @return kGood or Error code
   */
  virtual Status ListDir(const std::string& dir,
                         std::vector<std::string>* files) = 0;
  
  /**
   *  @name   ListDirRecursively
   *  @fn     virtual Status ListDirRecursively(const std::string& dir,
   *                                 std::vector<std::string>* files)
   *  @brief  List the content of a given directory `dir` and follow the 
   *          arborescence in order to list all files
   *  @param[in] dir  Directory to scan
   *  @param[out] files List of files/dirs in `dir`
   *  @return kGood or Error code
   */
  virtual Status ListDirRecursively(const std::string& dir,
                                    std::vector<std::string>* files);
  
  /**
   *  @name   FileProp
   *  @fn     virtual Status FileProp(const std::string& filename,
   *                                  FileProperty* prop) = 0
   *  @brief  Gather file properties for a given file
   *  @param[in] filename Path to the file
   *  @param[out] prop    File's properties
   *  @return kGood, kNotFound or error code.
   */
  virtual Status FileProp(const std::string& filename, FileProperty* prop) = 0;
  
  /**
   *  @name   DeleteFile
   *  @fn     virtual Status DeleteFile(const std::string& filename) = 0
   *  @brief  Delete a give file
   *  @param[in] filename File to delete
   *  @return Status of the operation
   */
  virtual Status DeleteFile(const std::string& filename) = 0;
  
  /**
   *  @name   CreateDir
   *  @fn     virtual Status CreateDir(const std::string& dir) = 0
   *  @brief  Create a directory
   *  @param[in] dir  path to the directory to create
   *  @return Status of the operation
   */
  virtual Status CreateDir(const std::string& dir) = 0;
  
  /**
   *  @name   IsDirectory
   *  @fn     virtual Status IsDirectory(const std::string& dir)
   *  @brief  Check if a given path is a directory
   *  @param[in] dir  Path to check if it is a directory
   *  @return Status of the operation
   */
  virtual Status IsDirectory(const std::string& dir);
  
  /**
   *  @name   CreateDirRecursively
   *  @fn     virtual Status CreateDirRecursively(const std::string& dir)
   *  @brief  Create a directory arborescence
   *  @param[in] dir  path to the directory to create
   *  @return Status of the operation
   */
  virtual Status CreateDirRecursively(const std::string& dir);
  
  /**
   *  @name   DeleteDir
   *  @fn     virtual Status DeleteDir(const std::string& dir) = 0
   *  @brief  Delete a directory
   *  @param[in] dir  path to the directory to delete
   *  @return Status of the operation
   */
  virtual Status DeleteDir(const std::string& dir) = 0;
  
  /**
   *  @name   DeleteDirRecursively
   *  @fn     virtual Status DeleteDirRecursively(const std::string& dir)
   *  @brief  Delete a directory arborescence
   *  @param[in] dir  path to the directory to delete
   *  @return Status of the operation
   */
  virtual Status DeleteDirRecursively(const std::string& dir);
  
  /**
   *  @name   RenameFile
   *  @fn     virtual Status RenameFile(const std::string& src,
   *                                    const std::string& dst) = 0;
   *  @brief  Change the name of file given in `src` into `dst`
   *  @param[in] src  Old file's name
   *  @param[in] dst  New file's name
   *  @return Status of the operation
   */
  virtual Status RenameFile(const std::string& src, const std::string& dst) = 0;
  
  /**
   *  @name   QueryFileSize
   *  @fn     virtual Status QueryFileSize(const std::string& filename, size_t* size) = 0
   *  @brief  Gather file size in bytes
   *  @param[in] filename File to query size
   *  @param[out] size    File's size
   *  @return Status of the operation
   */
  virtual Status QueryFileSize(const std::string& filename, size_t* size) = 0;
  
  /**
   *  @name   CopyFile
   *  @fn     virtual Status CopyFile(const std::string& src, const std::string& dst)
   *  @brief  Copy a file to a new location
   *  @param[in] src  File to copy
   *  @param[in] dst  Location where to copy it (dir + fname).
   *  @return Status of the operation
   */
  virtual Status CopyFile(const std::string& src, const std::string& dst);
};

}  // namespace FaceKit
#endif /* __FACEKIT_FILE_SYSTEM__ */
