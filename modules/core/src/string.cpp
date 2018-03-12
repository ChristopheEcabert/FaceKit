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
 *  @namespace  Path
 *  @brief      Utility functions for processing path
 */
namespace Path {
  
namespace internal {
std::string JoinImpl(std::initializer_list<std::string> parts) {
  std::string res;
  // Loop over all parts
  for (const auto& p : parts) {
    // Empty part -> skip
    if (p.empty()) continue;
    // First part ?
    if (res.empty()) {
      res = p;
      continue;
    }
    // Need to append
    if (res.back() == '/') {
      // res end with '/'
      if (IsAbsolute(p)) {
        // p starts with '/' -> remove it
        res.append(p.substr(1));
      } else {
        res.append(p);
      }
    } else {
      // res does not end with '/'
      if (IsAbsolute(p)) {
        // p starts with '/'
        res.append(p);
      } else {
        // p does not start with '/' -> need to add one
        res.append("/" + p);
      }
    }
  }
  return res;
}
}
  
/*
 *  @name   IsAbsolute
 *  @fn     bool IsAbsolute(const std::string& path)
 *  @brief  Check if a given `path`is absolute or not (i.e. start with '/')
 *  @param[in] path Path to check
 *  @return True if absolute, false otherwise.
 */
bool IsAbsolute(const std::string& path) {
  return !path.empty() && path[0] == '/';
}
  
/*
 *  @name   Dirname
 *  @fn     std::string Dirname(const std::string& path)
 *  @brief  Return the part in front of the last '/' in `path`. If there is no
 *          '/' give an empty string
 *  @param[in] path Path to file
 *  @return Complete file's directory or empty string.
 */
std::string Dirname(const std::string& path) {
  auto pos = path.rfind("/");
#ifdef WIN32
  if (pos == std::string::npos) {
    pos = path.rfind("\\");
  }
#endif
  if (pos == std::string::npos) {
    // No directory found
    return "";
  }
  // Check if path starts with '/'
  if (pos == 0) {
    return "/";
  }
  // Found something
  return path.substr(0, pos);
}
  
/*
 *  @name   Basename
 *  @fn     std::string Basename(const std::string& path)
 *  @brief  Extract file name from path (i.e. part after last '/'). If there
 *          is no '/' it returns the same as the input.
 *          Similar to `os.path.basename`
 *  @param[in] path Path to file
 *  @return File name
 */
std::string Basename(const std::string& path) {
  auto pos = path.rfind("/");
#ifdef WIN32
  if (pos == std::string::npos) {
    pos = path.rfind("\\");
  }
#endif
  if (pos == std::string::npos) {
    // No '/' found -> return path
    return path;
  }
  // start with '/'
  if (pos == 0) {
    return path.substr(1);
  }
  // Else '/' in the middle
  return path.substr(pos + 1);
}
  
/*
 *  @name   Extension
 *  @fn     std::string Extension(const std::string& path)
 *  @brief  Extract file's extension (i.e part after the last '.'). If there
 *          is not '.' return an empty string
 *  @param[in] path Path to file
 *  @return File's extension or empty string
 */
std::string Extension(const std::string& path) {
  auto pos = path.rfind(".");
  if (pos != std::string::npos) {
    return path.substr(pos + 1);
  } else {
    return std::string();
  }
}
  
/*
 *  @name   Clean
 *  @fn     std::string Clean(const std::string& path)
 *  @brief  Remove duplicate, add ./, remove ../ if any
 *  @param[in] path Path to clean
 *  @return cleaned path
 */
std::string Clean(const std::string& path) {
  std::string res(path.size(), 'x');
  const char* src = path.c_str();   // Iterator on source
  auto dst = res.begin();           // Iterator on destination (clean path)
  const bool is_abs = *src == '/';  // Indicate if path is absolute or not
  
  if (is_abs) {
    *dst++ = *src++;                // Copy first '/'
    while (*src == '/') ++src;      // Skip other slash
  }
  // Define backtracking limit
  std::string::const_iterator btrack_limit = dst;
  
  // Loop over path
  while (*src) {
    bool parsed = false;
    // Are we pointing on a '.' ?
    if (*src == '.') {
      // Reach .<something> check what's next (separator or end of string)
      if (!src[1] || src[1] == '/') {
        // reach './' or '.'
        if (*++src) { // Skip separator, stick to the same dir (/./<something>)
          ++src;
        }
        parsed = true;
      } else if (src[1] == '.' && (!src[2] || src[2] == '/')) {
        // Reach '..' or '../<something>'
        src += 2;
        if (dst != btrack_limit) {
          // We can backtrack the previous part
          for (--dst; dst != btrack_limit && dst[-1] != '/'; --dst) {
            // Empty.
          }
        } else if (!is_abs) {
          // Failed to backtrack and we can't skip it either. Rewind and copy.
          src -= 2;
          *dst++ = *src++;
          *dst++ = *src++;
          if (*src) {       // Add separator
            *dst++ = *src;
          }
          // We can never backtrack over a copied "../" part so set new limit.
          btrack_limit = dst;
        }
        if (*src) { // Skip separator at front
          ++src;
        }
        parsed = true;
      }
    }
    
    // char was not .<something>, therefore not treated -> copy till next
    // separator or end of string
    if (!parsed) {
      while (*src && *src != '/') {
        *dst++ = *src++;
      }
      // Copy separator if not reach the end of string
      if (*src) {
        *dst++ = *src++;
      }
    }
    // Skip remaining '/'
    while (*src == '/') ++src; 
  }
  
  // Calculate and check the length of the cleaned path.
  size_t length = dst - res.begin();
  if (length != 0) {
    // Remove trailing '/' except if it is root path ("/" ==> path_length := 1)
    if (length > 1 && res[length - 1] == '/') {
      --length;
    }
    res.resize(length);
  } else {
    // Clean path is empty -> put '.'
    res.assign(".");
  }
  return res;
}
  
/*
 *  @name SplitComponent
 *  @fn void SplitComponent(const std::string& path, std::string* dir, 
                            std::string* file, std::string* ext)
 *  @brief  Split path into directory + extension
 *  @param[in]  path  Path where to extract data
 *  @param[out] dir   Extracted directory
 *  @param[out] ext   Extracted extension
 */
void SplitComponent(const std::string& path, std::string* dir,
                    std::string* file, std::string* ext) {
  if (dir != nullptr) {
    *dir = Dirname(path);
  }
  if (file != nullptr) {
    std::string filename = Basename(path);
    auto pos = filename.rfind(".");
    *file = pos != std::string::npos ? filename.substr(0, pos) : filename;
  }
  if (ext != nullptr) {
    *ext = Extension(path);
  }
}
}  // namespace Path
  
  
/**
 *  @namespace  String
 *  @brief      Utility functions for processing string
 */
namespace String {

/*
 *  @name Split
 *  @fn static void Split(const std::string& string, const std::string delimiter,
                          std::vector<std::string>* parts);
 *  @brief  Split a given \p string into parts for a specific delimiter
 *  @param[in]  string    String to split
 *  @param[in]  delimiter Delimiter
 *  @param[out] parts     Splitted parts
 */
void Split(const std::string& string,
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
  
  
}  // namespace String
  

  
  
}  // namespace FaceKit
