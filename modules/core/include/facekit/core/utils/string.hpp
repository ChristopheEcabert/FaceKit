/**
 *  @file   "utils/string.hpp"
 *  @brief  Utility function for string handling
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   26/08/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_STRING_UTIL__
#define __FACEKIT_STRING_UTIL__

#include <string>
#include <vector>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @namespace  Path
 *  @brief      Utility functions for processing path
 */
namespace Path {
  
  /**
   *  @name   Join
   *  @fn     std::string Join(const T&&... parts)
   *  @tparam T Pack strings to merge together
   *  @brief  Merge filename component into a single valid path.
   *  @param[in] parts  Packed parameters to aggregate together
   *  @return Appended path
   */
  template<typename... T>
  std::string Join(const T&&... parts);
  
  /**
   *  @name   IsAbsolute
   *  @fn     bool IsAbsolute(const std::string& path)
   *  @brief  Check if a given `path`is absolute or not (i.e. start with '/')
   *  @param[in] path Path to check
   *  @return True if absolute, false otherwise.
   */
  bool IsAbsolute(const std::string& path);
  
  
  
  
  
}  // namespace Path


/**
 *  @namespace  String
 *  @brief      Utility functions for processing string
 */
namespace String {
  
/**
 *  @name Split
 *  @fn void Split(const std::string& string, const std::string delimiter,
                   std::vector<std::string>* parts);
 *  @brief  Split a given \p string into parts for a specific delimiter
 *  @param[in]  string    String to split
 *  @param[in]  delimiter Delimiter
 *  @param[out] parts     Splitted parts
 */
void FK_EXPORTS Split(const std::string& string, const std::string delimiter,
                      std::vector<std::string>* parts);
  
/**
 *  @name   LeadingZero
 *  @fn     std::string LeadingZero(const T number, const size_t N)
 *  @brief  Convert a number to string and complete it with leading zeros if
 *          needed
 *  @param[in]  number  Number to convert to string
 *  @param[in]  N       Number total of char wanted (i.e. N=3, number=12
 *                      -> 012)
 *  @tparam T Data type
 */
template<typename T>
std::string FK_EXPORTS LeadingZero(const T number, const size_t N) {
  std::string num = std::to_string(number);
  return std::string(N - num.length(), '0') + num;
}
  
}  // namespace String
  
  
  
  
  
  /**
   *  @name ExtractDirectory
   *  @fn static void ExtractDirectory(const std::string& path,
                                       std::string* dir,
                                       std::string* file,
                                       std::string* ext)
   *  @brief  Split path into directory + extension
   *  @param[in]  path  Path where to extract data
   *  @param[out] dir   Extracted directory
   *  @param[out] file  Extracted filename
   *  @param[out] ext   Extracted extension
   */
  static void ExtractDirectory(const std::string& path,
                               std::string* dir,
                               std::string* file,
                               std::string* ext);
  
  
  
}  // namespace FaceKit
#endif /* __FACEKIT_STRING_UTIL__ */
