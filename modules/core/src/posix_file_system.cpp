/**
 *  @file   posix_file_system.cpp
 *  @brief Interface to access POSIX file system (check if file/dir exist, ...).
 *         Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   02.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#if defined(__APPLE__) || defined(__linux__)
#define IS_POSIX
// Posix system
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#endif

#include "facekit/core/utils/string.hpp"
#include "facekit/core/sys/posix_file_system.hpp"
#include "facekit/core/sys/file_system_factory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/*
 *  @name   FileExists
 *  @fn     Status FileExist(const std::string& filename) override
 *  @brief  Check if a given path exists
 *  @param[in] filename Path to check existence
 *  @return kGood of kNotFound
 */
Status PosixFileSystem::FileExist(const std::string& filename) {
#ifdef IS_POSIX
  std::string fname = NormalizePath(filename);
  if (access(fname.c_str(), F_OK) != 0) {
    // Does not exist
    return Status(Status::Type::kNotFound, fname + " not found");
  }
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
/*
 *  @name   ListDir
 *  @fn     Status ListDir(const std::string& dir,
 *                                 std::vector<std::string>* files) override
 *  @brief  List the content of a given directory `dir`.
 *  @param[in] dir  Directory to scan
 *  @param[out] files List of files/dirs in `dir`
 *  @return kGood or Error code
 */
Status PosixFileSystem::ListDir(const std::string& dir,
                                std::vector<std::string>* files) {
#ifdef IS_POSIX
  std::string fname = NormalizePath(dir);
  files->clear();
  // Try to access folder
  DIR* dd = opendir(fname.c_str());
  if (dd == nullptr) {
    return Status(Status::Type::kInternalError,
                  "Can not open directory: " + fname);
  }
  // Ok, iterate over content
  struct dirent* entry;
  while ((entry = readdir(dd)) != nullptr) {
    std::string dname(entry->d_name);
    if (dname != "." && dname != "..") {
      files->push_back(Path::Join(fname, dname));
    }
  }
  // CLose
  closedir(dd);
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
/*
 *  @name   FileProp
 *  @fn     Status FileProp(const std::string& filename,
 *                                  FileProperty* prop) override
 *  @brief  Gather file properties for a given file
 *  @param[in] filename Path to the file
 *  @param[out] prop    File's properties
 *  @return kGood, kNotFound or error code.
 */
Status PosixFileSystem::FileProp(const std::string& filename,
                                 FileProperty* prop) {
#ifdef IS_POSIX
  std::string fname = NormalizePath(filename);
  struct stat sbuf;
  if (stat(fname.c_str(), &sbuf) != 0) {
    return Status(Status::Type::kInternalError, "Can not access: " + fname);
  } else {
    // Stats structure
    prop->size = sbuf.st_size;
    prop->is_dir = S_ISDIR(sbuf.st_mode);
  }
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
/*
 *  @name   DeleteFile
 *  @fn     Status DeleteFile(const std::string& filename) override
 *  @brief  Delete a give file
 *  @param[in] filename File to delete
 *  @return Status of the operation
 */
Status PosixFileSystem::DeleteFile(const std::string& filename) {
#ifdef IS_POSIX
  std::string fname = NormalizePath(filename);
  if (unlink(fname.c_str()) != 0) {
    return Status(Status::Type::kInternalError,
                  "Can not delete file: " + fname);
  }
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
/*
 *  @name   CreateDir
 *  @fn     Status CreateDir(const std::string& dir) override
 *  @brief  Create a directory
 *  @param[in] dir  path to the directory to create
 *  @return Status of the operation
 */
Status PosixFileSystem::CreateDir(const std::string& dir) {
#ifdef IS_POSIX
  std::string fdir = NormalizePath(dir);
  if (mkdir(fdir.c_str(), 0755) != 0) {
    return Status(Status::Type::kInternalError,
                  "Can not create: " + fdir);
  }
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
/*
 *  @name   DeleteDir
 *  @fn     Status DeleteDir(const std::string& dir) override
 *  @brief  Delete a directory
 *  @param[in] dir  path to the directory to delete
 *  @return Status of the operation
 */
Status PosixFileSystem::DeleteDir(const std::string& dir) {
#ifdef IS_POSIX
  auto fdir = NormalizePath(dir);
  if (rmdir(fdir.c_str()) != 0) {
    return Status(Status::Type::kInternalError,
                  "Can not delete: " + fdir);
  }
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
/*
 *  @name   RenameFile
 *  @fn     Status RenameFile(const std::string& src,
 *                                    const std::string& dst) override;
 *  @brief  Change the name of file given in `src` into `dst`
 *  @param[in] src  Old file's name
 *  @param[in] dst  New file's name
 *  @return Status of the operation
 */
Status PosixFileSystem::RenameFile(const std::string& src,
                                   const std::string& dst) {
#ifdef IS_POSIX
  auto fsrc = NormalizePath(src);
  auto fdst = NormalizePath(dst);
  if (rename(fsrc.c_str(), fdst.c_str()) != 0) {
    return Status(Status::Type::kInternalError,
                  "Can not rename: " + fsrc);
  }
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
/*
 *  @name   QueryFileSize
 *  @fn     Status QueryFileSize(const std::string& filename, size_t* size) override
 *  @brief  Gather file size in bytes
 *  @param[in] filename File to query size
 *  @param[out] size    File's size
 *  @return Status of the operation
 */
Status PosixFileSystem::QueryFileSize(const std::string& filename,
                                      size_t* size) {
#ifdef IS_POSIX
  auto fname = NormalizePath(filename);
  // Gather stat
  struct stat sbuf;
  if (stat(fname.c_str(), &sbuf) != 0) {
    *size = 0;
    return Status(Status::Type::kInternalError,
                  "Can not access: " + fname);
  }
  *size = sbuf.st_size;
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
// Register file system
REGISTER_FILE_SYSTEM("Posix", PosixFileSystem);
  
  
}  // namespace FaceKit
