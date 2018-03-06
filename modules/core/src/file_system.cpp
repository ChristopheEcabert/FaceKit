/**
 *  @file   file_system.cpp
 *  @brief Interface to access file system (check if file/dir exist, ...).
 *         Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   02.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <fstream>
#include <stack>
#include <algorithm>

#include "facekit/core/sys/file_system.hpp"
#include "facekit/core/sys/file_system_factory.hpp"
#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   NormalizePath
 *  @fn     virtual std::string NormalizePath(const std::string& path)
 *  @brief  Convert a given path into a standardized representation
 *  @param[in] path Path to normalize
 *  @return Normalized path
 */
std::string FileSystem::NormalizePath(const std::string& path) {
  if (path.empty()) {
    return path;
  } else {
    return Path::Clean(path);
  }
}
  
/*
 *  @name   FilesExists
 *  @fn     virtual bool FilesExists(const std::vector<std::string>& filenames,
 std::vector<Status>* status)
 *  @brief  Check if a list of files exists.
 *  @param[in] filenames  List of files to check the existence
 *  @param[out] status    List of status for each files. Optionnal can be
 *                        nullptr
 *  @return True if all files exists, false otherwise
 */
bool FileSystem::FilesExists(const std::vector<std::string>& filenames,
                             std::vector<Status>* status) {
  bool ret = true;
  // Loop over all files
  for (const auto& f : filenames) {
    // Check if exists
    Status s = FileExist(f);
    if (status != nullptr) {
      status->push_back(s);
    }
    if (!s.Good()) {
      ret = false;    // Stop, error happened
      break;
    }
    
  }
  return ret;
}
  
/*
 *  @name   IsDirectory
 *  @fn     virtual Status IsDirectory(const std::string& dir)
 *  @brief  Check if a given path is a directory
 *  @param[in] dir  Path to check if it is a directory
 *  @return Status of the operation
 */
Status FileSystem::IsDirectory(const std::string& dir) {
  // Check if file exist
  if (!FileExist(dir).Good()) {
    return Status(Status::Type::kInvalidArgument,
                  "Folder " + dir + " does not exists!");
  }
  // Gather properties
  FileProperty prop;
  if (!FileProp(dir, &prop).Good()) {
    return Status(Status::Type::kInternalError,
                  "Can not query properties for: " + dir);
  }
  if (prop.is_dir) {
    return Status();
  }
  return Status(Status::Type::kInvalidArgument, "Is not a directory");
}
  
  
/*
 *  @name   ListDirRecursively
 *  @fn     virtual Status ListDirRecursively(const std::string& dir,
 *                                 std::vector<std::string>* files)
 *  @brief  List the content of a given directory `dir` and follow the
 *          arborescence in order to list all files
 *  @param[in] dir  Directory to scan
 *  @param[out] files List of files/dirs in `dir`
 *  @return kGood or Error code
 */
Status FileSystem::ListDirRecursively(const std::string& dir,
                                      std::vector<std::string>* files) {
  std::stack<std::string> stack;
  stack.push(NormalizePath(dir));
  // Loop recursively, using stack to avoid function call overhead.
  Status status;
  while (!stack.empty()) {
    auto folder = stack.top();
    stack.pop();
    // Scan content
    std::vector<std::string> content;
    status = ListDir(folder, &content);
    if (!status.Good()) {
      // Somthing went wrong, -> stop recursion
      break;
    }
    // Check files/folders
    for (const auto& entry : content) {
      if (IsDirectory(entry).Good()) {
        // Add it to the stack
        stack.push(entry);
      } else {
        // Add to output
        files->push_back(entry);
      }
    }
  }
  return status;
}
  
/*
 *  @name   CreateDirRecursively
 *  @fn     virtual Status CreateDirRecursively(const std::string& dir)
 *  @brief  Create a directory arborescence
 *  @param[in] dir  path to the directory to create
 *  @return Status of the operation
 */
Status FileSystem::CreateDirRecursively(const std::string& dir) {
  // Get directory path, add '/' since normalize remove trailing slash
  auto folders = Path::Dirname(NormalizePath(dir) + "/");
  // Split path into individual folders
  std::vector<std::string> parts;
  String::Split(folders, "/", &parts);
  // Loop
  Status status;
  std::string path;
  for (const auto& p : parts) {
    // Check folder
    path = Path::Join(path, p);
    if (!this->FileExist(path).Good()) {
      status = this->CreateDir(path);
      if (!status.Good()) {
        break;
      }
    }
  }
  return status;
}
  
/*
 *  @name   DeleteDirRecursively
 *  @fn     virtual Status DeleteDirRecursively(const std::string& dir)
 *  @brief  Delete a directory arborescence
 *  @param[in] dir  path to the directory to delete
 *  @return Status of the operation
 */
Status FileSystem::DeleteDirRecursively(const std::string& dir) {
  
  std::deque<std::string> dir_to_rm;
  std::stack<std::string> stack;
  Status status;
  // Iterate over direction, dive in and delete file at same time
  auto root = NormalizePath(dir);
  auto dirname = Path::Dirname(root);
  stack.push(root);
  dir_to_rm.push_front(root);
  bool run = true;
  while (!stack.empty() && run) {
    // Get top folder
    std::string folder = stack.top();
    stack.pop();
    // Query content
    std::vector<std::string> content;
    status = this->ListDir(folder, &content);
    if (!status.Good()) {
      break;
    }
    // Loop over files and delete them, if dir add it the the queue to rm later
    for (const auto& f : content) {
      if (IsDirectory(f).Good()) {
        dir_to_rm.push_front(f);
        stack.push(f);
      } else {
        // Delete file
        status = this->DeleteFile(f);
        if (!status.Good()) {
          run = false;
          break;
        }
      }
    }
  }
  if (status.Good() && !dir_to_rm.empty()) {
    // Delete folders if no error so far
    for (const auto& folder : dir_to_rm) {
      status = this->DeleteDir(folder);
      if (!status.Good()) {
        break;
      }
    }
  }
  return status;
}
  
/*
 *  @name   CopyFile
 *  @fn     virtual Status CopyFile(const std::string& src, const std::string& dst)
 *  @brief  Copy a file to a new location
 *  @param[in] src  File to copy
 *  @param[in] dst  Location where to copy it (dir + fname).
 *  @return Status of the operation
 */
Status FileSystem::CopyFile(const std::string& src, const std::string& dst) {
  
  // Open src + dst
  std::string in_name = NormalizePath(src);
  std::ifstream in(in_name.c_str(), std::ios_base::in | std::ios_base::binary);
  std::string out_name = NormalizePath(dst);
  std::ofstream out(out_name.c_str(),
                    std::ios_base::out | std::ios_base::binary);
  if (!in.is_open() || !out.is_open()) {
    return Status(Status::Type::kInternalError,
                  "Can not copy: " + in_name);
  }
  // Stream created + open
  char buffer[FileSystem::kBufferSize];
  // Start copy
  do {
    in.read(&buffer[0], FileSystem::kBufferSize);
    out.write(&buffer[0], in.gcount());
  } while (in.gcount() > 0);
  // Check error
  Status status;
  if (!in.eof() || !out.good()) {
    status = Status(Status::Type::kInternalError,
                    "Error while copy data to: " + out_name);
  }
  // Close stream
  in.close();
  out.close();
  return status;
}
  
#pragma mark -
#pragma mark Proxy
  
/*
 *  @name   FileSystemProxy
 *  @fn     FileSystemProxy(void)
 *  @brief  Constructor
 */
FileSystemProxy::FileSystemProxy(void) {
  // Register
  FileSystemFactory::Get().Register(this);
}
  
  
}  // namespace FaceKit
