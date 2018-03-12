/**
 *  @file   windows_file_system.cpp
 *  @brief Interface to access Windows file system (check if file/dir exist, ...).
 *         Inspired by Tensorflow
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   06.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#if !defined(__APPLE__) && !defined(__linux__)
#define IS_WINDOWS
// Win system
#include <Shlwapi.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <locale>
#include <codecvt>

#include "facekit/core/utils/string.hpp"
#include "facekit/core/sys/windows_file_system.hpp"
#include "facekit/core/sys/file_system_factory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   NormalizePath
 *  @fn     std::string NormalizePath(const std::string& path) override
 *  @brief  Convert a given path into a standardized representation
 *  @param[in] path Path to normalize
 *  @return Normalized path
 */
std::string WindowsFileSystem::NormalizePath(const std::string& path) {
  // Skip normalization
  return path;
}

/*
 *  @name   FileExists
 *  @fn     Status FileExist(const std::string& filename) override
 *  @brief  Check if a given path exists
 *  @param[in] filename Path to check existence
 *  @return kGood of kNotFound
 */
Status WindowsFileSystem::FileExist(const std::string& filename) {
#ifdef IS_WINDOWS
  Status status;
  if (_access(filename.c_str(), 0) != 0) {
    status = Status(Status::Type::kNotFound, filename + " not found");
  }
  return status;
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
Status WindowsFileSystem::ListDir(const std::string& dir,
                                std::vector<std::string>* files) {
#ifdef IS_WINDOWS
  files->clear();
  std::string fdir = NormalizePath(dir);
  std::wstring norm_dir = Utf8ToWString(fdir);
  // Define path
  std::wstring path = norm_dir;
  if (!path.empty() && path.back() != '\\' && path.back() != '/') {
    path += L"\\*";
  } else {
    path += L'*';
  }
  
  WIN32_FIND_DATAW find_data;
  HANDLE find_handle = ::FindFirstFileW(path.c_str(), &find_data);
  
  
  return Status(Status::Type::kUnimplemented, "Not implemeted yet");
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
Status WindowsFileSystem::FileProp(const std::string& filename,
                                 FileProperty* prop) {
#ifdef IS_WINDOWS
  Status res;
  struct _stat sbuf;
  std::wstring wname = Utf8ToWString(NormalizePath(filename));
  if (_wstat(wname.c_str(), &sbuf) != 0) {
    res = Status(Status::Type::kInternalError, "Can not access: " + filename)
  } else {
    // Stats structure
    prop->size = sbuf.st_size;
    prop->is_dir = PathIsDirectoryW(wname.c_str());
  }
  return res;
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
Status WindowsFileSystem::DeleteFile(const std::string& filename) {
  
#ifdef IS_WINDOWS
  Status res;
  std::wstring fname = Utf8ToWString(filename);
  if (_wunlink(fname.c_str()) != 0) {
    res = Status(Status::Type::kInternalError,
                 "Can not delete file: " + fname);
  }
  return res;
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
Status WindowsFileSystem::CreateDir(const std::string& dir) {
#ifdef IS_WINDOWS
  Status res;
  std::wstring dname = Utf8ToWString(dir);
  if (_wmkdir(dname.c_str()) != 0) {
    res = Status(Status::Type::kInternalError,
                 "Can not create: " + dname);
  }
  return res;
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
Status WindowsFileSystem::DeleteDir(const std::string& dir) {
#ifdef IS_WINDOWS
  Status res;
  std::wstring dname = Utf8ToWString(dir);
  if (_wrmdir(dname.c_str()) != 0) {
    res = Status(Status::Type::kInternalError,
                 "Can not delete: " + dname);
  }
  return res;
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
Status WindowsFileSystem::RenameFile(const std::string& src,
                                   const std::string& dst) {
#ifdef IS_WINDOWS
  Status res;
  std::wstring wsrc = Utf8ToWString(NormalizePath(src));
  std::wstring wdst = Utf8ToWString(NormalizePath(dst));
  if (!::MoveFileExW(wsrc.c_str(), wdst.c_str(),
                     MOVEFILE_REPLACE_EXISTING)) {
    res = Status(Status::Type::kInternalError,
                 "Can not rename: " + src);
  }
  return res;
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
Status WindowsFileSystem::QueryFileSize(const std::string& filename,
                                      size_t* size) {
#ifdef IS_WINDOWS
  return Status(Status::Type::kUnimplemented, "Not implemeted yet");
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
 
/*
 *  @name   Utf8ToWString
 *  @fn     static std::wstring Utf8ToWString(const std::string& utf_str)
 *  @brief  Convert an UTF8 string into wide char string.
 *  @param[in] utf_str  UTF-8 String to convert
 *  @return Converted string
 */
std::wstring WindowsFileSystem::Utf8ToWString(const std::string& utf_str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(utf_str);
}
  
/*
 *  @name   WStringToUtf8
 *  @fn     static std::string WStringToUtf8(const std::wstring& wstr)
 *  @brief  Convert a wide char strings into UTF8 string
 *  @param[in] wstr Wide char string to convert
 *  @return UTF8 string
 */
std::string WindowsFileSystem::WStringToUtf8(const std::wstring& wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(wstr);
}
  
// Register file system
REGISTER_FILE_SYSTEM("Windows", WindowsFileSystem);
  
  
}  // namespace FaceKit
