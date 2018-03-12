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
namespace internal {
std::string JoinImpl(std::initializer_list<std::string> parts);
}
  
/**
 *  @name   Join
 *  @fn     std::string Join(const T&... parts)
 *  @tparam T Pack strings to merge together
 *  @brief  Merge filename component into a single valid path. Similar to 
 *          python `os.path.join` method
 *  @param[in] parts  Packed parameters to aggregate together
 *  @return Appended path
 */
template<typename... T>
std::string Join(const T&... parts) {
  return internal::JoinImpl({parts...});
}

/**
 *  @name   IsAbsolute
 *  @fn     bool IsAbsolute(const std::string& path)
 *  @brief  Check if a given `path`is absolute or not (i.e. start with '/').
 *  @param[in] path Path to check
 *  @return True if absolute, false otherwise.
 */
bool IsAbsolute(const std::string& path);

/**
 *  @name   Dirname
 *  @fn     std::string Dirname(const std::string& path)
 *  @brief  Return the part in front of the last '/' in `path`. If there is no
 *          '/' give an empty string.
 *          Similar to `os.path.dirname`
 *  @param[in] path Path to file
 *  @return Complete file's directory or empty string.
 */
std::string Dirname(const std::string& path);


/**
 *  @name   Basename
 *  @fn     std::string Basename(const std::string& path)
 *  @brief  Extract file name from path (i.e. part after last '/'). If there 
 *          is no '/' it returns the same as the input.
 *          Similar to `os.path.basename`
 *  @param[in] path Path to file
 *  @return File name
 */
std::string Basename(const std::string& path);

/**
 *  @name   Extension
 *  @fn     std::string Extension(const std::string& path)
 *  @brief  Extract file's extension (i.e part after the last '.'). If there
 *          is not '.' return an empty string
 *  @param[in] path Path to file
 *  @return File's extension or empty string
 */
std::string Extension(const std::string& path);
  
/**
 *  @name   Clean
 *  @fn     std::string Clean(const std::string& path)
 *  @brief  Remove duplicate, add ./
 *  @param[in] path Path to clean
 *  @return cleaned path
 */
std::string Clean(const std::string& path);
  
/**
 *  @name SplitComponent
 *  @fn void SplitComponent(const std::string& path, std::string* dir,
                            std::string* file, std::string* ext)
 *  @brief  Split path into directory + file + extension
 *  @param[in]  path  Path where to extract data
 *  @param[out] dir   Extracted directory, skipped if nullptr
 *  @param[out] file  Extracted filename, skipped if nullptr
 *  @param[out] ext   Extracted extension, skipped if nullptr
 */
void SplitComponent(const std::string& path, std::string* dir,
                    std::string* file, std::string* ext);
  
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
  
}  // namespace FaceKit
#endif /* __FACEKIT_STRING_UTIL__ */
