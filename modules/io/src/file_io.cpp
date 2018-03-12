/**
 *  @file   file_io.cpp
 *  @brief Handle reading/writing from various file
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   23.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#include <fstream>
#include <stack>

#include "tinydir/tinydir.h"

#include "facekit/io/file_io.hpp"
#include "facekit/core/logger.hpp"
#include "facekit/io/object_header.hpp"
#include "facekit/io/object_manager.hpp"


/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name LoadMat
 *  @fn int LoadMat(std::istream& stream, cv::Mat* matrix)
 *  @brief  Load a matrix from a given stream
 *  @param[in]  stream  Binary stream from which to load data
 *  @param[out] matrix  Loaded matrix
 *  @return -1 if error, 0 otherwise
 */
int IO::LoadMat(std::istream& stream, cv::Mat* matrix) {
  int err = -1;
  if (stream.good()) {
    int type = -1, r = -1, c = -1;
    stream.read(reinterpret_cast<char*>(&type), sizeof(type));
    stream.read(reinterpret_cast<char*>(&r), sizeof(r));
    stream.read(reinterpret_cast<char*>(&c), sizeof(c));
    if (type != -1) {
      // Init container
      matrix->create(r, c, type);
      // Read data
      stream.read(reinterpret_cast<char*>(matrix->data),
                  matrix->total() * matrix->elemSize());
      // Sanity check
      err = stream.good() ? 0 : -1;
    }
  }
  return err;
}
  
/*
 *  @name LoadTypedMat
 *  @fn int LoadTypedMat(std::istream& stream, cv::Mat* matrix)
 *  @brief  Load a matrix from a given stream and convert it to a specific
 *          type
 *  @tparam T Desired type for matrix
 *  @param[in]  stream  Binary stream from which to load data
 *  @param[out] matrix  Loaded matrix
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int IO::LoadTypedMat(std::istream& stream, cv::Mat* matrix) {
  int err = -1;
  if (stream.good()) {
    int type = -1, r = -1, c = -1;
    stream.read(reinterpret_cast<char*>(&type), sizeof(type));
    stream.read(reinterpret_cast<char*>(&r), sizeof(r));
    stream.read(reinterpret_cast<char*>(&c), sizeof(c));
    if (type != -1) {
      // Init container
      cv::Mat buff(r, c, type);
      // Read data
      stream.read(reinterpret_cast<char*>(buff.data),
                  buff.total() * buff.elemSize());
      // Convert
      buff.convertTo(*matrix, cv::DataType<T>::type);
      // Sanity check
      err = stream.good() ? 0 : -1;
    }
  }
  return err;
}
  
template int IO::LoadTypedMat<uint8_t>(std::istream& stream, cv::Mat* matrix);
template int IO::LoadTypedMat<int8_t>(std::istream& stream, cv::Mat* matrix);
template int IO::LoadTypedMat<uint16_t>(std::istream& stream, cv::Mat* matrix);
template int IO::LoadTypedMat<int16_t>(std::istream& stream, cv::Mat* matrix);
template int IO::LoadTypedMat<int32_t>(std::istream& stream, cv::Mat* matrix);
template int IO::LoadTypedMat<float>(std::istream& stream, cv::Mat* matrix);
template int IO::LoadTypedMat<double>(std::istream& stream, cv::Mat* matrix);
  
/*
 *  @name LoadMat
 *  @fn static int LoadMat(const std::string& path, cv::Mat* matrix)
 *  @brief  Load a matrix from a given file path
 *  @param[in]  path    Path to the file to open
 *  @param[out] matrix  Loaded matrix
 *  @return -1 if error, 0 otherwise
 */
int IO::LoadMat(const std::string& path, cv::Mat* matrix) {
  int err = -1;
  std::ifstream stream(path.c_str(), std::ios_base::binary);
  if (stream.is_open()) {
    err = IO::LoadMat(stream, matrix);
  }
  return err;
}
  
/*
 *  @name LoadTypedMat
 *  @fn static int LoadTypedMat(const std::string& path, cv::Mat* matrix)
 *  @brief  Load a matrix from a given file and convert it to a specific
 *          type
 *  @tparam T Desirpream  Binary stream from which to load data
 *  @param[in]  path    Path to the file to open
 *  @param[out] matrix  Loaded matrix
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int IO::LoadTypedMat(const std::string& path, cv::Mat* matrix) {
  int err = -1;
  std::ifstream stream(path.c_str(), std::ios_base::binary);
  if (stream.is_open()) {
    err = IO::LoadTypedMat<T>(stream, matrix);
  }
  return err;
}
  
template int IO::LoadTypedMat<uint8_t>(const std::string& path, cv::Mat* matrix);
template int IO::LoadTypedMat<int8_t>(const std::string& path, cv::Mat* matrix);
template int IO::LoadTypedMat<uint16_t>(const std::string& path, cv::Mat* matrix);
template int IO::LoadTypedMat<int16_t>(const std::string& path, cv::Mat* matrix);
template int IO::LoadTypedMat<int32_t>(const std::string& path, cv::Mat* matrix);
template int IO::LoadTypedMat<float>(const std::string& path, cv::Mat* matrix);
template int IO::LoadTypedMat<double>(const std::string& path, cv::Mat* matrix);
  
/*
 *  @name SaveMat
 *  @fn static int SaveMat(std::ostream& stream, const cv::Mat& matrix)
 *  @brief  Save a \p matrix into a given stream
 *  @param[in]  stream  Binary stream where to write
 *  @param[in]  matrix  Matrix to write
 *  @return -1 if error, 0 otherwise
 */
int IO::SaveMat(std::ostream& stream, const cv::Mat& matrix) {
  int err = -1;
  if (stream.good()) {
    int type = matrix.type();
    // Save properties
    stream.write(reinterpret_cast<const char*>(&type), sizeof(type));
    stream.write(reinterpret_cast<const char*>(&matrix.rows),
                 sizeof(matrix.rows));
    stream.write(reinterpret_cast<const char*>(&matrix.cols),
                 sizeof(matrix.cols));
    // Save data
    stream.write(reinterpret_cast<const char*>(matrix.data),
                 matrix.total() * matrix.elemSize());
    // Sanity check
    err = stream.good() ? 0 : -1;
  }
  return err;
}
  
/*
 *  @name SaveTypedMat
 *  @fn static int SaveTypedMat(std::ostream& stream, const cv::Mat& matrix)
 *  @brief  Save a \p matrix into a given stream with a specific type
 *  @tparam T output data type wanted
 *  @param[in]  stream  Binary stream where to write
 *  @param[in]  matrix  Matrix to write
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int IO::SaveTypedMat(std::ostream& stream, const cv::Mat& matrix) {
  int err = -1;
  if (stream.good()) {
    int type = cv::DataType<T>::type;
    // Save properties
    cv::Mat buff;
    matrix.convertTo(buff, type);
    stream.write(reinterpret_cast<const char*>(&type), sizeof(type));
    stream.write(reinterpret_cast<const char*>(&buff.rows),
                 sizeof(buff.rows));
    stream.write(reinterpret_cast<const char*>(&buff.cols),
                 sizeof(buff.cols));
    // Save data
    stream.write(reinterpret_cast<const char*>(buff.data),
                 buff.total() * buff.elemSize());
    // Sanity check
    err = stream.good() ? 0 : -1;
  }
  return err;
}
  
template int IO::SaveTypedMat<uint8_t>(std::ostream& stream, const cv::Mat& matrix);
template int IO::SaveTypedMat<int8_t>(std::ostream& stream, const cv::Mat& matrix);
template int IO::SaveTypedMat<uint16_t>(std::ostream& stream, const cv::Mat& matrix);
template int IO::SaveTypedMat<int16_t>(std::ostream& stream, const cv::Mat& matrix);
template int IO::SaveTypedMat<int32_t>(std::ostream& stream, const cv::Mat& matrix);
template int IO::SaveTypedMat<float>(std::ostream& stream, const cv::Mat& matrix);
template int IO::SaveTypedMat<double>(std::ostream& stream, const cv::Mat& matrix);
  
/*
 *  @name SaveMat
 *  @fn static int SaveMat(const std::string& filename, const cv::Mat& matrix)
 *  @brief  Save a \p matrix into a given stream
 *  @param[in]  filename  File's name
 *  @param[in]  matrix  Matrix to write
 *  @return -1 if error, 0 otherwise
 */
int IO::SaveMat(const std::string& filename, const cv::Mat& matrix) {
  int err = -1;
  std::ofstream stream(filename.c_str(), std::ios_base::binary);
  if (stream.is_open()) {
    err = IO::SaveMat(stream, matrix);
  }
  return err;
}
  
/*
 *  @name SaveTypedMat
 *  @fn static int SaveTypedMat(const std::string& filename, const cv::Mat& matrix)
 *  @brief  Save a \p matrix into a given stream with a specific type
 *  @tparam T output data type wanted
 *  @param[in]  filename  File's name
 *  @param[in]  matrix  Matrix to write
 *  @return -1 if error, 0 otherwise
 */
template<typename T>
int IO::SaveTypedMat(const std::string& filename, const cv::Mat& matrix) {
  int err = -1;
  std::ofstream stream(filename.c_str(), std::ios_base::binary);
  if (stream.is_open()) {
    err = IO::SaveTypedMat<T>(stream, matrix);
  }
  return err;
}
  
template int IO::SaveTypedMat<uint8_t>(const std::string& filename, const cv::Mat& matrix);
template int IO::SaveTypedMat<int8_t>(const std::string& filename, const cv::Mat& matrix);
template int IO::SaveTypedMat<uint16_t>(const std::string& filename, const cv::Mat& matrix);
template int IO::SaveTypedMat<int16_t>(const std::string& filename, const cv::Mat& matrix);
template int IO::SaveTypedMat<int32_t>(const std::string& filename, const cv::Mat& matrix);
template int IO::SaveTypedMat<float>(const std::string& filename, const cv::Mat& matrix);
template int IO::SaveTypedMat<double>(const std::string& filename, const cv::Mat& matrix);
  
#pragma mark -
#pragma mark Stream Utility
  
/*
 *  @name ScanStreamForObject
 *  @fn static int ScanStream(std::istream& stream, const size_t& id)
 *  @brief  Scan a given stream to find a certain type of object
 *  @param[in]  stream  Binary stream to search in
 *  @param[in]  id      ID of object to look for
 *  @return     0 if found, -1 otherwise (error / not found)
 */
int IO::ScanStream(std::istream& stream, const size_t& id) {
  int err = -1;
  if (stream.good()) {
    ObjectHeader hdr;
    // Iterate till finding the object or reaching the end of stream
    while (stream.good() && hdr.get_id() != id) {
      stream >> hdr;
      if (hdr.get_id() != id) {
        stream.seekg(hdr.get_size(), std::ios_base::cur);
      }
    }
    err = hdr.get_id() == id ? 0 : -1;
  }
  return err;
}

/*
 *  @name   StreamContent
 *  @fn static int StreamContent(std::istream& stream,
                                 std::vector<std::string>* content)
 *  @brief  Extract the content of a binary file
 *  @param[in]  stream  Binary stream
 *  @return -1 if error, 0 otherwise
 */
int IO::StreamContent(std::istream& stream,
                      std::vector<std::string>* content) {
  int err = -1;
  if (stream.good()) {
    ObjectHeader hdr;
    while (stream.good()) {
      // Get object info
      stream >> hdr;
      // Print content
      FACEKIT_LOG_INFO(ObjectManager::Get().GetName(hdr.get_id()));
      // Move to the next one
      stream.seekg(hdr.get_size(), std::ios_base::cur);
    }
    err = 0;
  }
  return err;
}
  
#pragma mark -
#pragma mark Folder utility
  
/*
 *  @name   SearchInFolder
 *  @fn     static void SearchInFolder(const std::string& root,
                                     const std::string& ext,
                                     std::vector<std::string>* files)
 *  @brief  Search recursively from a root folder for files with a specific
 *          extension
 */
void IO::SearchInFolder(const std::string& root,
                        const std::string& ext,
                        std::vector<std::string>* files) {
  std::stack<std::string> stack;
  stack.push(root);
  tinydir_dir dir;
  tinydir_file file;
  // Start recursive search
  while (!stack.empty()) {
    // Get actual folder
    std::string path = stack.top();
    stack.pop();
    // Open it
    tinydir_open(&dir, path.c_str());
    // Iterate over content
    while (dir.has_next) {
      // Read file
      tinydir_readfile(&dir, &file);
      // File or folder ?
      const auto file_str = std::string(file.name);
      const auto name = std::string(dir.path) + "/" + file_str;
      if (file.is_dir && (file_str != "." && file_str != "..")) {
        // Add to stack
        stack.push(name);
      } else {
        // Check if match extension
        auto p = name.rfind(ext);
        if (p != std::string::npos) {
          // Add to the list if file matching extension
          files->push_back(name);
        }
      }
      // Move to the next file/folder
      tinydir_next(&dir);
    }
    // Done iterating over folder, close it
    tinydir_close(&dir);
  }
}
  
}  // namespace FaceKit
