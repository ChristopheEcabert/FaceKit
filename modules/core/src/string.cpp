/**
 *  @file   string.cpp
 *  @brief  Utility function for string handling
 *
 *  @author Christophe Ecabert
 *  @date   26/08/16
 *  Copyright © 2016 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/utils/string.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
  
  
  
  
  
  
  
  
/**
 *  @namespace  String
 *  @brief      Utility functions for processing string
 */
namespace String {
  
}  // namespace String
  
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
void String::Split(const std::string& string,
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
void String::ExtractDirectory(const std::string& path,
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
}  // namespace FaceKit
