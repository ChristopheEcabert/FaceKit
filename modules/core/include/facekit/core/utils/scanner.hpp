/**
 *  @file   scanner.hpp
 *  @brief Simplified string parsing mechanism consuming part of the string by
 *         chaining call to function (One, Any, Many, ...).
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   25.05.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_SCANNER__
#define __FACEKIT_SCANNER__

#include <cstdint>
#include <string>

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
  
/**
 *  @class  Scanner
 *  @brief  Simplified string parsing mechanism consuming part of the string by
 *          chaining call to function (One, Any, Many, ...).
 *  @author Christophe Ecabert
 *  @date   25/05/18
 *  @ingroup core
 */
class Scanner {
 public:
  
#pragma mark -
#pragma mark Type definition
  
  /**
   *  @enum CharType
   *  @brief  Type of ascii char
   */
  enum class CharType : uint8_t {
    /** All */
    kAll,
    /** Space */
    kSpace,
    /** Letters only */
    kLetter,
    /** Number only */
    kNumber,
    /** Non zero numbers */
    kNonZeroNumber,
    /** Lower letter */
    kLowerLetter,
    /** Upper letter */
    kUpperLetter,
    /** Letters, Numbers */
    kLetterNumber,
    /** Lower letter, numbers */
    kLowerLetterNumber,
    /** Lower letter, numbers, _ */
    kLowerLetterNumberUnderscore,
    /** Letters, Numbers, . */
    kLetterNumberDot,
    /** Letters, Numbers, _ */
    kLetterNumberUnderscore,
    /** Letters, Numbers, ., _ */
    kLetterNumberDotUnderscore,
    /** Letter, numbers, -, _ */
    kLetterNumberDashUnderscore,
    /** Letter, numbers, -, ., / */
    kLetterNumberDashDotSlash,
    /** Letter, numbers, -, ., /, _ */
    kLetterNumberDashDotSlashUnderscore,
    /** Letter, numer, +, -, . */
    kLetterNumberPlusMinusDot
  };
  
#pragma mark -
#pragma mark Initialisation
  
  /**
   *  @name   Scanner
   *  @fn     explicit Scanner(const std::string& str)
   *  @brief  Constructor
   *  @param[in] str  String to parse, should outlive the lifespan of the 
   *                  scanner
   */
  explicit Scanner(const std::string& str);
  
  /**
   *  @name   Scanner
   *  @fn     Scanner(const Scanner& other) = delete
   *  @brief  Copy constructor
   *  @param[in] other  Object to copy from
   */
  Scanner(const Scanner& other) = delete;
  
  /**
   *  @name   operator=
   *  @fn     Scanner& operator=(const Scanner& rhs) = delete
   *  @brief  Assignment operator
   *  @param[in] rhs  Object to assign from
   *  @return Newly assigned object
   */
  Scanner& operator=(const Scanner& rhs) = delete;
  
  /**
   *  @name   ~Scanner
   *  @fn     ~Scanner(void) = default
   *  @brief  Destructor
   */
  ~Scanner(void) = default;
  
#pragma mark -
#pragma mark Usage
  
  /**
   *  @name   One
   *  @fn     Scanner& One(const CharType& type)
   *  @brief  Consume the next character of the given `type` from input.  If 
   *          the next character is not of a member of `type`, the Result will 
   *          return false.
   *  @param[in] type Type of char to search for.
   *  @return This scanner
   */
  Scanner& One(const CharType& type);
  
  /**
   *  @name   ZeroOrOneLiteral
   *  @fn     Scanner& ZeroOrOneLiteral(const std::string& s)
   *  @brief  Consume the next s.size() characters of the input, if they match 
   *          `s`. If they don't match `s`, this is a no-op.
   *  @param[in] s  Literal to match
   *  @return This scanner
   */
  Scanner& ZeroOrOneLiteral(const std::string& s);
  
  /**
   *  @name   OneLiteral
   *  @fn     Scanner& OneLiteral(const std::string& s)
   *  @brief  Consume the next s.size() characters of the input, if they match
   *          `s`. If they don't match `s`, then Result will return false.
   *  @param[in] s  Literal to match
   *  @return This scanner
   */
  Scanner& OneLiteral(const std::string& s);
  
  /**
   *  @name   Any
   *  @fn     Scanner& Any(const CharType& type)
   *  @brief  Consume characters from the input as long as they match `type`. 
   *          Zero characters is still considered a match, so it will never 
   *          cause Result to return false.
   *  @param[in] type Type of char to match
   *  @return This scanner
   */
  Scanner& Any(const CharType& type);
  
  /**
   *  @name   Many
   *  @fn     Scanner& Many(const CharType& type)
   *  @brief  Shortcut for .One(type).Any(type). Will set the error flag if zero
   *          char matches `type`.
   *  @param[in] type Type of char to match
   *  @return This scanner.
   */
  Scanner& Many(const CharType& type);
  
  /**
   *  @name   Eos
   *  @fn     Scanner& Eos(void)
   *  @brief  Check if reached the end of the string.
   *  @return This scanner
   */
  Scanner& Eos(void);
  
  /**
   *  @name   AnySpace
   *  @fn     Scanner& AnySpace(void)
   *  @brief  Detect any space
   *  @return This scanner
   */
  Scanner& AnySpace(void);
  
  /**
   *  @name   ScanUntil
   *  @fn     Scanner& ScanUntil(const char& c)
   *  @brief  This scans input until `c` is reached. `c` is NOT consumed.
   *  @return This scanner
   */
  Scanner& ScanUntil(const char& c);
  
  /**
   *  @name   ScanEscapedUntil
   *  @fn     Scanner& ScanEscapedUntil(const char& c)
   *  @brief  This scans input until `c` is reached. `c` is NOT consumed.
   *          Backslash escape sequences are skipped
   *  @return This scanner
   */
  Scanner& ScanEscapedUntil(const char& c);
  
  /**
   *  @name   RestartCapture
   *  @fn     Scanner& RestartCapture(void)
   *  @brief  Reset the capture start point. Later, when Result is called and 
   *          if it returns true, the capture returned will start at the 
   *          position at the time this was called.
   *  @return This scanner.
   */
  Scanner& RestartCapture(void);
  
  /**
   *  @name   StopCapture
   *  @fn     Scanner& StopCapture(void)
   *  @brief  Stop capturing input. Later, when Result is called and if it 
   *          returns true, the capture returned will end at the position at 
   *          the time this was called.
   *  @return This scanner.
   */
  Scanner& StopCapture(void);
  
  /**
   *  @name   Empty
   *  @fn     bool Empty(void) const
   *  @brief  Indicate if the scanner is empty or not.
   *  @return True if empty, false otherwise.
   */
  bool Empty(void) const;
  
  /**
   *  @name   Result
   *  @fn     bool Result(std::string* remaining = nullptr, std::string* capture = nullptr)
   *  @brief  Returns true if the input string successfully matched. When true 
   *          is returned, the remaining string is returned in `remaining` and 
   *          the captured string returned in `capture`, if non-NULL.
   *  @param[out] remaining Part of the string that as not been scanned
   *  @param[out] capture   Part of the string that has been captured.
   *  @return True if successfully matched, false otherwise.
   */
  bool Result(std::string* remaining = nullptr, std::string* capture = nullptr);
  
  
#pragma mark -
#pragma mark Private
 private:
  
  /**
   *  @name   ScanUntilImpl
   *  @fn     void ScanUntilImpl(const char& c, const bool& escaped)
   *  @brief  Scan until a certain char `c` is reached.
   *  @param[in] c  Char to reach
   *  @param[in] escaped  Indicate if char needs to be escaped.
   */
  void ScanUntilImpl(const char& c, const bool& escaped);
  
  /**
   *  @name   Error
   *  @fn     Scanner& Error(void)
   *  @brief  Indicate an error occured while scanning.
   *  @return This scanner
   */
  Scanner& Error(void);
  
  /**
   *  @name   Consume
   *  @fn     bool Consume(const std::string& s)
   *  @brief  Check if `s` match with the current position of the scanner. 
   *  @param[in]  s String to be matched
   *  @return True if `s` matched, false otherwise
   */
  bool Consume(const std::string& s);
  
  /**
   *  @name   IsLetter
   *  @fn     static bool IsLetter(const char& c)
   *  @brief  Check if a given char `c` is a letter [A-Za-z]
   *  @param[in] c  Char to control
   *  @return True if letter, false otherwise
   */
  static bool IsLetter(const char& c);
  
  /**
   *  @name   IsLowerLetter
   *  @fn     static bool IsLowerLetter(const char& c)
   *  @brief  Check if a given char `c` is a letter [a-z]
   *  @param[in] c  Char to control
   *  @return True if lower letter, false otherwise
   */
  static bool IsLowerLetter(const char& c);
  
  /**
   *  @name   IsNumber
   *  @fn     static bool IsNumber(const char& c)
   *  @brief  Check if a given char `c` is a number [0-9]
   *  @param[in] c  Char to control
   *  @return True if number, false otherwise
   */
  static bool IsNumber(const char& c);
  
  /**
   *  @name   IsSpace
   *  @fn     static bool IsSpace(const char& c)
   *  @brief  Check if a given char `c` is a space (space, tab, ...)
   *  @param[in] c  Char to control
   *  @return True if space, false otherwise
   */
  static bool IsSpace(const char& c);
  
  /**
   *  @name   Matches
   *  @fn     static bool Matches(const CharType& type, const char& c)
   *  @brief  Check if a given char `c` is of a given `type`
   *  @param[in] type Type of char to check
   *  @param[in] c    Char to control
   *  @return True if match, false otherwise
   */
  static bool Matches(const CharType& type, const char& c);

  /** Begining of the scanning region */
  const char* begin_;
  /** End of the scanning region */
  const char* end_;
  /** Start of the capture */
  const char* start_;
  /** End of capture */
  const char* stop_;
  /** Error flag, something wrong happened while scanning */
  bool error_;
  
  /** Unit testing purpose */
  friend class ScannerTest;
  
};
  
#pragma mark -
#pragma mark Implementation details
  
/*
 *  @name   IsLetter
 *  @fn     static bool IsLetter(const char& c)
 *  @brief  Check if a given char `c` is a letter [A-Za-z]
 *  @param[in] c  Char to control
 *  @return True if letter, false otherwise
 */
bool Scanner::IsLetter(const char& c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/*
 *  @name   IsLowerLetter
 *  @fn     static bool IsLowerLetter(const char& c)
 *  @brief  Check if a given char `c` is a letter [a-z]
 *  @param[in] c  Char to control
 *  @return True if lower letter, false otherwise
 */
bool Scanner::IsLowerLetter(const char& c) {
  return c >= 'a' && c <= 'z';
}

/*
 *  @name   IsNumber
 *  @fn     static bool IsNumber(const char& c)
 *  @brief  Check if a given char `c` is a number [0-9]
 *  @param[in] c  Char to control
 *  @return True if number, false otherwise
 */
bool Scanner::IsNumber(const char& c) {
  return c >= '0' && c <= '9';
}

/*
 *  @name   IsSpace
 *  @fn     static bool IsSpace(const char& c)
 *  @brief  Check if a given char `c` is a space (space, tab, ...)
 *  @param[in] c  Char to control
 *  @return True if space, false otherwise
 */
bool Scanner::IsSpace(const char& c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
          c == '\r');
}

/*
 *  @name   Matches
 *  @fn     static bool Matches(const CharType& type, const char& c)
 *  @brief  Check if a given char `c` is of a given `type`
 *  @param[in] type Type of char to check
 *  @param[in] c    Char to control
 *  @return True if match, false otherwise
 */
bool Scanner::Matches(const CharType& type, const char& c) {
  switch (type) {
    case CharType::kAll:
      return true;
      
    case CharType::kSpace:
      return IsSpace(c);
      
    case CharType::kLetter:
      return IsLetter(c);
      
    case CharType::kNumber:
      return IsNumber(c);
      
    case CharType::kNonZeroNumber:
      return IsNumber(c) && c != '0';
      
    case CharType::kLowerLetter:
      return IsLowerLetter(c);
      
    case CharType::kUpperLetter:
      return c >= 'A' && c <= 'Z';
      
    case CharType::kLetterNumber:
      return IsLetter(c) || IsNumber(c);
      
    case CharType::kLowerLetterNumber:
      return IsLowerLetter(c) || IsNumber(c);
      
    case CharType::kLowerLetterNumberUnderscore:
      return IsLowerLetter(c) || IsNumber(c) || c == '_';
      
    case CharType::kLetterNumberDot:
      return IsLetter(c) || IsNumber(c) || c == '.';
      
    case CharType::kLetterNumberUnderscore:
      return IsLetter(c) || IsNumber(c) || c == '_';
      
    case CharType::kLetterNumberDotUnderscore:
      return IsLetter(c) || IsNumber(c) || c == '.' || c == '_';
      
    case CharType::kLetterNumberDashUnderscore:
      return IsLetter(c) || IsNumber(c) || c == '-' || c == '_';
      
    case CharType::kLetterNumberDashDotSlash:
      return IsLetter(c) || IsNumber(c) || c == '-' || c == '.' || c == '/';
      
    case CharType::kLetterNumberDashDotSlashUnderscore:
      return (IsLetter(c) || IsNumber(c) || c == '-' || c == '.' || c == '/' ||
              c == '_');
      
    case CharType::kLetterNumberPlusMinusDot:
      return (IsLetter(c) || IsNumber(c) || c == '+' || c == '-' || c == '.');
  }
  return false;
}
  
}  // namespace String
}  // namespace FaceKit
#endif /* __FACEKIT_SCANNER__ */
