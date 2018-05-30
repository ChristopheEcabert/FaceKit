/**
 *  @file   scanner.cpp
 *  @brief Simplified string parsing mechanism consuming part of the string by
 *         chaining call to function (One, Any, Many, ...).
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   25.05.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/utils/scanner.hpp"

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
  
#pragma mark -
#pragma mark Initialisation
  
/*
 *  @name   Scanner
 *  @fn     explicit Scanner(const std::string& str)
 *  @brief  Constructor
 *  @param[in] str  String to parse
 */
Scanner::Scanner(const std::string& str) : str_(str),
                                           begin_(str_.data()),
                                           end_(begin_ + str_.size()),
                                           start_(nullptr),
                                           stop_(nullptr),
                                           error_(false) {
  RestartCapture();
}
  
#pragma mark -
#pragma mark Usage
  
/*
 *  @name   One
 *  @fn     Scanner& One(const CharType& type)
 *  @brief  Consume the next character of the given `type` from input.  If
 *          the next character is not of a member of `type`, the Result will
 *          return false.
 *  @param[in] type Type of char to search for.
 *  @return This scanner
 */
Scanner& Scanner::One(const CharType& type) {
  // Check if there is some char left and compare it
  if (Empty() || !Matches(type, begin_[0])) {
    return Error();
  }
  ++begin_;
  return *this;
}

/*
 *  @name   ZeroOrOneLiteral
 *  @fn     Scanner& ZeroOrOneLiteral(const std::string& s)
 *  @brief  Consume the next s.size() characters of the input, if they match
 *          `s`. If they don't match `s`, this is a no-op.
 *  @param[in] s  Literal to match
 *  @return This scanner
 */
Scanner& Scanner::ZeroOrOneLiteral(const std::string& s) {
  this->Consume(s);
  return *this;
}

/*
 *  @name   OneLiteral
 *  @fn     Scanner& OneLiteral(const std::string& s)
 *  @brief  Consume the next s.size() characters of the input, if they match
 *          `s`. If they don't match `s`, then Result will return false.
 *  @param[in] s  Literal to match
 *  @return This scanner
 */
Scanner& Scanner::OneLiteral(const std::string& s) {
  if (!this->Consume(s)) {
    return Error();
  }
  return *this;
}

/*
 *  @name   Any
 *  @fn     Scanner& Any(const CharType& type)
 *  @brief  Consume characters from the input as long as they match `type`.
 *          Zero characters is still considered a match, so it will never
 *          cause Result to return false.
 *  @param[in] type Type of char to match
 *  @return This scanner
 */
Scanner& Scanner::Any(const CharType& type) {
  while(!Empty() && Matches(type, begin_[0])) {
    ++begin_;
  }
  return *this;
}

/*
 *  @name   Many
 *  @fn     Scanner& Many(const CharType& type)
 *  @brief  Shortcut for .One(type).Any(type). Will set the error flag if zero
 *          char matches `type`.
 *  @param[in] type Type of char to match
 *  @return This scanner.
 */
Scanner& Scanner::Many(const CharType& type) {
  return One(type).Any(type);
}

/**
 *  @name   Eos
 *  @fn     Scanner& Eos(void)
 *  @brief  Check if reached the end of the string.
 *  @return This scanner
 */
Scanner& Scanner::Eos(void) {
  // Check if reached end of string -> scanner being empty
  if (!this->Empty()) {
    return Error();
  }
  return *this;
}

/*
 *  @name   AnySpace
 *  @fn     Scanner& AnySpace(void)
 *  @brief  Detect any space
 *  @return This scanner
 */
Scanner& Scanner::AnySpace(void) {
  return Any(CharType::kSpace);
}

/*
 *  @name   ScanUntil
 *  @fn     Scanner& ScanUntil(const char& c)
 *  @brief  This scans input until `c` is reached. `c` is NOT consumed.
 *  @return This scanner
 */
Scanner& Scanner::ScanUntil(const char& c) {
  this->ScanUntilImpl(c, false);
  return *this;
}

/*
 *  @name   ScanEscapedUntil
 *  @fn     Scanner& ScanEscapedUntil(const char& c)
 *  @brief  This scans input until `c` is reached. `c` is NOT consumed.
 *          Backslash escape sequences are skipped
 *  @return This scanner
 */
Scanner& Scanner::ScanEscapedUntil(const char& c) {
  this->ScanUntilImpl(c, true);
  return *this;
}

/*
 *  @name   RestartCapture
 *  @fn     Scanner& RestartCapture(void)
 *  @brief  Reset the capture start point. Later, when Result is called and
 *          if it returns true, the capture returned will start at the
 *          position at the time this was called.
 *  @return This scanner.
 */
Scanner& Scanner::RestartCapture(void) {
  start_ = begin_;
  stop_ = nullptr;
  return *this;
}
  
/*
 *  @name   StopCapture
 *  @fn     Scanner& StopCapture(void)
 *  @brief  Stop capturing input. Later, when Result is called and if it
 *          returns true, the capture returned will end at the position at
 *          the time this was called.
 *  @return This scanner.
 */
Scanner& Scanner::StopCapture(void) {
  stop_ = begin_;
  return *this;
}
  
/*
 *  @name   Empty
 *  @fn     bool Empty(void) const
 *  @brief  Indicate if the scanner is empty or not.
 *  @return True if empty, false otherwise.
 */
bool Scanner::Empty(void) const {
  return (end_ - begin_) == 0;
}
  
/*
 *  @name   Result
 *  @fn     bool Result(std::string* remaining = nullptr, std::string* capture = nullptr)
 *  @brief  Returns true if the input string successfully matched. When true
 *          is returned, the remaining string is returned in `remaining` and
 *          the captured string returned in `capture`, if non-NULL.
 *  @param[out] remaining Part of the string that as not been scanned
 *  @param[out] capture   Part of the string that has been captured.
 *  @return True if successfully matched, false otherwise.
 */
bool Scanner::Result(std::string* remaining,
                     std::string* capture) {
  if (!error_) {
    if (remaining) {
      remaining->assign(begin_, end_ - begin_);
    }
    if (capture) {
      const char* end = stop_ == nullptr ? begin_ : stop_;
      capture->assign(start_, end - start_);
    }
  }
  return !error_;
}
  
  
#pragma mark -
#pragma mark Private
  
/*
 *  @name   ScanUntilImpl
 *  @fn     void ScanUntilImpl(const char& c, const bool& escaped)
 *  @brief  Scan until a certain char `c` is reached.
 *  @param[in] c  Char to reach
 *  @param[in] escaped  Indicate if char needs to be escaped.
 */
void Scanner::ScanUntilImpl(const char& c, const bool& escaped) {
  // loop over all scanner's string
  while(true) {
    // Is scanner empty ?
    if (this->Empty()) {
      error_ = true;
      return;
    }
    
    // Get current char and check if matched
    const auto& ch = begin_[0];
    if (ch == c) {
      return;
    }
    // Does not match, move to the next one
    ++begin_;
    // Need escape ?
    if (escaped && ch == '\\') {
      if (this->Empty()) {
        error_ = true;
        return;
      }
      ++begin_; // skip the escaped char
    }
  }
}

/*
 *  @name   Error
 *  @fn     Scanner& Error(void)
 *  @brief  Indicate an error occured while scanning.
 *  @return This scanner
 */
Scanner& Scanner::Error(void) {
  error_ = true;
  return *this;
}
  
/*
 *  @name   Consume
 *  @fn     bool Consume(const std::string& s)
 *  @brief  Check if `s` match with the current position of the scanner.
 *  @param[in]  s String to be matched
 *  @return True if `s` matched, false otherwise
 */
bool Scanner::Consume(const std::string& s) {
  bool match = false;
  // Check if `s` is smaller than the string being scan
  if (s.length() <= (end_ - begin_)) {
    match = std::string(begin_, s.length()) == s;
    if (match) {
      begin_ += s.length();
    }
  }
  return match;
}

}  // namespace String
}  // namespace FaceKit
