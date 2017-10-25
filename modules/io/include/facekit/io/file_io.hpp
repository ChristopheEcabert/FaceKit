/**
 *  @file   file_io.hpp
 *  @brief Handle reading/writing from various file
 *  @ingroup io
 *
 *  @author Christophe Ecabert
 *  @date   23.09.17
 *  Copyright © 2017 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_FILE_IO__
#define __FACEKIT_FILE_IO__

#include <iostream>
#include <string>

#include "opencv2/core/core.hpp"

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  IO
 *  @brief  Handle reading/writing from various file
 *  @author Christophe Ecabert
 *  @date   23.09.17
 *  @ingroup io
 */
class FK_EXPORTS IO {
 public:
  
#pragma mark -
#pragma mark File I/0
  
  /**
   *  @name LoadMat
   *  @fn static int LoadMat(std::istream& stream, cv::Mat* matrix)
   *  @brief  Load a matrix from a given stream
   *  @param[in]  stream  Binary stream from which to load data
   *  @param[out] matrix  Loaded matrix
   *  @return -1 if error, 0 otherwise
   */
  static int LoadMat(std::istream& stream, cv::Mat* matrix);
  
  /**
   *  @name LoadTypedMat
   *  @fn static int LoadTypedMat(std::istream& stream, cv::Mat* matrix)
   *  @brief  Load a matrix from a given stream and convert it to a specific 
   *          type
   *  @tparam T Desired type for matrix
   *  @param[in]  stream  Binary stream from which to load data
   *  @param[out] matrix  Loaded matrix
   *  @return -1 if error, 0 otherwise
   */
  template<typename T>
  static int LoadTypedMat(std::istream& stream, cv::Mat* matrix);
  
  /**
   *  @name LoadMat
   *  @fn static int LoadMat(const std::string& path, cv::Mat* matrix)
   *  @brief  Load a matrix from a given file path
   *  @param[in]  path    Path to the file to open
   *  @param[out] matrix  Loaded matrix
   *  @return -1 if error, 0 otherwise
   */
  static int LoadMat(const std::string& path, cv::Mat* matrix);
  
  /**
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
  static int LoadTypedMat(const std::string& path, cv::Mat* matrix);
  
  /**
   *  @name SaveMat
   *  @fn static int SaveMat(std::ostream& stream, const cv::Mat& matrix)
   *  @brief  Save a \p matrix into a given stream
   *  @param[in]  stream  Binary stream where to write
   *  @param[in]  matrix  Matrix to write
   *  @return -1 if error, 0 otherwise
   */
  static int SaveMat(std::ostream& stream, const cv::Mat& matrix);
  
  /**
   *  @name SaveTypedMat
   *  @fn static int SaveTypedMat(std::ostream& stream, const cv::Mat& matrix)
   *  @brief  Save a \p matrix into a given stream with a specific type
   *  @tparam T output data type wanted
   *  @param[in]  stream  Binary stream where to write
   *  @param[in]  matrix  Matrix to write
   *  @return -1 if error, 0 otherwise
   */
  template<typename T>
  static int SaveTypedMat(std::ostream& stream, const cv::Mat& matrix);
  
  /**
   *  @name SaveMat
   *  @fn static int SaveMat(const std::string& filename, const cv::Mat& matrix)
   *  @brief  Save a \p matrix into a given stream
   *  @param[in]  filename  File's name
   *  @param[in]  matrix  Matrix to write
   *  @return -1 if error, 0 otherwise
   */
  static int SaveMat(const std::string& filename, const cv::Mat& matrix);
  
  /**
   *  @name SaveTypedMat
   *  @fn static int SaveTypedMat(const std::string& filename, const cv::Mat& matrix)
   *  @brief  Save a \p matrix into a given stream with a specific type
   *  @tparam T output data type wanted
   *  @param[in]  filename  File's name
   *  @param[in]  matrix  Matrix to write
   *  @return -1 if error, 0 otherwise
   */
  template<typename T>
  static int SaveTypedMat(const std::string& filename, const cv::Mat& matrix);
  
#pragma mark -
#pragma mark Stream Utility
  
  /**
   *  @name ScanStreamForObject
   *  @fn static int ScanStream(std::istream& stream, const size_t& id)
   *  @brief  Scan a given stream to find a certain type of object
   *  @param[in]  stream  Binary stream to search in
   *  @param[in]  id      ID of object to look for
   *  @return     0 if found, -1 otherwise (error / not found)
   */
  static int ScanStream(std::istream& stream, const size_t& id);
  
  /**
   *  @name   StreamContent
   *  @fn static int StreamContent(std::istream& stream,
                                   std::vector<std::string>* content)
   *  @brief  Extract the content of a binary file
   *  @param[in]  stream  Binary stream
   *  @param[out] content List of object found inside the \p stream
   *  @return -1 if error, 0 otherwise
   */
  static int StreamContent(std::istream& stream,
                           std::vector<std::string>* content);
  
#pragma mark -
#pragma mark Folder utility
  
  /**
   *  @name   SearchInFolder
   *  @fn     static void SearchInFolder(const std::string& root,
                                         const std::string& ext,
                                         std::vector<std::string>* files)
   *  @brief  Search recursively from a root folder for files with a specific
   *          extension
   */
  static void SearchInFolder(const std::string& root,
                             const std::string& ext,
                             std::vector<std::string>* files);
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_FILE_IO__ */
