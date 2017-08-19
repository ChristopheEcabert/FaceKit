/**
 *  @file   cmd_parser.hpp
 *  @brief  Utility class to parse command line argument
 *
 *  @author Christophe Ecabert
 *  @date   12/11/15
 *  Copyright (c) 2015 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_CMD_PARSER__
#define __FACEKIT_CMD_PARSER__

#include <string>
#include <vector>

#include "facekit/core/library_export.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  CmdLineParser
 *  @brief  Utility class to parse command line argument
 *  @author Christophe Ecabert
 *  @date   12/11/15
 */
class FK_EXPORTS CmdLineParser {

 public :
#pragma mark -
#pragma mark Type definition

  /**
   *  @enum ArgState
   *  @brief  Define the possible state of an argument
   */
  enum ArgState {
    /** Argument is required */
    kNeeded,
    /** Argument is optional */
    kOptional
  };

  /**
   *  @struct Args
   *  @brief  Class representing argument's attributes
   */
  struct Args {
    /** Key, single char preceed by '-' */
    std::string key;
    /** Arg's value */
    std::string value;
    /** State */
    ArgState state;
    /** Description */
    std::string description;

    /**
     *  @name Args
     *  @fn Args(void)
     *  @brief  Constructor
     */
    Args(void) {}

    /**
     *  @name Args
     *  @fn Args(const std::string& key,
                 const ArgState& state,
                 const std::string& description)
     *  @brief  Constructor
     *  @param[in]  key         Char flag indicating for this argument
     *  @param[in]  state       Argument's state #ArgState
     *  @param[in]  description Description of the arguments
     */
    Args(const std::string& key,
         const ArgState& state,
         const std::string& description) :
    key(key),
    state(state),
    description(description) {}
  };

#pragma mark -
#pragma mark Initialization

  /**
   *  @name CmdLineParser
   *  @fn CmdLineParser(void)
   *  @brief  Constructor
   */
  CmdLineParser(void);

  /**
   *  @name ~CmdLineParser
   *  @fn ~CmdLineParser(void)
   *  @brief  Destructor
   */
  ~CmdLineParser(void);

#pragma mark -
#pragma mark Process

  /**
   *  @name AddArgument
   *  @fn void AddArgument(const std::string& key,
                           const ArgState& state,
                           const std::string description)
   *  @brief  Add an argument needed by the system
   *  @param[in]  key         Char flag indicating for this argument
   *  @param[in]  state       Argument's state #ArgState
   *  @param[in]  description Description of the arguments
   *  @return Return -1 of key already set, 0 otherwise
   */
  int AddArgument(const std::string& key,
                  const ArgState& state,
                  const std::string& description);

  /**
   *  @name ParseCmdLine
   *  @fn int ParseCmdLine(const int argc, const char** argv)
   *  @brief  Parse command line argument
   *  @param[in]  argc  Number of element in the command line
   *  @param[in]  argv  Elements in the command line
   *  @return -1 If error, 0 otherwise
   */
  int ParseCmdLine(const int argc, const char** argv);

  /**
   *  @name HasArgument
   *  @fn bool HasArgument(const std::string& key, std::string* value) const;
   *  @brief  Check whether of not the given key is present in the command line
   *  @param[in]  key     Key to look for
   *  @param[out] value   The corresponding value for the given key if present
   *                      in the command, otherwise unset.
   *  @return True if the given key is present in the command line arguments,
   *          flase otherwise
   */
  bool HasArgument(const std::string& key, std::string* value) const;

  /**
   *  @name PrintHelp
   *  @fn void PrintHelp(void) const
   *  @brief  Print description for all arguments
   */
  void PrintHelp(void) const;

#pragma mark -
#pragma mark Private

 private :
  /** List of argument */
  std::vector<Args*> argument_;

};

}  // namespace FaceKit
#endif /* __FACEKIT_CMD_PARSER__ */
