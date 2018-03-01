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
 *  @class  String
 *  @brief  Utility function for string handling
 *  @author Christophe Ecabert
 *  @date   26/08/16
 *  @ingroup core
 */
class FK_EXPORTS String {
 public:
  
  /**
   *  @name Split
   *  @fn static void Split(const std::string& string,
                           const std::string delimiter,
                           std::vector<std::string>* parts);
   *  @brief  Split a given \p string into parts for a specific delimiter
   *  @param[in]  string    String to split
   *  @param[in]  delimiter Delimiter
   *  @param[out] parts     Splitted parts
   */
  static void Split(const std::string& string,
                    const std::string delimiter,
                    std::vector<std::string>* parts);
  
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
  
  /**
   *  @name   LeadingZero
   *  @fn     static std::string LeadingZero(const T number, const size_t N)
   *  @brief  Convert a number to string and complete it with leading zeros if 
   *          needed
   *  @param[in]  number  Number to convert to string
   *  @param[in]  N       Number total of char wanted (i.e. N=3, number=12 
   *                      -> 012)
   *  @tparam T Data type
   */
  template<typename T>
  static std::string LeadingZero(const T number, const size_t N) {
    std::string num = std::to_string(number);
    return std::string(N - num.length(), '0') + num;
  }
};
  
}  // namespace FaceKit


#endif /* __FACEKIT_STRING_UTIL__ */
