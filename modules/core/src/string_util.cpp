/**
 *  @file   string_util.cpp
 *  @brief  Utility function for string handling
 *
 *  @author Christophe Ecabert
 *  @date   26/08/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/string_util.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name Split
 *  @fn static void Split(const std::string& string,
                         const std::string delimiter,
                         std::vector<std::string>* parts);
 *  @brief  Split a given \p string into parts for a specific delimiter
 *  @param[in]  string    String to split
 *  @param[in]  delimiter Delimiter
 *  @param[out] parts     Splitted parts
 */
void StringUtil::Split(const std::string& string,
                       const std::string delimiter,
                       std::vector<std::string>* parts) {
  std::size_t from = 0;
  std::size_t idx;
  do {
    idx = string.find(delimiter, from);
    if (idx != std::string::npos) {
      parts->push_back(string.substr(from, idx - from));
      from = idx + delimiter.length();
    } else {
      parts->push_back(string.substr(from, string.length() - from));
    }
  } while(idx != std::string::npos);
}
  
/*
 *  @name ExtractDirectory
 *  @fn
 *  @brief  Split path into directory + extension
 *  @param[in]  path  Path where to extract data
 *  @param[out] dir   Extracted directory
 *  @param[out] ext   Extracted extension
 */
void StringUtil::ExtractDirectory(const std::string& path,
                                  std::string* dir,
                                  std::string* file,
                                  std::string* ext) {
  auto pdir = path.rfind("/");
  if (pdir != std::string::npos) {
    pdir += 1;
    *dir = path.substr(0, pdir);
  } else {
    pdir = 0;
  }
  auto pext = path.rfind(".");
  if (pext != std::string::npos) {
    *file = path.substr(pdir, pext - pdir);
    *ext = path.substr(pext + 1, path.length() - pext - 1);
  } else {
    *file = path.substr(pdir);
  }
}
  
/*
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
std::string StringUtil::LeadingZero(const T number, const size_t N) {
  std::string num = std::to_string(number);
  return std::string(N - num.length(), '0') + num;
}
  
template std::string StringUtil::LeadingZero(const int8_t, size_t);
template std::string StringUtil::LeadingZero(const uint8_t, size_t);
template std::string StringUtil::LeadingZero(const int16_t, size_t);
template std::string StringUtil::LeadingZero(const uint16_t, size_t);
template std::string StringUtil::LeadingZero(const int32_t, size_t);
template std::string StringUtil::LeadingZero(const uint32_t, size_t);
template std::string StringUtil::LeadingZero(const int64_t, size_t);
template std::string StringUtil::LeadingZero(const uint64_t, size_t);
template std::string StringUtil::LeadingZero(const size_t, size_t);
  
}  // namespace FaceKit
